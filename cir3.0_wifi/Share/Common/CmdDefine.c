/************************************************************
  Copyright (C), 2016, 712.Co.,Ltd.
  FileName: crc.c
  Author: zhaohualei      
  Description: 
  Version:       V1.0.0.1  
  Function List:   
  
  History:        
  <author>  <time>   <version >   <desc>
      
***********************************************************/
#include "cmddefine.h"
#include <string.h>

//�ɵ�Ԫ��д��ȡ����˿ں�
const _UnitJPPort g_UnitPortNetPortJPTable[24] = 
{
	{PORT_MMI,0,"MMI","MMI"},  		                            //MMI
	{PORT_MMI1,NETPORT_MMI1,"MMI1","MMI1"}, 		            //MMI1
	{PORT_MMI2,NETPORT_MMI2,"MMI2","MMI2"},                     //MMI2
	
	{PORT_MCU,0,"MCU","���Ƶ�Ԫ"},                              //MCU
	{PORT_MCU1,NETPORT_MCU1,"MCU1","���Ƶ�Ԫ1"},                //MCU1
	{PORT_MCU2,NETPORT_MCU2,"MCU2","���Ƶ�Ԫ2"},                //MCU2
	
	//{PORT_CONVERSE,0,"CONVERSE","ת����Ԫ"},                    //CONVERSE	
	{PORT_CONVERSE1,NETPORT_CONVERSE1,"CONVERSE1","ת����Ԫ1"}, //CONVERSE1
	{PORT_CONVERSE2,NETPORT_CONVERSE2,"CONVERSE2","ת����Ԫ2"}, //CONVERSE2
	
    //{PORT_LTE,0,"LTE","LTE��Ԫ"},                              //LTE
	{PORT_LTE1,NETPORT_LTE1,"LTE1","LTE��Ԫ1"},                //LTE1
	{PORT_LTE2,NETPORT_LTE2,"LTE2","LTE��Ԫ2"},                //LTE2

	{PORT_GPS,NETPORT_GPS,"GPS","���Ƕ�λ��Ԫ"},                //GPS
	{PORT_RECORD,NETPORT_RECORD,"RECORD","��¼��Ԫ"},           //RECODE
	{PORT_WIFI,NETPORT_WIFI,"WIFI","�����Ԫ"},                 //WIFI	
	{PORT_SWITCH,NETPORT_SWITCH,"SWITCH","������Ԫ"},           //SWITCH
	{PORT_400M,NETPORT_400M,"400M","400M��Ԫ"},                 //400M
	{PORT_800M,NETPORT_800M,"800M","800M��Ԫ"},	                //800M
	{PORT_IFUNIT,NETPORT_IFUNIT,"IF","�ӿڵ�Ԫ"},               //IF UNIT
	{PORT_450M,NETPORT_450M,"450M","450M��Ԫ"},                 //450M
	{PORT_BATTERY,NETPORT_BATTERY,"BATTERY","��Դ��Ԫ"},        //BATTERY
    
	{PORT_WEIHU,NETPORT_WEIHU,"WEIHU","ά��PC"},	           //WEIHU
};

