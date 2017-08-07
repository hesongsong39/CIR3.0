/*
* @brief LPC18xx/43xx CCAN driver
*
* @note
* Copyright(C) NXP Semiconductors, 2012
* All rights reserved.
*
* @par
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* LPC products.  This software is supplied "AS IS" without any warranties of
* any kind, and NXP Semiconductors and its licensor disclaim any and
* all warranties, express or implied, including all implied warranties of
* merchantability, fitness for a particular purpose and non-infringement of
* intellectual property rights.  NXP Semiconductors assumes no responsibility
* or liability for the use of the software, conveys no license or rights under any
* patent, copyright, mask work right, or any other intellectual property rights in
* or to any products. NXP Semiconductors reserves the right to make changes
* in the software without notification. NXP Semiconductors also makes no
* representation or warranty that such application will be suitable for the
* specified use without further testing or modification.
*
* @par
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors' and its
* licensor's relevant copyrights in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
*/

#ifndef __CCAN_H_
#define __CCAN_H_

#include <LPC43xx.H>
#include "lpc_types.h"
#include "lpc43xx_cgu.h"

#ifdef __cplusplus
extern "C" {
#endif
 
typedef struct { 
     __IO uint32_t CMDREQ;   
     __IO uint32_t CMDMSK;   
     __IO uint32_t MSK1;    
     __IO uint32_t MSK2;    
     __IO uint32_t ARB1;    
     __IO uint32_t ARB2;    
     __IO uint32_t MCTRL;   
     __IO uint32_t DA1;    
     __IO uint32_t DA2;    
     __IO uint32_t DB1;    
     __IO uint32_t DB2;    
     __I  uint32_t  RESERVED[13];
    } CCAN_IF_T;

typedef struct {      
     __IO uint32_t CNTL;     
     __IO uint32_t STAT;     
     __I  uint32_t EC;     
     __IO uint32_t BT;     
     __I  uint32_t INT;     
     __IO uint32_t TEST;     
     __IO uint32_t BRPE;     
     __I  uint32_t  RESERVED0;
     CCAN_IF_T IF[2];
     __I  uint32_t  RESERVED2[8];
     __I  uint32_t TXREQ1;    
     __I  uint32_t TXREQ2;    
     __I  uint32_t  RESERVED3[6];
     __I  uint32_t ND1;     
     __I  uint32_t ND2;     
     __I  uint32_t  RESERVED4[6];
     __I  uint32_t IR1;     
     __I  uint32_t IR2;     
     __I  uint32_t  RESERVED5[6];
     __I  uint32_t MSGV1;    
     __I  uint32_t MSGV2;    
     __I  uint32_t  RESERVED6[6];
     __IO uint32_t CLKDIV;    
    } LPC_CCAN_T;
    
/* CCAN Control register bit definitions */
#define CCAN_CTRL_INIT      (1 << 0) 
#define CCAN_CTRL_IE        (1 << 1) 
#define CCAN_CTRL_SIE       (1 << 2) 
#define CCAN_CTRL_EIE       (1 << 3) 
#define CCAN_CTRL_DAR       (1 << 5) 
#define CCAN_CTRL_CCE       (1 << 6) 
#define CCAN_CTRL_TEST      (1 << 7) 
/* CCAN STAT register bit definitions */
#define CCAN_STAT_LEC_MASK  (0x07)  /* Mask for Last Error Code */
#define CCAN_STAT_TXOK      (1 << 3) /* Transmitted a message successfully */
#define CCAN_STAT_RXOK      (1 << 4) /* Received a message successfully */
#define CCAN_STAT_EPASS     (1 << 5) /* The CAN controller is in the error passive state*/
#define CCAN_STAT_EWARN     (1 << 6) /*At least one of the error counters in the EC has reached the error warning limit of 96.*/
#define CCAN_STAT_BOFF      (1 << 7) /*The CAN controller is in busoff state.*/

typedef enum {
		CCAN_LEC_NO_ERROR,  
    CCAN_LEC_STUFF_ERROR, 
    CCAN_LEC_FORM_ERROR, 
    CCAN_LEC_ACK_ERROR,  
    CCAN_LEC_BIT1_ERROR, 
    CCAN_LEC_BIT0_ERROR, 
    CCAN_LEC_CRC_ERROR,  
   } CCAN_LEC_T;
   
/* CCAN INT register bit definitions */
#define CCAN_INT_NO_PENDING       0   
#define CCAN_INT_STATUS           0x8000 
#define CCAN_INT_MSG_NUM(n)       (n)  
/* CCAN TEST register bit definitions */
#define CCAN_TEST_BASIC_MODE      (1 << 2) 
#define CCAN_TEST_SILENT_MODE     (1 << 3) 
#define CCAN_TEST_LOOPBACK_MODE   (1 << 4) 
#define CCAN_TEST_TD_CONTROLLED   (0)  
#define CCAN_TEST_TD_MONITORED    (1 << 5) 
#define CCAN_TEST_TD_DOMINANT     (2 << 5) 
#define CCAN_TEST_TD_RECESSIVE    (3 << 5) 
#define CCAN_TEST_RD_DOMINANT     (0)  
#define CCAN_TEST_RD_RECESSIVE    (1 << 7)  
#define CCAN_SEG1_DEFAULT_VAL 5
#define CCAN_SEG2_DEFAULT_VAL 4
#define CCAN_SJW_DEFAULT_VAL  0
   
typedef enum {
    CCAN_RX_DIR,
    CCAN_TX_DIR,
   } CCAN_TRANSFER_DIR_T;

STATIC INLINE void Chip_CCAN_EnableInt(LPC_CCAN_T *pCCAN, uint32_t mask)
{
		pCCAN->CNTL |= mask;
}
 
STATIC INLINE void Chip_CCAN_DisableInt(LPC_CCAN_T *pCCAN, uint32_t mask)
{
   pCCAN->CNTL &= ~mask;
}
   
STATIC INLINE uint32_t Chip_CCAN_GetIntID(LPC_CCAN_T *pCCAN)
{
  return pCCAN->INT;
}
   
STATIC INLINE uint32_t Chip_CCAN_GetStatus(LPC_CCAN_T *pCCAN)
{
  return pCCAN->STAT;
}
 
STATIC INLINE void Chip_CCAN_SetStatus(LPC_CCAN_T *pCCAN, uint32_t val)
{
  pCCAN->STAT = val & 0x1F;
}
   
void Chip_CCAN_ClearStatus(LPC_CCAN_T *pCCAN, uint32_t val);
 
STATIC INLINE uint8_t Chip_CCAN_GetErrCounter(LPC_CCAN_T *pCCAN, CCAN_TRANSFER_DIR_T dir)
{
  return (dir == CCAN_TX_DIR) ? (pCCAN->EC & 0x0FF) : ((pCCAN->EC >> 8) & 0x0FF);
}
  
STATIC INLINE void Chip_CCAN_EnableTestMode(LPC_CCAN_T *pCCAN)
{
  pCCAN->CNTL |= CCAN_CTRL_TEST;
}
   
STATIC INLINE void Chip_CCAN_DisableTestMode(LPC_CCAN_T *pCCAN)
{
    pCCAN->CNTL &= ~CCAN_CTRL_TEST;
}
   
STATIC INLINE void Chip_CCAN_ConfigTestMode(LPC_CCAN_T *pCCAN, uint32_t cfg)
{
  pCCAN->TEST = cfg;
}
   
STATIC INLINE void Chip_CCAN_EnableAutoRetransmit(LPC_CCAN_T *pCCAN)
{
   pCCAN->CNTL &= ~CCAN_CTRL_DAR;
}
   
STATIC INLINE void Chip_CCAN_DisableAutoRetransmit(LPC_CCAN_T *pCCAN)
{
   pCCAN->CNTL |= CCAN_CTRL_DAR;
}

STATIC INLINE uint32_t Chip_CCAN_GetTxRQST(LPC_CCAN_T *pCCAN)
{
    return pCCAN->TXREQ1 | (pCCAN->TXREQ2 << 16);
}
   
void Chip_CCAN_Init(LPC_CCAN_T *pCCAN);

void Chip_CCAN_DeInit(LPC_CCAN_T *pCCAN);
   
 Status Chip_CCAN_SetBitRate(LPC_CCAN_T *pCCAN, uint32_t pClk,uint32_t bitRate);
   
#define CCAN_MSG_MAX_NUM                              32
   
typedef struct {
    uint32_t    id;  
    uint32_t    dlc; 
    uint8_t     data[8]; 
   } CCAN_MSG_OBJ_T;
   
typedef enum {
    CCAN_MSG_IF1 = 0,
    CCAN_MSG_IF2 = 1,
   } CCAN_MSG_IF_T;
   
/* bit field of IF command request n register */
#define CCAN_IF_CMDREQ_MSG_NUM(n)  (n)   /* Message number (1->20) */
#define CCAN_IF_CMDREQ_BUSY          0x8000   /* 1 is writing is progress, cleared when RD/WR done */
   
/* bit field of IF command mask register */
#define CCAN_IF_CMDMSK_DATAB        (1 << 0)  
#define CCAN_IF_CMDMSK_DATAA        (1 << 1)  
#define CCAN_IF_CMDMSK_W_TXRQST     (1 << 2)  
#define CCAN_IF_CMDMSK_R_NEWDAT     (1 << 2)  
#define CCAN_IF_CMDMSK_R_CLRINTPND  (1 << 3)  
#define CCAN_IF_CMDMSK_CTRL         (1 << 4)  
#define CCAN_IF_CMDMSK_ARB          (1 << 5)  
#define CCAN_IF_CMDMSK_MASK         (1 << 6)  
#define CCAN_IF_CMDMSK_WR           (1 << 7)  /*  Tranfer direction: Write */
#define CCAN_IF_CMDMSK_RD           (0)    /*  Tranfer direction: Read */
#define CCAN_IF_CMDMSK_TRANSFER_ALL (CCAN_IF_CMDMSK_CTRL | CCAN_IF_CMDMSK_MASK | CCAN_IF_CMDMSK_ARB | \
             CCAN_IF_CMDMSK_DATAB | CCAN_IF_CMDMSK_DATAA)
   
