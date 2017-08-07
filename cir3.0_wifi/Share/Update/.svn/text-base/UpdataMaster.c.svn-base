/**********************************************************************
* $Id$      App
* @file
* @brief
* @version  1.0.0
* @date     2016/12/18  11:20:15
* @author   LiMeng
* Copyright(C) 2015, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
***********************************************************************/
#include "UpdataMaster.h"
#include "update.h"
#include "updatecom.h"
#include "md5.h"
#include "crc.h"
#include "string.h"
#include "DebugOut.h"
#include "updateapi.h"
#include "stdio.h"
#include "cmddefine.h"
#include "DataTransfer.h"


#pragma pack(1)
typedef struct
{
    uint8_t data[SENDBUFLEN];
    uint16_t len;
} PACKDATA;
typedef struct
{
    uint8_t sta;
    uint32_t cnt;
} TIMER;
#pragma pack()
static  TRAN_CTRL 	TranCtl;
static  PACKDATA 	PackData;
static  T_FIFEINFO 	FileInfo;
static 	FILE *fp_Tran;
static 	char 	AuthFileName[MAXFILENUM][FILE_NAME_LENGTH];
//static 	uint8_t AuthNum				= 0;
static  uint8_t UpdateFileNum   	= 0;
static 	uint8_t Flag				= 0;
static 	uint8_t SendFileNum			= 0;
static 	uint8_t SendBuf[SENDBUFLEN]	= {0};
static  char  	LocalName[16];
//static uint16_t cnt2=0;
static uint8_t  g_rcv = 0;

#ifdef USETIMER_UPDATE
OS_TMR *TimerDetChannel;
OS_TMR *TimerFileInfo;
OS_TMR *TimerSendPack;
OS_TMR *TimerUpdate;
OS_TMR *TimerSta;
#endif
/*********************************************************************//**
 * @brief
 * @param[in]
 * @return 		None
 **********************************************************************/
Bool DectectProduct(uint8_t dPort,uint8_t ch)
{
    uint8_t txbuf[64] = {0};
    uint8_t data[]=DETECT712WORD;
//    return CCANDataSimpleSendPost(txbuf,sizeof(txbuf),INTERFACE_CAN1,OP_UPDATE,CMD_UPDATE_DETECT,GetOwnPort(),dPort,NULL,0,FALSE);
    return SendDataUpdate(txbuf,sizeof(txbuf),OP_UPDATE,CMD_UPDATE_CHANNEL_DETECT,GetOwnPort(),dPort,data,sizeof(data),ch);
}
/*********************************************************************//**
 * @brief
 * @param[in]
 * @return 		None
 **********************************************************************/
Bool UpdateMode(uint8_t mode)
{
    uint8_t txbuf[64] = {0};
//    ModeSetMMIUpdate(&mode,1);
    return CCANDataSimpleSendPost(txbuf,sizeof(txbuf),INTERFACE_CAN1,OP_UPDATE,CMD_UPDATE_MODECTRL,GetOwnPort(),PORT_BROADCAST,&mode,1,FALSE);
}
Bool ResetAllUpdate(void)
{
    uint8_t txbuf[64] = {0};
    return	SendDataUpdate(txbuf,sizeof(txbuf),OP_UPDATE,CMD_UPDATE_RESET,GetOwnPort(),PORT_BROADCAST,NULL,0,GetMasterCan());
}
/*********************************************************************//**
 * @brief
 * @param[in]	None
 * @return 		None
 **********************************************************************/
Bool MD5_Code_Update(char *pFileName, char *pMD5Arr)
{
    FILE *pF;
    md5_context ctx;
    uint8_t buff[MD5_BUFF_SIZE];
    uint32_t read;
    uint32_t sum=0;
    uint32_t size=0;
    float persent;
    float lastPst = 0;
    pF = fopen(pFileName, "rb");

    if(pF == NULL)
    {
        return FALSE;
    }

//	fseek(pF,0,SEEK_SET);
    fseek(pF,0,SEEK_END);
    size  = ftell(pF);
    fseek(pF,0,SEEK_SET);

    md5_starts(&ctx);
    fseek(pF,16,SEEK_SET);	//???????????
    while(!feof(pF))
    {
        //memset(buff, 0, sizeof(MD5_BUFF_SIZE));
        read = fread(buff, 1, MD5_BUFF_SIZE, pF);
        md5_update(&ctx, buff, read);
        sum += read;
        //0~10%
        persent = 10.0 * sum / size;
        if(persent - lastPst > PRGRS_STEP)
        {
            ProgressOut(TranCtl.port,UNENTER,TYPE_PROGRESS,NULL, persent);
            OSTimeDlyHMSM(0,0,0,1);
            lastPst = persent;
        }
    }
    md5_finish(&ctx,(uint8_t *)pMD5Arr);
    fclose(pF);
}
/*********************************************************************//**
 * @brief      U-Disk Task: U-Disk proc Task
 * @param[in]  p_arg  Argument passed to 'App_TaskUDisk()' by 'OSTaskCreate()'
 * @return     None
 **********************************************************************/
