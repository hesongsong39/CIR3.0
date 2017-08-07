/**********************************************************************
* $Id$      TLV320AIC3106              2013-08-26
*//**
* @file     TLV320AIC3106.c
* @brief    TLV320AIC3106音频编解码芯片驱动
* @version  1.0.0
* @date     2013/8/26 11:29:58
* @author   HaoZhilei
*
* Copyright(C) 2013, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#include "lpc43xx_i2c.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_gpio.h"
#include "TLV320AIC3106.h"
#include <stdio.h>


/*********************************************************************//**
 * @brief       TLV320AIC3106 initialization
 * @param[in]   None
 * @return      None
 **********************************************************************/
void AIC3106_Init(void)
{
	unsigned char regtmp;
	AIC3106_AGC_CFG_Type agccfg;
	uint8_t reg;

	AIC3106_Reg_write(REG_Page_Select, 0x00);       /* sets Page-0 as the active page */
	AIC3106_Reg_write(REG_Software_Reset, 0x08);    /* reset */

	AIC3106_Clk_set(8000, 16);                      /* 8K/16bit */

//------------------------------------AF_IN config------------------------------------------------
	AIC3106_Input_control(LINE1L_to_left_PGA, ENABLE, 10);		
	AIC3106_Gain_PGA_set(GAIN_left_PGA, 0);
	
	agccfg.enable = DISABLE;                        /* Left AGC disable */
	AIC3106_AGC_set(CFG_left_AGC, &agccfg);


//------------------------------------ADC config---------------------------------------------
	// MICBIAS output is powered to 2.5V
	AIC3106_Reg_write(REG_MICBIAS_Ctrl, (2 << 6));
	/*
     * Left ADC channel is powered up
     */
  AIC3106_Reg_read(REG_LINE1L_to_Left_ADC_Ctrl, &regtmp);
  AIC3106_Reg_write(REG_LINE1L_to_Left_ADC_Ctrl, (regtmp | (1<<2)) );
    

//------------------------------------DAC config---------------------------------------------
	/* Datapath Setup
	 * 左边DAC播放左声道数据
	 * 右边DAC播放右声道数据
	 * 特别注意：该寄存器还设定了参考采样率fs(ref)=48kHz
	 */
	AIC3106_Reg_write(REG_Codec_Datapath_Setup, (1 << 3));
	AIC3106_Reg_write(REG_DAC_Power_and_Output_Driver_Ctrl, (1 << 7)); /* Left DAC is powered up */

	AIC3106_Reg_write(REG_DAC_Output_Switching_Ctrl, (0 << 6)); /* DAC_L1 path */

	// Output common-mode voltage = 1.65 V
	AIC3106_Reg_read(REG_High_Power_Output_Stage_Ctrl, &reg);
	AIC3106_Reg_write(REG_High_Power_Output_Stage_Ctrl, (reg | (2 << 6)));


//------------------------------------AF_OUT config---------------------------------------------
	AIC3106_Digital_Vol_set(Left_DAC_Digital_Volume, 0);/* Left DAC Digital Volume: -36 dB */

	/* DAC_L1-->LEFT_LO
	 *	-Analog Volume: 0dB
	 */
	AIC3106_Analog_Vol_set(VOL_DAC_L1_to_HPLOUT, 0); 
	
	AIC3106_Output_control(HPLOUT_Output, HPLCOM_Differ_HPLOUT, 0);	
	AIC3106_Output_control(HPLCOM_Output, HPLCOM_Differ_HPLOUT, 0);

}

/*********************************************************************//**
 * @brief       Write data to a register of TLV320AIC3106
 * @param[in]   reg Register address
 * @param[out] 	data  data value.
 * @return      FUNC_OK/FUNC_ERR
 **********************************************************************/
