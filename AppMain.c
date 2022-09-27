#include "Common.h"

/*****************************************************************************
*                                                                          
* 							LITERAL CONSTANTS(DEFINES)
*                    
******************************************************************************/

/*****************************************************************************
*                                                                          
* 							TYPE DEFINITIONS(LOCAL)
*                    
******************************************************************************/

/*****************************************************************************
*                                                                          
* 							PUBLIC VARIABLES
*                    
******************************************************************************/



/*****************************************************************************
*                                                                          
* 							LOCAL VARIABLES
*                    
******************************************************************************/

/*****************************************************************************
*                                                                          
* 						FUNCTION DEFINITIONS(LOCAL)
*                    
******************************************************************************/	

/*==================================================================
* Function Name 	: IS_TEST_MODE
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
bool IS_OP(u8 OP_Type)
{
	bool bFlag = FALSE;
	
	switch(OP_Type)
	{
		case OP_MODE_NORMAL:
		{
			if(OperationMode == OP_MODE_NORMAL)	 bFlag = TRUE;
			break;
		}	

		case OP_MODE_HW_TEST:
		{
			if(OperationMode == OP_MODE_HW_TEST) bFlag = TRUE;
			break;
		}	

		case OP_MODE_APPROVAL:
		{
			if(OperationMode == OP_MODE_APPROVAL) bFlag = TRUE;
			break;
		}	
		
	}

	if(bFlag) 	return(TRUE);
	else		return(FALSE);	
}

/*==================================================================
* Function Name 	: PRINT_DIPSW_CONFIG
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void PRINT_DIPSW_CONFIG(void)
{		
	if(m_Config.DIP1_UseSDI12)			myputsln("SW1( ON ): SDI-1/2 - ON");
	else								myputsln("SW1(OFF ): SDI-1/2 - OFF");

	if(m_Config.DIP2_UseSDI34)			myputsln("SW2( ON ): SDI-3/4 - ON");
	else								myputsln("SW2(OFF ): SDI-3/4 - OFF");

	if(m_Config.DIP3_UseCANInterLock)	myputsln("SW3( ON ): CAN Interlock - ON");
	else								myputsln("SW3(OFF ): CAN Interlock - OFF");

	if(m_Config.DIP4_UseComMonitoring)	myputsln("SW4( ON ): COM Monitoring - ON");
	else								myputsln("SW4(OFF ): COM Monitoring - OFF");	
}


/*==================================================================
* Function Name 	: PRINT_DIPSW_CONFIG
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void PRINT_FILTER_MODE(void)
{		
//	myputsln(" ");
	
	switch(TagMode){

		case TAG_MODE_BYPASS:				myputsln("#TAG MODE : TAG_MODE_BYPASS"); 			break;
		case TAG_MODE_AUTO_RESEND:			myputsln("#TAG MODE : TAG_MODE_AUTO_RESEND");		break;		
		case TAG_MODE_TRAIN_INTERLOCK: 		myputsln("#TAG MODE : TAG_MODE_TRAIN_INTERLOCK");	break;
		//case TAG_MODE_PRE_N_ENTER: 			myputsln("#TAG MODE : TAG_MODE_PRE_N_ENTER");		break;		
		case TAG_MODE_TRAIN_ON: 			myputsln("#TAG MODE : TAG_MODE_TRAIN_ON");			break;		
		//case TAG_MODE_METAL: 				myputsln("#TAG MODE : TAG_MODE_METAL");				break;		
		default:							myputsln("#TAG MODE : **UNKNOWN**"); 				break;		
	}	
}

/*==================================================================
* Function Name 	: PRINT_GRF_TYPE
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void PRINT_GRF_TYPE(void)
{		
//	myputsln(" ");
	
	switch(GRFTypeCode){

		case GRF_TYPE_ST:		myputsln("#GRF TYPE : GRF_TYPE_ST"); 				break;
		case GRF_TYPE_WJ:		myputsln("#GRF TYPE : GRF_TYPE_WJ"); 				break;
		case GRF_TYPE_HD:		myputsln("#GRF TYPE : GRF_TYPE_HD"); 				break;
		default:				myputsln("#GRF TYPE : **UNKNOWN**"); 				break;
		
	}	
}



/*==================================================================
* Function Name 	: ResetEnterSensor
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void ResetEnterSensor(void)
{
	
	uFRONT_BUTTONS *pOld = &FrontBtnOld;				

	if(IsUseEnterSensor()){		

		(pOld->bits.TrnEntSensor1)? (Xout.LedTAG_TPS.bits.Ld_TRN_ENT1 = __led_on) : \
			 						(Xout.LedTAG_TPS.bits.Ld_TRN_ENT1 = __led_off);

		(pOld->bits.TrnEntSensor2)? (Xout.LedTAG_TPS.bits.Ld_TRN_ENT2 = __led_on) : \
			 						(Xout.LedTAG_TPS.bits.Ld_TRN_ENT2 = __led_off);
	}
	else{

		Xout.LedTAG_TPS.bits.Ld_TRN_ENT1 = __led_off;
		Xout.LedTAG_TPS.bits.Ld_TRN_ENT2 = __led_off;
	}	
}

/*==================================================================
* Function Name 	: ResetStopSensor
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void ResetStopSensor(void)
{
	
	uFRONT_BUTTONS *pOld = &FrontBtnOld;								

	if(IsUseStopSensor()){		

		(pOld->bits.TrnStopSensor1)? (Xout.LedTAG_TPS.bits.Ld_TRN_STOP1 = __led_on) : \
			 						(Xout.LedTAG_TPS.bits.Ld_TRN_STOP1 	= __led_off);

		(pOld->bits.TrnStopSensor2)? (Xout.LedTAG_TPS.bits.Ld_TRN_STOP2= __led_on) : \
			 						(Xout.LedTAG_TPS.bits.Ld_TRN_STOP2 	= __led_off);
	}
	else{

		Xout.LedTAG_TPS.bits.Ld_TRN_STOP1 	= __led_off;
		Xout.LedTAG_TPS.bits.Ld_TRN_STOP2 	= __led_off;
	}	
}


/*==================================================================
* Function Name 	: IsUseEnterSensor
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
u8 IsUseEnterSensor(void)
{
	if(DBG_ENABLE_ALL_DIPSW() == ON)				return(ON);	
	else{		
		
		if(m_Config.DIP1_UseSDI12 == DIP_SW_USED)	return(ON);
		else										return(OFF);	
	}
}

/*==================================================================
* Function Name 	: IsUseStopSensor
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
u8 IsUseStopSensor(void)
{	
	if(DBG_ENABLE_ALL_DIPSW() == ON)				return(ON);	
	else{		

		if(m_Config.DIP2_UseSDI34 == DIP_SW_USED)	return(ON);
		else										return(OFF);	
	}
}


/*==================================================================
* Function Name 	: IsUseCANInterlock
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
u8 IsUseCANInterlock(void)
{
	
	if(DBG_ENABLE_ALL_DIPSW() == ON)						return(ON); 
	else{		

		if(m_Config.DIP3_UseCANInterLock == DIP_SW_USED)	return(ON);
		else												return(OFF);	
	}
}


/*==================================================================
* Function Name 	: IsUseComMonitoring
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
u8 IsUseComMonitoring(void)
{
	if(DBG_ENABLE_ALL_DIPSW() == ON)						return(ON); 
	else{		
		
		if(m_Config.DIP4_UseComMonitoring == DIP_SW_USED)	return(ON);
		else												return(OFF);	
	}
}

/*==================================================================
* Function Name 	: IDSW_Read
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void IDSW_Read(u8 command)
{
	uID_SWITCH Id;
	u8 NewId;

	if(command == _check){
		
		if(IsChanged_LogicInput(SEL_RD_IDSW)){		
		
			Id.byte = LogicInput[SEL_RD_IDSW].BinInputOld.byte; 
			NewId = (Id.bits.ID_High * 10) + Id.bits.ID_Low;
			
			if(NewId < 100){
				if(NewId != IdSwitchOld){				
					
					IdSwitchOld = NewId;	
					myprintf("#NEW_ID :: (HIGH=%d, LOW=%d) \r\n", Id.bits.ID_High, Id.bits.ID_Low);		

					DefaultSetting(_check);

					RFID_ClearTagGrfOld();

					FND_DisplayTag();
				}			
			}
		}
	}
	else{ /* _reset */

		Id.byte = Read_Data(SEL_RD_IDSW);
		NewId = (Id.bits.ID_High * 10) + Id.bits.ID_Low;
		
		if(NewId < 100){

			IdSwitchOld = NewId;

			myprintf("#ID HIGH(%d) LOW(%d)\r\n", Id.bits.ID_High, Id.bits.ID_Low);
			myprintf("#ID VALUE(%d)\r\n", 		IdSwitchOld);
		}
	}
}


