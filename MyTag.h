#ifndef __MYTAG_H__
#define __MYTAG_H__

#include "Common.h"


/*****************************************************************************
*                                                                          
* 							LITERAL CONSTANTS(DEFINES)
*                    
******************************************************************************/
// Fnd ���ýð� (default 90sec)
#define TAGDEFS_TAG_DIS_ON_TIME					(u32)(90000)

// 1�� 30�ʰ� ������ 
#define TAGDEFS_TEST_TAG_DIS_ON_TIME			(u32)(90000)

// tag ���ž����� 30�� �Ŀ� �ڵ����� �� 
#define TAGDEFS_TEST_TAG_TIMEOUT				(u32)((60000)*(30))

// �� tag�� �ƴ��� Ȯ���ϴ� �ð� (���������ð� + 0.4s)
#define TAGDEFS_NOT_MY_TAG_CONFIRM_TIME			(u32) (TPUDEFS_TRAIN_ENTER_CHECK_TIME+(400))

// tag resend periodic time
#define TAGDEFS_TAG_RESEND_PERIOD				(u32)(50)

// tag delay resend time
#define TAGDEFS_TAG_DELAY_RESEND_TIME			(u32)(2000)

// tag delay resend time
#define TAGDEFS_TAG_DELAY_RESEND_TIME_ST		(u32)(3000)


// Glsstag ������ metal tag ���� ���ð� 
#define TAGDEFS_FIRST_METAL_TAG_WAIT_TIME		(u32)(5000)


// �±� ������ Ƚ�� 
#define TAGDEFS_TAG_TX_REPEAT_COUNT()			(2)

// 2�� ������, ������ ȸ�� 
#define TAGDEFS_TAG_DELAY_TX_REPEAT_COUNT()		(2)


#define TRF_MAKER_NUM_HYOCHON	0
#define TRF_MAKER_NUM_ROTTEM	1
#define TRF_MAKER_NUM_METRO		4
#define TRF_MAKER_NUM_HD_EV		7
#define TRF_MAKER_NUM_END		9
#define TRF_MAKER_NUM_NONE		0xff


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
bool TAG_IsBypassMode(void);
bool TAG_IsTrainOn(void);
bool TAG_IsGlassTag(u8 *data);
bool TAG_IsMetalTag(u8 *data);
bool TAG_IsBcdError(u8 *data);
bool TAG_IsMyGlassTag(TAG_TXD *pTag);
bool TAG_IsUnknownTagType(TAG_TXD *pTag);
bool TAG_IsResend(TAG_TXD *pTag);
void TAG_UpdatePreEnterTagData(void);
bool TAG_ConvToSTWJtagData(TAG_TXD *pTag);
bool TAG_CheckNotMyTag(void);
void TAG_SetTrainNoneMark(void);
void TAG_UpdatePreEnterMyTag(void);
bool TAG_IsTrainNoneTagRxed(void);

void TAG_BypAutoCmd(u8 *pRcvTag);
void TAG_TrainOnCmd(u8 *pRcvTag);
void TAG_TrainInterlockCmd(u8 *pRcvTag);

void TAG_AutoResend(void);
void TAG_TrainOnResend(void);
void TAG_IsDispOn(void);

#endif /*__MYTAG_H */
