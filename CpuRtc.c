#include "Common.h"
/*****************************************************************************
*                                                                          
* 							LITERAL CONSTANTS(DEFINES)
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
vu8 RTC_UpdateFlag;
vu8 RTC_100msCnt;

Time_s s_TimeStructVar;
Date_s s_DateStructVar;


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
* Function Name 	: MRTC_CheckLeapYear
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
u8 MRTC_CheckLeapYear(u16 Year)
{
	if((Year%400)==0)
	{
		return LEAP;
	}
	else if((Year%100)==0)
	{
		return NOT_LEAP;
	}
	else if((Year%4)==0)
	{
		return LEAP;
	}
	else
	{
		return NOT_LEAP;
	}
}

/*==================================================================
* Function Name 	: MRTC_GetMaxDay
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
u8 MRTC_GetMaxDay(void)
{	
	u16 year = s_DateStructVar.Year;
	u8 month = s_DateStructVar.Month;

	u8 maxDay = 31;

	if(month == 2){

		if(MRTC_CheckLeapYear(year) == LEAP) 	maxDay = 29;
		else									maxDay = 28;
	}
	else if((month == 4)||(month == 6)||(month == 9)||(month == 11)) maxDay = 30;	

	return(maxDay);
}

/*==================================================================
* Function Name 	: MRTC_GetYear
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
u16 MRTC_GetYear(void)
{
	return(s_DateStructVar.Year);
}

/*==================================================================
* Function Name 	: MRTC_GetMonth
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
u8 MRTC_GetMonth(void)
{
	return(s_DateStructVar.Month);
}


/*==================================================================
* Function Name 	: MRTC_GetDay
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
u8 MRTC_GetDay(void)
{
	return(s_DateStructVar.Day);
}

/*==================================================================
* Function Name 	: MRTC_GetHour
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
u8 MRTC_GetHour(void)
{	
	u8 time;
	
	MRTC_CalculateTime();

	time  = s_TimeStructVar.HourHigh * 10;
  	time += s_TimeStructVar.HourLow;

	return(time);
}

/*==================================================================
* Function Name 	: MRTC_GetMin
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
u8 MRTC_GetMin(void)
{	
	u8 time;
	
	MRTC_CalculateTime();

	time  = s_TimeStructVar.MinHigh * 10;
  	time += s_TimeStructVar.MinLow;

	return(time);
}

/*==================================================================
* Function Name 	: MRTC_GetMin
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
u8 MRTC_GetSec(void)
{	
	u8 time;
	
	MRTC_CalculateTime();

	time  = s_TimeStructVar.SecHigh * 10;
  	time += s_TimeStructVar.SecLow;

	return(time);
}

/*==================================================================
* Function Name 	: MRTC_GetmSec
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
u8 MRTC_Get100mSec(void)
{
	return(RTC_100msCnt);
}


/*==================================================================
* Function Name 	: MRTC_SetMenuDate
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
bool MRTC_SetMenuDate(u16 year, u8 month, u8 day)
{
	bool bSet = FALSE;

	//입력한날짜와 시간이 올바른지
	if((( month==4 || month==6 || month==9 || month==11) && day ==31) \
		|| (month==2 && day==31)|| (month==2 && day==30) \
		|| (month==2 && day==29 && (MRTC_CheckLeapYear(year) == NOT_LEAP)))
	{
		myputsln("#Incorrect Date Setting");
	}
	else{

		Date_Adjust(year, month, day);
		bSet = TRUE;
	}	

	return(bSet);
}


/*==================================================================
* Function Name 	: MRTC_SetMenuTime
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
bool MRTC_SetMenuTime(u8 hour, u8 minute, u8 sec)
{
	bool bSet = FALSE;
	
	if((hour <= 23) && (minute <= 59) && (sec <= 59)){

		Time_Adjust(hour,minute,sec);		
		bSet = TRUE;
	}
	else{
		myputsln("#Incorrect Time Setting");
	}	

	return(bSet);
}

/*==================================================================
* Function Name 	: Time_Adjust
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void Time_Adjust(uint32_t Tmp_HH, uint32_t Tmp_MM, uint32_t Tmp_SS)
{  
	// Wait until last write operation on RTC registers has finished	
	RTC_WaitForLastTask();	  
	
	// Change the current time	
	RTC_SetCounter((Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS)); 
	
	RTC_100msCnt 	= 0;
	
	// Wait until last write operation on RTC registers has finished 
	RTC_WaitForLastTask();	  
}


/*==================================================================
* Function Name 	: Date_Adjust
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void Date_Adjust(u16 yy,u8 mm,u8 dd)
{	
	BKP_WriteBackupRegister(BKP_DR4,yy);	//년 
	BKP_WriteBackupRegister(BKP_DR2,mm);	//월
	BKP_WriteBackupRegister(BKP_DR3,dd);	//일  
}


/*==================================================================
* Function Name 	: MRTC_SetClock
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void MRTC_SetClock(u8 *rx_d, u8 StationCommand)
{
	u16 y1;
	u8 mm,dd,h,m,s,wk;
	
	/* PSC time setting  */
	if(StationCommand)
	{
		y1= 2000 + rx_d[0]; 
		mm = rx_d[1];
		dd = rx_d[2];
		
		h = rx_d[3];
		m = rx_d[4];
		s = rx_d[5];

		// 요일은 반영하지 않음 (N/A)
		wk =  rx_d[6];		
	}
	else /* Console window, Debug Command */
	{	  
		y1 = C2D(rx_d[4]) * 256;			
		y1 = y1 + (C2D(rx_d[5]) * 16);	
		y1 = y1 + C2D(rx_d[6]);
  
		mm = C2D(rx_d[7]) * 16; 	
		mm = mm + C2D(rx_d[8]);
	
		dd = C2D(rx_d[9]) * 16; 	
		dd = dd + C2D(rx_d[10]);
	
		h = C2D(rx_d[11]) * 16; 	  
		h = h + C2D(rx_d[12]);			 
		
		m = C2D(rx_d[13]) * 16; 	
		m = m + C2D(rx_d[14]);
	
		s = C2D(rx_d[15]) * 16; 	
		s = s + C2D(rx_d[16]);

		// 요일은 반영하지 않음 (N/A)
		wk = C2D(rx_d[17]) * 16;		
		wk = wk + C2D(rx_d[18]);
	}

	//입력한날짜와 시간이 올바른지
	if((( mm==4 || mm==6 || mm==9 || mm==11) && dd ==31) || (mm ==2 && dd==31)|| (mm==2 && dd==30)|| \
	  (mm==2 && dd==29 && (MRTC_CheckLeapYear(y1)==0)) || (h > 23) || (m > 59) || (s > 59))
	{
		myprintf("\r\nINCORRECT DATE");
	}
	else{

		Date_Adjust(y1,mm,dd);
		Time_Adjust(h,m,s);
	}	
}