/*==================================================================
* Function Name 	: DIPSW_Read
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
bool DIPSW_Read(u8 command)
{
	u32 i;
	u8 newByteInput, lastByteInput;
	u8 newBitInput, lastBitInput;
	bool bIsChanged = FALSE;
	
	DIP_SWITCH *p = &DipSwitch;

	lastByteInput = p->BinInputOld.byte;			

	newByteInput = INVBYTE(Read_Data(SEL_RD_DIPSW)) & 0xF0; 	

	if(command == _check){		
		
		for(i = 0; i<= 7; i++){
			lastBitInput = tbit(lastByteInput, i);
		    newBitInput = tbit(newByteInput, i);
		    if(lastBitInput != newBitInput){
				p->FrozenCntrs[i]++;
				if(p->FrozenCntrs[i] >= DEFS_DIPSW_FROZEN_TIME){
					(newBitInput)? 	sbit(p->BinInputOld.byte, i) : \
					    			cbit(p->BinInputOld.byte, i); 
					p->FrozenCntrs[i] = 0;	    
					bIsChanged = TRUE;
				}
		    }else p->FrozenCntrs[i] = 0; 
		}
	}
	else{/* _reset */
		
		p->BinInputOld.byte = newByteInput;	
		memcpy((u8 *)&m_Config,(u8 *) &newByteInput, 1);			
	}

	return(bIsChanged);
}