//�ɶ˿ںŻ�ȡCAN NET��ַ
const _AddrPort g_AddrPortTable[24] = 
{
	{PORT_MMI, CANADDR_MMI, 0x00, 0x00, STORAGE_ADDR_EEPROM_MMI_CFG, STORAGE_ADDR_SPI_MMI_CFG, STORAGE_ADDR_SPIFI_MMI_CFG},                                                                 //MMI
	{PORT_MMI1,CANADDR_MMI1, NETADDR_IP4_MMI1, NETADDR_MAC6_MMI1, STORAGE_ADDR_EEPROM_MMI_CFG, STORAGE_ADDR_SPI_MMI_CFG, STORAGE_ADDR_SPIFI_MMI_CFG},                                       //MMI1
	{PORT_MMI2,CANADDR_MMI2, NETADDR_IP4_MMI2, NETADDR_MAC6_MMI2, STORAGE_ADDR_EEPROM_MMI_CFG, STORAGE_ADDR_SPI_MMI_CFG, STORAGE_ADDR_SPIFI_MMI_CFG},                                       //MMI2
	
	{PORT_MCU, CANADDR_MCU, 0x00, 0x00, STORAGE_ADDR_EEPROM_MCU_CFG, STORAGE_ADDR_SPI_MCU_CFG, STORAGE_ADDR_SPIFI_MCU_CFG},                                                                 //MCU	
	{PORT_MCU1,CANADDR_MCU1, NETADDR_IP4_MCU1, NETADDR_MAC6_MCU1, STORAGE_ADDR_EEPROM_MCU_CFG, STORAGE_ADDR_SPI_MCU_CFG, STORAGE_ADDR_SPIFI_MCU_CFG},                                       //MCU1
	{PORT_MCU2,CANADDR_MCU2, NETADDR_IP4_MCU2, NETADDR_MAC6_MCU2, STORAGE_ADDR_EEPROM_MCU_CFG, STORAGE_ADDR_SPI_MCU_CFG, STORAGE_ADDR_SPIFI_MCU_CFG},                                       //MCU2
	
	//{PORT_CONVERSE, CANADDR_CONVERSE, 0x00, 0x00, STORAGE_ADDR_EEPROM_CONVERSE_CFG, STORAGE_ADDR_SPI_CONVERSE_CFG, STORAGE_ADDR_SPIFI_CONVERSE_CFG},                                        //CONVERSE	
	{PORT_CONVERSE1, CANADDR_CONVERSE1, NETADDR_IP4_CONVERSE1, NETADDR_MAC6_CONVERSE1, STORAGE_ADDR_EEPROM_CONVERSE_CFG, STORAGE_ADDR_SPI_CONVERSE_CFG, STORAGE_ADDR_SPIFI_CONVERSE_CFG},   //CONVERSE1
	{PORT_CONVERSE2, CANADDR_CONVERSE2, NETADDR_IP4_CONVERSE2, NETADDR_MAC6_CONVERSE2, STORAGE_ADDR_EEPROM_CONVERSE_CFG, STORAGE_ADDR_SPI_CONVERSE_CFG, STORAGE_ADDR_SPIFI_CONVERSE_CFG},   //CONVERSE2
	
	//{PORT_LTE, CANADDR_LTE, 0x00, 0x00, STORAGE_ADDR_EEPROM_LTE_CFG, STORAGE_ADDR_SPI_LTE_CFG, STORAGE_ADDR_SPIFI_LTE_CFG},                                                                 //LTE
	{PORT_LTE1,CANADDR_LTE1, NETADDR_IP4_LTE1, NETADDR_MAC6_LTE1, STORAGE_ADDR_EEPROM_LTE_CFG, STORAGE_ADDR_SPI_LTE_CFG, STORAGE_ADDR_SPIFI_LTE_CFG},                                       //LTE1
	{PORT_LTE2,CANADDR_LTE2, NETADDR_IP4_LTE2, NETADDR_MAC6_LTE2, STORAGE_ADDR_EEPROM_LTE_CFG, STORAGE_ADDR_SPI_LTE_CFG, STORAGE_ADDR_SPIFI_LTE_CFG},                                       //LTE2
	 
	{PORT_GPS, CANADDR_GPS, NETADDR_IP4_GPS, NETADDR_MAC6_GPS, STORAGE_ADDR_EEPROM_GPS_CFG, STORAGE_ADDR_SPI_GPS_CFG, STORAGE_ADDR_SPIFI_GPS_CFG},                                          //GPS
	{PORT_RECORD, CANADDR_RECORD, NETADDR_IP4_RECORD, NETADDR_MAC6_RECORD, STORAGE_ADDR_EEPROM_RECORD_CFG, STORAGE_ADDR_SPI_RECORD_CFG, STORAGE_ADDR_SPIFI_RECORD_CFG},                     //RECODE
	{PORT_WIFI, CANADDR_WIFI, NETADDR_IP4_WIFI, NETADDR_MAC6_WIFI, STORAGE_ADDR_EEPROM_WIFI_CFG, STORAGE_ADDR_SPI_WIFI_CFG, STORAGE_ADDR_SPIFI_WIFI_CFG},                                   //WIFI
	{PORT_SWITCH, CANADDR_SWITCH, NETADDR_IP4_SWITCH, NETADDR_MAC6_SWITCH, STORAGE_ADDR_EEPROM_SWITCH_CFG, STORAGE_ADDR_SPI_SWITCH_CFG, STORAGE_ADDR_SPIFI_SWITCH_CFG},                     //SWITCH
	{PORT_400M, CANADDR_400M, NETADDR_IP4_400M, NETADDR_MAC6_400M, STORAGE_ADDR_EEPROM_400M_CFG,STORAGE_ADDR_SPI_400M_CFG, STORAGE_ADDR_SPIFI_400M_CFG},                                    //400M
	{PORT_800M, CANADDR_800M, NETADDR_IP4_800M, NETADDR_MAC6_800M, STORAGE_ADDR_EEPROM_800M_CFG,STORAGE_ADDR_SPI_800M_CFG, STORAGE_ADDR_SPIFI_800M_CFG},	                                //800M
	{PORT_IFUNIT, CANADDR_IFUNIT, NETADDR_IP4_IFUNIT, NETADDR_MAC6_IFUNIT, STORAGE_ADDR_EEPROM_IFUNIT_CFG,STORAGE_ADDR_SPI_IFUNIT_CFG, STORAGE_ADDR_SPIFI_IFUNIT_CFG},                      //IF UNIT
	{PORT_450M, CANADDR_450M, NETADDR_IP4_450M, NETADDR_MAC6_450M, STORAGE_ADDR_EEPROM_450M_CFG,STORAGE_ADDR_SPI_450M_CFG, STORAGE_ADDR_SPIFI_450M_CFG},                                    //450M
	{PORT_BATTERY, CANADDR_BATTERY, NETADDR_IP4_BATTERY, NETADDR_MAC6_BATTERY, STORAGE_ADDR_EEPROM_BATTERY_CFG, STORAGE_ADDR_SPI_BATTERY_CFG, STORAGE_ADDR_SPIFI_BATTERY_CFG},              //BATTERY
	
	//{PORT_PUBLIC, CANADDR_PUBLIC, NETADDR_IP4_PUBLIC, 0x00, 0x00, 0x00, 0x00},                            //PUBLIC
	{PORT_WEIHU, CANADDR_WEIHU, NETADDR_IP4_WEIHU, NETADDR_MAC6_WEIHU, 0x00, 0x00, 0x00},	                //WEIHU
	{PORT_BROADCAST, CANADDR_BRORDCAST, NETADDR_IP4_BROADCAST, 0x00, 0x00, 0x00, 0x00},                     //BROADCAST
};


