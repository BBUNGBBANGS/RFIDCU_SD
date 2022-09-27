#ifndef __COMMON_H
#define __COMMON_H

#include "stm32f10x.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "type.h"

/*****************************************************************************

									REVISION
	
******************************************************************************/
#define FW_VER_STR  "1.1.5"			// range : 0~9
#define FW_DATE_STR "2022.08.04"	// YEAR, MONTH, DAY

#define __STATION_RELEASE__

/*****************************************************************************

									EEPROM
	
******************************************************************************/

#define DEFAULT_ROM_CODE				(0x01)
#define RFIDCU_SET1_ROM_INIT_CODE		(DEFAULT_ROM_CODE)
#define RFIDCU_SET2_ROM_INIT_CODE		(DEFAULT_ROM_CODE)
#define RFIDCU_TAGCONF_ROM_INIT_CODE	(DEFAULT_ROM_CODE)

/*****************************************************************************/
// 24C04  32page x 16bytes : 512byte 0x00-0x1FF
// 0x00
#define RFIDCU_SET1_ROM_BASE_ADDR		(0x00)	// 0 to 15
// 0x10
#define RFIDCU_SET2_ROM_BASE_ADDR		(0x10)	// 16 to 31
// 0x20
#define RFIDCU_TAGCONF_ROM_BASE_ADDR	(0x20)	// 32 to 48

// 0xFF
#define EEPROM_TEST_ROM_ADDR			(0xFF)

#define RFIDCU_SET1_ROM_SIZE			(16)
#define RFIDCU_SET2_ROM_SIZE			(16)
#define RFIDCU_TAGCONF_ROM_SIZE			(16)

/** Default Setting Value */
#define MAX_RFIDCU_SET_SIZE				(16)

/*****************************************************************************
*																		   
*						RFID  DEFINITION 
*					 
******************************************************************************/
#define RFIDDEFS_DEEFAULT_ATTN()		(25)
#define RFIDDEFS_DEEFAULT_BYPASS()		(OFF)
#define RFIDDEFS_DEEFAULT_BEEP()		(OFF)

/*****************************************************************************
*                                                                          
* 								TEST or DEBUG(DEFINES)
*                    
******************************************************************************/
// DIP SW 신호 관계없이 강제 Enable (TEST용) 
#define DBG_ENABLE_ALL_DIPSW()		(OFF)	

// Flash Memory 로그데이터 디버그 출력
#define DBG_FLOG_PRINT()			(OFF)

// TX to MK packet 현시 
#define DBG_PRINT_PACKET_TO_MK()	(ON)

// RX from MK packet 현시 
#define DBG_PRINT_PACKET_FROM_MK()	(ON)

// 열차 정차 검지 콘솔 출력 
#define DBG_PRINT_TRAIN_POS()		(ON)

// COM1 단자로 Ascii 값으로 시간정보 추가하여 출력함 
#define DBG_COM1_PRINT_ASCII()		(ON)

// 리더기 tag packet중 tagdata만 출력함  
#define DBG_COM1_TAG_PRINT()		(ON)

// COM2 단자로 Ascii 값으로 시간정보 추가하여 출력함 
#define DBG_COM2_PRINT_ASCII()		(ON)

// 리더기 tag packet중 12byte tagdata만 출력함  
#define DBG_COM2_TAG_PRINT()		(ON)


/*****************************************************************************
*                                                                          
* 									공통사항 
*                    
******************************************************************************/

// CPU전압 2.7V 이하 검지시 something기능 구현 
//#define 	__PVD__

/** 
	RFID 
*/

// Use RSSI Protocol
#define RFIDEFS_USE_RSSI_PROTOCOL()				(ON)

// Default APulse Power Setting Mode
#define RFIDEFS_DEFAULT_AP_POWER__MODE()		(ON)

// Default Tag Filter Mode
#define RFIDDEFS_DEFAULT_TAG_MODE()				(TAG_MODE_TRAIN_ON)

