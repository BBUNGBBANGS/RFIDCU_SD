#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "Common.h"
/*****************************************************************************
*                                                                          
* 							LITERAL CONSTANTS(DEFINES)
*                    
******************************************************************************/	

enum{DIR_OUT= 0, DIR_IN};


/*****************************************************************************
*                                                                          
*							FUNCTION DEFINITIONS(EXTERN)
*                    
******************************************************************************/
void SET_I2C_PORT(uint16_t GPIO_Pin, u8 state);
void MEE_ByteWrite(u32 WriteAddr, u8* pBuffer);
u8 MEE_ByteRead(u32 Address);
void MEE_Delay(vu32 dly);

void MEE_PageWrite(u32 WriteAddr, u8* pBuffer, u16 NumByteToWrite);
void MEE_PageRead(u32 Address, u8* pBuffer, u16 NumByteToRead);

#endif  /*__EEPROM_H__*/