//�ɶ˿ںŻ�ȡ���õ�ַ
Bool GetCfgAddrByPort(uint8_t port, uint32_t *eepromaddr, uint32_t *sspaddr, uint32_t *spifiaddr) 
{
    uint8_t isize = sizeof(g_AddrPortTable) / sizeof(g_AddrPortTable[0]);
	
	uint8_t i = 0;
	
	if (port == 0x00 || (eepromaddr == NULL) || (sspaddr == NULL) || (spifiaddr == NULL))
	{
		return FALSE;
	}
	
	for (i = 0; i < isize; i++)
	{
		if(port == g_AddrPortTable[i].port)
		{
			*eepromaddr = g_AddrPortTable[i].eepromaddr;
			*sspaddr    = g_AddrPortTable[i].spiaddr;
			*spifiaddr  = g_AddrPortTable[i].spifiaddr;
			
			return TRUE;
		}
	}
	
	return FALSE;
}

//�ɶ˿ںŻ�ȡCAN��ַ
Bool GetCanAddrByPort(UNS_8 port, UNS_8 * canaddr) 
{
    INT_8 isize = sizeof(g_AddrPortTable) / sizeof(g_AddrPortTable[0]);
	
	INT_8 i = 0;
	
    if (port == 0x00 || canaddr == NULL)
	{
		return FALSE;
	}
    
	for(i = 0; i < isize; i++)
	{
		if(port == g_AddrPortTable[i].port)
		{
			*canaddr = g_AddrPortTable[i].canaddr;
			
			return TRUE;
		}
	}
	
    //���ӽӿڵ�Ԫ��̬�󶨶˿�
    //for()
    //{
        
    //}
    //���ӽӿڵ�Ԫ��̬�󶨶˿�
    
	if(i >= isize)
	{
		*canaddr = 0x00; //ͨ�õĵ�Ԫ��ַ
		
		return FALSE;
	}
	
	return TRUE;
}