/* bit field of IF mask 2 register */
#define CCAN_IF_MASK2_MXTD          (1 << 15)    /* 1 is extended identifier bit is used in the RX filter unit, 0 is not */
#define CCAN_IF_MASK2_MDIR(n)       (((n) & 0x01) <<  14) /* 1 is direction bit is used in the RX filter unit, 0 is not */
   
/* bit field of IF arbitration 2 register */
#define CCAN_IF_ARB2_DIR(n)         (((n) & 0x01) << 13) /* 1: Dir = transmit, 0: Dir = receive */
#define CCAN_IF_ARB2_XTD            (1 << 14)  /* Extended identifier bit is used*/
#define CCAN_IF_ARB2_MSGVAL         (1 << 15)  /* Message valid bit, 1 is valid in the MO handler, 0 is ignored */
   
/* bit field of IF message control register */
#define CCAN_IF_MCTRL_DLC_MSK        0x000F   /* bit mask for DLC */
#define CCAN_IF_MCTRL_EOB           (1 << 7)  /* End of buffer, always write to 1 */
#define CCAN_IF_MCTRL_TXRQ          (1 << 8)  /* 1 is TxRqst enabled */
#define CCAN_IF_MCTRL_RMTEN(n)      (((n) & 1UL) << 9) /* 1 is remote frame enabled */
#define CCAN_IF_MCTRL_RXIE          (1 << 10)  /* 1 is RX interrupt enabled */
#define CCAN_IF_MCTRL_TXIE          (1 << 11)  /* 1 is TX interrupt enabled */
#define CCAN_IF_MCTRL_UMSK          (1 << 12)  /* 1 is to use the mask for the receive filter mask. */
#define CCAN_IF_MCTRL_INTP          (1 << 13)  /* 1 indicates message object is an interrupt source */
#define CCAN_IF_MCTRL_MLST          (1 << 14)  /* 1 indicates a message loss. */
#define CCAN_IF_MCTRL_NEWD          (1 << 15)  /* 1 indicates new data is in the message buffer.  */
  