// 재부팅시 전원 off/on 실행 
#define RFIDDEFS_POWER_OFF_AFTER_RESET()		(ON)

// 재부팅후 f/w ver. reading
#define RFIDDEFS_FWVER_READ_AFTER_REBOOT()		(OFF)

// 재부팅후 attn 값 설정 
#define RFIDDEFS_ATTN_SET_AFTER_REBOOT()		(ON)


#ifdef __STATION_RELEASE__

	// 99번 태그 수신시 test mode 자동 실행 
	#define RFIDEFS_ENA_TEST_MODE_BY_TAG99()	(ON)	
	
#else 	

	// 99번 태그 수신시 test mode 자동 실행 
	#define RFIDEFS_ENA_TEST_MODE_BY_TAG99()	(OFF)	
	
#endif


/*****************************************************************************

									COMMON
	
******************************************************************************/
// 대기중 Enter버튼 누름시 tag 현시 정보 삭제 됨 
//#define HWDEFS_ENT_KEY_CLR_TAG_ON_FND()		(ON)

// CAN 주/예비 수신 패킷 비교하여 중복처리하지 않음 
#define CANDEFS_COMPARE_NEW_DATA()			(ON)


/*****************************************************************************
*																		   
*								I/O DEFINITION 
*					 
******************************************************************************/
/* (10ms * N) = (Frozen time ) ms */		
#define DEFS_DEFAULT_FROZEN_TIME		10
	
	
#define DEFS_TPU_SENSOR_FROZEN_TIME		20	/* 정위치 검지 센서 입력 */	
//#define DEFS_TPU_SENSOR_FROZEN_OFF_TIME	20	/* 200ms */
//#define DEFS_TPU_SENSOR_FROZEN_ON_TIME	20	/* 200ms */
	
#define DEFS_DIPSW_FROZEN_TIME 			30 
#define DEFS_IDSW_FROZEN_TIME 			50 
#define DEFS_FRONT_SW_FROZEN_TIME 		6
	
	
/* TIMER COUNT VALUE */
#define DEFS_TIME_10MS					10
#define DEFS_TIME_100MS					100
#define DEFS_HW_IN_FROZEN_TIME 			10  
#define DEFS_SDI_FROZEN_TIME 			10  /* 1count per 10ms = 10 * 10 = 100ms*/	

	
/*Logic device h/w level */
#define __led_on						0
#define __led_off						1
	
#define __led_all_on					0x00
#define __led_all_off					0xff
	
#define __fnd_on						1
#define __fnd_off						0
#define __fnd_all_off					0x00

/*****************************************************************************
*                                                                          
* 							Hardware Configuration 
*                    
******************************************************************************/
#define TIMn      		1	/* Max Timer  Unit */
#define UARTn  			5	/* Max USART Unit */
#define CANn			2	/* Max CAN */

#define MODEMn			2	/* Max Modem */


#define SDI_BIN_INPUT_QTY 			1
#define _SDI_SDCARD_INSERT			0

/* LSI	Definition */	
#define LSI_INPUT_QTY					3	
	
#define DEFS_LOGIC_IN_MASK_DEFAULT		0xFF
#define DEFS_LOGIC_IN_MASK_DIPSW		0xF0	


/*****************************************************************************
*                                                                          
* 							TIMER DEFINITION  
*                    
******************************************************************************/
/* Buffer Index :: TIMER[x] Number */
#define TIMER0    			0	/* We use TIM2 to count TIMER0 */
#define TIMER1    			1
#define TIMER2    			2

/*****************************************************************************
*                                                                          
* 							UART DEFINITION  
*                    
******************************************************************************/
/* USART Physical Port Number */
#define COM_GUI				0
#define COM_RFU1			1
#define COM_RFU2			2
#define COM_GRF				3
#define COM_RFID			4


