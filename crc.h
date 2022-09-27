#ifndef __CRC_H__
#define __CRC_H__

#include "Common.h"

/*****************************************************************************
*                                                                          
*							FUNCTION DEFINITIONS(GLOBAL)
*                    
******************************************************************************/
extern u8 crc7StrCalc(const u8 *pBuffer, u8 crc, u32 len);
extern u8 crc7ByteCalc(u8 crc, u8 data);
extern u16 crc16StrCalc(const u8 *pBuffer, u16 crc, u32 len);
extern u16 crc16ByteCalc(u16 crc, u8 data);

#endif /*__CRC_H */