//�ɶ˿ںŻ�ȡMAC��ַ
Bool GetMacAddrByPort(UNS_8 port, UNS_8 * macaddr6)
{
    INT_8 isize = sizeof(g_AddrPortTable) / sizeof(g_AddrPortTable[0]);
	
	INT_8 i = 0;
	
    if (port == 0x00 || macaddr6 == NULL)
	{
		return FALSE;
	}
    
	for(i = 0; i < isize; i++)
	{
		if(port == g_AddrPortTable[i].port)
		{
			*macaddr6 = g_AddrPortTable[i].mac6;
			
			return TRUE;
		}
	}
	
	if(i >= isize)
	{
		*macaddr6 = 0x00; //ͨ�õĵ�Ԫ��ַ
		
		return FALSE;
	}
	
	return TRUE;
}

//�ɶ˿ںŻ�ȡNET��ַ
Bool GetNetAddrByPort(UNS_8 port, UNS_8 * netaddr) 
{
    INT_8 isize = sizeof(g_AddrPortTable) / sizeof(g_AddrPortTable[0]);
	
	INT_8 i = 0;
	
    if (port == 0x00 || netaddr == NULL)
	{
		return FALSE;
	}
    
	for(i = 0; i < isize; i++)
	{
		if(port == g_AddrPortTable[i].port)
		{
			*netaddr = g_AddrPortTable[i].netaddr;
			
			return TRUE;
		}
	}
	
    //���ӽӿڵ�Ԫ��̬�󶨶˿�
    
    //���ӽӿڵ�Ԫ��̬�󶨶˿�
    
	if(i >= isize)
	{
		*netaddr = 0x00; //ͨ�õĵ�Ԫ��ַ
		
		return FALSE;
	}
	
	return TRUE;
}

//�ɶ˿ںŻ�ȡCAN��NET��ַ
Bool GetCanNetAddrByPort(UNS_8 port, UNS_8 * canaddr, UNS_8 * netaddr)
{
	INT_8 isize = sizeof(g_AddrPortTable) / sizeof(g_AddrPortTable[0]);
	
	INT_8 i = 0;
	
    if (port == 0x00 || canaddr == NULL || netaddr == NULL)
	{
		return FALSE;
	}
    
	for(i = 0; i < isize; i++)
	{
		if(port == g_AddrPortTable[i].port)
		{
			*canaddr = g_AddrPortTable[i].canaddr;
			*netaddr = g_AddrPortTable[i].netaddr;
			
			return TRUE;
		}
	}
	
    //���ӽӿڵ�Ԫ��̬�󶨶˿�
    
    //���ӽӿڵ�Ԫ��̬�󶨶˿�
    
	if(i >= isize)
	{
		*canaddr = 0x00; //ͨ�õĵ�Ԫ��ַ
		*netaddr = 0x00; //ͨ�õĵ�Ԫ��ַ
		
		return FALSE;
	}
	
	return TRUE;
}

