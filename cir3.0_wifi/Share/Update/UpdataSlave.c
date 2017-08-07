/**********************************************************************
* $Id$      UpdateSlave
* @file     UpdateSlave
* @brief    接收升级数据
* @version  1.0.0
* @date     2016/12/23  11:20:15
* @author   LiMeng
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
***********************************************************************/
#include "UpdataSlave.h"
#include "updatecom.h"
#include "updateapi.h"
#include "update.h"
#include "RTL.h"
#include "md5.h"
#include "string.h"
#include "ucos_ii.h"
#include "lpc43xx.h"
#include "cmddefine.h"
#include "DebugOut.h"
#include "DataTransfer.h"
#include "stdio.h"
#include "math.h"
//#define MEMCMP 1
Bool CreatCheckTack(void);
static  T_FIFEINFO FileInfo_Tran;
static  uint8_t SendBuf[SENDBUFLEN];
static uint8_t CheckSta=0;
static uint8_t Flag=0;
static uint8_t sSport;
OS_EVENT *CheckSem;
OS_STK App_TaskCheckUpdateProcStk[APP_TASKCHECKUPDATE_STK_SIZE];
	   
/*********************************************************************//**
 * @brief
 * @param[in]
 * @return
 **********************************************************************/
Bool AckDectect(uint8_t dPort,uint8_t ch)
{
    uint8_t txbuf[64] = {0};
    return SendDataUpdate(txbuf,sizeof(txbuf),OP_UPDATE,CMD_UPDATE_CHANNEL_DETECT_ACK,GetOwnPort(),dPort,&ch,1,ch);
    //return SendDataUpdate(txbuf,sizeof(txbuf),OP_UPDATE,CMD_UPDATE_DETECT_ACK,GetOwnPort(),dPort,&ch,1,GetMasterCan());
}
/*********************************************************************//**
 * @brief
 * @param[in]
 * @return
 **********************************************************************/
Bool MD5_Check(uint8_t *target_md5,uint32_t padd,uint32_t len)
{
    uint8_t mem[MD5_BUFF_SIZE];
    uint8_t md5[16];
    uint8_t i=0;
    uint16_t j,k;
    float persent;
    float lastPst = 90;
    uint32_t numwrite=0;
    md5_context ctx;

    j= len / MD5_BUFF_SIZE;
    k= len % MD5_BUFF_SIZE;
    md5_starts(&ctx);
    while(j--)
    {
        ReadFlash_Update(padd,(uint32_t *)mem,MD5_BUFF_SIZE);
        numwrite +=	MD5_BUFF_SIZE;
        padd 	 +=	MD5_BUFF_SIZE;
        md5_update(&ctx,mem, MD5_BUFF_SIZE);
		
        CheckSta = CHECK_ING;
        persent = 10.0 * numwrite / len + 90;
        if(persent - lastPst > 0.1)
        {
            ProgressOut(GetOwnPort(),UNENTER,TYPE_PROGRESS,NULL, persent);
            //系统延时，以开始任务调度，保证刷屏
            OSTimeDlyHMSM(0,0,0,1);
            lastPst = persent;
        }
    }
    if(k)
    {
        ReadFlash_Update(padd,(uint32_t *)mem,k);
        md5_update(&ctx,mem, k);
        numwrite += k;
        persent = 10.0 * numwrite / len +90;
        ProgressOut(GetOwnPort(),UNENTER,TYPE_PROGRESS,NULL, persent);
        OSTimeDlyHMSM(0,0,0,1);
//        CheckSta = CHECK_ING;
    }
    md5_finish(&ctx, md5);
    if(memcmp(md5,target_md5,16))
    {
        CheckSta = CHECK_FAILED;
        return FALSE;
    }
    else
    {
        CheckSta = CHECK_SUCCESS;
        return TRUE;
    }
//    return memcmp(md5,target_md5,16) ? FALSE:TRUE;
}
/*********************************************************************//**
 * @brief
 * @param[in]	None
 * @return 		None
 **********************************************************************/
uint8_t SaveFileToFlash(uint32_t offset,const uint8_t *data,uint16_t datalen)
{
    return(WriteFlash_Update(offset + FLASH_DATA_OFFSET_UPDATE,data,datalen));
}
/*********************************************************************//**
 * @brief
 * @param[in]	None
 * @return 		None
 **********************************************************************/
