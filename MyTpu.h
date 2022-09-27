#ifndef __MYTPU_H__
#define __MYTPU_H__

#include "Common.h"


/*****************************************************************************
*                                                                          
* 							LITERAL CONSTANTS(DEFINES)
*                    
******************************************************************************/
/** 
	TPU
*/

// 열차 진입 확인 카운트 
// 1count per 10ms : ex) 50 -> 50*10ms = 500ms
// sensor ON(0.2sec) + count(0.5sec) -> TRN_ENTER(0.7sec), 
// (note) 1st metal tag (1sec) after the enter sensor on
#define TPUDEFS_TRAIN_ENTER_CHECK_10MS_COUNT	(50)
#define TPUDEFS_TRAIN_ENTER_CHECK_TIME			(u32)(TPUDEFS_TRAIN_ENTER_CHECK_10MS_COUNT*10)

// 5Km/h 기준 off time = 300ms(max.)
// sensor OFF(0.2s) + count(0.5s) -> TRN_STOP(0.7sec)
#define TPUDEFS_TRAIN_STOP_CHECK_10MS_COUNT		(50)

// trn_enter -> unknown
#define TPUDEFS_TRAIN_UNKNOWN_CHECK_10MS_COUNT1	(80)

// trn stop -> unknown
#define TPUDEFS_TRAIN_UNKNOWN_CHECK_10MS_COUNT2	(300)

// trn_unknown -> trn_none
#define TPUDEFS_TRAIN_NONE_CHECK_10MS_COUNT		(300)

// 10량 전동차 최소 진입시간 20초 trn_enter time
#define TPUDEFS_MIN_TRAIN_ENTER_TIME_CAR10		(20000)

// ex) 10량 24s, 8량 21s, 6량 20s, (4량제외)
#define TPUDEFS_MIN_TRAIN_ENTER_TIME_CAR6		(18000)

// 4량 전동차 최소 진입시간 16초 trn_enter time
#define TPUDEFS_MIN_TRAIN_ENTER_TIME_CAR4		(16000)


// 10량 전동차 최대 진입시간 40초 trn_enter time
#define TPUDEFS_MAX_TRAIN_ENTER_TIME_CAR10		(40000)

// 열차정차후, 개폐연동 및 진출까지 30s + 30s = 60s 대기함   (trn_stop + trn_leave)
#define TPUDEFS_MIN_TRAIN_NONE_WAIT_TIME		(60000)

// 정차시간 경과후 열차 검지 유지 확인 
#define TPUDEFS_MAX_TRAIN_NONE_WAIT_TIME		(u32)((TPUDEFS_MIN_TRAIN_NONE_WAIT_TIME)+(3000))



/*****************************************************************************
*                                                                          
* 								TYPE DEFINITIONS
*                    
******************************************************************************/


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


/*****************************************************************************
*                                                                          
*							FUNCTION DEFINITIONS(EXTERN)
*                    
******************************************************************************/
void TPU_InitVal(void);
u8 TPU_GetEnterSensor(u8 command);
u8 TPU_GetStopSensor(u8 command);
TRAIN_POSITION TPU_GET_POS(void);
void TPU_SAVE_POS(TRAIN_POSITION position);
u32 TPU_GetTrainOnTime(void);
u32 TPU_GetTrainOffTime(void);
u32 TPU_GetTrainEnterTime(void);
bool TPU_Process(void);

#endif /*__MYTPU_H */