unsigned char AIC3106_Reg_write(AIC3106_REG reg, unsigned char data)
{
	I2C_M_SETUP_Type transferMCfg;
	unsigned char  tx_data[2];
	unsigned char dataTmp;

	tx_data[0] = reg;
	tx_data[1] = data;

	transferMCfg.sl_addr7bit = AIC3106_ADDR;
	transferMCfg.tx_data = &tx_data[0];
	transferMCfg.tx_length = 2;
	transferMCfg.rx_data = NULL;
	transferMCfg.rx_length = 0;
	transferMCfg.retransmissions_max = 5;

	//while (( i2c_mtx_setup.status & I2C_SETUP_STATUS_DONE) == 0);
	if (I2C_MasterTransferData(CODEC_I2C, &transferMCfg, I2C_TRANSFER_POLLING) == SUCCESS)
	{
		if(AIC3106_Reg_read(reg, &dataTmp) != FUNC_OK)
		{
			return FUNC_ERR;
		}
		if(dataTmp != data)
		{
			return FUNC_ERR;
		}

		return FUNC_OK;
	}

	return FUNC_ERR;
}

/*********************************************************************//**
 * @brief       Read data stored in register of TLV320AIC3106
 * @param[in]   reg Register address
 * @param[out] 	data  point to the buffer which is used for storing data.
 * @return      FUNC_OK/FUNC_ERR
 **********************************************************************/
unsigned char AIC3106_Reg_read(AIC3106_REG reg, unsigned char *data)
{
	I2C_M_SETUP_Type transferMCfg;
	unsigned char TXdata, RXdata;

	TXdata = reg;

	transferMCfg.sl_addr7bit = AIC3106_ADDR;
	transferMCfg.tx_data = &TXdata;
	transferMCfg.tx_length = 1;
	transferMCfg.rx_data = &RXdata;
	transferMCfg.rx_length = 1;
	transferMCfg.retransmissions_max = 5;

	if(I2C_MasterTransferData(CODEC_I2C, &transferMCfg, I2C_TRANSFER_POLLING) == SUCCESS)
	{
		//while (( i2c_mtxrx_setup.status & I2C_SETUP_STATUS_DONE) == 0);
		*data = RXdata;
		return FUNC_OK;
	}
	return FUNC_ERR;
}

/*********************************************************************//**
 * @brief       set the clock frequency of TLV320AIC3106
 * @param[in]   ClockFreq clock frequency
 * @return      FUNC_OK/FUNC_ERR
 **********************************************************************/
unsigned char AIC3106_Clk_set(unsigned int ClockFreq, unsigned char bit)
{
	unsigned char reg;
	unsigned char ret = FUNC_OK;


	/* BCLK have been setted when I2S interface is configured
	 * BCLK = 256 * fs;
	 */

	/* fs(ref) setting
	 * fs(ref) = 48k;
	 */
	AIC3106_Reg_read(REG_Codec_Datapath_Setup, &reg);
	AIC3106_Reg_write(REG_Codec_Datapath_Setup, ((reg & 0x3F) | (0 << 7)));

	switch(ClockFreq)
	{
		case 8000:
			/* ADC-fs = fs(ref)/6; DAC-fs = fs(ref)/6
			 * fs(ref) = 48k;
			 */
			reg = (10 << 4 | 10 << 0);
			break;

		case 16000:
			/* ADC-fs = fs(ref)/3; DAC-fs = fs(ref)/3
			 * fs(ref) = 48k;
			 */
			reg = (4 << 4 | 4 << 0);
			break;

		case 48000:
			/* ADC-fs = fs(ref)/1; DAC-fs = fs(ref)/1
			 * fs(ref) = 48k;
			 */
			reg = (0 << 4 | 0 << 0);
			break;

		default:
			ret = FUNC_ERR;
			break;
	}
	AIC3106_Reg_write(REG_Codec_Sample_Rate_Select, reg);

	/* When the PLL is enabled:
	 *   	fs(ref) = (PLLCLK_IN * K * R) / (2048 * P)
	 */
	AIC3106_Reg_write(REG_PLL_Programming_A, ((1 << 7) | (1 << 0)));	/* PLL is enabled, P = 1 */
	AIC3106_Reg_write(REG_PLL_Programming_B, (12 << 2));				/* J = 12 */
	AIC3106_Reg_write(REG_PLL_Programming_C, (0 << 0));					/* Eight most significant bits of D = 0 */
	AIC3106_Reg_write(REG_PLL_Programming_D, (0 << 2));					/* Six least significant bits of D = 0 */
	AIC3106_Reg_write(REG_Audio_Codec_Overflow_Flag, (1 << 0));	/* R = 1 */

	/* Slave mode, Place DOUT in high-impedance state */
	AIC3106_Reg_write(REG_Audio_Serial_Data_Interface_Ctrl_A, 1 << 5);

	reg = (0 << 6);							/* I2S mode */
	switch(bit)
	{
		case 16:											/* Audio data word length: 16 bits */
			reg |= (0 << 4);
			break;
		case 20:											/* Audio data word length: 20 bits */
			reg |= (1 << 4);
			break;
		case 24:											/* Audio data word length: 24 bits */
			reg |= (2 << 4);
			break;
		case 32:											/* Audio data word length: 32 bits */
			reg |= (3 << 4);
			break;
		default:
			ret = FUNC_ERR;
			break;
	}
	AIC3106_Reg_write(REG_Audio_Serial_Data_Interface_Ctrl_B, reg);

	AIC3106_Reg_write(REG_Clock_Generation_Ctrl, (0 << 4));  		/* PLLCLK _IN uses MCLK */

	return ret;

}