/*==================================================================
* Function Name 	: MRTC_CalculateTime
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void MRTC_CalculateTime(void)
{
  uint32_t TimeVar;
  
  TimeVar = RTC_GetCounter();
  TimeVar = TimeVar % RTC_SECONDS_IN_DAY;
  
  s_TimeStructVar.HourHigh	=(uint8_t)(TimeVar/3600)/10;
  s_TimeStructVar.HourLow	=(uint8_t)(TimeVar/3600)%10;
  
  s_TimeStructVar.MinHigh	=(uint8_t)((TimeVar%3600)/60)/10;
  s_TimeStructVar.MinLow	=(uint8_t)((TimeVar%3600)/60)%10;
  
  s_TimeStructVar.SecHigh	=(uint8_t)((TimeVar%3600)%60)/10;
  s_TimeStructVar.SecLow	=(uint8_t)((TimeVar %3600)%60)%10;
}

/*==================================================================
* Function Name 	: MRTC_DisplayDataAndTime
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void MRTC_DisplayDataAndTime(u8 command)
{
	s_DateStructVar.Year 	=(BKP_ReadBackupRegister(BKP_DR4));
	s_DateStructVar.Month	=(BKP_ReadBackupRegister(BKP_DR2));
	s_DateStructVar.Day  	=(BKP_ReadBackupRegister(BKP_DR3));  

	MRTC_CalculateTime();

	if(command == _reset){

		myputsln(" ");
	  	myprintf("#DATE: %d%d%d%d/%d/%d%d ", (s_DateStructVar.Year/1000), \
	  				((s_DateStructVar.Year/100)%10), \
	  				((s_DateStructVar.Year/10)%10),	\
					(s_DateStructVar.Year%10), \
					s_DateStructVar.Month, \
					(s_DateStructVar.Day/10), (s_DateStructVar.Day%10));

		myputsln(" ");
		
	  	myprintf("#TIME: %d%d:%d%d:%d%d", 	s_TimeStructVar.HourHigh, s_TimeStructVar.HourLow,\
									s_TimeStructVar.MinHigh,  s_TimeStructVar.MinLow, \
									s_TimeStructVar.SecHigh,  s_TimeStructVar.SecLow);    
		myputsln(" ");
	}
}



/*==================================================================
* Function Name 	: MRTC_InitClockVal
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void MRTC_InitClockVal(void)
{
	s_DateStructVar.Year 	=(BKP_ReadBackupRegister(BKP_DR4));
	s_DateStructVar.Month	=(BKP_ReadBackupRegister(BKP_DR2));
	s_DateStructVar.Day  	=(BKP_ReadBackupRegister(BKP_DR3));  

	MRTC_CalculateTime();  
}

/*==================================================================
* Function Name 	: MRTC_IncrementDate
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void MRTC_IncrementDate(void)		
{
	// (note) if (RTC_GetCounter() == 86400) RTC_SetCounter(0);	
	// increment (1 DAY)

	s_DateStructVar.Year	= BKP_ReadBackupRegister(BKP_DR4);
	s_DateStructVar.Month	= BKP_ReadBackupRegister(BKP_DR2);
	s_DateStructVar.Day		= BKP_ReadBackupRegister(BKP_DR3);	
	
  	// Month(1, 3, 5, 7, 8, 10, 12) == Day 31
	if( (s_DateStructVar.Month == 1) || (s_DateStructVar.Month == 3) || 
		(s_DateStructVar.Month == 5) || (s_DateStructVar.Month == 7) || 
		(s_DateStructVar.Month == 8) || (s_DateStructVar.Month == 10)|| 
		(s_DateStructVar.Month == 12))
	{	
		if(s_DateStructVar.Day < 31)
		{
			s_DateStructVar.Day++;
		}
		else
		{
			if(s_DateStructVar.Month != 12)
			{
				s_DateStructVar.Month++;
				s_DateStructVar.Day = 1;
			}
			else /* Month == 12 */
			{
				s_DateStructVar.Month = 1;
				s_DateStructVar.Day = 1;
				s_DateStructVar.Year++;
			}
		}
	}
	else if((s_DateStructVar.Month == 4) || (s_DateStructVar.Month == 6) ||
  		    (s_DateStructVar.Month == 9) || (s_DateStructVar.Month == 11))
	{	
		// Month(4, 6, 9, 11) == Day 30
		if(s_DateStructVar.Day < 30)
		{
		  	s_DateStructVar.Day++;
		}
		else
		{
		  	s_DateStructVar.Month++;
		  	s_DateStructVar.Day = 1;
		}
	}
	else if(s_DateStructVar.Month == 2)
	{
		if(s_DateStructVar.Day < 28)
		{
			s_DateStructVar.Day++;
		}
		else if(s_DateStructVar.Day == 28)
		{
			// Leap Year Correction (28 ---> 29)
			if(MRTC_CheckLeapYear(s_DateStructVar.Year))
			{
				s_DateStructVar.Day++;
			}
			else
			{
				s_DateStructVar.Month++;
				s_DateStructVar.Day = 1;
			}
		}
		else if(s_DateStructVar.Day == 29)
		{
			s_DateStructVar.Month++;
			s_DateStructVar.Day = 1;
		}
	}

	BKP_WriteBackupRegister(BKP_DR4,s_DateStructVar.Year);  
	BKP_WriteBackupRegister(BKP_DR2,s_DateStructVar.Month);
	BKP_WriteBackupRegister(BKP_DR3,s_DateStructVar.Day);	
}