#define CCAN_MSG_ID_STD_MASK        0x07FF
#define CCAN_MSG_ID_EXT_MASK        0x1FFFFFFF
   
void Chip_CCAN_TransferMsgObject(LPC_CCAN_T *pCCAN,
            CCAN_MSG_IF_T IFSel,
            uint32_t mask,
            uint32_t msgNum);
   
void Chip_CCAN_SetMsgObject (LPC_CCAN_T *pCCAN,
           CCAN_MSG_IF_T IFSel,
           CCAN_TRANSFER_DIR_T dir,
           Bool remoteFrame,
					 Bool EOBstate,
           uint8_t msgNum,
           const CCAN_MSG_OBJ_T *pMsgObj,
           uint32_t MaskBit);
           
   
void Chip_CCAN_GetMsgObject(LPC_CCAN_T *pCCAN, CCAN_MSG_IF_T IFSel, uint8_t msgNum, CCAN_MSG_OBJ_T *pMsgObj);
  
void Chip_CCAN_SetValidMsg(LPC_CCAN_T *pCCAN, CCAN_MSG_IF_T IFSel, uint8_t msgNum, Bool valid);
   
STATIC INLINE uint32_t Chip_CCAN_GetValidMsg(LPC_CCAN_T *pCCAN)
{
   return pCCAN->MSGV1 | (pCCAN->MSGV2 << 16);
}

