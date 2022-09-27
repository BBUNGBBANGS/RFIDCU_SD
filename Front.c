#include "Common.h"

/*****************************************************************************
*                                                                          
* 							LITERAL CONSTANTS(DEFINES)
*                    
******************************************************************************/
#define	KEYDEFS_TIMEOUT_READ_KEY	(50)	/* 50ms */
//#define KEYDEFS_REPEAT_COUNT		(60)	/* 50ms x 60 = 3.0sec*/
#define KEYDEFS_REPEAT_COUNT		(20)	/* 50ms x 20 = 1.0sec*/

#define KEYDEFS_REPEAT_SPEED_CHANGE_COUNT	(3)	
#define KEYDEFS_REPEAT_1ST_SPEED_COUNT		(8)/* 50ms x 8 = 0.4sec*/
#define KEYDEFS_REPEAT_2ND_SPEED_COUNT		(4)	/* 50ms x 4 = 0.2sec*/

	



/*****************************************************************************
*                                                                          
* 							TYPE DEFINITIONS(LOCAL)
*                    
******************************************************************************/

typedef struct	_FRONT_KEY
{	
	u8 	KeyData;
	u8 	KeyDataOld;	
	bool bNewData;
	bool bFirst;
	bool bPress;
	u32 CheckTimer;	
	u32 CntrForRepeat;
	u32 cntRepeatKeyRead;

}FRONT_KEY;


typedef struct	_COM_LED_STATUS
{	
	u32 timeout_rx_led;
	u32 timeout_tx_led;

}COM_LED_STATUS;

/*****************************************************************************
*                                                                          
* 							PUBLIC VARIABLES
*                    
******************************************************************************/

MENU_MODE 	MenuMode;
u8 menuParam;
u8 menuSetVal;
u8 menuClock;

s8 FndStr4[4];

bool bEnFndBlink = FALSE;
bool bFndBlinkRun= FALSE;

bool bLED_TestFlag;
bool bFndDisplaySet;		/** MDU파라미터 값 현시 */



/*****************************************************************************
*                                                                          
* 							LOCAL VARIABLES
*                    
******************************************************************************/



static FRONT_KEY FrontKey;

//static u32	LED_TestDisplayTimer;

static u8 FndBuf4[4];

static COM_LED_STATUS LedCAN[CANn];

static COM_LED_STATUS LedCOM[UARTn];



/*****************************************************************************
*                                                                          
* 						FUNCTION DEFINITIONS(LOCAL)
*                    
******************************************************************************/	
static void On_Digit(u8 DigitTen);
static void Off_Digit2(void);	
static void FND_DisplayTestTag(void);