Bool Fileauth(char *file)
{
#if 1
    uint8_t i;
    FILE *fp;
    char md5_New[16],md5_Target[16];
    char FilePath[FILE_NAME_LENGTH + sizeof(DEFAULT_PATH)+1] = DEFAULT_PATH;
    //文件名+路径
    strcat(FilePath,file);
    MD5_Code_Update(FilePath,md5_New);
    fp = fopen(FilePath,"rb"); //open file ,only read binary mode
    if(fp != NULL )
    {
        //文件开头的16个字节是该文件的MD5
        i= fread(md5_Target,sizeof(char),16,fp); //???????16???
        fclose(fp);
        if(i == 16)	//????
        {
            return(memcmp(md5_Target,md5_New,16) ? FALSE : TRUE);
        }
        return FALSE;
    }
    return FALSE;
#else
    return TRUE;
#endif
}

/*********************************************************************//**
 * @brief
 * @param[in]	name , jp
 * @return 		FALSE,TRUE
 **********************************************************************/
Bool GetJPByName(char *name,char *jp)
{
    char *add;
    if(name == NULL || jp == NULL)
        return FALSE;

    add = strstr((char *)name,"_");
    if((add != NULL) && (add - name  > 0))
    {
        memcpy(jp,name,(add-name));
        return TRUE;
    }
    return FALSE;
}
/*********************************************************************//**
 * @brief			通过文件名获取目标IP,port
 * @param[in]	None
 * @return 		None
 **********************************************************************/
uint8_t GetTatgetIP(char *target,uint8_t *ip,uint8_t *port)
{
    char UnitJP[UNIT_JP_LEN_MAX]= {0};
    if(target == NULL)
        return FALSE;
    if(GetJPByName(target,UnitJP) == FALSE)
    {
        Debug_Output_Ethernet(GetOwnPort(),"\nFile name :%s format error.",target);
        return FALSE;
    }
    else if(GetCanNetAddrByUnitJP((CHAR *)UnitJP,port,ip) == FALSE)
    {
        Debug_Output_Ethernet(GetOwnPort(),"\nCan't get add by: %s",target);
        return FALSE;
    }
    return TRUE;
}
/*********************************************************************//**
 * @brief
 * @param[in]	None
 * @return 		None
 **********************************************************************/
uint32_t UpdataLocal(char *name)
{
    FILE *fp;
    uint32_t numread=0;
    uint32_t numwrite=0;
    uint32_t add;
    uint32_t fileSize;
    char buffer[PACKSIZE];
    char FilePath[FILE_NAME_LENGTH + sizeof(DEFAULT_PATH)] = DEFAULT_PATH;
    float persent;
    float lastpst = 0.0;
    strcat(FilePath,name);		//????????,fopen????????,???????
    fp = fopen(FilePath,"rb");
    if(fp != NULL)
    {
        fseek(fp,0,SEEK_END);
        fileSize  = ftell(fp);										//????????
        fseek(fp,0,SEEK_SET);
        add= FLASH_DATA_OFFSET_UPDATE;
        while(!feof(fp))
        {
            numread = fread(buffer, sizeof(char), PACKSIZE, fp);
            if(numread == 0)
            {
                break;           /* ??U??????? */
            }
            memset((uint8_t *)buffer+numread, 0xFF, PACKSIZE-numread);    /* ?0xFF */
            OSSchedLock();
            /* Write */
            if (WriteFlash_Update(add, (uint8_t *)buffer, numread) != TRUE)
            {
                break;           /* ??,??????? */
            }
            OSSchedUnlock();
            add += numread;
            numwrite +=numread;
            //10%~100%
            persent = 90.0 * numwrite / fileSize + 10;
            if(persent - lastpst > PRGRS_STEP || persent >= 100)
            {
                ProgressOut(GetOwnPort(),UNENTER,TYPE_PROGRESS,NULL, persent);
                //系统延时，以开始任务调度，保证刷屏
                OSTimeDlyHMSM(0,0,0,1);
                lastpst = persent;
            }
        }
        fclose(fp);
        return ((fileSize == numwrite) ? fileSize : FALSE);
    }
    return FALSE;
}
/*********************************************************************//**
 * @brief		判断是否用于自身升级
 * @param[in]	None
 * @return 		None
 **********************************************************************/