#define UARTDEFS_BAUDRATE_GUI 		(115200)
//#define UARTDEFS_BAUDRATE_RFU1 		(38400)
//#define UARTDEFS_BAUDRATE_RFU2 		(38400)
#define UARTDEFS_BAUDRATE_GRF 		(115200)
#define UARTDEFS_BAUDRATE_RFID 		(115200)

#define UARTDEFS_MAX_FIFO_LEN 		256

/* Serial TX/RX */
#define TIMEOUT_SERIAL_RX_CHAR		(10)

//#define TIMEOUT_COM_LED_BLINK	   	(12) 
#define COM_TIMEOUT_LED_BLINK	   	(12) 

/*****************************************************************************
*																		   
*							CAN DEFINITION 
*					 
******************************************************************************/
/* Buffer Size */
#define CAN_MAX_LIST_SIZE         	(64)		/* Rx message buffer */
//#define CAN_MAX_TX_LIST_SIZE        (8)		/* Tx message buffer */
#define CAN_MAX_TX_LIST_SIZE        (16)		/* Tx message buffer */

#define CAN_MAX_TX_ERROR_COUNT		(255)	/* >= 255 */
#define CAN_MAX_RX_ERROR_COUNT		(128)   /* >= 128 */
	
/* Timer Count	*/

#define CAN_TIMEOUT_LED_BLINK	   	(12) 	//12ms 	
#define CAN_TIMEOUT_NEW_MESSAGE	  	(200) 	// 200ms 	
#define CAN_TIMEOUT_LINK		 	(8000) 	// 8sec 	

#define CAN_MAX_TX_IDLE_TIME		(50)	// 50ms	

/* #define CAN_BAUDRATE  1000*/ /* 1MBps   */
/* #define CAN_BAUDRATE  500*/  /* 500kBps */
/* #define CAN_BAUDRATE  250*/  /* 250kBps */
#define CAN_BAUDRATE  125    /* 125kBps */
/* #define CAN_BAUDRATE  100*/  /* 100kBps */ 
/* #define CAN_BAUDRATE  50*/   /* 50kBps  */ 
/* #define CAN_BAUDRATE  20*/   /* 20kBps  */                        
/* #define CAN_BAUDRATE  10*/   /* 10kBps  */ 


//#define CAN_PORT_LCU_M		0
//#define CAN_PORT_LCU_S		1

#define CAN_PORT_1			0
#define CAN_PORT_2			1

#define CAN_PORT_ALL		0xff

/*****************************************************************************
*                                                                          
* 								GPIO Pin Definition 
*                    
******************************************************************************/
/* PORTA IO Definetion --------------------------------------------------------*/
#define LED_A0	 						  GPIO_Pin_0
#define LED_A1	 						  GPIO_Pin_1
#define USART2_TX 						  GPIO_Pin_2
#define USART2_RX 						  GPIO_Pin_3
#define SPI_nCS							  GPIO_Pin_4
#define SPI_SCK 						  GPIO_Pin_5
#define SPI_MISO 						  GPIO_Pin_6
#define SPI_MOSI 						  GPIO_Pin_7

#define LED_A2	 						  GPIO_Pin_8
#define USART1_TX 						  GPIO_Pin_9
#define USART1_RX 						  GPIO_Pin_10
#define nRFU1_ON						  GPIO_Pin_11
#define nRFU2_ON						  GPIO_Pin_12
//#define  SWDIO							GPIO_Pin_13
//#define  SWCLK							GPIO_Pin_14

#define nLED_RUN                          GPIO_Pin_15
#define SPI_DLY	    					  GPIO_Pin_15


/* PORTB IO Definetion --------------------------------------------------------*/
#define nRST_ST                           GPIO_Pin_0
#define nCS_RD_IDSW 		              GPIO_Pin_1
//#define  BOOT1							GPIO_Pin_2
#define nINP_PBA_SEL0					  GPIO_Pin_3	
#define nINP_SD_INSERT					  GPIO_Pin_4	
#define nSEL_SD_MEM						  GPIO_Pin_5	
#define I2C_SCL	 						  GPIO_Pin_6
#define I2C_SDA	 						  GPIO_Pin_7