/*==================================================================
* Function Name  	: LED_Init
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void LED_Init(void)
{
	u32 i;

	sGR_OUT *pNew = &Xout;
	sGR_OUT *pOld = &XoutOld;

	for(i=0; i<CANn; i++){
		RESET_TIMER(&LedCAN[i].timeout_rx_led);
		RESET_TIMER(&LedCAN[i].timeout_tx_led);
	}	

	for(i=0; i<UARTn; i++){
		RESET_TIMER(&LedCOM[i].timeout_rx_led);
		RESET_TIMER(&LedCOM[i].timeout_tx_led);
	}	
	
	pOld->LedRF_CAN.byte = pNew->LedRF_CAN.byte = __led_all_off;;
	pOld->LedTAG_TPS.byte= pNew->LedTAG_TPS.byte= __led_all_off;;

	Write_Data(SEL_WR_LED_RF_CAN, pOld->LedRF_CAN.byte);	
	Write_Data(SEL_WR_LED_TAG_TPS, pOld->LedTAG_TPS.byte);	
}

/*==================================================================
* Function Name  	: LED_SetRxCOM
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void LED_SetRxCOM(u8 COMx, u8 state)
{
	uLED_RF_CAN    *p = &Xout.LedRF_CAN;

	switch(COMx){
		
		case  COM_RFU1:
		{			
			(state == ON) ? (p->bits.Ld_RF1_RX = __led_on) :
							(p->bits.Ld_RF1_RX = __led_off);				
			break;
		}	

		
		case  COM_RFU2:
		{			
			(state == ON) ? (p->bits.Ld_RF2_RX = __led_on) :
							(p->bits.Ld_RF2_RX = __led_off);				
			break;
		}			
	}

	RESET_TIMER(&LedCOM[COMx].timeout_rx_led);								
}

/*==================================================================
* Function Name  	: LED_SetTxCOM
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void LED_SetTxCOM(u8 COMx, u8 state)
{
	uLED_RF_CAN    *p = &Xout.LedRF_CAN;

	switch(COMx){

		case  COM_RFU1:
		{			
			(state == ON) ? (p->bits.Ld_RF1_TX = __led_on) :
							(p->bits.Ld_RF1_TX = __led_off);				
			break;
		}	

		
		case  COM_RFU2:
		{			
			(state == ON) ? (p->bits.Ld_RF2_TX = __led_on) :
							(p->bits.Ld_RF2_TX = __led_off);				
			break;
		}					
	}

	RESET_TIMER(&LedCOM[COMx].timeout_tx_led);								
}
	
/*==================================================================
* Function Name 	: LED_SetTagAlarm
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void LED_SetTagAlarm(u8 state)
{
	uLED_TAG_TPS    *p = &Xout.LedTAG_TPS;
	
	(state == ON) ? (p->bits.Ld_TAG_ALARM = __led_on) :
					(p->bits.Ld_TAG_ALARM = __led_off);					
}

/*==================================================================
* Function Name 	: LED_SetTagAlarm
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void LED_SetTagRx(u8 state)
{
	uLED_TAG_TPS    *p = &Xout.LedTAG_TPS;
	
	(state == ON) ? (p->bits.Ld_TAG_RX = __led_on) :
					(p->bits.Ld_TAG_RX = __led_off);		

	RESET_TIMER(&TimerCount.timeout_tag_rx_led);
}

/*==================================================================
* Function Name  	: LED_CheckTagRx
* Description    	: Turns off LED selected when blinking time is over
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void LED_CheckTagRx(void)
{	
	uLED_TAG_TPS    *p = &Xout.LedTAG_TPS;
	
	if(p->bits.Ld_TAG_RX == __led_on){
		if(CHK_TOV(TimerCount.timeout_tag_rx_led, COM_TIMEOUT_LED_BLINK)){
			p->bits.Ld_TAG_RX = __led_off;
		}
	}			
}

/*==================================================================
* Function Name  	: LED_CheckCOM
* Description    	: Turns off LED selected when blinking time is over
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void LED_CheckCOM(void)
{	
	u8 COMx;
	uLED_RF_CAN    *p = &Xout.LedRF_CAN;
	

	for(COMx = COM_RFU1; COMx <= COM_RFU2; COMx++){
		
		switch(COMx){
			
			case COM_RFU1:
			{							
				if(p->bits.Ld_RF1_TX == __led_on){
					if(CHK_TOV(LedCOM[COMx].timeout_tx_led, COM_TIMEOUT_LED_BLINK)){
						p->bits.Ld_RF1_TX = __led_off;
					}
				}			

				if(p->bits.Ld_RF1_RX == __led_on){
					if(CHK_TOV(LedCOM[COMx].timeout_rx_led, COM_TIMEOUT_LED_BLINK)){
						p->bits.Ld_RF1_RX = __led_off;
					}
				}															
				
				break;
			}				

			case COM_RFU2:
			{							
				if(p->bits.Ld_RF2_TX == __led_on){
					if(CHK_TOV(LedCOM[COMx].timeout_tx_led, COM_TIMEOUT_LED_BLINK)){
						p->bits.Ld_RF2_TX = __led_off;
					}
				}			

				if(p->bits.Ld_RF2_RX == __led_on){
					if(CHK_TOV(LedCOM[COMx].timeout_rx_led, COM_TIMEOUT_LED_BLINK)){
						p->bits.Ld_RF2_RX = __led_off;
					}
				}															
				
				break;
			}				
		}	
	}
}





/*==================================================================
* Function Name  	: LED_SetRxCAN
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void LED_SetRxCAN(u8 CANx, u8 state)
{
	uLED_RF_CAN    *p = &Xout.LedRF_CAN;

	switch(CANx){
		
		case  CAN_PORT_1:
		{			
			(state == ON) ? (p->bits.Ld_CAN1_RX = __led_on) :
							(p->bits.Ld_CAN1_RX = __led_off);				
			break;
		}	

		
		case  CAN_PORT_2:
		{			
			(state == ON) ? (p->bits.Ld_CAN2_RX = __led_on) :
							(p->bits.Ld_CAN2_RX = __led_off);				
			break;
		}			
	}

	RESET_TIMER(&LedCAN[CANx].timeout_rx_led);								
}


/*==================================================================
* Function Name  	: LED_SetTxCAN
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void LED_SetTxCAN(u8 CANx, u8 state)
{
	uLED_RF_CAN    *p = &Xout.LedRF_CAN;

	switch(CANx){

		case  CAN_PORT_1:
		{			
			(state == ON) ? (p->bits.Ld_CAN1_TX = __led_on) :
							(p->bits.Ld_CAN1_TX = __led_off);				
			break;
		}	

		
		case  CAN_PORT_2:
		{			
			(state == ON) ? (p->bits.Ld_CAN2_TX = __led_on) :
							(p->bits.Ld_CAN2_TX = __led_off);				
			break;
		}					
	}

	RESET_TIMER(&LedCAN[CANx].timeout_tx_led);								
}


/*==================================================================
* Function Name  	: LED_CheckCAN
* Description    	: Turns off LED selected when blinking time is over
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void LED_CheckCAN(void)
{	
	u8 CANx;
	
	uLED_RF_CAN    *p = &Xout.LedRF_CAN;

	for(CANx = CAN_PORT_1; CANx <= CAN_PORT_2; CANx++){
		
		switch(CANx){			
			
			case  CAN_PORT_1:
			{					
				if(!CAN_IsBusOff(CANx)){
					if(p->bits.Ld_CAN1_TX == __led_on){
						if(CHK_TOV(LedCAN[CANx].timeout_tx_led, CAN_TIMEOUT_LED_BLINK)){
							p->bits.Ld_CAN1_TX = __led_off;
						}
					}	
				}	
				else{
					if(p->bits.Ld_CAN1_TX == __led_off){
						p->bits.Ld_CAN1_TX = __led_on;
						RESET_TIMER(&LedCAN[CANx].timeout_tx_led);
					}
				}

				if(p->bits.Ld_CAN1_RX == __led_on){
					if(CHK_TOV(LedCAN[CANx].timeout_rx_led, CAN_TIMEOUT_LED_BLINK)){
						p->bits.Ld_CAN1_RX = __led_off;
					}
				}			

				break;
			}		

			case  CAN_PORT_2:
			{					
				if(!CAN_IsBusOff(CANx)){
					if(p->bits.Ld_CAN2_TX == __led_on){
						if(CHK_TOV(LedCAN[CANx].timeout_tx_led, CAN_TIMEOUT_LED_BLINK)){
							p->bits.Ld_CAN2_TX = __led_off;
						}
					}	
				}	
				else{
					if(p->bits.Ld_CAN2_TX == __led_off){
						p->bits.Ld_CAN2_TX = __led_on;
						RESET_TIMER(&LedCAN[CANx].timeout_tx_led);
					}
				}

				if(p->bits.Ld_CAN2_RX == __led_on){
					if(CHK_TOV(LedCAN[CANx].timeout_rx_led, CAN_TIMEOUT_LED_BLINK)){
						p->bits.Ld_CAN2_RX = __led_off;
					}
				}			

				break;
			}					
		}	
	}
}

/*==================================================================
* Function Name 	:  LED_Update
* Description		:  10ms 마다 호출됨
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void LED_Update(void)
{

	if(!bLED_TestFlag){
		
		WRITE_XOUT(SEL_WR_LED_RF_CAN);	
		WRITE_XOUT(SEL_WR_LED_TAG_TPS);		
	}
}

/*==================================================================
* Function Name 	: LED_Test
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void LED_Test(u8 data)
{
	sGR_OUT *pOld = &XoutOld;
	
	pOld->LedRF_CAN.byte = data;
	Write_Data(SEL_WR_LED_RF_CAN, pOld->LedRF_CAN.byte);	
	
	pOld->LedTAG_TPS.byte= data;	
	Write_Data(SEL_WR_LED_TAG_TPS, pOld->LedTAG_TPS.byte);	
}


/*==================================================================
* Function Name  	: On_Digit
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
static void On_Digit(u8 DigitTen)
{	
	// OFF digit common(-) of FND1 and 2 (CF) UL2803 is a inverter
	// 0: OFF, 1: ON
	
	if(DigitTen){		
		Xout.FndSel.bits.Fnd_Dig1 = __fnd_on;
		Xout.FndSel.bits.Fnd_Dig3 = __fnd_on;
	}else{
		Xout.FndSel.bits.Fnd_Dig2 = __fnd_on;
		Xout.FndSel.bits.Fnd_Dig4 = __fnd_on;
	}	

	WRITE_XOUT(SEL_WR_FND_SEL);/* FND */	
}


