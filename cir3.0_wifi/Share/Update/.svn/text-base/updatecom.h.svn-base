#ifndef __updatecom_h
#define __updatecom_h

#include "stdint.h"
#include "lpc_types.h"
#include "updateapi.h"
//#define CMD_UPDATE_RESULT				0X09
#define FILE_NAME_LENGTH 			  	64
//#define UNIT_JP_LEN_MAX  			 	(FILE_NAME_LENGTH / 2)
#define UNIT_JP_LEN_MAX  			 	(FILE_NAME_LENGTH)
#define MD5_BUFF_SIZE   			  	512
//传输分包设置
#define PACKSIZE   					  	512
#define SENDBUFLEN 					 	(PACKSIZE + 64)
#define MEMERASEUNIT 					(4<<10)
#define BLOCKSIZE 						(MEMERASEUNIT)
#define BLOCKLENGTH    					(BLOCKSIZE / PACKSIZE)

#define MAXUNITNUM 						16
#define MAXFILENUM 						MAXUNITNUM

#define BOOTLOADER_SIZE					(64<<10)  	//
#define BOOT_AREA_SIZE        		    (44<<10)		//4K，mx66512最小擦除单位是4K\ 44K Byte 4k 配置区+ 4*10k历史记录区

#ifdef  USE_SPIFLASH_UPDATE
#define SPI_BOOT_OFFSET_UPDATE        	 SPI_FLASH_UPDATE_OFFSET								//升级缓冲区偏移
#define SPI_BOOT_SIZE				  	 BOOT_AREA_SIZE										//BOOT配置区域大小
#define SPI_FLASH_DATA_OFFSET 		 	 (SPI_FLASH_UPDATE_OFFSET + SPI_BOOT_SIZE)				//升级缓冲区中数据缓冲偏移
#define SPI_FLASH_DATA_SIZE 		 	 (SPI_FLASH_UPDATE_OFFSET - SPI_BOOT_SIZE)				//升级缓冲区中数据缓冲大小

#define FLASH_BOOT_OFFSET_UPDATE	   SPI_BOOT_OFFSET_UPDATE
#define FLASH_BOOT_SIZE_UPDATE		   BOOT_AREA_SIZE
#define FLASH_DATA_OFFSET_UPDATE	   SPI_FLASH_DATA_OFFSET
#define FLASH_DATA_SIZE_UPDATE		   SPI_FLASH_DATA_SIZE
#define FLASH_TYPE_BOOT			   		MEMORY_TYPE_SPI

#elif defined USE_SPIFIFLASH_UPDATE
#define SPIFI_BOOT_OFFSET_UPDATE     	SPIFI_UPDATE_OFFSET
#define SPIFI_BOOT_SIZE_UPDATE		 	BOOT_AREA_SIZE
#define SPIFI_DATA_OFFSET_UPDATE     	(SPIFI_UPDATE_OFFSET + BOOT_AREA_SIZE)
#define SPIFI_DATA_SIZE_UPDATE		 	(SPIFI_UPDATE_SIZE   - BOOT_AREA_SIZE)

#define FLASH_BOOT_OFFSET_UPDATE		SPIFI_BOOT_OFFSET_UPDATE
#define FLASH_BOOT_SIZE_UPDATE			BOOT_AREA_SIZE
#define FLASH_DATA_OFFSET_UPDATE		SPIFI_DATA_OFFSET_UPDATE
#define FLASH_DATA_SIZE_UPDATE			SPIFI_DATA_SIZE_UPDATE
#define FLASH_TYPE_BOOT					MEMORY_TYPE_SPIFI
#endif


#define DEFAULT_PATH		 			"U0:\\update\\"
#define UPDATE_FILETYPE 				"*.CIR3"
#define BOOTCONFIG_SIGN 				"BootConfig"
#define	FILEHEAD_CIR3UPDATE 			"CIR3.0PACK"
#define BOOTCODENAME_UPDATE    			"BootRam.bin"
#define DETECT712WORD					"TJ712"
//LPC43XX 片内FLASH
#define FLASH_BANK_B_APP_ADD       		0X1B000000
#define FLASH_BACK_B_APP_SIZE			0X80000			//512k byte
#define FLASH_BANK_A_APP_ADD			0X1A010000
#define FLASH_BANK_A_APP_SIZE			(0X80000 - BOOTLOADER_SIZE)			//488k byte
#define FLASH_BANK_A_BOOT_ADD    		0X1A000000
#define DEFAULT_JUMP_ADD				0X1A010000		//应用程序默认的跳转地址
//RAM code
#define RAM_CODE_ROM_ADD				 0X10008000
#define RAM_CODE_ROM_SIZE				 0XA000				//40K
#define RAM_CODE_RAM_ADD				 0X10000000
#define RAM_CODE_RAM_SIZE				 0X8000				//32K

