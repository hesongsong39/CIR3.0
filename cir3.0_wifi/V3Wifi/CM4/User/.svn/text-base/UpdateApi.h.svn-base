#ifndef __UPDATEAPI_H__
#define __UPDATEAPI_H__

#include "lpc_types.h"
#include "updatecom.h"


typedef struct
{
uint32_t offset;
uint8_t type;
}MEMINFO;

//MMI.GPS单元使用SPIFI 之外的单元使用SPI falsh 作为升级缓冲区
//#define USE_SPIFIFLASH_UPDATE		//使用SPIFI作为升级缓冲区

//默认使用SPI FLASH
#ifdef USE_SPIFIFLASH_UPDATE		//SPIFI 
#undef USE_SPIFLASH_UPDATE			
#define SPIFI_UPDATE_OFFSET			  0x2000000 // SPIFI  OFFSET=32M
#define SPIFI_UPDATE_SIZE		      0X2000000 // SPIFI  SIZE  =32M
#else
#define USE_SPIFLASH_UPDATE
#define SPI_FLASH_UPDATE_OFFSET		  0x2000000 // SPI OFFSET=32M
#define SPI_UPDATE_SIZE 			  0x2000000 // SPI SIZE  =32M
#endif

#ifdef USE_SPIFLASH_UPDATE
#ifdef USE_SPIFIFLASH_UPDATE
#error SPIFI_FLASH SPI_FLASH  CONFLICT
#endif
#endif

uint8_t GetOwnPort(void);
uint8_t GetMasterCan(void);
Bool UpdateModeSet(void);
void  GetOffsetUpdate(uint32_t *offset,uint8_t *flash);
void ProgressMMI(uint8_t *buf,uint8_t len);

#endif
