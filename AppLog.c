#include "Common.h"
/*****************************************************************************
*                                                                          
* 							LITERAL CONSTANTS(DEFINES)
*                    
******************************************************************************/
uint8_t* menuStringEvent[2]=
{
	"(COM1) RESET",
	"(COM2) RESET"
};

uint8_t* menuStringMyTag[2]=
{
	"(NOT) MY-TAG",
	"(YES) MY-TAG"
};


uint8_t* menuStringTrainPosition[5]=
{
	"TRAIN_NONE ",
	"TRAIN_ENTER",
	"TRAIN_STOP ", 
	"TRAIN_LEAVE",
	"**UNKNOWN**"
};


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
u8 LogDeviceSel;

sLOG_INFO	FlashLogStu;

/*****************************************************************************
*                                                                          
* 								LOCAL VARIABLES
*                    
******************************************************************************/

/*****************************************************************************
*                                                                          
* 								FUNCTION DEFINITIONS(LOCAL)
*                    
******************************************************************************/	


/*==================================================================
* Function Name  	: LOG_printEvent
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void LOG_printEvent(u8 COMx, LOG_EVENT code, u8 data)
{
	u32 i;	
	u32 len;
	u8 str[32];		

	if(IsUseComMonitoring()){		
		
		/*[EVENT] 02/03 12:12:14.0 :: (COM1) RESET*/

		i = 0;
		{
			str[i++] =0x0D;
			str[i++] =0x0A;

			str[i++] = '[';		
			str[i++] = 'E';	str[i++] = 'V';	str[i++] = 'E';	str[i++] = 'N';	str[i++] = 'T';	
			str[i++] = ']';
			str[i++] = 0x20;	/* space */	
		}

		{
			str[i++] = D2C(s_DateStructVar.Month/10);	
			str[i++] = D2C(s_DateStructVar.Month%10);	
			str[i++] = '/';	/* '/' */	

			
			str[i++] = D2C(s_DateStructVar.Day/10);	
			str[i++] = D2C(s_DateStructVar.Day%10);	
			str[i++] = 0x20;	/* space */		
		}

		{			
			str[i++] = D2C(s_TimeStructVar.HourHigh);	
			str[i++] = D2C(s_TimeStructVar.HourLow);	
			str[i++] = ':';	/* '/' */	

			
			str[i++] = D2C(s_TimeStructVar.MinHigh);
			str[i++] = D2C(s_TimeStructVar.MinLow);
			str[i++] = ':';	/* '/' */	

			str[i++] = D2C(s_TimeStructVar.SecHigh);
			str[i++] = D2C(s_TimeStructVar.SecLow);
			str[i++] = '.';	/* '/' */				

			str[i++] = D2C(RTC_100msCnt);	

			str[i++] = 0x20;	/* space */		

			
			str[i++] = ':';	/* '/' */					
			str[i++] = ':';	/* '/' */					

			str[i++] = 0x20;	/* space */		

			COMx_Write(COMx, (u8 *)&str[0], i);
		}			

		
		i = 0;	
		if(COMx == COM_RFU1){
			len = myu8strlen(menuStringEvent[LOG_EVENT_RESET]);	
			mymemcpy(&str[i], menuStringEvent[LOG_EVENT_RESET], len); 
		}
		else{
			len = myu8strlen(menuStringEvent[LOG_EVENT_RESET+1]);	
			mymemcpy(&str[i], menuStringEvent[LOG_EVENT_RESET+1], len);
		}	
			
		
		i+=len;
		
		COMx_Write(COMx, (u8 *)&str[0], i);		
	}	
}	

