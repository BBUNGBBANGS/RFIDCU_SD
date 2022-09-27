#ifndef __APPLOG_H__
#define __APPLOG_H__

#include "Common.h"


/*****************************************************************************
*                                                                          
* 							LITERAL CONSTANTS(DEFINES)
*                    
******************************************************************************/
#define RECORD_TEST_ADDR		(u32)(0x0000)  
#define RECORD_INIT_NUM			(u32)(1)  
#define RECORD_MAX_NUM			(u32)(104856)  
#define RECORD_SIZE				(20)
#define RECORD_COUNT_INFO_SIZE	(5)


/*****************************************************************************
*                                                                          
* 								TYPE DEFINITIONS
*                    
******************************************************************************/

typedef struct {

	u32 RecordPosition;
	u8 RecordFull;

}sLOG_INFO;


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
void LOG_printEvent(u8 COMx, LOG_EVENT code, u8 data);
void LOG_printTAGConfirm(u8 COMx);
void LOG_printTPUPos(u8 COMx);
void LOG_printRFIDtag(u8 *data, bool bMetalTag, bool bBCDError);

void LOG_InitVal(void);
void LOG_DevSel(u8 UseFlashMemory);
void LOG_EraseRecPos(void);
u32 LOG_GetRecPos(void);
void LOG_WriteData(const u8 *pBuffer, u32 NumByteToWrite);
void LOG_ReadRecord(u32 RecordNumber);
void LOG_Process(void);

#endif /*__APPLOG_H */