/*********************************************************************//**
 * @brief       set PGA gain of TLV320AIC3106 (Page0/Reg15~16)
 * @param[in]   pga 	Left/Right ADC PGA
 * @param[in]   gain 	PGA Gain Setting, should be:
								-0~119 (0dB~59.5dB): 0.5dB step
 								-Muted_PGA_Gain: mute PGA
 * @return      FUNC_OK/FUNC_ERR
 **********************************************************************/
unsigned char AIC3106_Gain_PGA_set(AIC3106_GAIN_PGA pga, unsigned char gain)
{
	unsigned char ret;
	if(pga == GAIN_left_PGA)
	{
		ret = AIC3106_Reg_write(REG_Left_ADC_PGA_Gain_Ctrl, gain);    /* gain/2 dB e.g. 119/2=59.5dB */
	}
	if(pga == GAIN_right_PGA)
	{
		ret = AIC3106_Reg_write(REG_Right_ADC_PGA_Gain_Ctrl, gain);
	}
	return ret;
}

//               ___
//------------->|   |
//------------->|   |
//--input------>| + |---------->PGA->ADC
//------------->|   |
//------------->|___|
//
/*********************************************************************//**
 * @brief       Input Control of TLV320AIC3106 (Page0/Reg17~24)
 * @param[in]   in Input Class
 * @param[in]   differ	Single-Ended vs Fully Differential Control, should be:
								-ENABLE: Fully Differentia
								-DISABLE: Single-Ended
 * @param[in]		gain	Input Level Control, should be:
								-0~8 (0dB~-12dB): -1.5dB step
								-Input_PGA_Not_Connected: IS not connected to the ADC PGA
 * @return      FUNC_OK/FUNC_ERR
 **********************************************************************/