#define MEMORY_TYPE_SPIFI  				1
#define MEMORY_TYPE_SPI   				2
#define MEMORY_TYPE_BANK_A 				3
#define MEMORY_TYPE_BANK_B 				4
#define MEMORY_TYPE_EMMC   				5
#define MEMORY_TYPE_RAM					6
#define MEMORY_TYPE_EEPROM 				7

#define ENTRY_UPDATE_MODE  			0X01
#define OUT_UPDATE_MODE				0
#define ENTER						0x02
#define UNENTER 					0x01
#define TYPE_STR 					0X01
#define TYPE_PROGRESS				0X02

//#define STAT_0 1
//#define STAT_1 2
//#define STAT_2 3
//#define STAT_3 4
//#define STAT_4 5
//#define STAT_5 6
//#define STAT_6 7
//#define STAT_7 8
//#define STAT_8 9
//#define STAT_9 10

#define STA_NULL			0
#define STA_DETETHERNET 	1
#define STA_DETCAN			2
#define STA_WAITNETDETACK	3
#define STA_GETFILENAME		4
#define STA_SENDFILEINFO	5
#define STA_WAITFILEINFOACK	6
#define STA_START			9
#define STA_WAITCANDETACK	10
#define STA_SENDBLOCK		11
#define STA_RESENDBLOCK		12
#define STA_WAITBLOCKACK	13
#define STA_WAIT_CHECK		14

#define CHECK_NULL	  0
#define CHECK_ING  	  2
#define CHECK_SUCCESS 3
#define CHECK_FAILED  4

//功能开关
#define USETIMER_UPDATE			//使用UCOS的timer进行超时判断
//#define DETAIL_DEBUGUPDATE	//详细的升级调试信息
//#define PACKCRC 1				//每一包数据进行CRC
//#define WRITECHK				//写入读出对比
#define MMI_ETHERNET_ON			//MMI单元使用以太网用于升级

#define RETRY_TIMES					8	//重试次数
//#define TIMEOUT_UPDATE 			50  //MS
#define STEP_UPDATE	 				1	//MS
#define UDISK_QUITTIME				(10000 /STEP_UPDATE)	//提示U盘拔出10S超时
#define DETECT_TIMEOUT				200						//ms 	通道探测超时时间			
#define FILEINFO_TIMEOUT			200						//ms  	发送文件信息超时时间
#define SENDPACK_TIMEOUT			500 					//ms	分包发送超时时间，ucos TIMER在当前工程下定时分辨率是100MS,SENDPACK_TIMEOUT应该大于200，
#define CHECK_STA_TIMEOUT			200
#define PRGRS_STEP					0.1f
typedef enum
{
    SPIFI_FLASH						=MEMORY_TYPE_SPIFI,
    SPI_FLASH						=MEMORY_TYPE_SPI,
    INTERNAL_FLASH_BANK_A			=MEMORY_TYPE_BANK_A,
    INTERNAL_FLASH_BANK_B			=MEMORY_TYPE_BANK_B,
    EMMC							=MEMORY_TYPE_EMMC,
    RAM 							=MEMORY_TYPE_RAM,
    EEPROM							=MEMORY_TYPE_EEPROM
} MEMORY_TYPE;
#pragma pack(1)
typedef struct
{
	uint8_t sta;  //TRUE success FALSE failed
    char  	 	filename[FILE_NAME_LENGTH];	
} FILE_RESULT;
typedef struct
{
    uint16_t packCnt;
    uint16_t blockCnt;
    uint8_t  blockPack;		//第N块的第X包
} PACK_ACK;
typedef struct
{
    char  	 	filename[FILE_NAME_LENGTH];
    char 		unitName[12];
    uint16_t 	packCnt;
    uint8_t  	packSta;
    uint16_t 	blockCnt;
    uint8_t  	blockSta;
    uint8_t  	blockPack;		//第N块的第X包
    uint32_t 	offset;
    uint8_t  	port;
    uint8_t 	ip;
    uint8_t  	ch;
    uint16_t  	reBlock;
    uint16_t  	lastReBlock;
    uint8_t  	reBlockSta;
} TRAN_CTRL;