#define CAN1_RX                           GPIO_Pin_8
#define CAN1_TX                           GPIO_Pin_9
#define USART3_TX 						  GPIO_Pin_10
#define USART3_RX 						  GPIO_Pin_11
#define CAN2_RX                           GPIO_Pin_12
#define CAN2_TX                           GPIO_Pin_13
#define nCS_RD_DIPSW                      GPIO_Pin_14
#define nCS_RD_PUSHSW	                  GPIO_Pin_15


/* PORTC IO Definetion --------------------------------------------------------*/
#define nDIN0	 						  GPIO_Pin_0
#define nDIN1	 						  GPIO_Pin_1
#define nDIN2	 						  GPIO_Pin_2
#define nDIN3	 						  GPIO_Pin_3
#define nDIN4	 						  GPIO_Pin_4
#define nDIN5	 						  GPIO_Pin_5
#define nDIN6	 						  GPIO_Pin_6
#define nDIN7	 						  GPIO_Pin_7

#define nLED_LAT						  GPIO_Pin_8
#define nOE_WR	  				      	  GPIO_Pin_9
#define UART4_TX 						  GPIO_Pin_10
#define UART4_RX 						  GPIO_Pin_11
#define UART5_TX 						  GPIO_Pin_12
#define nRFID_ON						  GPIO_Pin_13

#define XTAL_32768KHZ_0					  GPIO_Pin_14
#define XTAL_32768KHZ_1					  GPIO_Pin_15

/* PORTD IO Definetion---------------------------------------------------------*/
#define UART5_RX 						  GPIO_Pin_2


/*****************************************************************************
*                                                                          
* 							Hardware Configuration (LOGIC I/O)
*                    
******************************************************************************/
typedef enum _DeviceSelType 
{
	/* INPUTS */
	SEL_RD_DIPSW = 0,
	SEL_RD_IDSW,
	SEL_RD_SDI,		

	/* OUTPUTS */	
	SEL_WR_LED_RF_CAN,	
	SEL_WR_LED_TAG_TPS,
	SEL_WR_FND1,
	SEL_WR_FND2,
	SEL_WR_FND_SEL,	
	
	NOT_SEL 			
}DeviceSelType;

/*****************************************************************************
*                                                                          
* 									PUBLIC VARIABLES
*                    
******************************************************************************/
/*===================================================================		  
	
						Binary Data Input /Output
											
=====================================================================*/

typedef union _BYTE_VAL{
	u8 byte;
	struct {
		u8 b0:1;
		u8 b1:1;
		u8 b2:1;
		u8 b3:1;
		u8 b4:1;
		u8 b5:1;
		u8 b6:1;
		u8 b7:1;
	}bits;
		
}BYTE_VAL, BYTE_BITS;


/*===================================================================		  	

							SINGLE DATA INPUT									
							
=====================================================================*/

typedef struct	_PARAM_SDI
{	
	u8 BinInputOld[SDI_BIN_INPUT_QTY];	
	u32 FrozenCntrs[SDI_BIN_INPUT_QTY]; 
}PARAM_SDI;


typedef struct	_SDIO_STATUS
{
	u8 SDCARD_Insert   			: 1;  	
	
}SDIO_STATUS;


typedef struct	_PARAM_SDIO
{	
	PARAM_SDI	Sdi;
	SDIO_STATUS NewData; /* save new input after checking chattering */

}PARAM_SDIO;


/*===================================================================		  	

							DIP SWITCH INPUT									
							
=====================================================================*/

typedef struct	_DIP_SWITCH
{	
	BYTE_VAL BinInputOld;	
	u32 FrozenCntrs[8];	
}DIP_SWITCH;

