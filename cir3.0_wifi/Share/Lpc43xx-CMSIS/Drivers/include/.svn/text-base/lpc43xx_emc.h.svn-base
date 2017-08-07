/**********************************************************************
* $Id: lpc43xx_emc.h 8765 2011-12-08 00:51:21Z nxp21346 $		lpc43xx_emc.h		2011-12-07
*//**
* @file		lpc43xx_emc.h
* @brief	Contains all functions support for Clock Generation and Control
* 			firmware library on lpc43xx
* @version	1.0
* @date		07. December. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors’
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/

#ifndef __LPC43XX_EMC_H_
#define __LPC43XX_EMC_H_

#include "lpc_types.h"
#include "LPC43xx.h"

typedef struct
{
    uint8_t  CSn;         // 0/1/2/3
    uint8_t DataWidth;    // 8bit, 16bit, 32bit
    uint8_t PageMode;   // 1: asynchronous page mode enabled
    uint8_t ByteLane;   // 1: Read when active bits are low, 0: read when acrive bits are High
    uint8_t ExtendedWait;  //1: enabled
    uint8_t AddressMirror; // 1: reset memory map, 0: normal memory map

    uint8_t  WaitWEn;       // Delay from the chip select to the write enable (CCLK cycles) 
    uint8_t  WaitOEn;       // Delay from the chip select or address change to output enable (CCLK cycles)
    uint8_t  WaitRd;        // Delay from the chip select to the read access (CCLK cycles)
    uint8_t  WaitPage;      // Delay for asynchronous page mode sequential accesses (CCLK cycles)
    uint8_t  WaitWr;        // Delay from the chip select to the write access (CCLK cycles)
    uint8_t  WaitTurn;      // The number of bus turnaround cycles
} EMC_STATIC_MEM_Config_Type;

/*EMC static memory registers enum*/
typedef enum
{
  EMC_STA_MEM_WAITWEN,
  EMC_STA_MEM_WAITOEN,
  EMC_STA_MEM_WAITRD,
  EMC_STA_MEM_WAITPAGE,
  EMC_STA_MEM_WAITWR,
  EMC_STA_MEM_WAITTURN,
} EMC_STA_MEM_PAR;

/** @defgroup EMC_Private_Macros EMC Private Macros
 * @{
 */

/***********************************************************************
 * EMC Control Register (EMCControl)
 **********************************************************************/
/* Control register mask */
#define EMC_Control_MASK            ((uint32_t )0x07)
/* Control register EMC: Enable control. */
#define EMC_Control_E               ((uint32_t )(1<<0))
/* Control register EMC: Address mirror control. */
#define EMC_Control_M               ((uint32_t )(1<<1))
/* Control register EMC: Low-power mode control. */
#define EMC_Control_L               ((uint32_t )(1<<2))

/***********************************************************************
 * EMC Status Register (EMCStatus)
 **********************************************************************/
/* Status register mask */
#define EMC_Status_MASK             ((uint32_t )0x07)
/* Status register EMC: Busy. */
#define EMC_Status_B                ((uint32_t )(1<<0))
/* Status register EMC: Write buffer status. */
#define EMC_Status_S                ((uint32_t )(1<<1))
/* Status register EMC: Self-refresh acknowledge.. */
#define EMC_Status_SA               ((uint32_t )(1<<2))

/***********************************************************************
 * EMC Configuration register (EMCConfig)
 **********************************************************************/
/* EMC Configuration register : Little Endian. */
#define EMC_Config_Little_Endian_Mode       ((uint32_t )(0<<0))
/* EMC Configuration register : Big Endian. */
#define EMC_Config_Big_Endian_Mode      ((uint32_t )(1<<0))
/* EMC Configuration: Endian Mask */
#define EMC_Config_Endian_Mask           (0x01<<0)

/* EMC Configuration register: CLKOUT ratio 1:1. */
#define EMC_Config_CCLK_1_1             ((uinr32_t)(0<<8))
/* EMC Configuration register: CLKOUT ratio 1:1. */
#define EMC_Config_CCLK_1_2             ((uinr32_t)(1<<8))

/* EMC Configuration register mask */
#define EMC_Config_MASK             ((uint32_t)(0x101))

/***********************************************************************
 * Static Memory Extended Wait Register (EMCStaticExtendedWait)
 **********************************************************************/