unsigned char AIC3106_Input_control(AIC3106_Input in, unsigned char differ, unsigned char gain)
{
	unsigned char ret;

	switch(in)
	{
		case MIC3L_to_left_PGA:
			ret = AIC3106_Reg_write(REG_MIC3L_or_R_to_Left_ADC_Ctrl, (gain & 0x0F) << 4);
			break;
		case MIC3R_to_left_PGA:
			ret = AIC3106_Reg_write(REG_MIC3L_or_R_to_Left_ADC_Ctrl, gain & 0x0F);
			break;
		case MIC3L_to_right_PGA:
			ret = AIC3106_Reg_write(REG_MIC3L_or_R_to_Right_ADC_Ctrl, (gain & 0x0F) << 4);
			break;
		case MIC3R_to_right_PGA:
			ret = AIC3106_Reg_write(REG_MIC3L_or_R_to_Right_ADC_Ctrl, gain & 0x0F);
			break;

		case LINE1L_to_left_PGA:
		case LINE1R_to_right_PGA:
			ret = AIC3106_Reg_write(REG_LINE1L_to_Left_ADC_Ctrl + in, ((differ << 7)
			                        | (gain << 3)
			                        | (1 << 2)
			                        | (0 << 0)));
			break;
		case LINE2L_to_left_PGA:
		case LINE2R_to_right_PGA:
			ret = AIC3106_Reg_write(REG_LINE1L_to_Left_ADC_Ctrl + in, ((differ << 7)
			                        | (gain << 3)
			                        | (0 << 2)));
			break;
		case LINE1R_to_left_PGA:
		case LINE1L_to_right_PGA:
			ret = AIC3106_Reg_write(REG_LINE1L_to_Left_ADC_Ctrl + in, ((differ << 7) | (gain << 3)));
			break;

		default:
			ret = FUNC_ERR;
			break;
	}

	return ret;
}

/*********************************************************************//**
 * @brief       config AGC of TLV320AIC3106 (Page0/Reg26~35)
 * @param[in]   agc Left/Right AGC
 * @param[in]   cfg AGC configuration struct
 * @return      FUNC_OK/FUNC_ERR
 **********************************************************************/
unsigned char AIC3106_AGC_set(AIC3106_AGC agc, AIC3106_AGC_CFG_Type *cfg)
{
	if(agc == CFG_left_AGC)
	{
		if(cfg->enable == DISABLE)
		{
			AIC3106_Reg_write(REG_Left_AGC_Ctrl_A, 0 << 7);
			return FUNC_OK;
		}
		AIC3106_Reg_write(REG_Left_AGC_Ctrl_A, ((cfg->enable << 7) | (cfg->target_level << 4)
		                                        | (cfg->attack_time << 2) | (cfg->decay_time)));
		AIC3106_Reg_write(REG_Left_AGC_Ctrl_B, (cfg->max_gain << 1));
		AIC3106_Reg_write(REG_Left_AGC_Ctrl_C, ((cfg->hysteresis << 6) | (cfg->noise_hold << 1) | (cfg->clip_stepping)));
		AIC3106_Reg_write(REG_Left_AGC_Gain, cfg->gain);
		AIC3106_Reg_write(REG_Left_AGC_Noise_Gate_Debounce, (cfg->noise_debounce << 3 | (cfg->signal_debounce)));
	}
	else
	{
		if(cfg->enable == DISABLE)
		{
			AIC3106_Reg_write(REG_Right_AGC_Ctrl_A, 0 << 7);
			return FUNC_OK;
		}
		AIC3106_Reg_write(REG_Right_AGC_Ctrl_A, ((cfg->enable << 7) | (cfg->target_level << 4)
		                  | (cfg->attack_time << 2) | (cfg->decay_time)));
		AIC3106_Reg_write(REG_Right_AGC_Ctrl_B, (cfg->max_gain << 1));
		AIC3106_Reg_write(REG_Right_AGC_Ctrl_C, ((cfg->hysteresis << 6) | (cfg->noise_hold << 1) | (cfg->clip_stepping)));
		AIC3106_Reg_write(REG_Right_AGC_Gain, cfg->gain);
		AIC3106_Reg_write(REG_Right_AGC_Noise_Gate_Debounce, (cfg->noise_debounce << 3 | (cfg->signal_debounce)));
	}
	return FUNC_OK;
}

/*********************************************************************//**
 * @brief       set Digital Volume of TLV320AIC3106 (Page0/Reg43~44)
 * @param[in]   dv 	Left DAC/Right DAC
 * @param[in]   vol	Digital Volume Control Setting, should be:
								-0~127(0dB~-63.5dB): -0.5dB step
								-Digital_Mute_Vol: DAC channel is muted
 * @return      FUNC_OK/FUNC_ERR
 **********************************************************************/
