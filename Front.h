#ifndef __FRONT_H__
#define __FRONT_H__

#include "Common.h"


/*****************************************************************************
*                                                                          
* 							LITERAL CONSTANTS(DEFINES)
*                    
******************************************************************************/

#define KEYDEFS_MENU	0x01
#define KEYDEFS_UP		0x02
#define KEYDEFS_DN		0x04
#define KEYDEFS_ENT		0x08
//#define KEYDEFS_ESC		0x10
	
	
#define KEYDEFS_NULL	0x00


/** 7-Segment mapping */
/* 1 : ON, 0: OFF */

/*
			  0
			 ---
		  5 | 6	|  1
			 ---
		  4 |   |  2
			 --- . 7
			  3
*/

#define FND_OFF			0x00

#define FND_DISP        0x80
#define FND_DOT         0x80
#define FND_BAR			0x40

#define FND_NUM_0       0x3F
#define FND_NUM_1       0x06
#define FND_NUM_2       0x5B
#define FND_NUM_3       0x4F
#define FND_NUM_4       0x66
#define FND_NUM_5       0x6D
#define FND_NUM_6       0x7D
#define FND_NUM_7       0x27
#define FND_NUM_8       0x7F
#define FND_NUM_9       0x6F
#define FND_ALP_A       0x77
#define FND_ALP_B       0x7C

#define FND_ALP_C       0x39
#define FND_ALP_SC      0x58

#define FND_ALP_D       0x5E
#define FND_ALP_E       0x79
#define FND_ALP_F       0x71
#define FND_ALP_G       0x3D
#define FND_ALP_H       0x76
#define FND_ALP_I       0x04
#define FND_ALP_J       0x07
#define FND_ALP_K       FND_NUM_0	// N/A
#define FND_ALP_L       0x38
#define FND_ALP_SL      0x04

#define FND_ALP_M       FND_NUM_0	// N/A
#define FND_ALP_N       0x54
#define FND_ALP_SN      0x54
//#define FND_ALP_N       0x37

#define FND_ALP_O       0x5c
#define FND_ALP_P       0x73
#define FND_ALP_Q       0x67
#define FND_ALP_R       0x50
#define FND_ALP_S       FND_NUM_5
#define FND_ALP_T       0x78
#define FND_ALP_U       0x3E
#define FND_ALP_SU       0x1C
#define FND_ALP_V       FND_ALP_U
#define FND_ALP_W       FND_NUM_0
#define FND_ALP_X       FND_NUM_0
#define FND_ALP_Y       0X66
#define FND_ALP_Z       FND_NUM_0
#define FND_LEFT_BRACE  0x39
#define FND_RIGHT_BRACE 0x0F

#define FND_ALL_ON		0xFF

/** 7-Segment string */
#define FND_OFF_MSG					"  "
#define FND_BAR_MSG					"--"
#define FND_DOT_MSG					".."

// SELF TEST
#define FND_SELF_TEST               "SF"	/* Self Test */


// 4Digit characters
#define FND4_READY					"----"	
#define FND4_SET					" SET"  /* 명령 유효 SEt*/
#define FND4_INVALID				"NONE"  /* 명령 실행 불가 nonE*/
#define FND4_STOP					"STOP"	/* 정지메뉴 StoP */
#define FND4_DEFAULT_MEM            "dEFt"	// Factory Default
#define FND4_1234		            "1234"	
#define FND4_ERROR_EEPROM	        "AL01"	// AL01 
#define FND4_AP_POWER_MODE          "P-UP"	
#define FND4_MK_POWER_MODE          "P-DN"	

/*****************************************************************************
*                                                                          
* 							PUBLIC VARIABLES(Export)
*                    
******************************************************************************/	


/*****************************************************************************
*                                                                          
*							FUNCTION DEFINITIONS(EXTERN)
*                    
******************************************************************************/
void LED_Init(void);
void LED_SetRxCOM(u8 COMx, u8 state);
void LED_SetTxCOM(u8 COMx, u8 state);
void LED_SetTagAlarm(u8 state);
void LED_SetTagRx(u8 state);	
void LED_CheckTagRx(void);

void LED_CheckCOM(void);
void LED_SetRxCAN(u8 CANx, u8 state);
void LED_SetTxCAN(u8 CANx, u8 state);
void LED_CheckCAN(void);
void LED_Update(void);
void LED_Test(u8 data);

void FND_Init(void);
void FND_ChangeDigit(void);
void FND_Output4Char(s8 *s);
void FND_Display4Num(u32 value);
void FND_FWVer(void);
void FND_all_on(void);

void FND_DisplayTag(void);
void FND_DisplayClockMenu(u8 disMode);
bool FND_DisplayMenu(void);
void FND_ReadyOutput(void);
void FND_MenuNone(void);
void FND_DisplaySet(void);
void FND_DisplayInvalid(void);

void MENU_Init(void);
void KEY_Init(void);
bool KEY_Read(void);
u8 KEY_BootRead(void);
bool KEY_Process(void);
void MENU_Init(void);
void MENU_ValueClock(u8 Key);
void MENU_Process(void);

#endif /* __FRONT_H */