/*==================================================================
* Function Name  	: LOG_printTAGConfirm
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void LOG_printTAGConfirm(u8 COMx)
{
	u32 i;	
	u32 len;
	u8 str[32];		
	u8 temp;

	if(IsUseComMonitoring()){
		
	//if(DBG_COM1_PRINT_ASCII()){		
	
		//MRTC_CalculateTime();
		
		/*[ TAG ] 02/03 12:12:14.0 :: Not MyTAG(1,99) */

		i = 0;
		{
			str[i++] =0x0D;
			str[i++] =0x0A;

			str[i++] = '[';		
			str[i++] = ' ';	str[i++] = 'T';	str[i++] = 'A';	str[i++] = 'G';	str[i++] = ' ';	
			str[i++] = ']';
			str[i++] = 0x20;	/* space */	
		}

		{
			str[i++] = D2C(s_DateStructVar.Month/10);	
			str[i++] = D2C(s_DateStructVar.Month%10);	
			str[i++] = '/';	/* '/' */	

			
			str[i++] = D2C(s_DateStructVar.Day/10);	
			str[i++] = D2C(s_DateStructVar.Day%10);	
			str[i++] = 0x20;	/* space */		
		}

		{			
			str[i++] = D2C(s_TimeStructVar.HourHigh);	
			str[i++] = D2C(s_TimeStructVar.HourLow);	
			str[i++] = ':';	/* '/' */	

			
			str[i++] = D2C(s_TimeStructVar.MinHigh);
			str[i++] = D2C(s_TimeStructVar.MinLow);
			str[i++] = ':';	/* '/' */	

			str[i++] = D2C(s_TimeStructVar.SecHigh);
			str[i++] = D2C(s_TimeStructVar.SecLow);
			str[i++] = '.';	/* '/' */				

			str[i++] = D2C(RTC_100msCnt);	

			str[i++] = 0x20;	/* space */		

			
			str[i++] = ':';	/* '/' */					
			str[i++] = ':';	/* '/' */					

			str[i++] = 0x20;	/* space */		

			COMx_Write(COMx, (u8 *)&str[0], i);
		}			

		
		i = 0;	
		if(tagGrfOld.bNotMytag){

			// (NO!) MyTAG[1,99]
			len = myu8strlen(menuStringMyTag[OFF]);		
			mymemcpy(&str[i], menuStringMyTag[OFF], len); 	
		}
		else{

			// (YES) MyTAG[1,99]
			len = myu8strlen(menuStringMyTag[ON]);		
			mymemcpy(&str[i], menuStringMyTag[ON], len); 	
		}

		i+=len;

		str[i++] = '[';
		str[i++] = D2C(tagGrfOld.RouteNo);			
		str[i++] = ',';	

		temp = (tagGrfOld.OrgNo >> 4) & 0x0F;
		str[i++] = D2C(temp);			

		temp = tagGrfOld.OrgNo & 0x0F;
		str[i++] = D2C(temp);			
		str[i++] = ']';			
		
		COMx_Write(COMx, (u8 *)&str[0], i);		
	}	
}	


/*==================================================================
* Function Name  	: LOG_printTPUPos
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void LOG_printTPUPos(u8 COMx)
{
	u32 i;	
	u32 len;
	u8 str[32];		

	if(IsUseComMonitoring()){
		
	//if(DBG_COM1_PRINT_ASCII()){		
	
		//MRTC_CalculateTime();
		
		/*[TRAIN] 02/03 12:12:14.0 :: TRN_ENTER OXOX*/

		i = 0;
		{
			str[i++] =0x0D;
			str[i++] =0x0A;

			str[i++] = '[';		
			str[i++] = 'T';	str[i++] = 'R';	str[i++] = 'A';	str[i++] = 'I';	str[i++] = 'N';	
			str[i++] = ']';
			str[i++] = 0x20;	/* space */	
		}

		{
			str[i++] = D2C(s_DateStructVar.Month/10);	
			str[i++] = D2C(s_DateStructVar.Month%10);	
			str[i++] = '/';	/* '/' */	

			
			str[i++] = D2C(s_DateStructVar.Day/10);	
			str[i++] = D2C(s_DateStructVar.Day%10);	
			str[i++] = 0x20;	/* space */		
		}

		{			
			str[i++] = D2C(s_TimeStructVar.HourHigh);	
			str[i++] = D2C(s_TimeStructVar.HourLow);	
			str[i++] = ':';	/* '/' */	

			
			str[i++] = D2C(s_TimeStructVar.MinHigh);
			str[i++] = D2C(s_TimeStructVar.MinLow);
			str[i++] = ':';	/* '/' */	

			str[i++] = D2C(s_TimeStructVar.SecHigh);
			str[i++] = D2C(s_TimeStructVar.SecLow);
			str[i++] = '.';	/* '/' */				

			str[i++] = D2C(RTC_100msCnt);	

			str[i++] = 0x20;	/* space */		

			
			str[i++] = ':';	/* '/' */					
			str[i++] = ':';	/* '/' */					

			str[i++] = 0x20;	/* space */		

			COMx_Write(COMx, (u8 *)&str[0], i);
		}			

		
		i = 0;	
		len = myu8strlen(menuStringTrainPosition[TPU_GET_POS()]);		
		mymemcpy(&str[i], menuStringTrainPosition[TPU_GET_POS()], len);		

		i+=len;

		str[i++]	= '(';
		if(FrontBtnOld.bits.TrnEntSensor1)	str[i++]	= '1';
		else								str[i++]	= '0';

		if(FrontBtnOld.bits.TrnEntSensor2)	str[i++]	= '1';
		else								str[i++]	= '0';

		str[i++] = '$';	/* space */ 	

		if(FrontBtnOld.bits.TrnStopSensor1) str[i++]	= '1';
		else								str[i++]	= '0';

		if(FrontBtnOld.bits.TrnStopSensor2) str[i++]	= '1';
		else								str[i++]	= '0';

		str[i++]	= ')';
		
		COMx_Write(COMx, (u8 *)&str[0], i);		
	}	
}	




