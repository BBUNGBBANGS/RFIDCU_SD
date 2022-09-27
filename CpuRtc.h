#ifndef __CPURTC_H__
#define __CPURTC_H__

#include "Common.h"


/*****************************************************************************
*                                                                          
* 							LITERAL CONSTANTS(DEFINES)
*                    
******************************************************************************/
/* RTC Clock/64 is output on tamper pin(PC.13) */
//#define	MRTC_RTCClockOutput_Enable 

//#define MRTC_FlashUnlock_Enable
//#define MRTC_RTCClockOutput_Enable


/* Exported constants --------------------------------------------------------*/
#if 0
#define BATTERY_REMOVED 	98
#define BATTERY_RESTORED 	99

#define CONFIGURATION_DONE 	0xAAAA
#define CONFIGURATION_RESET 0x0000
#define OCTOBER_FLAG_SET 	0x4000
#define MARCH_FLAG_SET 		0x8000

#define DEFAULT_DAY 		20
#define DEFAULT_MONTH 		6
#define DEFAULT_YEAR 		2022
#define DEFAULT_HOURS 		9
#define DEFAULT_MINUTES 	0
#define DEFAULT_SECONDS 	0
#endif

#define SECONDS_IN_DAY 		(u32)(86399)

//(note) RTC_Count (0 to 86399), count = 86400, 23:59:59 = 86399, 24:00:00 = 86400
#define RTC_SECONDS_IN_DAY 	(u32)(86400)


#define LEAP 				1
#define NOT_LEAP 			0

/*****************************************************************************
*                                                                          
* 								TYPE DEFINITIONS
*                    
******************************************************************************/
/* Time Structure definition */
typedef struct _Time_s
{
  uint8_t SecLow;
  uint8_t SecHigh;
  uint8_t MinLow;
  uint8_t MinHigh;
  uint8_t HourLow;
  uint8_t HourHigh;
}Time_s;



/* Date Structure definition */
typedef struct _Date_s
{
  uint8_t Month;
  uint8_t Day;
  uint16_t Year;
}Date_s;




/*****************************************************************************
*                                                                          
* 								TYPE DEFINITIONS(LOCAL)
*                    
******************************************************************************/


/*****************************************************************************
*                                                                          
* 								PUBLIC VARIABLES
*                    
******************************************************************************/
extern	vu8 RTC_UpdateFlag;
extern	vu8 RTC_100msCnt;

extern Time_s s_TimeStructVar;
extern Date_s s_DateStructVar;


/*****************************************************************************
*                                                                          
*							FUNCTION DEFINITIONS(EXTERN)
*                    
******************************************************************************/

u8 MRTC_CheckLeapYear(u16 Year);
u8 MRTC_GetMaxDay(void);
u16 MRTC_GetYear(void);
u8 MRTC_GetMonth(void);
u8 MRTC_GetDay(void);
u8 MRTC_GetHour(void);
u8 MRTC_GetMin(void);
u8 MRTC_GetSec(void);
u8 MRTC_Get100mSec(void);
bool MRTC_SetMenuDate(u16 year, u8 month, u8 day);
bool MRTC_SetMenuTime(u8 hour, u8 minute, u8 sec);

void Time_Adjust(uint32_t Tmp_HH, uint32_t Tmp_MM, uint32_t Tmp_SS);
void Date_Adjust(u16 yy,u8 mm,u8 dd);
void MRTC_SetClock(u8 *rx_d, u8 StationCommand);
void MRTC_CalculateTime(void);
void MRTC_DisplayDataAndTime(u8 command);
void MRTC_InitClockVal(void);
void MRTC_IncrementDate(void);
void MRTC_CheckDayElapsed(void);
void MRTC_Configuration(void);
void MRTC_InitVal(void);
void MRTC_Init(void);


#endif /*__CPURTC_H */