/*==================================================================
* Function Name 	: MRTC_CheckDayElapsed
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void MRTC_CheckDayElapsed(void)
{
  uint8_t DaysElapsed;

	/*	- Reset RTC Counter when Time is 24:00:00  
		- 60sec x 60 = 3600 x 24 = 86400 
		- RTC Count 0 to 86399 : 0sec to 23:59:59
		
		(note) if (RTC_GetCounter() == 86400) RTC_SetCounter(0);	
		(note) #define SECOND_IN_DAY 86399
	*/	

	if (RTC_GetCounter() >= (RTC_SECONDS_IN_DAY))	
	{
		// 초과된 day 수만큼 업데이이트 한다. 		
		for(DaysElapsed = 0; DaysElapsed < (RTC_GetCounter() / (RTC_SECONDS_IN_DAY)) ;DaysElapsed++)
		{
		  	MRTC_IncrementDate();
		}

		// (note) if (RTC_GetCounter() == 86400) RTC_SetCounter(0);	
		RTC_SetCounter(RTC_GetCounter() % (RTC_SECONDS_IN_DAY));
  	}
}


/*==================================================================
* Function Name 	: MRTC_Configuration
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void MRTC_Configuration(void)
{
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	  

	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);

	/* Reset Backup Domain */
	BKP_DeInit(); 			 

	/* Enable LSE (External Low Speed Oscillator)*/
	RCC_LSEConfig(RCC_LSE_ON);  

	/* Wait till LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){} 

	/* Select LSE as RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);

	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();
	{	
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();	  

		/* Enable the RTC Second */
		RTC_ITConfig(RTC_IT_SEC, ENABLE);

		/* Wait until last write operation on RTC registers has finished */	
		RTC_WaitForLastTask();	  
	}

	/* Set RTC prescaler: set RTC period to 1sec */
	/* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
	RTC_SetPrescaler(32767);	  

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();	  

	// Unlock the Flash Program Erase controller 
	#ifdef MRTC_FlashUnlock_Enable
		FLASH_Unlock();			  
	#endif
}


/*==================================================================
* Function Name 	: MRTC_InitVal
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void MRTC_InitVal(void)
{	
	RTC_UpdateFlag 	= FALSE;
	RTC_100msCnt	= 0;
}


/*==================================================================
* Function Name 	: MRTC_Init
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void MRTC_Init(void)
{	
	if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{	
		/* Backup data register value is not correct or not yet programmed (when
	       the first time the program is executed) */

		myputsln("#RTC ::  Not Yet Configured");

		/* RTC Configuration */
		MRTC_Configuration(); 

		//myprintf("\r\n RTC configured....");

		/* Adjust date by predefined default values*/
		Date_Adjust(2022,6,18);		

		/* Adjust time by predefined default values*/
		Time_Adjust(20,00,0);	

		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);	 
	}
	else
	{
		myputsln("#RTC :: Already Configured");

		/* RTC Configuration */
		//MRTC_Configuration(); 

		#if 1
		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();			  

		{
			/* Wait until last write operation on RTC registers has finished */
			RTC_WaitForLastTask();	
			
			/* Enable the RTC Second */	
			RTC_ITConfig(RTC_IT_SEC, ENABLE); 

			/* Wait until last write operation on RTC registers has finished */
			RTC_WaitForLastTask();		
		}
		#endif
	} 

	/* PC.13 is GPIO function */
	//#ifdef MRTC_RTCClockOutput_Enable
	{
		// Enable PWR and BKP clocks	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

		// Allow access to BKP Domain  
		PWR_BackupAccessCmd(ENABLE);		  

		// To output RTCCLK/64 on Tamper pin, the tamper functionality must be disabled 
		BKP_TamperPinCmd(DISABLE);		  

		// Enable RTC Clock Output on Tamper Pin
		//BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
		BKP_RTCOutputConfig(BKP_RTCOutputSource_None);
	}
	//#endif

	/* Clear reset flags */
	RCC_ClearFlag();		

	/* rtc_count > 86399 , we need to change the current date and time */
	MRTC_CheckDayElapsed();	

	// Init Date and Time value
	MRTC_InitClockVal();
}

