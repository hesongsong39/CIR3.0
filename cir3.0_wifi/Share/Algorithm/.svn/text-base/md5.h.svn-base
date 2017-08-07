#ifndef _MD5_H
#define _MD5_H

//\#include "config.h"

#include "lpc_types.h"
#include "RTL.h"
#include "ptcir.h"
#ifndef uint8
#define uint8  unsigned char
#endif

#ifndef uint32
#define uint32 unsigned long int
#endif

typedef struct
{
    uint32 total[2];
    uint32 state[4];
    uint8 buffer[64];
}
md5_context;

void md5_starts( md5_context *ctx );
void md5_update( md5_context *ctx, uint8 *input, uint32 length );
void md5_finish( md5_context *ctx, uint8 digest[16] );

extern BOOL Cal_MD5_Code(BYTE *pbSrcData, unsigned long int iLen, BYTE *pMD5Arr);
extern BOOL Cal_MD5_Code2(char *pFileName, unsigned char *pMD5Arr);

#endif /* md5.h */