/*==================================================================
* Function Name 	:  E2P_Test
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void E2P_Test(void)
{	
	u32 timeout_10ms;	
	u8	temp;

	/* EEPROM TEST (10ms)*/	
	{	
		temp = 0x55;	
		MEE_ByteWrite(EEPROM_TEST_ROM_ADDR, &temp); /* writing */	

		RESET_TIMER(&timeout_10ms);
		while (!CHK_TOV(timeout_10ms, DEFS_TIME_10MS))
		{
			/* ... */
		}		
		
		/* EEPROM TEST :: AT24C04 READING */
		if(temp == MEE_ByteRead(EEPROM_TEST_ROM_ADDR)){
			bE2PROM_InitOK = TRUE;			
		}
		else{			
			bE2PROM_InitOK = FALSE;
		}	
	}
}



/*==================================================================
* Function Name 	:  E2P_SaveRfidAttn
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void E2P_SaveRfidAttn(void)
{
	if(RFID_IsAttnValueOkay(TagConf.rfidAttn)){
		TagConf.romTagConf = RFIDCU_TAGCONF_ROM_INIT_CODE;
		MEE_PageWrite(RFIDCU_TAGCONF_ROM_BASE_ADDR, \
						(u8*)&TagConf.romTagConf, 	\
						RFIDCU_TAGCONF_ROM_SIZE);
	}
}


/*==================================================================
* Function Name 	:  E2P_SaveTagBypassMode
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void E2P_SaveTagBypassMode(void)
{
	if(TagConf.tagBypass <= ON){
		TagConf.romTagConf = RFIDCU_TAGCONF_ROM_INIT_CODE;
		MEE_PageWrite(RFIDCU_TAGCONF_ROM_BASE_ADDR, \
						(u8*)&TagConf.romTagConf, 	\
						RFIDCU_TAGCONF_ROM_SIZE);
	}
}

/*==================================================================
* Function Name 	:  E2P_SaveTagBypassMode
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void E2P_SaveRfidBeep(void)
{
	if(TagConf.rfidBeep <= ON){
		TagConf.romTagConf = RFIDCU_TAGCONF_ROM_INIT_CODE;
		MEE_PageWrite(RFIDCU_TAGCONF_ROM_BASE_ADDR, \
						(u8*)&TagConf.romTagConf, 	\
						RFIDCU_TAGCONF_ROM_SIZE);
	}
}

/*==================================================================
* Function Name 	:  E2P_SaveApPowerMode
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void E2P_SaveApPowrMode(void)
{
	if(TagConf.ApulsePowerMode <= ON){
		TagConf.romTagConf = RFIDCU_TAGCONF_ROM_INIT_CODE;
		MEE_PageWrite(RFIDCU_TAGCONF_ROM_BASE_ADDR, \
						(u8*)&TagConf.romTagConf, 	\
						RFIDCU_TAGCONF_ROM_SIZE);
	}
}


/*==================================================================
* Function Name 	:  DefaultSetting
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void DefaultSetting(u8 command)
{
	u8 mode;
		
	IDSW_Read(command);
		
	DIPSW_Read(command);	

	{
	
		GRFTypeCode = GRF_TYPE_ST;
		
		if(IdSwitchOld == 99) 		OperationMode = OP_MODE_HW_TEST;
		else if(IdSwitchOld == 98) 	OperationMode = OP_MODE_APPROVAL;
		else						OperationMode = OP_MODE_NORMAL;

		if((OperationMode == OP_MODE_HW_TEST) || (OperationMode == OP_MODE_APPROVAL)){
			TagMode = TAG_MODE_BYPASS;
		}
		else{
			
			mode = IdSwitchOld % 10;
			if(mode >= TAG_MODE_END) mode = RFIDDEFS_DEFAULT_TAG_MODE();

			TagMode = mode;						

			mode = IdSwitchOld / 10;		

			if(mode < GRF_TYPE_END) GRFTypeCode = mode;
		}	
		
		PRINT_GRF_TYPE();
		PRINT_FILTER_MODE();
	}	
}

/*==================================================================
* Function Name 	:  FactoryDefault
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void FactoryDefault(void)
{
	u32 timeout_10ms;	
	
	TagConf.romTagConf 	= 0x00;

	MEE_PageWrite(RFIDCU_TAGCONF_ROM_BASE_ADDR, 	\
						(u8*)&TagConf.romTagConf, 	\
						RFIDCU_TAGCONF_ROM_SIZE);
	

	RESET_TIMER(&timeout_10ms);
	while (!CHK_TOV(timeout_10ms, DEFS_TIME_10MS)){
		Console_CheckSendData();
	}	
}


/*==================================================================
* Function Name 	:  LoadSetting
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void LoadSetting(u8 command)
{
	u32 i;
	u8 data[MAX_RFIDCU_SET_SIZE];

	TagConf.romTagConf 	= RFIDCU_TAGCONF_ROM_INIT_CODE;
	TagConf.tagTest  	= OFF;	
	TagConf.tagBypass	= RFIDDEFS_DEEFAULT_BYPASS();
	TagConf.rfidAttn 	= RFIDDEFS_DEEFAULT_ATTN();	
	TagConf.rfidBeep 	= RFIDDEFS_DEEFAULT_BEEP();	

	if(command != _reset) TagConf.ApulsePowerMode = RFIDEFS_DEFAULT_AP_POWER__MODE(); 
	
	memset((u8 *)&data[0], 0, MAX_RFIDCU_SET_SIZE);

	if(bE2PROM_InitOK){
		
		MEE_PageRead(RFIDCU_TAGCONF_ROM_BASE_ADDR, &data[0], MAX_RFIDCU_SET_SIZE);

		myputs("#TAG CONF ");	

		for(i = 0; i < RFIDCU_TAGCONF_ROM_SIZE; i++)	myputxx(data[i]);	
		/* 유효 데이터 확인 */
		if(data[0] == RFIDCU_TAGCONF_ROM_INIT_CODE){

			myputsln("(+)");			

			//if(data[1] <= ON) TagConf.tagTest 	= data[1];
			if(data[2] <= ON) TagConf.tagBypass	= data[2];
			
			if(RFID_IsAttnValueOkay(data[3])) TagConf.rfidAttn	= data[3];
			
			if(data[4] <= ON) TagConf.rfidBeep = data[4];		

			if(command != _reset){ 
				if(data[5] <= ON) TagConf.ApulsePowerMode = data[5];
			}

			
		}	
		else
			myputsln("(-)");
	}

	if(TagConf.ApulsePowerMode) rfidMinAttnValue = RFID_AP_MIN_ATTN_VALUE;
	else						rfidMinAttnValue = RFID_MIN_ATTN_VALUE;

	if(TagConf.rfidAttn < rfidMinAttnValue){
		TagConf.rfidAttn = RFIDDEFS_DEEFAULT_ATTN();	
	}		
}


