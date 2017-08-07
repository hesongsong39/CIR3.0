#ifndef __ETHERNET_SET_H_
#define __ETHERNET_SET_H_


#include "lpc_types.h"


//设置MAC地址。格式:"Mac1.Mac2.Mac3.Mac4.Mac5.Mac6"
void SetMacAddr(uint8_t Mac1, uint8_t Mac2, uint8_t Mac3, uint8_t Mac4, uint8_t Mac5, uint8_t Mac6);

//设置主机名。pName:为主机名；iNameLen:为主机名的长度,不能超过15
void SetHostName(char *pName, int iNameLen);

//设置IP地址。格式:"Ip1.Ip2.Ip3.Ip4"
void SetIpAdr(uint8_t Ip1, uint8_t Ip2, uint8_t Ip3, uint8_t Ip4);

//设置子网掩码。格式:"Mask1.Mask2.Mask3.Mask4"
void SetNetMask(uint8_t Mask1, uint8_t Mask2, uint8_t Mask3, uint8_t Mask4);

//设置缺省网关。格式:"GW1.GW2.GW3.GW4"
void SetDefWG(uint8_t GW1, uint8_t GW2, uint8_t GW3, uint8_t GW4);

#endif

