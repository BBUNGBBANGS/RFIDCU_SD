#ifndef __UTIL_H
#define __UTIL_H

#include "Common.h"

/*****************************************************************************
*                                                                          
*							FUNCTION DEFINITIONS(EXTERN)
*                    
******************************************************************************/
void Console_CheckSendData(void);
void COMx_CheckSendData(u8 COMx);

void EchoUart(u8 nPORT);
void COMx_Write(u8 COMx, const u8 *data, u32 length);	

bool IsNumberString(const s8* str, u32 base);	
bool IsBCD(const u8 *c);
u8 BCD2D(u8 byte);
u8 D2BCD(u8 byte);
s8 D2C(u8 c);
s8 C2D(s8 c);

void char2asc(s8 *array, u8 byte);
u8 asc2bin(u8 *s);
void dec2str(u8 dec, s8 *str);
void dec1str(u8 dec, s8 *str);


u8	INVBIT(u8 byte);
void mytoggle(u8 *byte);
u8	INVBYTE(u8 byte);


void replacetochar(s8 * str, s8 oldchar, s8 newchar);
s32 myatoi(const s8* str, u32 base);
void myitoa(u32 value, s8* const pDestination, u32 base);
s8 mytoupper(s8 c);
s8 *mytoupper_string(s8 *str);
s8 mytolower(s8 c);
s8 *mytolower_string(s8 *str);
s32 mystrncmp(const s8 *s1,const s8 *s2, u32 n );
s32 mystrcmp(const s8 *s1, const s8 *s2);
bool mymemcmp(const void *s1, const void *s2, u32 n);
bool mynewcmp(u8 *pOld, u8 *pNew);
u32 mystrlen(const s8 *str);
u32 myu8strlen(const u8 *str);
s8 *mystrcat(s8 *s1, const s8 *s2);
void mystrncpy(s8* s1, s8* s2, u32 numBytes);
void mymemcpy(void *pDestination, const void *pSource, u32  numBytes);
void myprintf(char *fmt, ...);
void myputs(const s8 *s);
void myputsn(const s8 *s, u32 n);
void myputsln(const s8 *s);
void myputchar(s8 c);
void myputxx(u8 c);
void myputxln(u8 c);
u16 swap_u16(u16 value);
u32 swap_u32(u32 value);
bool Check32bitsCheckSum(u8 *s, u32 n);
u8 GetXorCheckSum(u8 *buf, u32 n);

bool __SET_RELEASE_DATE(u8 *pBuffer);
bool __SET_VER(u8 *pBuffer);

#endif /*__UTIL_H */
