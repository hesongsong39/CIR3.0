#include "updateapi.h"
//#include "cmddefine.h"
//#include "SPIFLASH.h"
#include "AppDefine.h"
//#include "updatecom.h"
//#include "MX66L51235F.h"
//#include "Task.h"

/*********************************************************************//**
 * @brief			获取本单元的CAN端口号
 * @param[in]	None
 * @return 		端口号 uint8_t
 **********************************************************************/ 
uint8_t GetOwnPort(void)
{
    return(g_ParaInfo.SelfPort);
}
/*********************************************************************//**
 * @brief			获取本单元名称简写
 * @param[in]	None
 * @return 		端口号 uint8_t
 **********************************************************************/
uint8_t GetOwnName(char * name)
{
        memcpy(name,"WIFI",sizeof("WIFI"));
}
/*********************************************************************//**
 * @brief			获取当前主用CAN总线
 * @param[in]	None
 * @return 		uint8_t
 **********************************************************************/
uint8_t GetMasterCan()
{
    return g_ParaInfo.MasterCanBus;
}
Bool UpdateModeSet(void)
{

}
/*********************************************************************//**
 * @brief			
 * @param[in]	
 * @return 		
 **********************************************************************/
void  GetOffsetUpdate(uint32_t *offset,uint8_t *flash)
{
	*offset = 0x82000000;
	*flash  = MEMORY_TYPE_SPI;
}
/*********************************************************************//**
 * @brief			
 * @param[in]	
 * @return 		
 **********************************************************************/
void ProgressMMI(uint8_t *buf,uint8_t len)
{

}