/*==================================================================
* Function Name  	: Off_Digit2
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
static void Off_Digit2(void)
{
	Xout.FndSel.bits.Fnd_Dig1 = __fnd_off;
	Xout.FndSel.bits.Fnd_Dig2 = __fnd_off;		
	Xout.FndSel.bits.Fnd_Dig3 = __fnd_off;
	Xout.FndSel.bits.Fnd_Dig4 = __fnd_off;	
	
	WRITE_XOUT(SEL_WR_FND_SEL);
}

/*==================================================================
* Function Name 	:  FND_Init
* Description		:  
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void FND_Init(void)
{
	FndBuf4[0] = FND_OFF;
	FndBuf4[1] = FND_OFF;
	FndBuf4[2] = FND_OFF;
	FndBuf4[3] = FND_OFF;
	Write_Data(SEL_WR_FND1, FND_OFF);
	Write_Data(SEL_WR_FND2, FND_OFF);
	
	Off_Digit2();
}	

/*==================================================================
* Function Name  	: Fnd_ChangeDigit
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void FND_ChangeDigit(void)
{	
	static u8 bDigitTen   = 0;	
	static u8 NextFndState= ON;
		
	if(IS_TIMEOUT(&TimerCount.FndDisTime, 10)){

		if(bFndDisplaySet && CHK_TOV(TimerCount.FndDisSETtag, 800)){			
			
			bFndDisplaySet = FALSE;	

			if(MenuMode == MENU_VALUE){
				MenuMode = MENU_NONE;
				FND_MenuNone();		
			}		
			else if(MenuMode == MENU_VALUE_CLOCK){
				MenuMode = MENU_VALUE;
				FND_DisplayMenu();
			}
		}	

		// Update FND display
		{
			Off_Digit2();

			bDigitTen ^= 0x01;						
			
			if((MenuMode != MENU_NONE) && !bFndDisplaySet){

				if(bEnFndBlink){
					// 버튼취급후 0.5초 경과시 부터 점멸 개시 
					if(!bFndBlinkRun){						
						if(CHK_TOV(TimerCount.FndBlinkTime, 500)){
							bFndBlinkRun = TRUE;
							NextFndState  = OFF;
							RESET_TIMER(&TimerCount.FndBlinkTime);
						}
						else
							NextFndState = ON;
					}									
				}		
				else{ 
					
					NextFndState = ON;
					RESET_TIMER(&TimerCount.FndBlinkTime);
				}	
				
				if(bFndBlinkRun){

					if(IS_TIMEOUT(&TimerCount.FndBlinkTime, 500)){
						mytoggle(&NextFndState);
					}	
				}
				else
					NextFndState = ON;
					

				if(NextFndState == OFF){

					if((MenuMode == MENU_VALUE)|| (MenuMode == MENU_VALUE_CLOCK)){
						
						if((menuParam == PARAM_ATTN)||(MenuMode == MENU_VALUE_CLOCK)){
							
							/* [nn] --> 0 1 2 3 : 1, 2 off*/
							if(bDigitTen){/* Digit 1 (1x) : 10의자리*/		

								Write_Data(SEL_WR_FND1, FND_OFF);
								Write_Data(SEL_WR_FND2, FndBuf4[0]);

								On_Digit(1);
							}		
							else{/* Digit 2 (x1) : 1의자리 */		

								Write_Data(SEL_WR_FND1, FndBuf4[3]);
								Write_Data(SEL_WR_FND2, FND_OFF);
								
								On_Digit(0);
							}
						}
						else if(menuParam != PARAM_CLOCK){/*' ON', 'OFF' */
	

							Write_Data(SEL_WR_FND1, FND_OFF);
							Write_Data(SEL_WR_FND2, FND_OFF);
								
							if(bDigitTen) 	On_Digit(1);
							else			On_Digit(0);							
						}
					}					
				}				
			}
			else 
				NextFndState = ON;

			if(NextFndState == ON){
				
				if(bDigitTen){/* Digit 1 (1x) : 10의자리*/		

					Write_Data(SEL_WR_FND2, FndBuf4[0]);
					Write_Data(SEL_WR_FND1, FndBuf4[2]);					

					On_Digit(1);
				}		
				else{/* Digit 2 (x1) : 1의자리 */		

					Write_Data(SEL_WR_FND2, FndBuf4[1]);
					Write_Data(SEL_WR_FND1, FndBuf4[3]);
					
					
					On_Digit(0);
				}
			}
		}
	}
}

