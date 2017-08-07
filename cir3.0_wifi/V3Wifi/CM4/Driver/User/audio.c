/**********************************************************************
* $Id$      Audio              2014-07-01
*//**
* @file     Audio.c
* @brief    ��Ƶ��غ���
* @version  2.0.0
* @date     2014/7/1 ���ڶ� 13:56:02
* @author   HaoZhilei
*
* Copyright(C) 2014, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
#include <ucos_ii.h>
#include <stdio.h>
#include <string.h>
#include "lpc43xx_cgu.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_i2c.h"
#include "lpc43xx_i2s.h"
#include "lpc43xx_gpio.h"
#include "TLV320AIC3106.h"
#include "audio.h"




/* PUBLIC VARIABLES **************************************************/


/* PUBLIC VARIABLES **************************************************/
AudioQueue_Type	g_AQRX;
AudioQueue_Type	g_AQTX;


/* PRIVATE VARIABLES **************************************************/
static uint32_t tx_offset = 0;              /* ��Ƶ����ƫ���� */
static uint32_t rx_offset = 0;



/*********************************************************************//**
 * @brief     Audio Queue Initialization
 * @param[in] None
 * @return    None
 **********************************************************************/
void Audio_Q_Init(void)
{
    memset(&g_AQRX, 0, sizeof(g_AQRX));
    memset(&g_AQTX, 0, sizeof(g_AQTX));
}

/*********************************************************************//**
 * @brief      Audio Queue Addition
 *             ��������һ�����ȵ�����
 * @param[in]  pQ      point to Queue
 * @param[in]  pdata   point to what to being added
 * @param[in]  plen    length of what to being added
 * @return     TRUE/FALSE
 **********************************************************************/
Bool Audio_Q_AddTail(AudioQueue_Type *pQ, int16_t *pdata, uint16_t plen)
{
		OS_CPU_SR  cpu_sr;
	
    if(AQ_IS_FULL(pQ))
        return FALSE;
    if(plen > AUDIO_BUFFER_SIZE)
        return FALSE;
		
		OS_ENTER_CRITICAL();
		memset(&pQ->buf[pQ->wr & AUDIO_QUEUE_MASK][0], 0, sizeof(short)*AUDIO_BUFFER_SIZE);
    memcpy(&pQ->buf[pQ->wr & AUDIO_QUEUE_MASK][0], pdata, plen*sizeof(short));
    pQ->wr++;
		OS_EXIT_CRITICAL();

    return TRUE;
}

/*********************************************************************//**
 * @brief      Audio Queue Remove
 *             �Ӷ�����ȡ��һ�����ȵ�����
 * @param[in]  pQ      point to Queue
 * @param[in]  pdata   point to what to being Removed
 * @param[in]  plen    length of what to being Removed
 * @return     TRUE/FALSE
 **********************************************************************/
Bool Audio_Q_RemoveTail(AudioQueue_Type *pQ, int16_t *pdata, uint16_t plen)
{
		OS_CPU_SR  cpu_sr;
	
    if(AQ_IS_EMPTY(pQ))
        return FALSE;
    if(plen < AUDIO_BUFFER_SIZE)
        return FALSE;
		
		OS_ENTER_CRITICAL();
		memcpy(pdata, &pQ->buf[pQ->rd & AUDIO_QUEUE_MASK][0], AUDIO_BUFFER_SIZE*sizeof(short));
    pQ->rd++;
		OS_EXIT_CRITICAL();

    return TRUE;
}

/*********************************************************************//**
 * @brief     Get item num of Audio Queue
 *            ������еĳ�Ա����
 * @param[in] pCQ   point to Queue
 * @return    the num of items
 **********************************************************************/
uint8_t Audio_Q_GetSize(AudioQueue_Type *pQ)
{
    return AQ_DATA_COUNT(pQ);
}


/*********************************************************************//**
 * @brief      Audio Queue Get value
 *             ����ȡ��ͨ������,�����ͷŶ��пռ�
 * @param[in]  pCQ  point to Queue
 * @return     point to data
 **********************************************************************/
Bool Audio_Q_GetValue(AudioQueue_Type *pQ, int16_t *pdata, uint16_t plen)
{
	OS_CPU_SR  cpu_sr;
	
	if(AQ_IS_EMPTY(pQ))
		return FALSE;
	if(plen > AUDIO_BUFFER_SIZE)
    return FALSE;
	
	OS_ENTER_CRITICAL();
	memcpy(pdata, &pQ->buf[pQ->rd & AUDIO_QUEUE_MASK][0], plen);
	OS_EXIT_CRITICAL();
	
	return TRUE;
}

/*********************************************************************//**
 * @brief       I2S IRQ Handler
 * @param[in]   None
 * @return      None
 **********************************************************************/