Bool BlockAck(uint8_t dPort,PACK_ACK *ack,uint8_t ch)
{
    return SendDataUpdate(SendBuf,SENDBUFLEN,OP_UPDATE,CMD_UPDATE_FILEDATA_ACK,GetOwnPort(),dPort,(uint8_t *)(ack),sizeof(PACK_ACK),ch);
}
/*********************************************************************//**
 * @brief
 * @param[in]	None
 * @return 		None
 **********************************************************************/
Bool SlaveUpdateReceive(Tran_ptCIR_Frame *pTF,uint8_t ch)
{
    uint8_t err;

    if(pTF != NULL)
    {
        switch(pTF->iCmd)
        {
        case CMD_UPDATE_CHANNEL_DETECT: //???????
        {
            uint8_t ip[]= {NETADDR_IP1,NETADDR_IP2,NETADDR_IP3};
            if(pTF->iDport == GetOwnPort())
            {
                if(strcmp((char *)pTF->pData,DETECT712WORD) != 0)
                    return FALSE;
                //数据来自以太网
                err = !memcmp(pTF->szSaddr,ip,3);
                if(pTF->iDaddrlen == 4 && pTF->iSaddrlen == 4 && err)
                {
#ifdef MMI_ETHERNET_ON
                    //所有单元使用以太网
                    Debug_Update("\nReceive Detect by ethernet");
                    err = AckDectect(pTF->iSport,INTERFACE_LAN);
#else
                    if(pTF->iSport == PORT_MMI1 || pTF->iSport == PORT_MMI2)
                    {
                        //MMI 不使用以太网则立即使用CAN总线
                        Debug_Update("\nReceive Detect by CanBus");
                        err = AckDectect(pTF->iSport,GetMasterCan());
                    }
                    else
                    {
                        //非MMI单元全部使用以太网
                        Debug_Update("\nReceive Detect by ethernet");
                        err = AckDectect(pTF->iSport,INTERFACE_LAN);
                    }
#endif
                }
                else
                {
                    Debug_Update("\nReceive Detect by CanBus");
                    err = AckDectect(pTF->iSport,GetMasterCan());
                }
                if(!err)
                {
                    Debug_Update("Ack detect error");
                    return FALSE;
                }
                Flag = 1;
                return TRUE;
            }
        }
        break;
        case CMD_UPDATE_FILEINFO:
        {
            if(Flag == 1)
            {
                Flag = 2;
                memset(&FileInfo_Tran,0,sizeof(FileInfo_Tran));
                memcpy(&FileInfo_Tran,pTF->pData,pTF->iDataLen);
                if(SendDataUpdate(SendBuf,SENDBUFLEN,OP_UPDATE,CMD_UPDATE_FILEINFO_ACK,GetOwnPort(),pTF->iSport,NULL,0,ch))
                {
                    if(ch == INTERFACE_LAN)
                        ProgressOut(GetOwnPort(),UNENTER,TYPE_STR,"文件传输(以太网)",0);
                    else
                        ProgressOut(GetOwnPort(),UNENTER,TYPE_STR,"文件传输(CAN总线)",0);
                    Debug_Update("%s -> %d bytes %d block %d pack",FileInfo_Tran.filename,FileInfo_Tran.fileSize,FileInfo_Tran.blockNum,FileInfo_Tran.packnum);
                    Flag = 2;
                    return TRUE;
                }
                else
                {
                    Debug_Update("Ack file info error");
                    return FALSE;
                }
            }
        }
        break;
        case CMD_UPDATE_FILEDATA:
        {
            static uint32_t s_lastBlockPack = BLOCKLENGTH -1;
            static uint8_t seqSign=TRUE;
            uint32_t writeCost;
            if(pTF->iDataLen > sizeof(PACKSTR))
                break;
            if(Flag == 2)
            {
                PACKSTR *pPack=(PACKSTR *)(pTF->pData);
                //长度判断,防止超长
                if(pPack->len > PACKSIZE)
                    break;
#ifdef PACKCRC
                if(!CheckCalCRC16((unsigned char *)pPack,sizeof(PACKSTR)))
                {
                    seqSign=FALSE;
                    Debug_Update("%d pack crc error",pPack->packCnt);
                }
                else
#endif
                {
                    //包连续性检查，如果不连续，停止写入，停止ACK
                    err = (pPack->blockPack % BLOCKLENGTH) - ((s_lastBlockPack + 1) % BLOCKLENGTH);
                    if(err != 0)
                    {
                        uint16_t errBlock,errPack;
                        //第X块的第0包
                        if(pPack->blockPack == 0)
                        {
                            if(pPack->blockCnt != 0)
                            {
                                errBlock = pPack->blockCnt-1;
                            }
                            else
                            {
                                errBlock = 0 ;
                            }
                        }
                        else
                        {
                            errBlock = pPack->blockCnt;
                        }
                        errPack =  (pPack->blockPack + BLOCKLENGTH - 1) % BLOCKLENGTH;
                        seqSign=FALSE;
                        Debug_Update("%d block %d pack lost----------------------------------",errBlock,errPack);
                    }
                    s_lastBlockPack = pPack->blockPack;
                    //保存数据到缓冲区
                    if(seqSign)
                    {
                        uint8_t wr = FALSE;
#ifdef WRITECHK
                        uint8_t rd = TRUE;
                        uint8_t cp = FALSE;
                        uint8_t cmpBuf[PACKSIZE];
                        OSSchedLock();
                        wr = SaveFileToFlash(pPack->offset,pPack->data,pPack->len);
                        rd = ReadFlash_Update(pPack->offset + FLASH_DATA_OFFSET_UPDATE,(uint32_t *)cmpBuf,pPack->len);
                        OSSchedUnlock();
                        cp = memcmp(cmpBuf,pPack->data,pPack->len);
                        if(!wr || !rd || (cp != 0))
                            Debug_Update("Write %s ; Read %s ; Cmp %s",wr ? "ok":"err",rd ? "ok":"err",cp?"err":"ok");
#else
                        writeCost = OSTimeGet();
                        OSSchedLock();
                        wr = SaveFileToFlash(pPack->offset,pPack->data,pPack->len);
                        OSSchedUnlock();
                        writeCost = OSTimeGet() - writeCost;
                        if(!wr)
                        {
                            Debug_Update("Write errorxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
                        }
//#ifdef DETAIL_DEBUGUPDATE
//                        else
//                        {
//                            static uint16_t packcnt=0;
//                            if(pPack->packCnt != 0)
//                            {
//                                if(packcnt < pPack->packCnt)
//                                {
//                                    err = pPack->packCnt - packcnt;
//                                    if(err != 1)
//                                    {
//                                        Debug_Update("不连续 lastpack %d nowpack %d**********************************************",packcnt,pPack->packCnt);
//                                    }
//                                }
//                                else
//                                {
//                                    err = packcnt - pPack->packCnt;
//                                    if(err != BLOCKLENGTH -1)
//                                    {
//                                        Debug_Update("不连续 lastpack %d nowpack %d**********************************************",packcnt,pPack->packCnt);
//                                    }
//                                }
//                            }
//                            packcnt = pPack->packCnt;
//                            Debug_Update("%d seq %d pack %d len %d block %d blockpack %d offset %d resend",err,pPack->packCnt,pPack->len,pPack->blockCnt,pPack->blockPack,pPack->offset,pPack->blockSta);
//                        }
//#endif
#endif
                    }
//#ifdef DETAIL_DEBUGUPDATE
//                    //if(seqSign)
//                        Debug_Update("\nRec %d block %d pack.",pPack->blockCnt,pPack->blockPack);
//#endif
                    //每一块或者文件结尾回复
                    if(pPack->blockPack +1 == BLOCKLENGTH || pPack->packCnt +1 == FileInfo_Tran.packnum)
                    {
                        static PACK_ACK packAck;
                        float persent;
                        packAck.blockCnt  = pPack->blockCnt;
                        packAck.packCnt   = pPack->packCnt;
                        packAck.blockPack = pPack->blockPack;
                        if(seqSign)
                        {
                            err= BlockAck(pTF->iSport,&packAck,ch);
//                            if(err != TRUE)
                            {
                                Debug_Update("\nACK %d blocks %dpack %s.",pPack->blockCnt,pPack->packCnt,err ? "success":"false");
                            }
                        }
                        else
                        {
                            seqSign = TRUE;
                            //block错误，重发
                            return FALSE;
                        }
                        //更新进度条 10 ~90%
                        persent = 80.0 * (pPack->packCnt+1) / FileInfo_Tran.packnum +10;
                        ProgressOut(GetOwnPort(),UNENTER,TYPE_PROGRESS,NULL, persent);
						
                        //+1转换为正常计数逻辑
                        if(pPack->packCnt +1 == FileInfo_Tran.packnum)
                        {
                            Debug_Update("\nRec finish  %d blocks %d packs.",pPack->blockCnt,pPack->packCnt);
                            if(CreatCheckTack())
                            {
								sSport = pTF->iSport;
                                OSTimeDlyHMSM(0,0,0,1);
                               OSSemPost(CheckSem);
                            }
                        }
						else
						{
                        Debug_Update("\nRec %d blocks %d times,cost %d ms.",pPack->blockCnt,pPack->blockSta,writeCost);						
						}
                    }
                    return TRUE;
                }
            }
        }
        break;
        case CMD_UPDATE_RESET:
        {
            //if(Flag ==3)
            {
                ResetToBootloader();
            }
        }
        break;
        case CMD_UPDATE_QUE_STATE:
        {
            switch(CheckSta)
            {
            case CHECK_ING:
            {
                SendDataUpdate(SendBuf,SENDBUFLEN,OP_UPDATE,CMD_UPDATE_QUE_STATE_ACK,GetOwnPort(),pTF->iSport,&CheckSta,1,GetMasterCan());
//                OSSchedLock();
                CheckSta = CHECK_NULL;
//                OSSchedUnlock();				
            }
            break;
//            case CHECK_FAILED:
//            case CHECK_SUCCESS:
//            {
//                SendDataUpdate(SendBuf,SENDBUFLEN,OP_UPDATE,CMD_UPDATE_QUE_STATE_ACK,GetOwnPort(),pTF->iSport,&CheckSta,1,GetMasterCan());
//            }
//            break;
            }
            Debug_Update("Rec state query");
        }
        break;
        default:
            break;
        }
    }
}


void App_TaskCheckUpdate(void *p_arg)
{
    uint8_t err;
    while(1)
    {
        OSSemPend(CheckSem,0,&err);
        err = MD5_Check(FileInfo_Tran.md5,FLASH_DATA_OFFSET_UPDATE+16,FileInfo_Tran.fileSize-16);
        if(err)
        {
            SendDataUpdate(SendBuf,SENDBUFLEN,OP_UPDATE,CMD_UPDATE_QUE_STATE_ACK,GetOwnPort(),sSport,&CheckSta,1,GetMasterCan());
			OSTimeDlyHMSM(0,0,0,1);
            OSSchedLock();
            WriteBootConfigArea(FileInfo_Tran.fileSize);
            OSSchedUnlock();
            Flag = 3;
        }
		Debug_Update("Check result: %s",err ? "SUCCESS":"FAILED");		
    }
}
Bool CreatCheckTack(void)
{
    static uint8_t i=0;
    CheckSem = OSSemCreate(0);
    if(i == 0)
    {
        i = OSTaskCreateExt((void (*)(void *)) App_TaskCheckUpdate,
                            (void           *) 0,
                            (OS_STK         *)&App_TaskCheckUpdateProcStk[APP_TASKCHECKUPDATE_STK_SIZE - 1],
                            (INT8U           ) APP_CFG_TASK_CHECKUPDATE_PRIO,
                            (INT16U          ) APP_CFG_TASK_CHECKUPDATE_PRIO,
                            (OS_STK         *)&App_TaskCheckUpdateProcStk[0],
                            (INT32U          ) APP_TASKCHECKUPDATE_STK_SIZE,
                            (void           *) 0,
                            (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

        Debug_Output_Ethernet(GetOwnPort(),"\nUpdateCheck_task create %s,error code 0x%x.",i == OS_ERR_NONE ? "success":"failed" ,i);
        return((i  == OS_ERR_NONE) ? TRUE : FALSE);	//任务创建失败:return 0 i=0,成功：return 1 i!=0;
    }
    return (TRUE);
}