/**
	Configuration Setting  Input
*/
typedef struct	_PARAM_SWITCH
{	
	/*LSB(B0) */		
	//u8 bSET1:1;
	u8 DIP_N_A					: 4;	
		
	u8 DIP1_UseSDI12			: 1;	/* ON: nn 사용 */
	u8 DIP2_UseSDI34			: 1;	/* ON: nn 사용 */
	u8 DIP3_UseCANInterLock		: 1;	/* ON: nn 사용 */
	u8 DIP4_UseComMonitoring	: 1;	/* ON: nn 사용 */
	/*MSB(B7) */
}PARAM_SWITCH;

/*===================================================================		  	

							LSI Data Input 											
							
=====================================================================*/
typedef struct	_PARAM_LDI
{	
	BYTE_VAL 	BinInputOld;	
	u32 		FrozenCntrs[8];	
	
}PARAM_LDI;


/**
	DCU ID Setting Switch Input 
*/
typedef union _uID_SWITCH{

	u8	byte;	
	struct{			

		u8 ID_Low					: 4;	
		u8 ID_High					: 4;			
		
	}bits;
	
} uID_SWITCH;


/**
	Front Push Buttons Input
*/
typedef union _uFRONT_BUTTONS{

	u8	byte;	
	struct{
			
		u8 MenuBtn					: 1;
		u8 UpBtn					: 1;
		u8 DownBtn					: 1;
		u8 EnterBtn 				: 1;	/* LampTest */
		
		u8 TrnEntSensor1			: 1;	/* Train Enter Sensor1 */
		u8 TrnEntSensor2			: 1;	/* Train Enter Sensor2 */
		u8 TrnStopSensor1			: 1;	/* Train Stop Sensor1 */
		u8 TrnStopSensor2			: 1;	/* Train Stop Sensor2 */		
		
	}bits;
} uFRONT_BUTTONS;


/**
	LED_RF_CAN
*/

typedef union _U_LED_RF_CAN{
	u8 byte;
	struct{	
		
		
		u8 Ld_RF1_TX	: 1;
		u8 Ld_RF1_RX	: 1;
		u8 Ld_RF2_TX	: 1;
		u8 Ld_RF2_RX	: 1;		

		
		u8 Ld_CAN1_TX	: 1;
		u8 Ld_CAN1_RX	: 1;
		u8 Ld_CAN2_TX	: 1;
		u8 Ld_CAN2_RX	: 1;		

	}bits;
}uLED_RF_CAN;


/**
	LED_TAG_TPS
*/

typedef union _U_LED_TAG_TPS{
	u8 byte;
	struct{	
		
		
		u8 Ld_TAG_RX	: 1;
		u8 Ld_TAG_ALARM	: 1;
		
		u8 Ld_TRN_ENT1  : 1;
		u8 Ld_TRN_ENT2  : 1;
		u8 Ld_TRN_STOP1 : 1;
		u8 Ld_TRN_STOP2 : 1;

		u8 Reserved		: 2;		

	}bits;
}uLED_TAG_TPS;


/**
	FND_SEL
*/
typedef union _uFND_SEL{
	u8	byte;	
	
	struct{
		
		u8 Fnd_Dig1				: 1;	/* (LOW) FND2, DIGIT 10*/
		u8 Fnd_Dig2				: 1;	/* (LOW) FND2, DIGIT 1*/
		
		u8 Fnd_Dig3				: 1;	/* (HIGH) FND1, DIGIT 10*/
		u8 Fnd_Dig4				: 1;	/* (HIGH) FND1, DIGIT 1*/ 
		
		u8 Reserved				: 4;	
		
	}bits;
} uFND_SEL;


/**
	Front Status 
*/

typedef struct _sGR_OUT{	

	uLED_RF_CAN		LedRF_CAN;	
	uLED_TAG_TPS	LedTAG_TPS;
	uFND_SEL		FndSel;
	
} sGR_OUT;