Bool JudgeFile(char *name)
{
    char port;
    char UnitJP[UNIT_JP_LEN_MAX]= {0};
    if(name == NULL)
        return FALSE;
    if(GetJPByName(name,UnitJP))
    {
        if(GetPortByUnitJP((CHAR *)UnitJP,&port))
        {
            if(port == GetOwnPort())
                return TRUE;
        }
    }
    return FALSE;
}
/*********************************************************************//**
 * @brief
 * @param[in]
 * @return
 **********************************************************************/
void UpdataOrderAdjust(char (*file)[FILE_NAME_LENGTH],uint8_t num)
{
}
/*********************************************************************//**
 * @brief
 * @param[in]	None
 * @return 		None
 **********************************************************************/
uint8_t CheckFileUpdate(void)
{
    int8_t FileNum=0;
    FINFO info;
    uint8_t err;
    char  path[FILE_NAME_LENGTH] = DEFAULT_PATH;
    info.fileID = 0;
    strcat(path,UPDATE_FILETYPE);
    //在U0:\\UPDATA\\下搜索*.cir3文件
    while(ffind(path,&info) == 0) //
    {
        Debug_Update("\nFind update file: %s.",info.name);
        memcpy(AuthFileName[FileNum],info.name,strlen(info.name) +1);
        FileNum++;
    }
    if(FileNum > 0)	//
    {
        //调整升级顺序
        UpdataOrderAdjust(AuthFileName,FileNum);
        UpdateFileNum = FileNum;
        Flag = STA_START;
        UpdateMode(ENTRY_UPDATE_MODE);
        g_rcv = TRUE;
        return TRUE;
    }
    else
    {
        Debug_Update("\nNo valid file.");
    }
    return FALSE;
}
/*********************************************************************//**
 * @brief
 * @param[in]	None
 * @return 		None
 **********************************************************************/
Bool SendFileInfo(char *file,char *buff,uint16_t buff_size, PACKDATA *data)
{
    T_FIFEINFO *pData;
    char   FilePath[FILE_NAME_LENGTH + sizeof(DEFAULT_PATH)] = DEFAULT_PATH;
    pData  = (T_FIFEINFO *)(data->data);
    //防止文件名超长
    if(strlen(file)+1 > FILE_NAME_LENGTH)
    {
        return FALSE;
    }
    //防止多次重发，多次打开文件
    if(fp_Tran == NULL)
    {
        //文件全路径 :U0:\update\XXX.CIR3
        strcat(FilePath,file);
        fp_Tran = fopen(FilePath,"rb");
    }
    if(fp_Tran  != NULL)
    {
        memcpy(pData->filename,file,strlen(file)+1);
        //获取文件大小
        fseek(fp_Tran,0,SEEK_SET);
        fseek(fp_Tran,0,SEEK_END);
        pData->fileSize  = ftell(fp_Tran);
        fseek(fp_Tran,0,SEEK_SET);
        //文件前16个字节是该文件的MD5值
        if(fread(pData->md5,sizeof(uint8_t),16,fp_Tran)==16)
        {
            //恢复指针到文件起始位置
            fseek(fp_Tran,0,SEEK_SET);
            //包数量
            pData->packnum = pData->fileSize / PACKSIZE + (pData->fileSize % PACKSIZE ? 1:0);
            pData->packSize = PACKSIZE;		//包大小 512
            pData->bockSize = BLOCKSIZE;	//块大小 4K
            //块数量
            pData->blockNum = pData->packnum / BLOCKLENGTH + (pData->packnum % BLOCKLENGTH ? 1:0);
//            pData->ch = TranCtl.ch;
//            memcpy(pData->unitName,TranCtl.unitName,strlen(TranCtl.unitName));
            data->len = sizeof(T_FIFEINFO);
            return SendDataUpdate((uint8_t *)buff,buff_size,OP_UPDATE,CMD_UPDATE_FILEINFO,GetOwnPort(),TranCtl.port,(uint8_t *)(data->data),data->len,TranCtl.ch);
        }
        return FALSE;
    }
    return FALSE;
}
/*********************************************************************//**
 * @brief
 * @param[in]	None
 * @return 		None
 **********************************************************************/
