#ifndef __ETHERNET_SET_H_
#define __ETHERNET_SET_H_


#include "lpc_types.h"


//����MAC��ַ����ʽ:"Mac1.Mac2.Mac3.Mac4.Mac5.Mac6"
void SetMacAddr(uint8_t Mac1, uint8_t Mac2, uint8_t Mac3, uint8_t Mac4, uint8_t Mac5, uint8_t Mac6);

//������������pName:Ϊ��������iNameLen:Ϊ�������ĳ���,���ܳ���15
void SetHostName(char *pName, int iNameLen);

//����IP��ַ����ʽ:"Ip1.Ip2.Ip3.Ip4"
void SetIpAdr(uint8_t Ip1, uint8_t Ip2, uint8_t Ip3, uint8_t Ip4);

//�����������롣��ʽ:"Mask1.Mask2.Mask3.Mask4"
void SetNetMask(uint8_t Mask1, uint8_t Mask2, uint8_t Mask3, uint8_t Mask4);

//����ȱʡ���ء���ʽ:"GW1.GW2.GW3.GW4"
void SetDefWG(uint8_t GW1, uint8_t GW2, uint8_t GW3, uint8_t GW4);

#endif