/*==================================================================
* Function Name  	: RFID_ClearTagTxd
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void LOG_printRFIDtag(u8 *data, bool bMetalTag, bool bBCDError)
{
	u32 i, len;	
	s8 str[40];		
	u8 temp, Bcc;
	u8 tagData[RFID_TAG_PACKET_SIZE];
	u8 maker;
	u16 rssi;

	if(IsUseComMonitoring()){
		
	//if(DBG_COM1_PRINT_ASCII()){		
	
		//MRTC_CalculateTime();
		
		/*[0000] 02/03 12:12:14.0 :: -45.6, */

		i = 0;
		{
			str[i++] =0x0D;
			str[i++] =0x0A;

			str[i++] = '[';		
			str[i++] = 'A';	

			temp = myTagRxNumber/100;			
			str[i++] = D2C(temp/10);	str[i++] = D2C(temp%10);

			temp = myTagRxNumber%100;
			str[i++] = D2C(temp/10);	str[i++] = D2C(temp%10); 

			str[i++] = ']';
			str[i++] = 0x20;	/* space */	
		}

		{
			str[i++] = D2C(s_DateStructVar.Month/10);	
			str[i++] = D2C(s_DateStructVar.Month%10);	
			str[i++] = '/';	/* '/' */	

			
			str[i++] = D2C(s_DateStructVar.Day/10);	
			str[i++] = D2C(s_DateStructVar.Day%10);	
			str[i++] = 0x20;	/* space */		
		}

		{			
			str[i++] = D2C(s_TimeStructVar.HourHigh);	
			str[i++] = D2C(s_TimeStructVar.HourLow);	
			str[i++] = ':';	/* '/' */	

			
			str[i++] = D2C(s_TimeStructVar.MinHigh);
			str[i++] = D2C(s_TimeStructVar.MinLow);
			str[i++] = ':';	/* '/' */	

			str[i++] = D2C(s_TimeStructVar.SecHigh);
			str[i++] = D2C(s_TimeStructVar.SecLow);
			str[i++] = '.';	/* '/' */				

			str[i++] = D2C(RTC_100msCnt);	

			str[i++] = 0x20;	/* space */		

			if(bBCDError)	str[i++] = 'E';	/* '/' */	
			else			str[i++] = ':';	/* '/' */		

			if(bMetalTag)	str[i++] = 'M';	/* '/' */	
			else{

				maker = data[MK_IDX_05+9];
				
				if((maker != TRF_MAKER_NUM_HYOCHON) && (maker != TRF_MAKER_NUM_ROTTEM)){

					if(maker <= TRF_MAKER_NUM_END){
						str[i++] = D2C(maker);	/* '/' */					
					}
					else{
						str[i++] = 'E';
					}
				}
				else{
					str[i++] = ':';	/* '/' */	
				}
			}	

			str[i++] = 0x20;	/* space */		

			COMx_Write(COM_RFU1, (u8 *)&str[0], i);
		}			

		/*	RSSI tag data 수신함 */
		
		if(bTagWithRssi){

			i= 0;

			rssi = data[MK_IDX_RSSI_HIGH];
			rssi *= 256;
			rssi += data[MK_IDX_RSSI_LOW]; 

			// 0 to 999
			if(rssi < RFID_RSSI_DATA_MIN) rssi = RFID_RSSI_DATA_MIN;
			
			tagRssiValue = 0xFFFF - rssi;

			rssi = tagRssiValue;
			
			str[i++] = '(';
			if(rssi == 0) 	str[i++] = '+';	/* + */
			else			str[i++] = '-';	/* - */

			// 100 to 999 :: -10.0 ~ -99.9*/
			if(rssi >= 100){
				str[i++] = D2C(rssi /100);	
				str[i++] = D2C((rssi %100) /10);	
			}
			else{/* 1 ot 99 :: - 0.1 ~ - 9.9 */
				str[i++] = 0x20;	/* space */ 	
				str[i++] = D2C(rssi /10);	
			}
			
			str[i++] = '.'; /* . */
			str[i++] = D2C(rssi %10);

			str[i++] = ')';		
			str[i++] = 0x20;	/* space */ 	
			str[i++] = 0x20;	/* space */ 	
			
			COMx_Write(COM_RFU1, (u8 *)&str[0], i);
		}


		//16byte :: (30 00) 05 64 95 03 05 01 78 00 10 01 01 78 (7B 4A) 	
		//14byte ::         05 64 95 03 05 01 78 00 10 01 01 78	(7B 4A) 	
		//12byte ::         05 64 95 03 05 01 78 00 10 01 01 78	
		//                  05 64 81 03 01 01 99 01 10 01 00 99, F7 73 [01] (77 FD)
		
		if(DBG_COM1_TAG_PRINT()){

			if(bTagWithRssi) 	len = MK_IDX_05+17; /* tag data with rssi (17byte)*/
			else				len = MK_IDX_05+12; /* only tag data (12byte)*/
			
			for(i=MK_IDX_05; i<len; i++){
			//for(i=MK_IDX_05; i<(MK_IDX_05+14); i++){
				str[0] = D2C(data[i]/16);
				str[1] = D2C(data[i]%16);
				str[2] = 0x20;	/* space */	
				COMx_Write(COM_RFU1, (u8 *)&str[0], 3);
			}	
		}
		else{			

			if(bTagWithRssi) 	len = (data[MK_IDX_LEN]+8);
			else				len = (data[MK_IDX_LEN]+6);	
				
			// STX ~ ETX (Total 29byte or 31byte(with RSSI))
			for(i=0; i<len; i++){
				str[0] = D2C(data[i]/16);
				str[1] = D2C(data[i]%16);
				str[2] = 0x20;	/* space */	
				COMx_Write(COM_RFU1, (u8 *)&str[0], 3);
			}	
		}
	}
	else{		

		// RSSI data 2byte 제거하여 minerva gui에서 확인토록 패킷 변환함 
		
		if(bTagWithRssi){

			//RFID_RSSI_TAG_PACKET_SIZE
			memcpy((u8*)&tagData[0], (u8 *)&data[0], RFID_TAG_PACKET_SIZE);	 		

			tagData[MK_IDX_LEN] = 0x19;

			/* BCC */
			{
				Bcc  = tagData[MK_IDX_RXID];	
				Bcc ^= tagData[MK_IDX_TXID];	
				Bcc ^= tagData[MK_IDX_LEN];

				// 4+ 0x17 = 0x1B  (27ea)
				len  = 	MK_IDX_COM_C1+tagData[MK_IDX_LEN];				
				for(i = MK_IDX_COM_C1; i< len; i++)Bcc ^= tagData[i];	
				
				tagData[MK_IDX_BCC] = Bcc;
			}
			
			tagData[MK_IDX_ETX] = 0x03;
		}
		
		COMx_Write(COM_RFU1, (u8 *)&data[0], RFID_TAG_PACKET_SIZE);
	}
}	


