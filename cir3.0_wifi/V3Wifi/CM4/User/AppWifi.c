#include "AppWifi.h"
#include "EthernetUdp.h"
#include "DataTransfer.h"

void WifiOut(TranItem *pTI,uint16_t len)
{
  uint8_t IP[]= {238,0,100,1};
  UDP_DataSend(IP,NETPORT_WEIHU,(uint8_t *)pTI,len);
}