#if 0
/*==================================================================
* Function Name 	: MRTC_ConsoleSetting
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void MRTC_ConsoleSetting(void)
{
	MENU_MODE menu_step;
	u8 menu_value;
	
	bool bConsolEnter;

	if(bConsolEnter){
		bConsolEnter = FALSE;

		switch(menu_step){
			
			case MENU_NONE:
			{
				myprintf(	"\r\n-----------------------------"
							"\r\n    TAG CONFIG               "
							"\r\n-----------------------------"
							"\r\nD : Display config           "
							"\r\nT : Time Setting             "
							"\r\nE : Exit                     "
							"\r\n-----------------------------"
							"\r\nSelect ? ");
				break;
			}

			case MENU_NUM:
			{	
				
				myputsln("# Set Date(YYMMDD) (ex)220702   ");					
				myputsln("# YY(22~99),MM(01~12),DD(01~31) ");					
				  myputs("# Enter 6 Digits(YYMMDD)? ");
				
				myputsln("# HH(00~23),MM(00~59),SS(00~59) ");					
				myputsln("# Set Time(HHMMSS) (ex)231015 ? ");		
	   			  myputs("# Enter 6 Digits(HHMMSS)? ");

				/*
				myputs("# Set Years(22~99) ? ");		
				myputs("# Set Months(1~12) ? ");		
				myputs("# Set Days(1~31) ? ");		
				myputs("# Set Hours(0~23) ? ");		
				myputs("# Set Minutes(0~59) ? ");					
				myputs("# Set Seconds(0~59) ? ");	
				*/
				break;
			}	

			case MENU_VALUE:
			{

				break;
			}			
		}
	}
}
#endif