void I2S0_IRQHandler(void)
{
    uint32_t txlevel, rxlevel, i;

    if(I2S_GetIRQStatus(LPC_I2S0,I2S_RX_MODE) == ENABLE)
    {
        rxlevel = I2S_GetLevel(LPC_I2S0,I2S_RX_MODE);
        if((rxlevel >= 4)&&(AQ_DATA_COUNT(&g_AQRX) < AUDIO_QUEUE_SIZE))  /* ����������δ�� */
        {
            for(i=0; i<rxlevel; i++)
            {
                *(uint32_t *)&g_AQRX.buf[AUDIO_QUEUE_MASK&g_AQRX.wr][rx_offset] = LPC_I2S0->RXFIFO;
                rx_offset +=2;
                if(rx_offset >= AUDIO_BUFFER_SIZE)
                {
                    rx_offset = 0;
                    g_AQRX.wr++;
                    break;                 /* ����ѭ������ֹԽ�� */
                }
            }
        }
    }

    if(I2S_GetIRQStatus(LPC_I2S0,I2S_TX_MODE) == ENABLE)
    {
        txlevel = I2S_GetLevel(LPC_I2S0,I2S_TX_MODE);

        if((txlevel <= 4)&&(AQ_DATA_COUNT(&g_AQTX) != 0))      /* ������������ */
        {
            for(i=0; i<8-txlevel; i++)
            {
                LPC_I2S0->TXFIFO = *(uint32_t *)&g_AQTX.buf[AUDIO_QUEUE_MASK&g_AQTX.rd][tx_offset];
                tx_offset +=2;
                if(tx_offset >= AUDIO_BUFFER_SIZE)
                {
                    tx_offset = 0;
                    g_AQTX.rd++;
                    break;              /* ����ѭ������ֹԽ�� */
                }
            }
        }
        if(AQ_IS_EMPTY(&g_AQTX))
        {
            Audio_Stop(TX);            /* �����ݿɷ���رշ����ж� */
        }
    }
}

/*********************************************************************//**
 * @brief       I2S initialization
 * @param[in]   none
 * @return      None
 **********************************************************************/
void I2S_INIT(void)
{
    I2S_MODEConf_Type I2S_ClkConfig;
    I2S_CFG_Type I2S_ConfigStruct;

    /* PLL0audio configuration*/
    CGU_EntityConnect(CGU_CLKSRC_XTAL_OSC, CGU_CLKSRC_PLL0_AUDIO);
    CGU_SetPLL0audio(8000);
    CGU_EnableEntity(CGU_CLKSRC_PLL0_AUDIO, ENABLE);

    /* PLL0Audio -> I2S */
    CGU_EntityConnect(CGU_CLKSRC_PLL0_AUDIO, CGU_BASE_APLL);
    CGU_EnableEntity(CGU_BASE_APLL, ENABLE);
    I2S_Init(LPC_I2S0);

    /* setup:
     * 		- wordwidth: 16 bits
     * 		- mono mode
     * 		- master mode for I2S_TX
     * 		- Frequency = 8 kHz			G.729A support 8K/16bit
     */

    /* Audio Config*/
    I2S_ConfigStruct.wordwidth = I2S_WORDWIDTH_16;
    I2S_ConfigStruct.mono = I2S_MONO;                       /* ������ */
    I2S_ConfigStruct.stop = I2S_STOP_ENABLE;                /* ���ö�FIFO �ķ��ʣ�������ͨ�����ھ���ģʽ */
    I2S_ConfigStruct.reset = I2S_RESET_ENABLE;
    I2S_ConfigStruct.ws_sel = I2S_MASTER_MODE;
    I2S_ConfigStruct.ws_halfperiod = 32;                    /* WCLK = 32 * fs */
    I2S_ConfigStruct.mute = I2S_MUTE_DISABLE;
    I2S_Config(LPC_I2S0,I2S_TX_MODE,&I2S_ConfigStruct);     /* ������Ƶ������� */

    I2S_ConfigStruct.ws_sel = I2S_MASTER_MODE;
    I2S_Config(LPC_I2S0,I2S_RX_MODE,&I2S_ConfigStruct);     /* ������Ƶ�������� */

    /* Clock Mode Config*/
    I2S_ClkConfig.clksel = I2S_CLKSEL_PLLAUDIO;             /* ѡ��PLL0AUDIO��Ϊʱ��Դ */
    I2S_ClkConfig.fpin = I2S_4PIN_DISABLE;                  /* ����ģ���ֹ����ģʽ */
    I2S_ClkConfig.mcena = I2S_MCLK_ENABLE;
    I2S_ModeConfig(LPC_I2S0,&I2S_ClkConfig,I2S_TX_MODE);

    I2S_ClkConfig.clksel = I2S_CLKSEL_MCLK;
    I2S_ClkConfig.fpin = I2S_4PIN_ENABLE;                   /* ����4����ģʽѡ�񣬼�I2S ���ͺͽ���ģ��֮�乲��SCK��WS�ź� */
    I2S_ClkConfig.mcena = I2S_MCLK_DISABLE;
    I2S_ModeConfig(LPC_I2S0,&I2S_ClkConfig,I2S_RX_MODE);

    I2S_FreqConfig_PLL0(LPC_I2S0, 32, I2S_TX_MODE);         /* MCLK = 32 * BCLK */

    /* TX FIFO depth is 4 */
    I2S_IRQConfig(LPC_I2S0,I2S_TX_MODE,4);

    /* RX FIFO depth is 4 */
    I2S_IRQConfig(LPC_I2S0,I2S_RX_MODE,4);

    NVIC_ClearPendingIRQ(M0_I2S0_OR_I2S1_OR_QEI_IRQn);
    NVIC_EnableIRQ(M0_I2S0_OR_I2S1_OR_QEI_IRQn);

    I2S_Start(LPC_I2S0);
		
		//Ĭ�Ϸ��͡�������Ƶͨ·���ǹر�״̬
		Audio_Stop(RX);
		Audio_Stop(TX);
}