#ifdef NEWSTR
typedef struct
{
    char  		filename[FILE_NAME_LENGTH];
    char 		unitName[12];
    uint32_t 	fileSize;
    uint16_t 	packnum;
    uint16_t 	packSize;
    uint16_t 	blockNum;
    uint16_t 	bockSize;
    uint8_t  	ch;
    uint8_t  	md5[16];
} T_FIFEINFO;
typedef struct
{
    uint8_t  blockPack;		//第N块的第X包
    uint8_t  data[PACKSIZE];
    uint16_t len;
    uint16_t packCnt;		//包号
    uint8_t  packSta;		//重发次数，首次发送：0
    uint16_t blockCnt;		//块号
    uint8_t  blockSta;		//块重发次数
    uint32_t offset;		//当前pack对应的偏移
#ifdef PACKCRC
    uint16_t crc;			//CRC
#endif
} PACKSTR;

#else
typedef struct
{
    char  		filename[FILE_NAME_LENGTH];
    uint32_t 	fileSize;
    uint16_t 	packnum;
    uint16_t 	packSize;	
    uint16_t 	blockNum;	
    uint16_t 	bockSize;	
    uint8_t  	md5[16];
} T_FIFEINFO;
typedef struct
{
    uint8_t  blockPack;		//第N块的第X包
    uint32_t packCnt;		//包号
    uint32_t blockCnt;		//块号
    uint32_t offset;		//当前pack对应的偏移
    uint8_t  blockSta;		//块重发次数	首次发送：0	
    uint16_t len;	
    uint8_t  data[PACKSIZE];
} PACKSTR;
typedef struct
{
	uint8_t blksta;		//块状态 00 异常 01 正常
	uint32_t blkcnt;	//块号
    uint32_t packCnt;	//包号
}PACKACK;
#endif

typedef struct
{
    uint8_t 		sign[12];
    uint8_t     	new_updata;		//发起升级标记
    uint8_t 		data_active;	//数据完整性标记
    MEMORY_TYPE		flash_type;		//保存的位置 spi flash/spifi flash/emmc flash/
    uint32_t 		data_offset;	//数据偏移位置
    uint32_t 		data_length;	//数据长度
    uint8_t 		Status;			//升级状态 1：success 0：failed
    uint16_t 		crc;
} BOOT_CONFIG;
typedef struct
{
    uint8_t 			sign[12];
    uint8_t     		new_updata;		//发起升级标记
    uint8_t 			data_active;	//数据完整性标记
    MEMORY_TYPE			flash_type;		//保存的位置 spi flash/spifi flash/emmc flash/
    uint32_t 			data_offset;	//数据偏移位置
    uint32_t 			data_length;	//数据长度
    uint8_t 			Status;			//升级状态 1：success 0：failed
    uint16_t 			crc;
} BOOTSIGN;
//根据CIR3.0升级.doc
typedef struct
{
    uint8_t 	md5[16];
    uint8_t 	sign[12];//固定为“CIR3.0PACK”
    uint8_t 	version;
    uint8_t	 	uint_jp[12];
    uint16_t 	file_num;
} CIR3_HEAD;
typedef struct
{
    uint32_t 		config_offset;	//数据偏移位置
    uint32_t 		config_length;	//数据长度
    uint32_t 		file_offset;	//数据偏移位置
    uint32_t 		file_length;	//数据长度
} CONFIG_FILE_OFFSET;
typedef struct
{
    uint8_t 	   	 sign[12];		//标示，固定为"config"
    uint8_t			 num[4];		//下面数据长度
    uint8_t			 unit_code;		//单元代号
    uint8_t 	   	 unit_jp[12];
    uint8_t			 name[FILE_NAME_LENGTH];		//
    uint8_t			 md5[16];
    MEMORY_TYPE	 	 type;
    uint32_t		 offset;
    uint32_t		 size;
    uint16_t		 crc16;
} FILE_CONFIG;
typedef struct
{
    uint8_t port;
    uint8_t enter;
    uint8_t type;
    uint8_t front;
    uint8_t rgb[3];
    uint8_t reserved1;
    uint8_t reserved2;
    uint8_t strlength;
    uint8_t str[1];
} PROGRESS;
#pragma pack()
#endif