uint16_t SendPack(char *buff,uint16_t buff_size, PACKDATA *data)
{
    uint16_t numread = 0;
    PACKSTR  *pData;
    uint16_t crc;
    pData = ( PACKSTR *)(data->data);  //只对数据操作
    if(!feof(fp_Tran))
    {
        pData->offset = ftell(fp_Tran);	//获取当前文件指针偏移
        numread = fread(pData->data, sizeof(uint8_t), PACKSIZE, fp_Tran);
        if(numread > 0)
        {
            pData->len 	     = numread;
            pData->packCnt   = pData->offset / PACKSIZE ;// + (pData->offset % PACKSIZE  ? 1 : 0);
            pData->blockCnt  = pData->offset / BLOCKSIZE;
//			if(pData->blockCnt)
//			{
//				pData->blockCnt += (pData->offset % BLOCKSIZE ? 1 : 0);
//			}
            pData->blockPack = pData->packCnt % BLOCKLENGTH;
            //首次发送
            if(pData->blockCnt != TranCtl.reBlock)
            {
                TranCtl.packSta  = 0;
                TranCtl.blockSta = 0;
                TranCtl.reBlockSta = 0;
//                pData->packSta	 = 0;
                pData->blockSta  = 0;
            }
            else
            {
                //重发
//                pData->packSta  = TranCtl.packSta;
                pData->blockSta = TranCtl.blockSta;
            }
#ifdef PACKCRC
            crc = (uint16_t )CalCRC16((unsigned char *)pData,sizeof(PACKSTR)-2);
            pData->crc = crc >>8; //高位
            pData->crc += crc<<8; //地位
#endif
//            if(pData->blockCnt != 355)
            {
                data->len = sizeof( PACKSTR);
                if(SendDataUpdate((uint8_t *)buff,buff_size,OP_UPDATE,CMD_UPDATE_FILEDATA,GetOwnPort(),TranCtl.port,(uint8_t *)data,data->len,TranCtl.ch))
                {
                    TranCtl.blockCnt  = pData->blockCnt;
                    TranCtl.blockPack = pData->blockPack;
                    TranCtl.packCnt   = pData->packCnt;
                    TranCtl.offset    = pData->offset;
                    return  pData->len;
                }
                else
                {

                }
            }
//			else
//			{
//                Debug_Update("discard 355 block discard");		
//					return TRUE;
//			}
        }
        return FALSE;
    }
    return FALSE;
}
/*********************************************************************//**
 * @brief
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void ExtTime(void)
{

}
#ifdef USETIMER_UPDATE
/*********************************************************************//**
 * @brief
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void TimerDetBusCallback (OS_TMR *ptmr, void *p_arg)
{
    static uint16_t cnt=0;
    if(Flag == STA_WAITNETDETACK)
    {
        cnt++;
        Flag = STA_DETETHERNET;
        if(cnt > RETRY_TIMES)
        {
            cnt = 0;
            //通过以太网探测失败，则通过CAN探测
            Flag = STA_DETCAN;
        }
    }
    else
    {
        if(Flag == STA_WAITCANDETACK)
        {
            cnt++;
            Flag = STA_DETCAN;
            if(cnt > RETRY_TIMES)
            {
                cnt = 0;
                //通过CAN探测失败，则发送下一个文件
                ProgressOut(TranCtl.port,UNENTER,TYPE_STR,"目标未响应",0);
                Flag = STA_GETFILENAME;
                SendFileNum++;
            }
        }
    }
}
/*********************************************************************//**
 * @brief
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void TimerFileInfoCallback(OS_TMR *ptmr,void *p_arg)
{
    static uint16_t cnt=0;
    if(Flag == STA_WAITFILEINFOACK)
    {
        cnt++;
        Flag = STA_SENDFILEINFO;
        if(cnt > RETRY_TIMES)
        {
            cnt = 0;
            //发送文件信息失败，则开始发送下一个文件
            Flag = STA_GETFILENAME;
            SendFileNum++;
        }
    }
}
/*********************************************************************//**
 * @brief
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void TimerSendPackCallback(OS_TMR *ptmr,void *p_arg)
{
    //static uint16_t block;
    if(Flag == STA_WAITBLOCKACK)
    {
        Flag = STA_RESENDBLOCK;
    }
}
/*********************************************************************//**
 * @brief
 * @param[in]	None
 * @return 		None
 **********************************************************************/
