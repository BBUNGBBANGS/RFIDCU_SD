#ifndef __APPMAIN_H__
#define __APPMAIN_H__

#include "Common.h"


/*****************************************************************************
*                                                                          
* 							LITERAL CONSTANTS(DEFINES)
*                    
******************************************************************************/


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

bool IS_OP(u8 OP_Type);
u8 IsUseEnterSensor(void);
u8 IsUseStopSensor(void);
u8 IsUseCANInterlock(void);
u8 IsUseComMonitoring(void);

void PRINT_DIPSW_CONFIG(void);
void IDSW_Read(u8 command);
bool DIPSW_Read(u8 command);

void E2P_Test(void);
void E2P_SaveRfidAttn(void);
void E2P_SaveTagBypassMode(void);
void E2P_SaveRfidBeep(void);
void E2P_SaveApPowrMode(void);

void DefaultSetting(u8 command);
void FactoryDefault(void);
void LoadSetting(u8 command);
void SysInit(void);
void App_InitVal(void);
void AppMain(void);

#endif  /*__APPMAIN_H__*/