INLINE uint32_t Chip_CCAN_GetInterruptMsg(LPC_CCAN_T *pCCAN)
{
   return pCCAN->IR1 | (pCCAN->IR2 << 16);
}

   
STATIC INLINE void Chip_CCAN_ClearMsgIntPend(LPC_CCAN_T *pCCAN,
               CCAN_MSG_IF_T IFSel,
               uint8_t msgNum,
               CCAN_TRANSFER_DIR_T dir)
{
    Chip_CCAN_TransferMsgObject(pCCAN, IFSel, CCAN_IF_CMDMSK_RD | CCAN_IF_CMDMSK_R_CLRINTPND, msgNum);
}
   
INLINE void Chip_CCAN_ClearNewDataFlag(LPC_CCAN_T *pCCAN, CCAN_MSG_IF_T IFSel, uint8_t msgNum)
{
    Chip_CCAN_TransferMsgObject(pCCAN, IFSel, CCAN_IF_CMDMSK_RD | CCAN_IF_CMDMSK_R_NEWDAT, msgNum);
}
   
void Chip_CCAN_Send (LPC_CCAN_T *pCCAN, CCAN_MSG_IF_T IFSel, Bool remoteFrame, CCAN_MSG_OBJ_T *pMsgObj);
  
void Chip_CCAN_AddReceiveID(LPC_CCAN_T *pCCAN, CCAN_MSG_IF_T IFSel, uint32_t id ,Bool EOBstate);
  
void Chip_CCAN_DeleteReceiveID(LPC_CCAN_T *pCCAN, CCAN_MSG_IF_T IFSel, uint32_t id);

uint8_t getFreeMsgObject(LPC_CCAN_T *pCCAN);
void configTimming(LPC_CCAN_T *pCCAN,uint32_t ClkDiv,uint32_t BaudRatePrescaler,uint8_t SynJumpWidth,uint8_t Tseg1,uint8_t Tseg2);
void freeMsgObject(LPC_CCAN_T *pCCAN, CCAN_MSG_IF_T IFSel, uint8_t msgNum);
void Chip_CCAN_ClearStatus(LPC_CCAN_T *pCCAN, uint32_t val);

 
#ifdef __cplusplus
   }
#endif
   
#endif /* __CCAN_18XX_43XX_H_ */