/**
	Timer 관련
*/
typedef struct _sTIMER_CMD{

	
	u32 period_2sec;    	/**< 2sec */
    u32 period_500ms;       /**< 500ms */	
	u32 period_200ms;		/**< 200ms */
	u32 period_10ms;        /**< 10ms */	
	u32 period_10ms_tpu;    /**< 10ms */
	
	u32 period_rtc_100ms;

	u32 com_link_checktime; /**< 8sec */
	u32 FndDisTime;
	u32 FndBlinkTime;
	u32 FndDisSETtag;

   	u32 timeout_tag_rx_led;
} sTIMER_CMD;



/*===================================================================
*                                                                          
* 								UART FIFO
*                    
===================================================================*/
typedef struct _UART_FIFO
{
	u8 buffer[UARTDEFS_MAX_FIFO_LEN];
	vu32 write;	// receive
	vu32 read;			
}UART_FIFO;


/*****************************************************************************
*                                                                          
* 							PUBLIC VARIABLES
*                    
******************************************************************************/
// Switch OFF = 1, On = 0
typedef struct	_PARAM_RF4U
{	/*LSB(B0) */					/*DipSwitch On is 0 */

	u8 bNoUseRFID		: 1;			/* (1) RFID (X) / (0)RFID(O)  */	
	u8 bFrontNetworkID	: 1;	/* (1) FRONT ID */ 
	u8 bNoUseGRF3		: 1;	/* 0 : 중앙부 설치 */
	u8 ManChGroup 		: 2;
	//u8 b2WayLine:1;	/* 2LINE or 4LINE (0)	*/	
	//u8 bLowLine:1;	/* 하행 / 상행	  (0)	*/
	//u8 bNo1PLF:1; /* 1승강장 / 2승강장  */			
	u8 RF_Attn			: 2;  /* RFU ATTN 0 ~ 7 */ 
	u8 RemoteConfMode	: 1;
	//u8 nDBG_POWER:1;	
	/*	DIPSW-ON(0)  : SET_RF_CH+POWER(0xA3) : RFCU has a same power as RFCVT
		OFF(1) : SET_RF_CH(0xA0)	*/	
		
	/*MSB(B7) */
}PARAM_RF4U;


typedef struct _PgmEnvSTR
{
	struct{
		/*LSB(B0) */			
		u8 Reserved:5;	/* RF-ID Tag Number */
		
		u8 Rf:1;	/* request message */	
		u8 Alarm:1;
		u8 RfId:1;		
		/*MSB(B7) */	
	}Head;
	
	struct{
		/*LSB(B0) */			
		u8 RouteNo:3;
		u8 Reserved:1;
		u8 CarCount:4;		
		/*MSB(B7) */	
	}RfIdRouteNo;

	struct{
		/*LSB(B0) */			
		u8 OrgNo:7;
		u8 CarNo:1; 	
		/*MSB(B7) */	
	}RfIdCarNo;

	struct{
		/*LSB(B0) */ 
		u8 Modem0:1;
		u8 Modem1:1;		
		u8 Modem2:1;
		u8 Modem3:1;		

		//u8 Modem4:1; /* N/A */
		//u8 Modem5:1; /* N/A */
		u8 Dip1_EnRFID:1;		// V1.4.9
		u8 Dip8_ManConf:1;		// V1.4.9
		
		u8 RfidReader:1;
		u8 Restart:1;		
		/*MSB(B7) */	
	}Alarm;
	
	struct{
		/*LSB(B0) */ 
		u8 rf_freq:4;
		u8 ModemNo:3;		
		u8 Reserved:1;
		/*MSB(B7) */	
	}Rf_Ch;
	
	struct{
		/*LSB(B0) */ 
		u8 RouteNo:3;		
		u8 Reserved:1;
		u8 CarType:4;
		/*MSB(B7) */	
	}RfRouteNo;
	
	struct{
		/*LSB(B0) */ 
		u8 OrgNo:7; 	
		u8 CarNo:1;
		/*MSB(B7) */	
	}RfCarNo;
	
	struct{
		/*LSB(B0) */		
		u8 LeftOpen:1;		
		u8 RightOpen:1;
		u8 Manual:1;	
		u8 DoorLamp:1;

		u8 MakerNo: 4;		
		/*MSB(B7) */	
	}RfCarStatus;

	/* add other valuables under line------------------------------------------ */
	
	u8 RfCh[4][2];		
}PgmEnvSTR;