/*==================================================================
* Function Name 	: FND_Output4Char
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void FND_Output4Char(s8 *s)
{
	u8 i, data[4], indx[4];
    const u8 FND_Map[36] = { 
        FND_NUM_0, FND_NUM_1, FND_NUM_2, FND_NUM_3,
        FND_NUM_4, FND_NUM_5, FND_NUM_6, FND_NUM_7,
        FND_NUM_8, FND_NUM_9, 

		FND_ALP_A, FND_ALP_B, FND_ALP_C, FND_ALP_D,
        FND_ALP_E, FND_ALP_F, FND_ALP_G, FND_ALP_H,
        FND_ALP_I, FND_ALP_J, FND_ALP_K, FND_ALP_L,
        FND_ALP_M, FND_ALP_N, FND_ALP_O, FND_ALP_P,
        FND_ALP_Q, FND_ALP_R, FND_ALP_S, FND_ALP_T,
        FND_ALP_U, FND_ALP_V, FND_ALP_W, FND_ALP_X,
        FND_ALP_Y, FND_ALP_Z		
    };

	data[0] = *s;
    data[1] = *(s+1);	
    data[2] = *(s+2);	
    data[3] = *(s+3);	

	// char index
    for (i = 0; i < 4; i++)
	{
        if (data[i] >= 'A' && data[i] <= 'Z'){
			indx[i] = (data[i] - 'A') + 10;
			FndBuf4[i] = FND_Map[indx[i]];
        }	
		else if(data[i] == 'u'){
			FndBuf4[i] = FND_ALP_SU;
		}
		else if(data[i] == 'c'){
			FndBuf4[i] = FND_ALP_SC;
		}
		else if(data[i] == 'l'){
			FndBuf4[i] = FND_ALP_SL;
		}
		else if(data[i] == 'n'){
			FndBuf4[i] = FND_ALP_N;
		}
		else if(data[i]== ' '){
			FndBuf4[i] = FND_OFF;
		}
		else if(data[i]== '-'){
			
			FndBuf4[i] = FND_BAR;
		}
		else if(data[i] == '.'){
			
			FndBuf4[i] = FND_DOT;
		}
		else if(data[i] == '['){
			
			FndBuf4[i] = FND_LEFT_BRACE;			
		}		
		else if(data[i] == ']'){
			
			FndBuf4[i] = FND_RIGHT_BRACE;			
		}
        else{
			indx[i] = data[i] - '0';
			FndBuf4[i] = FND_Map[indx[i]];
        }		
    }	
}


/*==================================================================
* Function Name 	: FND_Display4Num
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void FND_Display4Num(u32 value)
{	
	// value는 32bit Display 표시는 9999까지 한다. 
	if(value > 9999) value %= 10000;
	
	FndStr4[0] = (value / 1000) + '0';		
	FndStr4[1] = ((value % 1000)/100) + '0';
	FndStr4[2] = ((value % 100)/10) + '0';
	FndStr4[3] = ((value % 10)) + '0';
	
	FND_Output4Char(&FndStr4[0]);
}

/*==================================================================
* Function Name 	: FND_FWVer
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void FND_FWVer(void)
{			
	u8 data[3]={0, 0, 0};
	
	#ifdef __APPROVAL_TEST__
		FndStr4[0] = 'A';	
	#else
		#ifdef __MOCKUP_SELF_TEST__		
			FndStr4[0] = 'E';	
		#else
			
			FndStr4[0] = 'F';							
		#endif
	#endif

	if(__SET_VER(&data[0])){
		
		FndStr4[1] = data[0] + '0';	
		FndStr4[2] = data[1] + '0';	
		FndStr4[3] = data[2] + '0';	
	}
	else{
		mystrncpy((s8*)&FndStr4[0], "F---", 4);	
	}

	FND_Output4Char(&FndStr4[0]);

	FndBuf4[1] |= FND_DOT;	/* n.n.n */
	FndBuf4[2] |= FND_DOT;	/* n.n.n */	
}