/* StaticExtendedWait register EMC: External wait time out. */
#define EMC_StaticExtendedWait_EXTENDEDWAIT(n)          ((uint32_t )(n & 0x3ff))

/***********************************************************************
 * Static Memory Configuration registers (EMCStaticConfig0-3)
 **********************************************************************/
/* StaticConfig register EMC: Memory width (MW). */
#define EMC_STATIC_CFG_MEMWIDTH_POS     (0)
#define EMC_STATIC_CFG_MEMWIDTH_BMASK       (0x03<<EMC_STATIC_CFG_MEMWIDTH_POS)
#define EMC_StaticConfig_MW(n)          ((uint32_t )((n<<EMC_STATIC_CFG_MEMWIDTH_POS) & EMC_STATIC_CFG_MEMWIDTH_BMASK))

/* StaticConfig register EMC: Memory width 8bit . */
#define EMC_STATIC_CFG_MW_8BITS         (EMC_StaticConfig_MW(0))

/* StaticConfig register EMC: Memory width 16bit . */
#define EMC_STATIC_CFG_MW_16BITS        (EMC_StaticConfig_MW(1))

/* StaticConfig register EMC: Memory width 32bit . */
#define EMC_STATIC_CFG_MW_32BITS        (EMC_StaticConfig_MW(2))

/* StaticConfig register EMC: Page mode (PM) */
#define EMC_STATIC_CFG_PAGEMODE_POS             (3)
#define EMC_STATIC_CFG_PAGEMODE_MASK                (1<<EMC_STATIC_CFG_PAGEMODE_POS)

#define EMC_CFG_PM_DISABLE                  (0<<EMC_STATIC_CFG_PAGEMODE_POS)
#define EMC_CFG_PM_ASYNC_ENABLE                 (1<<EMC_STATIC_CFG_PAGEMODE_POS)


/* StaticConfig register EMC: Chip select polarity (PC) */
#define EMC_STATIC_CFG_CHIPPOLARITY_POS         (6)
#define EMC_STATIC_CFG_CHIPPOLARITY_MASK            (1<<EMC_STATIC_CFG_CHIPPOLARITY_POS)

#define EMC_CFG_BYTELAND_PC_ACTIVE_LO               (0<<EMC_STATIC_CFG_CHIPPOLARITY_POS)
#define EMC_CFG_BYTELAND_PC_ACTIVE_HI               (1<<EMC_STATIC_CFG_CHIPPOLARITY_POS)


/* StaticConfig register EMC: Byte lane state (PB) */
#define EMC_STATIC_CFG_BYTELAND_POS             (7)
#define EMC_STATIC_CFG_BYTELAND_MASK                (1<<EMC_STATIC_CFG_BYTELAND_POS)

#define EMC_CFG_BYTELAND_READ_BITSHIGH              (0<<EMC_STATIC_CFG_BYTELAND_POS)
#define EMC_CFG_BYTELAND_READ_BITSLOW               (1<<EMC_STATIC_CFG_BYTELAND_POS)


/* StaticConfig register EMC: Extended wait (EW) */
#define EMC_STATIC_CFG_EXTWAIT_POS              (8)
#define EMC_STATIC_CFG_EXTWAIT_MASK             (1<<EMC_STATIC_CFG_EXTWAIT_POS)

#define EMC_CFG_EW_DISABLED                 (0<<EMC_STATIC_CFG_EXTWAIT_POS)
#define EMC_CFG_EW_ENABLED                  (1<<EMC_STATIC_CFG_EXTWAIT_POS)


/* StaticConfig register EMC: Buffer enable (B) */
#define EMC_STATIC_CFG_BUFENABLE_POS                (19)
#define EMC_STATIC_CFG_BUFENABLE_MASK               (1<<EMC_STATIC_CFG_BUFENABLE_POS)

#define EMC_CFG_BUF_DISABLED                    (0<<EMC_STATIC_CFG_BUFENABLE_POS)
#define EMC_CFG_BUF_ENABLED                 (1<<EMC_STATIC_CFG_BUFENABLE_POS)

/* StaticConfig register EMC: Write protect (P) */
#define EMC_STATIC_CFG_WRIEPROTECT_POS              (20)
#define EMC_STATIC_CFG_WRIEPROTECT_MASK             (1<<EMC_STATIC_CFG_WRIEPROTECT_POS)