/*********************************************************************//**
 * @brief       audio initialization
 * @param[in]   none
 * @return      None
 **********************************************************************/
Bool Audio_INIT(pFnfI2Spininit fpininit,pFnfCodecReset fCodecReset)
{
		if((fpininit == NULL)||(fCodecReset == NULL))
		{
				return FALSE;
		}
		//I2S�����Ƶ���г�ʼ��
		Audio_Q_Init();
		//Codec��ʼ��
		fCodecReset();
		AIC3106_Init();
		//I2S��ʼ��
		fpininit();
    I2S_INIT();
		
		return TRUE;
}

/*********************************************************************//**
 * @brief       audio setup
 * @param[in]   md setup RX/TX/ALL mode
 * @return      None
 **********************************************************************/
void Audio_Setup(AUDIO_MODE md)
{
    if (md == TX)
    {
        I2S_IRQCmd(LPC_I2S0, I2S_TX_MODE, ENABLE);
    }
    else if (md == RX)
    {
        I2S_IRQCmd(LPC_I2S0, I2S_RX_MODE, ENABLE);
    }
    else
    {
        I2S_IRQCmd(LPC_I2S0, I2S_TX_MODE, ENABLE);
        I2S_IRQCmd(LPC_I2S0, I2S_RX_MODE, ENABLE);
    }
}


/*********************************************************************//**
 * @brief       audio stop
 * @param[in]   md setup RX/TX/ALL mode
 * @return      None
 **********************************************************************/
void Audio_Stop(AUDIO_MODE md)
{
    if (md == TX)
    {
        I2S_IRQCmd(LPC_I2S0, I2S_TX_MODE, DISABLE);
    }
    else if (md == RX)
    {
        I2S_IRQCmd(LPC_I2S0, I2S_RX_MODE, DISABLE);
    }
    else
    {
        I2S_IRQCmd(LPC_I2S0, I2S_TX_MODE, DISABLE);
        I2S_IRQCmd(LPC_I2S0, I2S_RX_MODE, DISABLE);
    }
}

/*********************************************************************//**
 * @brief       audio stop
 * @param[in]   md setup RX/TX/ALL mode
 * @return      None
 **********************************************************************/
Bool Audio_StatusQuery(AUDIO_MODE md)
{
		FunctionalState FunctionalStatus = DISABLE;
    if (md == TX)
    {
        FunctionalStatus = I2S_GetIRQStatus(LPC_I2S0, I2S_TX_MODE);
    }
    else if (md == RX)
    {
        FunctionalStatus = I2S_GetIRQStatus(LPC_I2S0, I2S_RX_MODE);
    }
		
		if(FunctionalStatus == DISABLE)
		{
				return FALSE;
		}
		else
		{
				return TRUE;
		}
}

/*********************************************************************//**
 * @brief       Get length of RX data
 * @param[in]   None
 * @return      length
 **********************************************************************/
uint32_t Audio_GetRXLength(void)
{
    return rx_offset;
}
/*********************************************************************//**
 * @brief       Get length of TX data
 * @param[in]   None
 * @return      length
 **********************************************************************/
uint32_t Audio_GetTXLength(void)
{
    return tx_offset;
}

/*********************************************************************//**
 * @brief       Set length of TX data
 * @param[in]   None
 * @return      length
 **********************************************************************/
void Audio_GetRXSet(void)
{
     rx_offset = 0;
}
/*********************************************************************//**
 * @brief       Set length of TX data
 * @param[in]   None
 * @return      length
 **********************************************************************/
void Audio_GetTXSet(void)
{
     tx_offset = 0;
}


/**
 * END
 */