//void TimerUpdateCallback(OS_TMR *ptmr,void *p_arg)
//{
//    static uint8_t cntFileInfo=0;
//    static uint8_t cntBlock=0;
//    static uint8_t cntDetCan=0;
//    static uint8_t cntDetEthernet=0;
//    switch(Flag)
//    {
//    case STA_WAITBLOCKACK:
//    {
//        static uint16_t block=0;
//        if(block != TranCtl.blockCnt)
//        {
//            cntBlock = 0;
//        }
//        else
//        {
//            cntBlock ++;
//            if(cntBlock > RETRY_TIMES)
//            {
//                cntBlock = 0;
//                block	 = 0;
//                Flag 	 = STA_GETFILENAME;
//                SendFileNum++;
//            }
//        }
//        block = TranCtl.blockCnt;
//    }
//    break;
//    case STA_WAITFILEINFOACK:
//    {
//        cntFileInfo++;
//        Flag = STA_SENDFILEINFO;
//        if(cntFileInfo > RETRY_TIMES)
//        {
//            cntFileInfo = 0;
//            //发送文件信息失败，则开始发送下一个文件
//            Flag = STA_GETFILENAME;
//            SendFileNum++;
//        }
//    }
//    break;
//    case STA_WAITCANDETACK:
//    {
//        cntDetCan++;
//        Flag = STA_DETCAN;
//        if(cntDetCan > RETRY_TIMES)
//        {
//            cntDetCan = 0;
//            //通过CAN探测失败，则发送下一个文件
//            Flag = STA_GETFILENAME;
//            SendFileNum++;
//        }
//    }
//    break;
//    case STA_WAITNETDETACK:
//    {
//        cntDetEthernet++;
//        Flag = STA_DETETHERNET;
//        if(cntDetEthernet > RETRY_TIMES)
//        {
//            cntDetEthernet = 0;
//            //通过以太网探测失败，则通过CAN探测
//            Flag = STA_DETCAN;
//        }
//    }
//    break;
//    default:
//        break;
//    }
//}
void TimerStaCallback(OS_TMR *ptmr,void *p_arg)
{
    uint8_t err;
    static uint8_t cnt;
    if(cnt ++ > RETRY_TIMES)
    {
        OSTmrStop(TimerSta,OS_TMR_OPT_NONE,0,&err);
        ProgressOut(TranCtl.port,UNENTER,TYPE_STR,"超时，校验失败",0);
        Debug_Update("超时，校验失败");
        SendFileNum ++;
        Flag = STA_GETFILENAME;
        cnt =0;
    }
}
/*********************************************************************//**
 * @brief
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void CreatTimerUpdate()
{
    INT8U err;
    INT32U period;

    period	= DETECT_TIMEOUT * 1.0 /1000 * OS_TMR_CFG_TICKS_PER_SEC;
    TimerDetChannel = OSTmrCreate(0,period,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)TimerDetBusCallback  ,(INT8U *)0,(INT8U *)"TIMERDET",&err);

    period  = FILEINFO_TIMEOUT * 1.0 /1000 * OS_TMR_CFG_TICKS_PER_SEC;
    TimerFileInfo	= OSTmrCreate(0,period,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)TimerFileInfoCallback,(INT8U *)0,(INT8U *)"TIMEFILEINFO",&err);

    period  = SENDPACK_TIMEOUT * 1.0 /1000 * OS_TMR_CFG_TICKS_PER_SEC;
    TimerSendPack	= OSTmrCreate(0,period,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)TimerSendPackCallback,(INT8U *)0,(INT8U *)"TIMERSENDPACK",&err);

    period  = CHECK_STA_TIMEOUT * 1.0 /1000 * OS_TMR_CFG_TICKS_PER_SEC;
    TimerSta     = OSTmrCreate(0,period,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)TimerStaCallback,(INT8U *)0,"TIMERUPDATE",&err);
}
#endif
/*********************************************************************//**
 * @brief
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void MasterUpdateTran(void)
{
    uint8_t err;
    uint8_t enter=0;
    switch(Flag)
    {
    case STA_START:	//
    {
        SendFileNum =0;
//        memset(&TranCtl,0,sizeof(TranCtl));
        Flag = STA_GETFILENAME;
        GetOwnName(LocalName);
        CreatTimerUpdate();
    }
    break;
    case STA_GETFILENAME:	//获取待传输文件的文件名
    {
        if(SendFileNum < UpdateFileNum)
        {
            Debug_Update("Start send %s",AuthFileName[SendFileNum]);
            memset(&TranCtl ,0,sizeof(TranCtl));
            memset(&FileInfo,0,sizeof(FileInfo));
            //通过文件名获取目标IP,port
            if(GetTatgetIP(AuthFileName[SendFileNum],&TranCtl.ip,&TranCtl.port) == FALSE)
            {
                Debug_Update("Filename  format error");
                Flag = STA_GETFILENAME;
                SendFileNum++;
                break;
            }
            GetUnitJPByPort(TranCtl.port,TranCtl.unitName);
            //第一个文件时不换行
            enter = SendFileNum ? ENTER : UNENTER;
            ProgressOut(TranCtl.port,enter,TYPE_STR,"开始鉴权",0);
            if(Fileauth(AuthFileName[SendFileNum]))
            {
                ProgressOut(TranCtl.port,UNENTER,TYPE_STR,"鉴权通过",0);
                Debug_Update("\n%s auth success",AuthFileName[SendFileNum]);

                memcpy(TranCtl.filename,AuthFileName[SendFileNum],strlen((char *)AuthFileName[SendFileNum])+1);
                if(JudgeFile(AuthFileName[SendFileNum]))
                {
                    int32_t filesize;
                    ProgressOut(TranCtl.port,UNENTER,TYPE_STR,"文件传输(U盘)",0);
                    filesize=UpdataLocal(AuthFileName[SendFileNum]); //式財芎
                    if(filesize)
                    {
                        ProgressOut(TranCtl.port,UNENTER,TYPE_STR,"文件传输成功",0);
                        OSSchedLock();
                        WriteBootConfigArea(filesize);
                        OSSchedUnlock();
                    }
                    Flag = STA_GETFILENAME;
                    SendFileNum++;
                }
                else
                {
                    if( fp_Tran != NULL)
                    {
                        fclose(fp_Tran);
                        fp_Tran = NULL;
                    }
                    Flag = STA_DETETHERNET;
                }
            }
            else
            {
                ProgressOut(TranCtl.port,UNENTER,TYPE_STR,"鉴权未通过",0);
                Debug_Update("\n%s auth fail",AuthFileName[SendFileNum]);
                SendFileNum ++;
            }
        }
        else if(SendFileNum  >= UpdateFileNum)
        {
            static int16_t cnter=0;
            g_rcv = FALSE;
            if(cnter * STEP_UPDATE % 1000 == 0)
            {
                char str[64];
                uint16_t persent;
                //
                persent = abs(UDISK_QUITTIME  - cnter * STEP_UPDATE) / 1000;
                sprintf(str,"请拔出U盘完成升级，%d秒后重启",persent);
                enter = cnter ? UNENTER : ENTER;
                ProgressOut(GetOwnPort(),enter,TYPE_STR,str,0);
            }
            //加100 确保最后一次进度发送出去
            if(cnter > UDISK_QUITTIME +100)
            {
                cnter = 0;
                SendDataUpdate(SendBuf,SENDBUFLEN,OP_UPDATE,CMD_UPDATE_RESET,GetOwnPort(),PORT_BROADCAST,NULL,0,GetMasterCan());
                //适当延时，确保命令发送出去
                OSTimeDlyHMSM(0,0,0,50);
                ResetToBootloader();
                Flag = STA_NULL;
            }
            cnter++;
        }
    }
    break;
    case STA_DETCAN:
    {
        //通过CAN总线探测
        err = DectectProduct(TranCtl.port,GetMasterCan());
        if(err)
        {
            OSTmrStart(TimerDetChannel,&err);
            Flag = STA_WAITCANDETACK;
            ProgressOut(TranCtl.port,UNENTER,TYPE_STR,"CAN通道检测",0);
            Debug_Update("\n%s Detect %s by CAN Bus",LocalName,TranCtl.unitName);
        }
        else
        {
            Debug_Update("\n%s Send Detect error",LocalName);
        }
    }
    break;
    case STA_DETETHERNET:
    {
        //通过网络
        err = DectectProduct(TranCtl.port,INTERFACE_LAN);
        if(err)
        {
            OSTmrStart(TimerDetChannel,&err);
            Flag = STA_WAITNETDETACK;
            ProgressOut(TranCtl.port,UNENTER,TYPE_STR,"网络通道检测",0);
            Debug_Update("\n%s Detect %s by Ethernet",LocalName,TranCtl.unitName);
        }
        else
        {
            Debug_Update("\n%sSend Detect error",LocalName);
        }
    }
    break;
    case STA_SENDFILEINFO: //发送文件信息
    {
        err = SendFileInfo(AuthFileName[SendFileNum],(char *)SendBuf,SENDBUFLEN,&PackData);
        if(err)
        {
            Flag = STA_WAITFILEINFOACK;	//
            memcpy(&FileInfo,PackData.data,sizeof(FileInfo));
            Debug_Update("\n%s:Send %s infomation by %s.",LocalName,AuthFileName[SendFileNum],TranCtl.ch== INTERFACE_LAN ? "Ethernet":"CAN Bus");
            OSTmrStart(TimerFileInfo,&err);
        }
    }
    break;
    case STA_SENDBLOCK:	//发送文件包
    {
        uint16_t packSta;
        packSta = SendPack((char *)SendBuf,SENDBUFLEN,&PackData);
        if( packSta!= FALSE)
        {
            //flash在首包的位置会进行擦除，耗时大
            if(TranCtl.blockPack ==0)
            {
                OSTimeDlyHMSM(0,0,0,50);
            }
            if(TranCtl.ch != INTERFACE_LAN)
                OSTimeDlyHMSM(0,0,0,2);
            if(TranCtl.blockPack+1 == BLOCKLENGTH || TranCtl.packCnt+1 == FileInfo.packnum)
            {
				OSSchedLock();
                //等待回复，开始计时
                Flag = STA_WAITBLOCKACK;
				OSSchedUnlock();
                Debug_Update("%s send %d blocks %d times by %s .",LocalName,TranCtl.blockCnt,TranCtl.reBlockSta,TranCtl.ch== INTERFACE_LAN ? "Ethernet":"CAN Bus");
                OSTmrStart(TimerSendPack,&err);
            }
            else
            {

            }
            if(TranCtl.packCnt +1 == FileInfo.packnum && TranCtl.blockCnt +1 == FileInfo.blockNum)
            {
                //发送完成
//                Debug_Update("\n%s %s send finish",LocalName,FileInfo.filename);
                OSTmrStop(TimerSendPack,OS_TMR_OPT_NONE,0,&err);
            }
#ifdef DETAIL_DEBUGUPDATE
            Debug_Update("Send %d blocks %d pack.",TranCtl.blockCnt,TranCtl.blockPack);
#endif			
        }
        else
        {
            if(TranCtl.packCnt +1 == FileInfo.packnum && TranCtl.blockCnt +1 == FileInfo.blockNum)
            {
                //发送完成
//                Debug_Update("\n%s %s send finish",LocalName,FileInfo.filename);
                OSTmrStop(TimerSendPack,OS_TMR_OPT_NONE,0,&err);
            }
            else
            {
                Debug_Update("%d send  error**************************",TranCtl.blockPack);
                OSTmrStop(TimerSendPack,OS_TMR_OPT_NONE,0,&err);
				OSSchedLock();
                Flag = STA_GETFILENAME;
                SendFileNum++;
				OSSchedUnlock();
            }
        }
    }
    break;
    case STA_RESENDBLOCK:
    {
//        TranCtl.reBlock = TranCtl.blockCnt;
        OSTmrStop(TimerSendPack,OS_TMR_OPT_NONE,0,&err);
        if(TranCtl.lastReBlock != TranCtl.blockCnt)
        {
            TranCtl.reBlockSta =1;	 //首次重发
        }
        else
        {
            TranCtl.reBlockSta ++;
        }
        TranCtl.lastReBlock = TranCtl.blockCnt;
        TranCtl.reBlock 	= TranCtl.blockCnt;
        if(TranCtl.reBlockSta > RETRY_TIMES)
        {
            //超过重发次数
            ProgressOut(TranCtl.port,UNENTER,TYPE_STR,"文件传输出错",0);
            Debug_Update("%s %d block %d pack %d times %d reblocksta文件传输出错",TranCtl.unitName,TranCtl.blockCnt,TranCtl.packCnt,TranCtl.blockSta,TranCtl.reBlockSta);
            Flag = STA_GETFILENAME;
            SendFileNum++;
            break;
        }
        if(!fseek(fp_Tran,TranCtl.blockCnt * BLOCKSIZE,SEEK_SET))	//重新发送块
        {
            Flag = STA_SENDBLOCK;
            Debug_Update("\nTime out resend %d blocks  %d times++++++++++++++++++++++++++++++++",TranCtl.blockCnt,TranCtl.reBlockSta);
        }
    }
    break;
    case STA_WAIT_CHECK:
    {   static uint16_t cnt;
        if(cnt++ > 500)
        {
            cnt = 0;
            SendDataUpdate((uint8_t *)SendBuf,SENDBUFLEN,OP_UPDATE,CMD_UPDATE_QUE_STATE,GetOwnPort(),TranCtl.port,NULL,0,TranCtl.ch);
            Debug_Update("Send state query");
            OSTmrStart(TimerSta,&err);
        }
    }
    break;
    default:
        break;
    }
}
/*********************************************************************//**
 * @brief
 * @param[in]	None
 * @return 		None
 **********************************************************************/