#define EMC_CFG_WRITEPROTECT_DISABLED               (0<<EMC_STATIC_CFG_WRIEPROTECT_POS)
#define EMC_CFG_WRITEPROTECT_ENABLED                (1<<EMC_STATIC_CFG_WRIEPROTECT_POS)

/***********************************************************************
 * Static Memory Write Enable Delay registers (EMCStaticWaitWen0-3)
 **********************************************************************/
/* StaticWaitWen register EMC: Wait write enable (WAITWEN). */
#define EMC_StaticWaitWen_WAITWEN(n)    ((uint32_t )(n & 0x0f))

/***********************************************************************
 * Static Memory Output Enable Delay registers (EMCStaticWaitOen0-3)
 **********************************************************************/
/* StaticWaitOen register EMC: Wait output enable (WAITOEN). */
#define EMC_StaticWaitOen_WAITOEN(n)    ((uint32_t )(n & 0x0f))

/***********************************************************************
 * Static Memory Read Delay registers (EMCStaticWaitRd0-3)
 **********************************************************************/
/* StaticWaitRd register EMC: Non-page mode read wait states or asynchronous page mode
read first access wait state (WAITRD) */
#define EMC_StaticWaitRd_WAITRD(n)      ((uint32_t )(n & 0x1f))

/***********************************************************************
 * Static Memory Page Mode Read Delay registers (EMCStaticwaitPage0-3)
 **********************************************************************/
/* StaticwaitPage register EMC: Asynchronous page mode read after the first
read wait states (WAITPAGE). */
#define EMC_StaticwaitPage_WAITPAGE(n)  ((uint32_t )(n & 0x1f))

/***********************************************************************
 * Static Memory Write Delay registers (EMCStaticWaitwr0-3)
 **********************************************************************/
/* StaticWaitwr register EMC: Write wait states (WAITWR). */
#define EMC_StaticWaitwr_WAITWR(n)      ((uint32_t )(n & 0x1f))

/***********************************************************************
 * Static Memory Turn Round Delay registers (EMCStaticWaitTurn0-3)
 **********************************************************************/
/* StaticWaitTurn register EMC: Bus turnaround cycles (WAITTURN). */
#define EMC_StaticWaitTurn_WAITTURN(n)  ((uint32_t )(n & 0x0f))

/***********************************************************************
 * Delay Control register (EMCDLYCTL)
 **********************************************************************/
#define EMC_DLYCTL_CMDDLY(n)        ((uint32_t)(n&0x1F))
#define EMC_DLYCTL_FBCLKDLY(n)      ((uint32_t)((n&0x1F)<<8))
#define EMC_DLYCTL_CLKOUT0DLY(n)    ((uint32_t)((n&0x1F)<<16))
#define EMC_DLYCTL_CLKOUT1DLY(n)    ((uint32_t)((n&0x1F)<<24))

/***********************************************************************
 * EMC Calibration register (EMCCAL)
 **********************************************************************/
#define EMC_CAL_CALVALUE(n)         ((uint32_t)(n&0xFF))
#define EMC_CAL_START               ((uint32_t)(1<<14))
#define EMC_CAL_DONE                ((uint32_t)(1<<15))


/***********************************************************************
 * EMC Function Return codes
 **********************************************************************/
typedef int32_t EMC_FUNC_CODE;
#define  EMC_FUNC_OK                            (0)
#define  EMC_FUNC_ERR                           (0x01)
#define  EMC_FUNC_INVALID_PARAM                 (0x02)

/**
 * @}
 */




#define __CRYSTAL        (12000000UL)    /* Crystal Oscillator frequency          */
#define __PLLMULT		 (15)
#define __PLLOUTHZ		 (__CRYSTAL * __PLLMULT)
#define __EMCDIV		 (2)
#define __EMCHZ			 (__PLLOUTHZ / __EMCDIV)


/* SDRAM Address Base for DYCS0*/
#define SDRAM_BASE_ADDR 0x28000000
#define FLASH_BASE_ADDR 0x1C000000

#define EMC_SDRAM_WIDTH_8_BITS		0
#define EMC_SDRAM_WIDTH_16_BITS		1
#define EMC_SDRAM_WIDTH_32_BITS		2