/*==================================================================
* Function Name 	: FND_all_on
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void FND_all_on(void)
{
	FndBuf4[0] = FND_ALL_ON;
	FndBuf4[1] = FND_ALL_ON;
	FndBuf4[2] = FND_ALL_ON;
	FndBuf4[3] = FND_ALL_ON;
}


/*==================================================================
* Function Name 	: FND_DisplayTag
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void FND_DisplayTag(void)
{
	u8 temp;
	bool bDispTagCnt = FALSE;

	if(MenuMode == MENU_NONE){
		
		// Bypass Mode Display
		if(TagConf.tagTest){

			mystrncpy((s8*)&FndStr4[0], "T---", 4);		
		}
		else if(bAutoTagTestRun){
			mystrncpy((s8*)&FndStr4[0], "T-TT", 4);		
		}
		else if(TagConf.tagBypass||(TagMode == TAG_MODE_BYPASS)){
			
			mystrncpy((s8*)&FndStr4[0], "B---", 4);					
		}		
		else if(TagMode == TAG_MODE_AUTO_RESEND){

			mystrncpy((s8*)&FndStr4[0], "A---", 4);		
		}		
		else if(TagMode >= TAG_MODE_TRAIN_INTERLOCK){
			
			switch(TPU_GET_POS()){	

				case TRN_NONE:
				{
					mystrncpy((s8*)&FndStr4[0], "----", 4);

					break;
				}
				
				case TRN_ENTER:		mystrncpy((s8*)&FndStr4[0], "E---", 4);	break;		
				case TRN_STOP:	    mystrncpy((s8*)&FndStr4[0], "P---", 4);	break;	
				case TRN_LEAVE:		mystrncpy((s8*)&FndStr4[0], "L---", 4);	break;		
				case TRN_UNKNOWN:	mystrncpy((s8*)&FndStr4[0], "U---", 4);	break;				
				default: break;	
			}		
		}
		else{
			
			mystrncpy((s8*)&FndStr4[0], "F---", 4);		
		}		

		if(TagConf.tagTest||bAutoTagTestRun){

			FND_DisplayTestTag();
		}
		else{			
		
			if(tagGrfOld.bTagOnTime &&  (tagGrfOld.cntTagSum > 0)){

				if(TPU_GET_POS() == TRN_NONE) FndStr4[0] = 'R'; 

				if((TagConf.tagBypass==OFF) && (TagMode == TAG_MODE_TRAIN_INTERLOCK)){	

					bDispTagCnt = FALSE;
					
					if(tagGrfOld.bNotMytag)					FndStr4[1] = 'N';
					else if(TAG_IsTrainNoneTagRxed())		FndStr4[1] = 'c'; 								
					else if(tagGrfOld.bNextTagCheckSend)	FndStr4[1] = 'D'; 						
					else									bDispTagCnt = TRUE;
				}	
				else if((TagConf.tagBypass==OFF) && (TagMode == TAG_MODE_TRAIN_ON)){	

					bDispTagCnt = FALSE;					

					if(tagGrfOld.bNextTagCheckSend)			FndStr4[1] = 'D'; 						
					else									bDispTagCnt = TRUE;
					
				}
				else{

					bDispTagCnt = TRUE;
				}	
					

				if(bDispTagCnt){				
					// 1 ... 9
					temp = 	(tagGrfOld.cntTagSum % 9)+1;				
					dec1str(temp, &FndStr4[1]);	
				}

				temp = (tagGrfOld.OrgNo >> 4) * 10;
				temp += (tagGrfOld.OrgNo & 0x0F);

				// 편성번호 
				if(temp < 100){
					dec2str(temp, &FndStr4[2]);
				}
				

				FND_Output4Char(&FndStr4[0]);			
				
				// x.c.--
				if(tagGrfOld.cntTagSum > 9) FndBuf4[0] |= FND_DOT;
				
				// x c.--
				FndBuf4[1] |= FND_DOT;				
				
			}
			else{

				FND_Output4Char(&FndStr4[0]);
			}		
		}
	}
}

/*==================================================================
* Function Name 	: FND_DisplayClockMenu
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void FND_DisplayClockMenu(u8 disMode)
{
	bool bDisOn = TRUE;
	
	if(disMode == _clock_param){/* MENU_VALUE 에서 현시 */

		switch(menuClock){
		
			case MENU_CLOCK_YEAR:	menuSetVal = (u8)(MRTC_GetYear()%100);										
									mystrncpy((s8*)&FndStr4[0], "D1  ", 4); 					
									break;				

			case MENU_CLOCK_MONTH:	menuSetVal = MRTC_GetMonth();	
									mystrncpy((s8*)&FndStr4[0], "D2  ", 4); 					
									break;
									
			case MENU_CLOCK_DAY:	menuSetVal = MRTC_GetDay(); 		
									mystrncpy((s8*)&FndStr4[0], "D3  ", 4); 					
									break;
									
			case MENU_CLOCK_HOUR:	menuSetVal = MRTC_GetHour();									
									mystrncpy((s8*)&FndStr4[0], "H1  ", 4);
									break;
									
			case MENU_CLOCK_MIN:	menuSetVal = MRTC_GetMin(); 
									
									mystrncpy((s8*)&FndStr4[0], "H2  ", 4);
									break;
									
			case MENU_CLOCK_SEC:	menuSetVal = MRTC_GetSec(); 
									mystrncpy((s8*)&FndStr4[0], "H3  ", 4);
									break;
			
			default: 				bDisOn = FALSE; 
									break;
		}

		if(bDisOn){			
			
			dec2str(menuSetVal, &FndStr4[2]);
			FND_Output4Char(&FndStr4[0]);
			FndBuf4[1] |= FND_DOT;
		}	
	}
	else{ /* _clock_setval */ /* MENU_VALUE_CLOCK 에서 현시 */

		bEnFndBlink = TRUE;
		
		if(menuSetVal < 100){
			mystrncpy((s8*)&FndStr4[0], "[--]", 4);	
			dec2str(menuSetVal, &FndStr4[1]);
			FND_Output4Char(&FndStr4[0]);
		}
	}
}


