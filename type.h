#ifndef __TYPE_H
#define __TYPE_H

/*****************************************************************************
*                                                                          
* 									MACRO
*                    
******************************************************************************/
#define sbit(x,y) (x |= (1<<y)) 
#define cbit(x,y) (x &= ~(1<<y))
#define tbit(x,y) (x & (1<<y)) 
#define xbit(x,y) (x ^= (1<<y)) /* inversion */

/*****************************************************************************
*                                                                          
* 								TYPE DEFINITIONS(GLOBAL)
*                    
******************************************************************************/
#ifndef NULL
#define NULL		((void *) 0)
#endif


#ifndef _SIZE_T
#define _SIZE_T
//typedef unsigned int size_t;
typedef u32 size_t;
#endif


#define ON	0x01
#define OFF	0x00
#define _alarm	0xFF

typedef u8 bool;
#define FALSE 0
#define TRUE 1

enum { MANUAL_MODE = 1, AUTO_MODE = 0 };        /**< 수동/자동 */
enum { COMM_DISCONNECT = 1, COMM_CONNECT = 0 }; /**< 통신 단락/연결 */



// LOG DEVICE
#define LOG_SDCARD_MEM		(0)
#define LOG_FLASH_MEM 		(1)

// PCB P/N
#define PCB_A0	0x00
#define PCB_A1	0x01
#define PCB_A2	0x02
#define PCB_A3	0x03

#define CAN_PORT_MAIN 		(0)
#define CAN_PORT_BACKUP 	(1)
//#define CAN_PORT_ALL 		(2)

#define MAIN_PORT		(0)	/* Buffer Index */	
#define SUB_PORT		(1) /* Buffer Index */
#define ALL_PORT		(2)
#define NONE_PORT		(0xff)


#define _clock_param	(0)
#define _clock_setval	(1)


#define _reset 			(0)
#define _check			(1)
#define _detect			(2)
#define _read			(3)

#define _remote			(0)
#define _manual			(1)

#define _after			(0)
#define _before			(1)

enum { DIP_SW_UNUSED = 0, DIP_SW_USED = 1 };


typedef enum _StateType 
{
	IDLE_STATE, RX_STATE, FIRST_TX_STATE, TX_STATE, 			/* MODEM STATE */		
	OFF_STATE, ON_STATE, INIT_STATE, SETUP_STATE, RUN_STATE		/* RESET STATE */
	
}StateType; 


/* 전면 MENU 버튼 */
typedef enum _MENU_MODE
{
	MENU_NONE =0, 
	MENU_NUM, 
	MENU_VALUE, 
	MENU_VALUE_CLOCK /* clock 설정시 사용 */
		
}MENU_MODE;


typedef enum _MENU_PARAM
{
	PARAM_TEST = 0, 
	PARAM_ATTN, 
	PARAM_BYPASS, 
	PARAM_BEEP, 
	PARAM_CLOCK, 
	PARAM_END		
		
}MENU_PARAM;

typedef enum _MENU_CLOCK
{
	MENU_CLOCK_YEAR =0, 
	MENU_CLOCK_MONTH, 
	MENU_CLOCK_DAY, 
	MENU_CLOCK_HOUR, 
	MENU_CLOCK_MIN, 
	MENU_CLOCK_SEC,
	MENU_CLOCK_END
		
}MENU_CLOCK;

typedef enum _OP_MODE
{	
	OP_MODE_NORMAL		= 0,
	OP_MODE_APPROVAL	= 98,
	OP_MODE_HW_TEST		= 99	
		
}OP_MODE;


typedef enum _TRAIN_POSITION
{
	TRN_NONE = 0,		
	TRN_ENTER,	
	TRN_STOP,	
	TRN_LEAVE,	
	TRN_UNKNOWN			
	
}TRAIN_POSITION;


typedef enum _TAG_MODE
{
	TAG_MODE_BYPASS 		= 0, /* all tag data */
	TAG_MODE_AUTO_RESEND	= 1, /* all tag data, 지연 및 재전송 모드, 불완전 tag정보 복원*/	
	TAG_MODE_TRAIN_INTERLOCK= 2, /* 1st glass, 1st metal, 2nd metal, last glass */
	TAG_MODE_TRAIN_ON	 	= 3, /*            1st metal, 2nd metal, last glass */
	TAG_MODE_END		
	/* Reserved 4, 5, 6, 7, 8, 9 */
	
	//TAG_MODE_PRE_N_ENTER 	= 4, /* 1st glass, 1st metal, 2nd metal    		 	*/			
	//TAG_MODE_METAL 		= 5, /*            1st metal, 2nd metal             */			
	
	
}TAG_MODE;

typedef enum _GRF_TYPE
{
	GRF_TYPE_ST = 0,
	GRF_TYPE_WJ = 1,
	GRF_TYPE_END,
	
	GRF_TYPE_HD
		
}GRF_TYPE;		

typedef enum _TAG_CONV
{
	TAG_CV_NONE			= 0, 	/* 변경없음  */
	TAG_CV_GLASS		= 1,	/* Glass Tag값   */
	TAG_CV_TO_METAL 	= 2,	/* Glass Tag값metal 변경 */
	
	TAG_CV_ERR_SN1 		= 3,
	TAG_CV_ERR_SN2 		= 4,	
	TAG_CV_ERR_ID1 		= 5,
	
	TAG_CV_ERR_ID2 		= 7,
	TAG_CV_ERR_MAKER	= 8,
	TAG_CV_ERR_TRN_CNT	= 9,	/* 차량 편성 량수 에러 */
	
	TAG_CV_MAKER		= 12,	/* 0x0C */
	TAG_CV_ERR_BCD		= 13,	/* 0x0D */
	TAG_CV_METRO_TO_HYOCHOEN = 14,	/* 0x0E : 교통공사 tag값 00 변경for STEN */	

	TAG_ERR_END		
	
}TAG_CONV;

typedef enum _TAG_TYPE
{
	TAG_TYPE_NONE		= 0, 	/* 변경없음  */
	TAG_TYPE_GLASS_DELAY= 1,	
	TAG_TYPE_END		
	
}TAG_TYPE;

typedef enum _LOG_EVENT
{
	LOG_EVENT_RESET		= 0, 	/* RESET  */
	LOG_EVENT_END
	
}LOG_EVENT;


#endif /*__TYPE_H */