#define EMC_SDRAM_SIZE_16_MBITS		0
#define EMC_SDRAM_SIZE_64_MBITS		1
#define EMC_SDRAM_SIZE_128_MBITS	2
#define EMC_SDRAM_SIZE_256_MBITS	3
#define EMC_SDRAM_SIZE_512_MBITS	4

#define EMC_SDRAM_DATA_BUS_16_BITS	0
#define EMC_SDRAM_DATA_BUS_32_BITS	1

#define EMC_B_ENABLE 					(1 << 19)
#define EMC_ENABLE 						(1 << 0)
#define EMC_CE_ENABLE 					(1 << 0)
#define EMC_CS_ENABLE 					(1 << 1)
#define EMC_CLOCK_DELAYED_STRATEGY 		(0 << 0)
#define EMC_COMMAND_DELAYED_STRATEGY 	(1 << 0)
#define EMC_COMMAND_DELAYED_STRATEGY2 	(2 << 0)
#define EMC_COMMAND_DELAYED_STRATEGY3 	(3 << 0)
#define EMC_INIT(i) 					((i) << 7)
#define EMC_NORMAL 						(0)
#define EMC_MODE 						(1)
#define EMC_PRECHARGE_ALL 				(2)
#define EMC_NOP 						(3)

/* The Hitex LPC18xx Evaluation board contains a 64Mb SDRAM with a 16-bit data bus */
#if defined(HITEX_BOARD)

#define SDRAM_SIZE_BYTES		(1024UL * 1024UL * 8UL)
#define SDRAM_WIDTH				EMC_SDRAM_WIDTH_16_BITS
#define SDRAM_SIZE_MBITS		EMC_SDRAM_SIZE_64_MBITS
#define SDRAM_DATA_BUS_BITS		EMC_SDRAM_DATA_BUS_16_BITS			
#define SDRAM_COL_ADDR_BITS		8		

#elif defined(KEIL_BOARD)
// TODO adapt these for the MCB4357 !!!!
#define SDRAM_SIZE_BYTES		(1024UL * 1024UL * 16UL)
#define SDRAM_WIDTH				EMC_SDRAM_WIDTH_32_BITS
#define SDRAM_SIZE_MBITS		EMC_SDRAM_SIZE_128_MBITS
#define SDRAM_DATA_BUS_BITS		EMC_SDRAM_DATA_BUS_32_BITS			
#define SDRAM_COL_ADDR_BITS		8		

#else
#error Please define KEIL_BOARD or HITEX_BOARD
#endif

// CLK0_DELAY needs to be tuned depending on the external memory bus speed.
// For operation at 102 MHz it should be 6 or 7
#define CLK0_DELAY     7

/* Public Functions ----------------------------------------------------------- */
/** @defgroup EMC_Public_Functions EMC Public Functions
 * @{
 */
EMC_FUNC_CODE EMC_StaMemConfigMW(uint32_t index , uint32_t mem_width);
EMC_FUNC_CODE EMC_StaMemConfigPM(uint32_t index , uint32_t page_mode);
EMC_FUNC_CODE EMC_StaMemConfigPC(uint32_t index , uint32_t pol_val);
EMC_FUNC_CODE EMC_StaMemConfigPB(uint32_t index , uint32_t pb_val);
EMC_FUNC_CODE EMC_StaMemConfigEW(uint32_t index , uint32_t ex_wait);
EMC_FUNC_CODE EMC_StaMemConfigB(uint32_t index , uint32_t buf_val);
EMC_FUNC_CODE EMC_StaMemConfigpP(uint32_t index , uint32_t per_val);
EMC_FUNC_CODE EMC_SetStaMemoryParameter(uint32_t index ,EMC_STA_MEM_PAR par, uint32_t val);

/**
 * @}
 */
 
 
void MemoryPinInit(void);
void EMCFlashInit(void);
void vEMC_InitSRDRAM(uint32_t u32BaseAddr, uint32_t u32Width, uint32_t u32Size, uint32_t u32DataBus, uint32_t u32ColAddrBits);
void emc_WaitUS(volatile uint32_t us);
void emc_WaitMS(uint32_t ms);
EMC_FUNC_CODE StaticMem_Init(EMC_STATIC_MEM_Config_Type* pConfig);

#endif /* __LPC43XX_EMC_H_ */

/**
 * @}
 */