/*==================================================================
* Function Name 	: FND_DisplayMenu
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
bool FND_DisplayMenu(void)
{		
	bool bDisOn = TRUE;

	if(MenuMode == MENU_NUM){	
		
		switch(menuParam){
			case PARAM_TEST:	mystrncpy((s8*)&FndStr4[0], "TEST", 4);		break;	
			case PARAM_ATTN:	
			{
				if(TagConf.ApulsePowerMode)
					mystrncpy((s8*)&FndStr4[0], "POuT", 4);	
				else			
					mystrncpy((s8*)&FndStr4[0], "ATTN", 4);		break;	
			}					
			case PARAM_BYPASS:	mystrncpy((s8*)&FndStr4[0], "PASS", 4);		break;				
			case PARAM_BEEP:	mystrncpy((s8*)&FndStr4[0], "BEEP", 4);		break;	
			case PARAM_CLOCK:	mystrncpy((s8*)&FndStr4[0], "cLOc", 4);		break;	
			default: 			bDisOn = FALSE; 							break;
		}	

		if(bDisOn) FND_Output4Char(&FndStr4[0]);	
	}
	else if(MenuMode == MENU_VALUE){

		if(menuParam == PARAM_CLOCK){

			// ex)YE.22, th.06, dA.29....
			FND_DisplayClockMenu(_clock_param);	
		}
		else{

			bEnFndBlink = TRUE;
				
			switch(menuParam){

				case PARAM_TEST:	
				{
					if(menuSetVal) 	mystrncpy((s8*)&FndStr4[0], " RuN", 4);// run	
					else			mystrncpy((s8*)&FndStr4[0], "STOP", 4);
					break;
				}					
						
				case PARAM_ATTN:
				{
					mystrncpy((s8*)&FndStr4[0], "[--]", 4);								
					if(RFID_IsAttnValueOkay(menuSetVal)){
						if(TagConf.ApulsePowerMode){
							if(menuSetVal >= RFID_AP_MIN_ATTN_VALUE){
								dec2str((menuSetVal-RFID_AP_MIN_ATTN_VALUE)+1, &FndStr4[1]);	
								// [01] ~ [10]
							}
						}	
						else
							dec2str(menuSetVal, &FndStr4[1]);						
					}	
					break;
				}	
				
				case PARAM_BYPASS:
				{
					if(menuSetVal) 	mystrncpy((s8*)&FndStr4[0], "  0N", 4);		
					else			mystrncpy((s8*)&FndStr4[0], " 0FF", 4);	
					break;
				}	
				
				
				case PARAM_BEEP:	
				{
					if(menuSetVal) 	mystrncpy((s8*)&FndStr4[0], "  0N", 4);		
					else			mystrncpy((s8*)&FndStr4[0], " 0FF", 4);	
					break;
				}					

				default: 	bDisOn = FALSE; 	break;			
			}

			if(bDisOn) FND_Output4Char(&FndStr4[0]);	
		}			
	}
	else if(MenuMode == MENU_VALUE_CLOCK){
		
		// ex)[nn] 
		FND_DisplayClockMenu(_clock_setval);	
	}	

	return(bDisOn);
}



/*==================================================================
* Function Name 	: FND_DisplayTestTag
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
static void FND_DisplayTestTag(void)
{
	u8 temp;
		
	if(tagTestOld.bTagOnTime &&  (tagTestOld.cntTagSum > 0)){	
		
		// 1 ... 9
		temp = 	(tagTestOld.cntTagSum % 9)+1;				
		dec1str(temp, &FndStr4[1]);	

		if(TagConf.tagTest){
			
			temp = (tagTestOld.OrgNo >> 4) * 10;
			temp += (tagTestOld.OrgNo & 0x0F);

			// 편성번호 
			if(temp < 100){
				dec2str(temp, &FndStr4[2]);
			}
		}
		else if(bAutoTagTestRun){
			
			FndStr4[2] = 'T';
			FndStr4[3] = 'T';
		}
			
		FND_Output4Char(&FndStr4[0]);			
		
		// x.c--
		if(tagTestOld.cntTagSum > 9) FndBuf4[0] |= FND_DOT;
		// xc.--
		FndBuf4[1] |= FND_DOT;									
	}
	else{

		FND_Output4Char(&FndStr4[0]);
	}		
}


/*==================================================================
* Function Name 	: FND_ReadyOutput
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void FND_ReadyOutput(void)
{	
	if(MenuMode == MENU_NONE){
		
		FND_DisplayTag();									
	}
}

/*==================================================================
* Function Name 	: FND_MenuNone
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void FND_MenuNone(void)
{
	if(MenuMode == MENU_NONE){
					
		FND_DisplayTag();				
	}
}

/*==================================================================
* Function Name 	: FND_MenuNone
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void FND_DisplaySet(void)
{
	FND_Output4Char(FND4_SET);
	bFndDisplaySet = TRUE;
	RESET_TIMER(&TimerCount.FndDisSETtag);	
}						


/*==================================================================
* Function Name 	: FND_DisplayInvalid
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void FND_DisplayInvalid(void)
{
	FND_Output4Char(FND4_INVALID);
	bFndDisplaySet = TRUE;
	RESET_TIMER(&TimerCount.FndDisSETtag);	
}						


/*==================================================================
* Function Name  	: KEY_Init
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void KEY_Init(void)
{
	FrontKey.CheckTimer = GetTickCount(TIMER0);
	FrontKey.bFirst 	= FALSE;
	FrontKey.bPress 	= FALSE;
	FrontKey.KeyData 	= KEYDEFS_NULL;
	FrontKey.KeyDataOld = KEYDEFS_NULL;

	bLED_TestFlag = FALSE;
}

/*==================================================================
* Function Name  	: KEY_READ
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
bool KEY_Read(void)
{
	u8 rData;
	FRONT_KEY *p = &FrontKey;	

	if(IS_TIMEOUT(&p->CheckTimer,KEYDEFS_TIMEOUT_READ_KEY)){
		
		rData = INVBYTE(Read_Data(SEL_RD_SDI)) & 0x0F;

		if(rData == KEYDEFS_NULL) p->KeyDataOld = KEYDEFS_NULL;
		else{
			if(rData != p->KeyDataOld){

				p->bNewData = TRUE;
				p->CntrForRepeat 	= 0;
				p->cntRepeatKeyRead = 0;
				p->KeyData = rData;
				p->bPress = TRUE;	
				
			}else {/* repeat */

				p->KeyData = rData;
				p->bPress = TRUE;	
				p->CntrForRepeat++;				
			}			
		}	
	}

	return(p->bPress);
}


/*==================================================================
* Function Name 	: KEY_BootRead(void)
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/	
u8 KEY_BootRead(void)
{
	u32 timeout_10ms, timeout;
	u8 rData = 0x00;
	
	Clear_WatchDog();			
	
	RESET_TIMER(&timeout);
	RESET_TIMER(&timeout_10ms);	
	
	while (!CHK_TOV(timeout, 250)){	

		if(CHK_TOV(timeout_10ms, DEFS_TIME_10MS)){
			RESET_TIMER(&timeout_10ms);

			rData = INVBYTE(Read_Data(SEL_RD_SDI)) & 0x0F;			
		}

		Console_CheckSendData();
	}		
	
	Clear_WatchDog();			

	return(rData);
}

/*==================================================================
* Function Name  	: KEY_Process
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
bool KEY_Process(void)
{
	bool bRead 	 = FALSE;

	FRONT_KEY *p = &FrontKey;

	if(KEY_Read()){
		p->bPress = FALSE;
		
		if(p->bNewData){
			p->bNewData = FALSE;
			p->KeyDataOld = p->KeyData;

			myputs("#KEY [NEW] :: ");

			if(p->KeyData == KEYDEFS_MENU)		myputsln("#MENU KEY");
			else if(p->KeyData == KEYDEFS_UP)	myputsln("#UP KEY");
			else if(p->KeyData == KEYDEFS_DN)	myputsln("#DOWN KEY");
			else if(p->KeyData == KEYDEFS_ENT)	myputsln("#ENTER KEY");
			
			//myputxln(p->KeyData);			
			
			bRead = TRUE;
			
		}else{/* key repeat */

			if(MenuMode == MENU_NONE){

				if(p->CntrForRepeat > KEYDEFS_REPEAT_COUNT){
					p->CntrForRepeat = 0;
					
					myputs("#KEY [RPT] :: ");
					//myputxln(p->KeyData);			

					/*
					if(HWDEFS_ENT_KEY_CLR_TAG_ON_FND() && p->KeyData == KEYDEFS_ENT){
						myputsln("#LED TEST MODE :: ALL ON");					
						bLED_TestFlag = TRUE;
						RESET_TIMER(&LED_TestDisplayTimer);

						LED_Test(__led_all_on);						
					}
					*/
					
					
					//bRead = TRUE;
				}
			}
			else{
				
				if(!bFndDisplaySet && ((p->KeyData == KEYDEFS_UP) || (p->KeyData == KEYDEFS_DN))){
					
					if((MenuMode == MENU_VALUE_CLOCK)	||
					   ((MenuMode == MENU_VALUE) && (menuParam == PARAM_ATTN)) ){
					   
						// 3번이하는 0.8sec간격으로 설정값 변경함 
						if(p->cntRepeatKeyRead < KEYDEFS_REPEAT_SPEED_CHANGE_COUNT){
							
							if(p->CntrForRepeat >=  KEYDEFS_REPEAT_1ST_SPEED_COUNT){
								p->CntrForRepeat = 0;
								
								p->cntRepeatKeyRead++;
								bRead = TRUE;
							}	
						}								
						else{
							// 4번째 부터는 
							p->cntRepeatKeyRead = KEYDEFS_REPEAT_SPEED_CHANGE_COUNT;
							if(p->CntrForRepeat >=  KEYDEFS_REPEAT_2ND_SPEED_COUNT){
								p->CntrForRepeat = 0;					
								bRead = TRUE;
							}	
						}			
					}
				}
			}			
		}
	}		
	
	return(bRead);
}	