uint8_t MasterUpdateReceive(Tran_ptCIR_Frame *pTF)
{
    uint16_t packNum;
    uint8_t err;
    uint8_t name[15];
    if(pTF->iOp == OP_UPDATE && g_rcv)
    {
        switch(pTF->iCmd)
        {
        case CMD_UPDATE_CHANNEL_DETECT_ACK: //??????????????
        {
            if(pTF->iSport == TranCtl.port && (Flag == STA_WAITCANDETACK || Flag == STA_WAITNETDETACK))
            {
                switch(*pTF->pData)
                {
                case INTERFACE_LAN:
                case INTERFACE_CAN1:
                case INTERFACE_CAN2:
                {
                    //停止计时
                    OSTmrStop(TimerDetChannel,OS_TMR_OPT_NONE,0,&err);
                    TranCtl.ch = *pTF->pData;
                    GetUnitJPByPort(pTF->iSport,name);
                    Debug_Update("\n%s receive detect ack from %s,Channel:%s.",LocalName,name,TranCtl.ch == INTERFACE_LAN?"ETHERNET":"CAN");
                    Flag = STA_SENDFILEINFO;	//开始发送文件信息
                }
                break;
                default:
                {
                    Debug_Update("Channel Error");
                }
                break;
                }
            }
        }
        break;
        case CMD_UPDATE_FILEINFO_ACK:
        {
            if(Flag == STA_WAITFILEINFOACK)
            {
                OSTmrStop(TimerFileInfo,OS_TMR_OPT_NONE,0,&err);
                //开始发送块
                Flag = STA_SENDBLOCK;
                Debug_Update("\n%s receive file information ack from %s.",LocalName,TranCtl.unitName);
                //memset(&PackData,0,sizeof( PACKDATA));
                fseek(fp_Tran,0,SEEK_SET);				//接收到ack，就从头开始发送文件
            }
        }
        break;
        case CMD_UPDATE_FILEDATA_ACK:
        {
            if(Flag == STA_WAITBLOCKACK)
            {
                PACK_ACK packAck;
                OSTmrStop(TimerSendPack,OS_TMR_OPT_NONE,0,&err);
                memcpy(&packAck,pTF->pData,pTF->iDataLen);
                Debug_Update("\n%s:receive %d block %d pack ack.",LocalName,packAck.blockCnt,packAck.packCnt);
                //不进行BLOCK,PACK连续性检测，因为有重发和分块发送
                //接收完成
                if(packAck.packCnt+1 == FileInfo.packnum)
                {
					OSSchedLock();
                    Flag = STA_WAIT_CHECK;
					OSSchedUnlock();
                    Debug_Update("\n%sReceive final ack",LocalName);
                    fclose(fp_Tran);
                    //等待校验结果
                }
                else
                {
					OSSchedLock();
                    Flag = STA_SENDBLOCK;		//发送下一块
					OSSchedUnlock();
                }
            }
        }
        break;
        case CMD_UPDATE_QUE_STATE_ACK:
        {
//            if(Flag == STA_WAIT_CHECK)
            {
                uint8_t sta;
                sta = pTF->pData[0];
                switch(sta)
                {
                case CHECK_ING:
                {
                    OSTmrStop(TimerSta,OS_TMR_OPT_NONE,0,&err);
                }
                break;
                case CHECK_FAILED:
                case CHECK_SUCCESS:
                {
                    OSTmrStop(TimerSta,OS_TMR_OPT_NONE,0,&err);
                    ProgressOut(TranCtl.port,UNENTER,TYPE_STR,sta == CHECK_SUCCESS?"文件传输成功":"文件校验错误",0);
                    Debug_Update("\nReceive %s checkout %s.",FileInfo.filename,sta == CHECK_SUCCESS ? "success":"failed");
                    Flag = 	STA_GETFILENAME;	//切换下一个文件
                    SendFileNum++;
                }
                break;
                default:
                    break;
                }
            }
        }
        break;
        default :
            break;
        }
    }
}