//�ɵ�Ԫ���ֻ�ȡCAN��ַ
Bool GetCanAddrByUnitJP(CHAR *szUnitJP, UNS_8 * canaddr) 
{
    UNS_8 port;
    
	if(szUnitJP == NULL)
    {
        return FALSE;
    }
    
	GetPortByUnitJP(szUnitJP, &port);
	
	return GetCanAddrByPort(port, canaddr);
}

//�ɵ�Ԫ���ֻ�ȡNET��ַ
Bool GetNetAddrByUnitJP(CHAR *szUnitJP, UNS_8 * netaddr) 
{
    UNS_8 port;
	
    if(szUnitJP == NULL)
    {
        return FALSE;
    }
    
	GetPortByUnitJP(szUnitJP, &port);
	
	return GetNetAddrByPort(port, netaddr);
}

//�ɵ�Ԫ��ƴ��ȡCAN��ַ����̫����ַ
Bool GetCanNetAddrByUnitJP(CHAR *szUnitJP, UNS_8 * canaddr, UNS_8 * netaddr)
{
	UNS_8 port;
	
   	if(szUnitJP == NULL)
    {
        return FALSE;
    }
    
	GetPortByUnitJP(szUnitJP, &port);
	
	return GetCanNetAddrByPort(port, canaddr, netaddr);
}

//�ɵ�Ԫ��ƴ��ȡ�˿ں�
Bool GetPortByUnitJP(CHAR *szUnitJP, UNS_8 *port)
{
	INT_8 isize = sizeof(g_UnitPortNetPortJPTable) / sizeof(g_UnitPortNetPortJPTable[0]);
	
	INT_8 i = 0;
	
	if(szUnitJP == NULL)
	{
		 return FALSE;
	}
	
	for(i = 0; i < isize; i++)
	{
		if(!strcmp(szUnitJP, g_UnitPortNetPortJPTable[i].szJP))
		{
		  *port = g_UnitPortNetPortJPTable[i].port;
			
			return TRUE;
		}
	}
	
	if(i >= isize)
	{
		*port = 0x00; //ͨ�õĶ˿ں�
		
		return FALSE;
	}
	
	return TRUE;
}

//�ɶ˿ںŻ�ȡ��Ԫ��ƴ
Bool GetUnitJPByPort(UNS_8 port, CHAR *szUnitJP)
{
	INT_8 isize = sizeof(g_UnitPortNetPortJPTable) / sizeof(g_UnitPortNetPortJPTable[0]);
	
	INT_8 i = 0;
	
	if(port == 0x00 || szUnitJP == NULL)
	{
		 return FALSE;
	}
	
	for(i = 0; i < isize; i++)
	{
		if(port == g_UnitPortNetPortJPTable[i].port)
		{
		  strcpy(szUnitJP, g_UnitPortNetPortJPTable[i].szJP);
			
			return TRUE;
		}
	}
	
	if(i >= isize)
	{
		*szUnitJP = 0;
		
		return FALSE;
	}
	
	return TRUE;
}

//�ɶ˿ںŻ�ȡ����˿ں�
Bool GetNetPortByPort(UNS_8 port, UNS_16 *netport)
{
    INT_8 isize = sizeof(g_UnitPortNetPortJPTable) / sizeof(g_UnitPortNetPortJPTable[0]);
	
	INT_8 i = 0;
	
    if(port == 0x00 || netport == NULL)
    {
        return FALSE;
    }
    
	for(i = 0; i < isize; i++)
	{
		if(port == g_UnitPortNetPortJPTable[i].port)
		{
		    *netport = g_UnitPortNetPortJPTable[i].netport;
			
			return TRUE;
		}
	}
	
    //���ӽӿڵ�Ԫ��̬�󶨶˿�
    
    //���ӽӿڵ�Ԫ��̬�󶨶˿�
    
	if(i >= isize)
	{
		*netport = 0;
		
		return FALSE;
	}
	
	return TRUE;
}