/*==================================================================
* Function Name  	: MENU_Init
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void MENU_Init(void)
{
	MenuMode 	= MENU_NONE;	
	menuParam 	= PARAM_ATTN;			
	menuSetVal	= 0;
}

/*==================================================================
* Function Name  	: MENU_ValueClock
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void MENU_ValueClock(u8 Key)
{

	if(Key == KEYDEFS_MENU){				
		/* Display Previous menu */
		MenuMode = MENU_VALUE;								
		FND_DisplayMenu();	
	}
	else if(Key == KEYDEFS_UP){

		if(menuClock == MENU_CLOCK_YEAR){
			if(++menuSetVal > 99) menuSetVal = 22;
		}
		else if(menuClock == MENU_CLOCK_MONTH){
			if(++menuSetVal > 12) menuSetVal = 1;
		}
		else if(menuClock == MENU_CLOCK_DAY){
			if(++menuSetVal > MRTC_GetMaxDay()) menuSetVal = 1;
		}
		else if(menuClock == MENU_CLOCK_HOUR){
			if(++menuSetVal > 23) menuSetVal = 0;
		}
		else if(menuClock == MENU_CLOCK_MIN){
			if(++menuSetVal > 59) menuSetVal = 0;
		}
		else if(menuClock == MENU_CLOCK_SEC){
			if(++menuSetVal > 59) menuSetVal = 0;
		}				

		FND_DisplayMenu();
	}
	else if(Key == KEYDEFS_DN){

		if(menuClock == MENU_CLOCK_YEAR){
			if(menuSetVal > 22) menuSetVal--;
			else				menuSetVal = 99;
		}
		else if(menuClock == MENU_CLOCK_MONTH){
			if(menuSetVal > 1)  menuSetVal--;
			else				menuSetVal = 12;					
		}
		else if(menuClock == MENU_CLOCK_DAY){
			if(menuSetVal > 1)  menuSetVal--;
			else				menuSetVal = MRTC_GetMaxDay();					
		}
		else if(menuClock == MENU_CLOCK_HOUR){
			if(menuSetVal > 1)  menuSetVal--;
			else				menuSetVal = 23;					
		}
		else if(menuClock == MENU_CLOCK_MIN){
			if(menuSetVal > 1)  menuSetVal--;
			else				menuSetVal = 59;					
		}
		else if(menuClock == MENU_CLOCK_SEC){
			if(menuSetVal > 1)  menuSetVal--;
			else				menuSetVal = 59;					
		}				

		FND_DisplayMenu();
	}
	else if(Key == KEYDEFS_ENT){

		if(menuClock == MENU_CLOCK_YEAR){

			if(MRTC_SetMenuDate((u16)(2000+menuSetVal), MRTC_GetMonth(), MRTC_GetDay())){

				FND_DisplaySet();
			}
			else{
				FND_DisplayInvalid();
			}										
		}
		else if(menuClock == MENU_CLOCK_MONTH){
			if(MRTC_SetMenuDate(MRTC_GetYear(), menuSetVal, MRTC_GetDay())){
				FND_DisplaySet();
			}
			else{
				FND_DisplayInvalid();
			}										
		}
		else if(menuClock == MENU_CLOCK_DAY){
			if(MRTC_SetMenuDate(MRTC_GetYear(), MRTC_GetMonth(), menuSetVal)){
				FND_DisplaySet();
			}
			else{
				FND_DisplayInvalid();
			}										
		}
		else if(menuClock == MENU_CLOCK_HOUR){
			if(MRTC_SetMenuTime(menuSetVal, MRTC_GetMin(), MRTC_GetSec())){
				FND_DisplaySet();
			}
			else{
				FND_DisplayInvalid();
			}																			
		}
		else if(menuClock == MENU_CLOCK_MIN){
			/* 분설정시 초는 0으로 초기회 한다. */
			if(MRTC_SetMenuTime(MRTC_GetHour(), menuSetVal, 0)){

				RTC_100msCnt 	= 0;	
				RESET_TIMER(&TimerCount.period_rtc_100ms);	
				
				FND_DisplaySet();
			}
			else{
				FND_DisplayInvalid();
			}																			
		}
		else if(menuClock == MENU_CLOCK_SEC){
			if(MRTC_SetMenuTime(MRTC_GetHour(), MRTC_GetMin(), menuSetVal)){

				RTC_100msCnt 	= 0;	
				RESET_TIMER(&TimerCount.period_rtc_100ms);	
				
				FND_DisplaySet();
			}
			else{
				FND_DisplayInvalid();
			}																			
		}
	}
}	

