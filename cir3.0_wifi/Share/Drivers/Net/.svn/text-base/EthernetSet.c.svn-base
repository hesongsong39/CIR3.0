#include "EthernetSet.h"
#include <string.h>
#include "Net_config.h"


/*****************Extern variables ************************************/
/* Net_Config.c */
extern U8 own_hw_adr[ETH_ADRLEN]; //MAC地址设置
extern LOCALM nlocalm[3]; //本地IP设置
extern U8 lhost_name[16]; //主机名


//设置MAC地址。格式:"Mac1.Mac2.Mac3.Mac4.Mac5.Mac6"
void SetMacAddr(uint8_t Mac1, uint8_t Mac2, uint8_t Mac3, uint8_t Mac4, uint8_t Mac5, uint8_t Mac6)
{
	own_hw_adr[0] = Mac1;
	own_hw_adr[1] = Mac2;
	own_hw_adr[2] = Mac3;
	own_hw_adr[3] = Mac4;
	own_hw_adr[4] = Mac5;
	own_hw_adr[5] = Mac6;
}


//设置主机名。pName:为主机名；iNameLen:为主机名的长度,不能超过15
void SetHostName(char *pName, int iNameLen)
{
	if ((pName == NULL) || (iNameLen == 0) || (iNameLen > 15))
	{
		return;
	}
	
	memset(lhost_name, 0, sizeof(lhost_name));
	memcpy(lhost_name, pName, iNameLen);
}


//设置IP地址。格式:"Ip1.Ip2.Ip3.Ip4"
void SetIpAdr(uint8_t Ip1, uint8_t Ip2, uint8_t Ip3, uint8_t Ip4)
{
	nlocalm[0].IpAdr[0] = Ip1;
	nlocalm[0].IpAdr[1] = Ip2;
	nlocalm[0].IpAdr[2] = Ip3;
	nlocalm[0].IpAdr[3] = Ip4;
}

//设置子网掩码。格式:"Mask1.Mask2.Mask3.Mask4"
void SetNetMask(uint8_t Mask1, uint8_t Mask2, uint8_t Mask3, uint8_t Mask4)
{
	nlocalm[0].NetMask[0] = Mask1;
	nlocalm[0].NetMask[1] = Mask2;
	nlocalm[0].NetMask[2] = Mask3;
	nlocalm[0].NetMask[3] = Mask4;
}

//设置缺省网关。格式:"GW1.GW2.GW3.GW4"
void SetDefWG(uint8_t GW1, uint8_t GW2, uint8_t GW3, uint8_t GW4)
{
	nlocalm[0].DefGW[0] = GW1;
	nlocalm[0].DefGW[1] = GW2;
	nlocalm[0].DefGW[2] = GW3;
	nlocalm[0].DefGW[3] = GW4;
}