/*==================================================================
* Function Name 	: LOG_InitVal
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void LOG_InitVal(void)
{	
	LogDeviceSel = LOG_FLASH_MEM;
}


/*==================================================================
* Function Name 	: LOG_DevSel
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void LOG_DevSel(u8 UseFlashMemory)
{	
	LogDeviceSel = UseFlashMemory;
	
	// LOG_FLASH_MEM(0), LOG_SDCARD_MEM(1)
	(UseFlashMemory)? GPIO_SetBits(GPIOB, nSEL_SD_MEM) : GPIO_ResetBits(GPIOB, nSEL_SD_MEM);  
}

/*==================================================================
* Function Name  	: LOG_EraseRecPos
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void LOG_EraseRecPos(void)
{	
	u8 data[RECORD_COUNT_INFO_SIZE] = {0,};     	
	
	if(FlashLogStu.RecordFull || (FlashLogStu.RecordPosition > 0)){		

		FlashLogStu.RecordFull 		= 0;
		FlashLogStu.RecordPosition 	= 0;	
		
		data[0] = FlashLogStu.RecordFull;
		data[1] = (u8)(FlashLogStu.RecordPosition >> 24);
		data[2] = (u8)(FlashLogStu.RecordPosition >> 16);
		data[3] = (u8)(FlashLogStu.RecordPosition >> 8);
		data[4] = (u8)(FlashLogStu.RecordPosition);

		// 0x1F FFF4 (12byte)
		SPI_FLASH_BufferWrite((u8*)&data[0],(RECORD_MAX_NUM+1)*RECORD_SIZE, RECORD_COUNT_INFO_SIZE);
	}								
}


/*==================================================================
* Function Name 	:  LOG_GetRecPos
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
u32 LOG_GetRecPos(void)
{
	u8 rData[RECORD_COUNT_INFO_SIZE] = {0,};	
	u32 rec_pos;
	u8 rec_full;
	
	// 0x1F FFF4 (12byte)
	SPI_FLASH_BufferRead((u8*)&rData[0], (RECORD_MAX_NUM+1)*RECORD_SIZE , RECORD_COUNT_INFO_SIZE);	

	rec_full= rData[0];
	
	rec_pos = rData[1];
	rec_pos = (rec_pos << 8) | rData[2];
	rec_pos = (rec_pos << 8) | rData[3];
	rec_pos = (rec_pos << 8) | rData[4];

	if(rec_pos > RECORD_MAX_NUM){

		rec_full = 0;
		rec_pos  = 0;
	}	
	else{
		FlashLogStu.RecordFull     = rec_full;
		FlashLogStu.RecordPosition = rec_pos;
	}		

	return(rec_pos);
}


/*==================================================================
* Function Name 	: LOG_WriteData
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void LOG_WriteData(const u8 *pBuffer, u32 NumByteToWrite)
{
	u32 i,j;
	u8 data[RECORD_SIZE] = {0,};	

	/*	
	s_DateStructVar.Year 	= (BKP_ReadBackupRegister(BKP_DR4));
	s_DateStructVar.Month 	= (BKP_ReadBackupRegister(BKP_DR2));
	s_DateStructVar.Day 	= (BKP_ReadBackupRegister(BKP_DR3));  	
	*/
	
	MRTC_CalculateTime();
		
	data[0] = (u8)(s_DateStructVar.Year % 1000);
	data[1] = s_DateStructVar.Month;
	data[2] = s_DateStructVar.Day;	
	data[3] = (s_TimeStructVar.HourHigh * 10) + s_TimeStructVar.HourLow;
	data[4] = (s_TimeStructVar.MinHigh * 10) + s_TimeStructVar.MinLow;
	data[5] = (s_TimeStructVar.SecHigh * 10) + s_TimeStructVar.SecLow;
	data[6] = (RTC_100msCnt);	

	j = 7;
	
  	for(i=0; i < NumByteToWrite; i++){
		
		data[j] = pBuffer[i];	j++;
	}	
		
	SPI_FLASH_BufferWrite((u8*)&data[0], FlashLogStu.RecordPosition*RECORD_SIZE, RECORD_SIZE);	
	
	if(++FlashLogStu.RecordPosition > RECORD_MAX_NUM){

		FlashLogStu.RecordPosition  = 0;
		FlashLogStu.RecordFull		= 1;
	}

	/* record info backup */
	{
		data[0] = FlashLogStu.RecordFull;
		data[1] = (u8)(FlashLogStu.RecordPosition >> 24);
		data[2] = (u8)(FlashLogStu.RecordPosition >> 16);
		data[3] = (u8)(FlashLogStu.RecordPosition >> 8);
		data[4] = (u8)(FlashLogStu.RecordPosition);

		// 0x1F FFF4 (12byte)
		SPI_FLASH_BufferWrite((u8*)&data[0],(RECORD_MAX_NUM+1)*RECORD_SIZE, RECORD_COUNT_INFO_SIZE);	
	}
}


/*==================================================================
* Function Name 	: LOG_ReadRecord
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void LOG_ReadRecord(u32 RecordNumber)
{  
	u32 i;
	u8 data[RECORD_SIZE] = {0,};
  
	SPI_FLASH_BufferRead((u8*)&data[0], (RecordNumber * RECORD_SIZE), RECORD_SIZE);	

	if(DBG_FLOG_PRINT()){

		myputsln(" ");	
		myprintf("\r\n#REC(%d) : ", RecordNumber);

		for(i=0; i < RECORD_SIZE; i++) myprintf("%02X ",data[8+i]);
		myputsln(" ");	
	}
}


/*==================================================================
* Function Name 	: LOG_Process
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void LOG_Process(void)
{	
	// every 1sec
	if(RTC_UpdateFlag){
		
		RTC_UpdateFlag 	= FALSE;
		
		MRTC_DisplayDataAndTime(_read);
	}		
	else{

		if(IS_TIMEOUT(&TimerCount.period_rtc_100ms, DEFS_TIME_100MS)){	
			

			// 0 to 9
			if(RTC_100msCnt < 9) RTC_100msCnt++;
		}	
	}
}