//�ɶ˿ںŻ�ȡ��Ԫ��������
Bool GetUnitChinaNameByPort(UNS_8 port, CHAR *szUnitChinaName)
{
	INT_8 isize = sizeof(g_UnitPortNetPortJPTable) / sizeof(g_UnitPortNetPortJPTable[0]);
	
	INT_8 i = 0;
	
	if(port == 0x00 || szUnitChinaName == NULL)
	{
		 return FALSE;
	}
	
	for(i = 0; i < isize; i++)
	{
		if(port == g_UnitPortNetPortJPTable[i].port)
		{
		  strcpy(szUnitChinaName, g_UnitPortNetPortJPTable[i].szChinaName);
			
			return TRUE;
		}
	}
	
	if(i >= isize)
	{
		*szUnitChinaName = 0;
		
		return FALSE;
	}
	
	return TRUE;
}

//��ҵ������
Bool GetCanPriDataTypeByOpCmd(UNS_8 srcport, UNS_8 desport, UNS_8 op, UNS_8 cmd, UNS_8 * canpri, UNS_8 * datatype)
{
	if(op == 0x32) //��������
	{
		*canpri = 3;
		*datatype = 0;
		
		return TRUE;
	}
	else //����
	{
		*canpri = 2;
		*datatype = 1;
	}
	
	return FALSE;
}

/*
//�ۺ���Ϣ��������
Bool SetIntergrationInfo_Tn(_Intergration_Info *stIngrationInfo, UNS_8 *bData, UNS_16 iLen)
{
	  if(stIngrationInfo == NULL || bData == NULL || iLen == 0)
		{
			return FALSE;
		}
		
		stIngrationInfo->bTnLen = MIN(MAX_TN_LEN, iLen);
		
	  memcpy((void *)stIngrationInfo->bTn, (void *)bData, stIngrationInfo->bTnLen);
		
		return TRUE;
}

Bool GetIntergrationInfo_Tn(UNS_8 *pTn, _Intergration_Info *stIngrationInfo)
{
	if(pTn == NULL || stIngrationInfo == NULL)
	{
		return FALSE;
	}
	
	memcpy(pTn, stIngrationInfo->bTn, stIngrationInfo->bTnLen); 

  return TRUE;	
}

Bool SetIntergrationInfo_En(_Intergration_Info *stIngrationInfo, UNS_8 *bData, UNS_16 iLen)
{
	if(stIngrationInfo == NULL || bData == NULL || iLen == 0)
		{
			return FALSE;
		}
		
		stIngrationInfo->bEnLen = MIN(MAX_EN_LEN, iLen);
		
	  memcpy((void *)stIngrationInfo->bEn, (void *)bData, stIngrationInfo->bEnLen);
		
		return TRUE;
}

Bool GetIntergrationInfo_En(UNS_8 *pEn, _Intergration_Info *stIngrationInfo)
{
	if(pEn == NULL || stIngrationInfo == NULL)
	{
		return FALSE;
	}
	
	memcpy(pEn, stIngrationInfo->bEn, stIngrationInfo->bEnLen); 

  return TRUE;	
}

Bool SetIntergrationInfo_Line(_Intergration_Info *stIngrationInfo, UNS_8 *bData, UNS_16 iLen)
{
	if(stIngrationInfo == NULL || bData == NULL || iLen == 0)
		{
			return FALSE;
		}
		
		stIngrationInfo->bLineLen = MIN(MAX_LINE_LEN, iLen);
		
	  memcpy((void *)stIngrationInfo->bLine, (void *)bData, stIngrationInfo->bLineLen);
		
		return TRUE;
}

Bool GetIntergrationInfo_Line(UNS_8 *pLine, _Intergration_Info *stIngrationInfo)
{
	if(pLine == NULL || stIngrationInfo == NULL)
	{
		return FALSE;
	}
	
	memcpy(pLine, stIngrationInfo->bLine, stIngrationInfo->bLineLen); 

  return TRUE;	
}
*/