/*==================================================================
* Function Name 	:  SysInit
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void SysInit(void)
{
	
}


/*==================================================================
* Function Name 	: App_InitVal
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void App_InitVal(void)
{
	/* NONE */
}
	

/*==================================================================
* Function Name 	: AppMain
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void AppMain(void)
{

	if(IS_TIMEOUT(&TimerCount.period_500ms, 500)){                        			
		
		if(!bRFIDBootCheckTime){

			Toggle_RunLED();			
		}		
	}		


	if(IS_TIMEOUT(&TimerCount.period_10ms, DEFS_TIME_10MS)){		

		// DIP Switch
		if(DIPSW_Read(_check)){				
			memcpy((u8 *)&m_Config,(u8 *) &DipSwitch.BinInputOld.byte, 1);		
			myprintf("#DIPSW=%#x \r\n", DipSwitch.BinInputOld.byte);
			PRINT_DIPSW_CONFIG();

			ResetEnterSensor();

			ResetStopSensor();
			
		}			

		// ID Switch
		IDSW_Read(_check);	

		TAG_IsDispOn();
	}	

	
	// Echo TEST
	if(IS_OP(OP_MODE_HW_TEST)){
		
		EchoUart(COM_GUI);	
		EchoUart(COM_RFU1);	
		EchoUart(COM_RFU2);	
		EchoUart(COM_GRF);		
		EchoUart(COM_RFID);		
	}


	LED_CheckCOM();	

	LED_CheckTagRx();
	
	/* UPDATES Front LED */
	LED_Update();	
	 	
	/* Key */
	if(KEY_Process())	MENU_Process();

	// Train Position
	TPU_Process();		
	
	/* RFID */
	RFID_Process();

	GRF_Process();
	
	/* Modem 1, 2*/	
	RFU_Process();
	
	CAN_Process(CAN_PORT_1);
	CAN_Process(CAN_PORT_2);

	/* RFIDCU 상태 정보 매 2초 마다 전송 */
	CAN_RFIDCU_TestResult();	    	

	LOG_Process();
	
	/* SD_Card */
	SD_Card_Data_Write();

	FND_ChangeDigit();
}