/*==================================================================
* Function Name  	: MENU_Process
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void MENU_Process(void)
{
	u8 Key = FrontKey.KeyData;

	if((Key == KEYDEFS_MENU) ||(Key == KEYDEFS_UP) 	|| (Key == KEYDEFS_DN) 
		|| (Key == KEYDEFS_ENT)){

		bEnFndBlink = FALSE;

		// 버튼 누름시 0.5초간 점멸 정지 
		bFndBlinkRun =FALSE;
		RESET_TIMER(&TimerCount.FndBlinkTime);	
	}
	
	switch(MenuMode){

		case MENU_NONE:
		{
			if(Key == KEYDEFS_MENU){
				
				MenuMode  = MENU_NUM;	
				menuParam = PARAM_TEST;	
				FND_DisplayMenu();	
			}			
			break;
		}				

		case MENU_NUM:			
		{			
			if(Key == KEYDEFS_MENU){ /* (ESC) menu 종료 */
				
				MenuMode = MENU_NONE;	FND_MenuNone();	
			}					
			else if(Key == KEYDEFS_UP){ 
				
				if(++menuParam >= PARAM_END) menuParam = PARAM_TEST;

				FND_DisplayMenu();								
			}
			else if(Key == KEYDEFS_DN){	
				
				if(menuParam > PARAM_TEST) 	menuParam--;						
				else						menuParam = PARAM_CLOCK;				
				FND_DisplayMenu();
			}
			else if(Key == KEYDEFS_ENT){			

				MenuMode = MENU_VALUE;
				{		
					/* test mode 실행중일경우, stop 표시하여 enter 누를시 즉시 종료 토록 함 */
					if(menuParam == PARAM_TEST){					
						
						if(TagConf.tagTest)	menuSetVal = 0; /* stop */
						else				menuSetVal = 1;	/* run */									
					}
					else if(menuParam == PARAM_ATTN){					

						menuSetVal = TagConf.rfidAttn;
					}
					else if(menuParam == PARAM_BYPASS){ /* 현재 설정상태를 표시함*/
						
						if(TagConf.tagBypass)	menuSetVal = 1; /* on */
						else					menuSetVal = 0;	/* off*/			
					}	
					else if(menuParam == PARAM_BEEP){	/* 현재 설정상태를 표시함*/

						if(TagConf.rfidBeep)	menuSetVal = 1;
						else					menuSetVal = 0;					
					}			
					else if(menuParam == PARAM_CLOCK){					

						/* ex) clock menu 진입시, year설정 메뉴및 현 설정값 표시 함  'YE.22' */
						menuClock 	= MENU_CLOCK_YEAR;										
					}
					
					
					FND_DisplayMenu();
				}					
			}
			
			break;
		}
		
		case MENU_VALUE:
		{		
			if(Key == KEYDEFS_MENU){		
				
				/* Display Previous menu */
				MenuMode = MENU_NUM;								
				FND_DisplayMenu();	
			}
			else if(Key == KEYDEFS_UP){ /* (4) 파라미터 설정값 변경�*/						
				
				if((menuParam == PARAM_TEST)||(menuParam == PARAM_BYPASS)||(menuParam == PARAM_BEEP))
				{
					mytoggle(&menuSetVal);	
				}					
				else if(menuParam == PARAM_ATTN){					

					if(++menuSetVal > RFID_MAX_ATTN_VALUE) menuSetVal = rfidMinAttnValue;						
				}
				else if(menuParam == PARAM_CLOCK){									
					
					if(++menuClock >= MENU_CLOCK_END) menuClock = MENU_CLOCK_YEAR;						
				}
				
				FND_DisplayMenu();								
			}
			else if(Key == KEYDEFS_DN){	/* (4) 파라미터 설정값 변경�*/							
				if((menuParam == PARAM_TEST)||(menuParam == PARAM_BYPASS)||(menuParam == PARAM_BEEP))
				{
					mytoggle(&menuSetVal);	
				}					
				else if(menuParam == PARAM_ATTN){					
						
					if(menuSetVal > rfidMinAttnValue) 	menuSetVal--;	
					else								menuSetVal = RFID_MAX_ATTN_VALUE;					
				}
				else if(menuParam == PARAM_CLOCK){					

					if(menuClock > MENU_CLOCK_YEAR) menuClock--;										
					else							menuClock = MENU_CLOCK_SEC;							
				}
				
				FND_DisplayMenu();												
			}
			else if(Key == KEYDEFS_ENT){/* MENU_VALUE */

				if(menuParam == PARAM_CLOCK){
					
					MenuMode = MENU_VALUE_CLOCK;					
					FND_DisplayMenu();
				}
				else{					

					if(menuParam == PARAM_TEST){

						TagConf.tagTest = menuSetVal;	FND_DisplaySet();

						RFID_ClearTagTestOld();

						if(TagConf.tagTest == 0){
							bAutoTagTestRun = FALSE;
						}						

						if(RFID_IsRun())Mk_BeginReadTag();
					}
					else if(menuParam == PARAM_ATTN){						  					

						if(TagConf.rfidAttn != menuSetVal){

							TagConf.rfidAttn = menuSetVal;
							
							E2P_SaveRfidAttn();	
						}

						FND_DisplaySet();	

						if(RFID_IsAlive()) RFID_SetAttn(_reset);							
						
					}
					else if(menuParam == PARAM_BYPASS){

						if(TagConf.tagBypass != menuSetVal){ 
							TagConf.tagBypass = menuSetVal; 
							E2P_SaveTagBypassMode();		

							// bypass 선택시 수신tag 초기화 
							if(TagConf.tagBypass)RFID_ClearTagGrfOld();
						}						

						FND_DisplaySet();
					}
					else if(menuParam == PARAM_BEEP){

						if(TagConf.rfidBeep != menuSetVal){
							TagConf.rfidBeep = menuSetVal;  
							E2P_SaveRfidBeep();						
						}	
						
						FND_DisplaySet();			

						if(RFID_IsRun())Mk_BeginReadTag();
					}
				}
			}	

			break;
		}	

		case MENU_VALUE_CLOCK:
		{			
			MENU_ValueClock(Key);
			break;
		}
	}
}