/*****************************************************************************
*                                                                          
* 									HEADER
*                    
******************************************************************************/

#include "crc.h"
#include "util.h"
#include "CAN.h"
#include "EEPROM.h"
#include "FlashMem.h"
#include "SD_Card.h"

#include "Front.h"
#include "rfcom.h"
#include "rfid.h"
#include "grf.h"
#include "MyTpu.h"
#include "MyTag.h"

#include "CpuRtc.h"
#include "AppLog.h"

#include "AppMain.h"

/*****************************************************************************
*                                                                          
* 							PUBLIC VARIABLES(Export)
*                    
******************************************************************************/	
extern 	USART_TypeDef* COM_PORT[UARTn];

extern	u32 		Timer[TIMn];	/* Time Base Counter */
extern	UART_FIFO	sio_tx[UARTn];	/* UART TX Buffer */
extern	UART_FIFO	sio_rx[UARTn];	/* UART RX Buffer */
	
extern	PARAM_LDI		LogicInput[LSI_INPUT_QTY]; 
extern	PARAM_SDIO		m_SDIO;
	
extern	uFRONT_BUTTONS	FrontBtn, FrontBtnOld;
	
extern	DIP_SWITCH		DipSwitch;
extern	PARAM_SWITCH	m_Config;
	
	/* Outputs */	
extern	sGR_OUT 		Xout, XoutOld;		
	
extern	sTIMER_CMD		TimerCount;
	
	
extern	u8 MyPCBNumber;
extern	u8 MyDeviceID;
extern	u8 IdSwitchOld;
	
extern OP_MODE 	OperationMode;
extern u8    	TagMode;
extern u8 		GRFTypeCode;

	
extern	bool bE2PROM_InitOK;

extern u32 BootCount;

extern PgmEnvSTR  m_PgmEnv; 	/* CAN Tx Message */

extern bool bRFIDBootCheckTime;

extern bool 	bFirstBootFlag;
extern u32		FirstBootTimer;

extern u8 		rfidMinAttnValue;


/* DEBUG Varibles     ---------------------------------------------------------*/

/*****************************************************************************
*                                                                          
*							FUNCTION DEFINITIONS(EXTERN)
*                    
******************************************************************************/


void Serial_PutChar(uint8_t c);
void Serial_PutString(uint8_t *s);
void Delay(uint32_t nTime);
void TimingDelay_Decrement(void);	
void assert_failed(uint8_t* file, uint32_t line);
void Clear_WatchDog(void);
u8 Toggle_RunLED(void);
u32 GetTickCount (u8 TimerNumber );

void RESET_TIMER(u32 *pTimerCounter);
u32 m_Elapsed (u8 TimerNumber, u32 FormerTime);
u32 getTimeElapsed(u32 currentTime, u32 previousTimeTag);
void Delay_ms (u32 remainingTime);
bool IS_TIMEOUT(u32 *pTimerCounter, u32 MaxTimeLimit);

bool CHK_TOV(u32 TimerCounter, u32 MaxTimeLimit);
void SET_UART_TX_IT_CONFIG(u8 COMx, u8 state);
void SET_UART_RX_IT_CONFIG(u8 COMx, u8 state);
void UART_TxBytes(u8 COMx, u8 *str, u32 cnt);
void UART_CheckSend(u8 COMx);
void SET_OUT_EN(u8 state);
void Disable_InOutDevCsPin(void);
void inout_sel(DeviceSelType Device);
u8 Read_Data(DeviceSelType Device);
void Write_Data(DeviceSelType Device, u8 data);
bool IsChanged_LogicInput(DeviceSelType Device);
bool IsChanged_Sdi_Index(u8 pointNum);
void WRITE_XOUT(DeviceSelType Device);

#endif /*__COMMON_H */
