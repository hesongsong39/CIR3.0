/**********************************************************************
* $Id$      eMMC              2016-03-16
*//**
* @file     eMMC.c
* @brief    SD/MMC�ӿڲ����ⲿeMMC
* @version  1.0.0
* @date     2016/03/17 ������ 08:20:15
* @author   ZhangYanMing
*
* Copyright(C) 2016, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
***********************************************************************/
/************************** Includes **********************************/
#include "lpc_types.h"
#include "lpc43xx.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_sdif.h"
#include "lpc43xx_sdmmc.h"
#include "lpc_sdmmc.h"
#include "eMMC.h"


/*****************Private variables ***********************************/
static uint32_t sdio_wait_exit = 0;
static struct _mci_card_struct sdcardinfo;
/*****************Public variables ************************************/


/*****************Extern variables ************************************/
extern uint32_t SystemCoreClock;
#define System_CPU_NANOSEC(x) (((uint64_t)(x) * SystemCoreClock)/1000000000)


/*******************************************************************************
* @brief			us�ӳ�
* @param[in]  ΢����
* @return			None
*******************************************************************************/
static void SystemWaitUs (uint32_t us) 
{
		uint32_t cyc = us * System_CPU_NANOSEC(1000)/4;
		while(cyc--);
}

/*******************************************************************************
* @brief			Ms�ӳ�
* @param[in]  ������
* @return			None
*******************************************************************************/
static void SystemWaitMs(uint32_t time)
{
		SystemWaitUs(time * 1000);
}


/*******************************************************************************
* @brief		SDIO interrupt handler callback
* @param[in]    rinsts Optional input parameter
* @return		Return value is 0, not currently used
*******************************************************************************/
static uint32_t sdmmc_irq(uint32_t rinsts)
{
	NVIC_DisableIRQ(SDIO_IRQn);
	sdio_wait_exit = 1;
	return 0;
}

/*********************************************************************//**
* @brief 		Sets up the SD event driven wakeup
* @param[in]	bits Status bits to poll for command completion
* @return 		None
**********************************************************************/
static void sdmmc_setup_wakeup(uint32_t bits)
{
		NVIC_ClearPendingIRQ(SDIO_IRQn);
		sdio_wait_exit = 0;
		LPC_SDMMC->INTMASK = bits;
		NVIC_EnableIRQ(SDIO_IRQn);
}

/*********************************************************************//**
* @brief 		A better wait callback for SDMMC driven by the IRQ flag
* @param[in]	bits Status bits to poll for command completion
* @return 		0 on success, or failure condition (-1)
**********************************************************************/
static uint32_t sdmmc_irq_driven_wait(uint32_t bits)
{
	uint32_t status;

	while (sdio_wait_exit == 0);

	/* Get status and clear interrupts */
	status = LPC_SDMMC->RINTSTS;
	LPC_SDMMC->RINTSTS = status;
	LPC_SDMMC->INTMASK = 0;

	return status;
}

/*********************************************************************//**
* @brief 		Delay callback for timed SDIF/SDMMC functions
* @param[in]	time Number of milliSeconds to wait
* @return 		None
**********************************************************************/
void sdmmc_waitms(uint32_t time)
{
	SystemWaitMs(time);
}



/*******************************************************************************
* @brief		Wait for card program to finish
* @param[in]  None
* @return			None
*******************************************************************************/
void wait_for_program_finish(void)
{
	while (sdmmc_get_state() != SDMMC_TRAN_ST);
}

/*********************************************************************//**
 * @brief		EMMC��ʼ������
 * @param[in]  width ���ݽ�����ȣ�1��4��8��Ĭ��Ϊ1
 * @param[in]  pFnpininit  EMMC���ų�ʼ������ָ��
 * @return 		TRUE/FALSE
 **********************************************************************/
Bool EmmcInit(uint8_t width,pFnpininit fpininit)
{
		uint32_t  sdio_clk = 0;
		if(fpininit == NULL)
		{
			return FALSE;
		}
		
		fpininit();
		
		sdif_reset(1);
		CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_BASE_SDIO);
		
		/* The SDIO driver needs to know the SDIO clock rate */
		sdio_clk = CGU_GetPCLKFrequency(CGU_PERIPHERAL_SDIO);

		/*  sdio with sdio_clk */
		sdif_init(sdio_clk, sdmmc_irq);
	
		sdmmc_waitms(250);
		/* Enumerate the card once detected. Note this function may
	   block for a little while. */
		if (!sdmmc_acquire(sdmmc_setup_wakeup, sdmmc_irq_driven_wait,sdmmc_waitms, &sdcardinfo))
		{
				return FALSE;
		}
		//����width��Ĭ�ϵ���ģʽ����
		if(sdmmc_width_set(width) == FALSE)
		{
				return FALSE;
		}
		//����emmc�������У�Ĭ�ϵ��٣�26MHZ
		sdmmc_highspeed_set();
		sdcardinfo.sdck_det = sdif_card_ndetect;
		sdcardinfo.sdck_wp = sdif_card_wp_on;
		sdcardinfo.sd_setpow = sdif_power_onoff;
		sdcardinfo.sd_setrst = sdif_reset;
		
		return TRUE;
}


/*********************************************************************//**
 * @brief		EMMCд����
 * @param[in]  StartSector,��ʼ����
 * @param[in]  SectorNum,д��������Ŀ
 * @param[in]  buf,д�����ݻ���
 * @param[in]  wlen,д�����
 * @return 		TRUE/FALSE
 **********************************************************************/
Bool Emmc_Write(uint32_t StartSector,uint8_t SectorNum,uint8_t *buf,uint32_t wlen)
{
		uint32_t EndSector = 0;
		
		if(wlen%512 != 0)
		{
			 return FALSE;
		}
		
		if(SectorNum != (wlen/512))
		{
			 return FALSE;
		}
		
		EndSector = StartSector + SectorNum - 1;
		
		wait_for_program_finish();
		
		if (sdmmc_write_blocks(buf,StartSector,EndSector) == 0)
		{
			 return FALSE;
		}
		
		return TRUE;
}


/*********************************************************************//**
 * @brief		EMMC������
 * @param[in]  StartSector,��ʼ����
 * @param[in]  SectorNum,��ȡ������Ŀ
 * @param[in]  buf,��ȡ���ݻ���
 * @param[in]  rlen,��ȡ����
 * @return 		TRUE/FALSE
 **********************************************************************/
Bool Emmc_Read(uint32_t StartSector,uint8_t SectorNum,uint8_t *buf,uint32_t rlen)
{
		uint32_t EndSector = 0;
		
		if(rlen%512 != 0)
		{
			 return FALSE;
		}
		
		if(SectorNum != (rlen/512))
		{
			 return FALSE;
		}
		
		EndSector = StartSector + SectorNum - 1;
		
		wait_for_program_finish();
		
		if (sdmmc_read_blocks(buf,StartSector,EndSector) == 0)
		{
			 return FALSE;
		}
		
		return TRUE;
}