unsigned char AIC3106_Digital_Vol_set(AIC3106_Digital_Volume dv, unsigned char vol)
{
	unsigned char ret;
	if(dv == Left_DAC_Digital_Volume)
	{
		ret = AIC3106_Reg_write(REG_Left_DAC_Digital_Volume_Ctrl, vol);    /* gain/2 dB e.g. 119/2=59.5dB */
	}
	if(dv == Right_DAC_Digital_Volume)
	{
		ret = AIC3106_Reg_write(REG_Right_DAC_Digital_Volume_Ctrl, vol);
	}
	return ret;
}

//               ___
//-LINE2L------>|   |
//-PGAL-------->|   |
//-DACL-------->| + |---------->output stage
//-LINE2R------>|   |
//-PGAR-------->|   |
//-DACR-------->|___|
/*********************************************************************//**
 * @brief       1. control output datapatch ...to...
								2. set Analog Volume of TLV320AIC3106
								3. Page0/Reg45~50,Reg52~57,Reg59~64,
								Reg66~71,Reg73~78,Reg80~85,Reg87~92
 * @param[in]   av 	each analog signal coming into the output stage
 * @param[in]   vol	Analog Volume Control Setting, see Table 5 at <TLV320AIC3106.pdf>
								-0~118(0dB~-78.3dB): 0.5dB or 0.6dB step
								-118~127: mute
 * @return      FUNC_OK/FUNC_ERR
 **********************************************************************/
unsigned char AIC3106_Analog_Vol_set(AIC3106_Analog_Volume av, unsigned char vol)
{
	unsigned char ret;

	ret = AIC3106_Reg_write(REG_LINE2L_to_HPLOUT_Volume_Ctrl + av, (1 << 7 | vol));

	return ret;
}

/*********************************************************************//**
 * @brief       1. Output Level Control of TLV320AIC3106 (Page0/Reg51,58,65,72,79,86,93)
 								2. Output Driver Control of TLV320AIC3106 (Page0/Reg37,38)
									(1) configured as differential
									(2) configured as constant VCM output
									(3) configured as independent single-ended output
 * @param[in]   out Output level class
 * @param[in]   drv Output Driver class, only when output is HPLCOM_Output or HPRCOM_Output
 								-HPLCOM_Differ_HPLOUT...
 								-NONE_Output_Driver
 * @param[in]   gain Output Level Control
 								-0~9 (0dB~9dB): 1dB step
 * @return      FUNC_OK/FUNC_ERR
 **********************************************************************/
unsigned char AIC3106_Output_control(AIC3106_Output out, AIC3106_Output_Driver drv, unsigned char gain)
{
	unsigned char ret;
	unsigned char reg;
	AIC3106_OUTPUT_LEVEL_CTRL_Type cfg;

	cfg.output_level = gain;
	cfg.mute = 1;
	cfg.drive = 0;
	cfg.power_up = 1;

	ret = AIC3106_Reg_write(REG_HPLOUT_Output_Level_Ctrl + out * 7, ((cfg.output_level << 4)
	                        |	(cfg.mute << 3)
	                        | (cfg.drive << 2)
	                        | (cfg.power_up)));
	/* Output Driver CFG
	 * when output is HPLCOM_Output or HPRCOM_Output
	 */
	if((out == HPLCOM_Output) || (out == HPRCOM_Output))
	{
		if(drv < HPRCOM_Differ_HPROUT)
		{
			AIC3106_Reg_read(REG_DAC_Power_and_Output_Driver_Ctrl, &reg);
			ret = AIC3106_Reg_write(REG_DAC_Power_and_Output_Driver_Ctrl, (reg | (drv << 4)));
		}
		else
		{
			ret = AIC3106_Reg_write(REG_High_Power_Output_Driver_Ctrl, (drv - 3) << 3);
		}
	}

	return ret;
}




/**
 * @}
 */

