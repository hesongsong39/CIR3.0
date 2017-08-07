#ifndef _UPDATAMASTER_H_
#define _UPDATAMASTER_H_

#include "lpc_types.h"
#include "ptcir.h"

void MasterUpdate(Tran_ptCIR_Frame *pTF);
void MasterUpdateTran(void);
uint8_t MasterUpdateReceive(Tran_ptCIR_Frame *pTF);
uint8_t CheckFileUpdate(void);
Bool ResetAllUpdate(void);

void ExtTime(void);
#endif

