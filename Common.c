#include "Common.h"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USART_TypeDef* COM_PORT[UARTn] = {USART1, USART2, USART3, UART4, UART5};

static __IO uint32_t TimingDelay;

#if 0
/* Private function prototypes -----------------------------------------------*/
#define __GNUC__
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
  {
	Clear_WatchDog();
  }

  return ch;
}
#endif

/**
  * @brief  Print a character on the HyperTerminal
  * @param  c: The character to be printed
  * @retval None
  */
void Serial_PutChar(uint8_t c)
{
  USART_SendData(USART1, c);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
  {
  	Clear_WatchDog();
  }
}

/**
  * @brief  Print a string on the HyperTerminal
  * @param  s: The string to be printed
  * @retval None
  */
void Serial_PutString(uint8_t *s)
{
  while (*s != '\0')
  {
    Serial_PutChar(*s);
    s++;
  }
}


/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*==================================================================
* Function Name  	: Clear_WatchDog
* Description    	: Generate the strobe pulse to reset the watchdog
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void Clear_WatchDog(void)
{
	static u8 bStrobe = 0x01;

	bStrobe ^= 0x01;
	(bStrobe)? GPIO_SetBits(GPIOB, nRST_ST): GPIO_ResetBits(GPIOB, nRST_ST);
}

/*==================================================================
* Function Name 	: Toggle_RunLED
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
u8 Toggle_RunLED(void)
{
	static u8 bRunLED = 0x00;/* ON */

	bRunLED ^= 0x01;
	(bRunLED)? GPIO_ResetBits(GPIOA, nLED_RUN): GPIO_SetBits(GPIOA, nLED_RUN);
	return(bRunLED);
}

/*==================================================================
* Function Name  	: GetTickCount
* Description    	: Get Current Timer's Value
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
u32 GetTickCount (u8 TimerNumber )
{
    return Timer[TimerNumber]; 
}


/*==================================================================
* Function Name  : RESET_TIMER
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void RESET_TIMER(u32 *pTimerCounter)
{
	*pTimerCounter = Timer[TIMER0];			
}


/*==================================================================
* Function Name  	: m_Elapsed
* Description    	: get elapsed time value
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
u32 m_Elapsed (u8 TimerNumber, u32 FormerTime)
{
	//Timer[n]: current time
    if (Timer[TimerNumber] >= FormerTime){
		return (Timer[TimerNumber] - FormerTime);
    }else{
    	return (~(FormerTime) + Timer[TimerNumber]);
    }	
}


/*==================================================================
* Function Name  	: getTimeElapsed
* Description    	: get elapsed time value
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
u32 getTimeElapsed(u32 currentTime, u32 previousTimeTag)
{
	u32 etime = 0;
	
    if (currentTime >= previousTimeTag){

		etime = currentTime - previousTimeTag;
		
    }else{
    
    	etime = ~(previousTimeTag) + currentTime;
    }	

	return(etime);
}

/*==================================================================
* Function Name  	: Delay_ms
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void Delay_ms (u32 remainingTime)
{
    u32 now = GetTickCount(TIMER0);

    while (m_Elapsed(TIMER0, now) < remainingTime){
		
    }
}


/*==================================================================
* Function Name 	: IS_TIMEOUT
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
bool IS_TIMEOUT(u32 *pTimerCounter, u32 MaxTimeLimit)
{
	if(m_Elapsed(TIMER0, *pTimerCounter) >=  MaxTimeLimit){

		*pTimerCounter = Timer[TIMER0];		
		
		return(TRUE);
	}	
	else{
		return(FALSE);
	}	
}


/*==================================================================
* Function Name 	: CHK_TOV
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
bool CHK_TOV(u32 TimerCounter, u32 MaxTimeLimit)
{
	if(m_Elapsed(TIMER0, TimerCounter) >=  MaxTimeLimit)return(TRUE);
	else 												return(FALSE);
}


/*==================================================================
* Function Name  	: SET_UART_TX_IT_CONFIG()
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void SET_UART_TX_IT_CONFIG(u8 COMx,u8 state)
{	
	(state == ON) ? USART_ITConfig(COM_PORT[COMx], USART_IT_TXE, ENABLE): \
       		  	    USART_ITConfig(COM_PORT[COMx], USART_IT_TXE, DISABLE);	
}

/*==================================================================
* Function Name  	: SET_UART_RX_IT_CONFIG()
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void SET_UART_RX_IT_CONFIG(u8 COMx, u8 state)
{	
	(state == ON) ? USART_ITConfig(COM_PORT[COMx], USART_IT_RXNE, ENABLE): \
					USART_ITConfig(COM_PORT[COMx], USART_IT_RXNE, DISABLE);	
}

/*==================================================================
* Function Name  	: UART_TxBytes
* Description    	:  Send data to UART by using the Tx Interrupt
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void UART_TxBytes(u8 COMx, u8 *str, u32 cnt)
{
	UART_FIFO *p;	
	u32 i, tmp_wr;	
	
	p = &sio_tx[COMx];	

	tmp_wr = p->write;	
	for(i = 0; i < cnt; i++){	
	
		p->buffer[tmp_wr++] = str[i];
		if(tmp_wr >= UARTDEFS_MAX_FIFO_LEN) tmp_wr = 0;
	}	
	
	p->write = tmp_wr;	
	
	
	SET_UART_TX_IT_CONFIG(COMx, ON);	
	
//	LED_SetTxCOM(COMx, ON);	
}

/*==================================================================
* Function Name  	: UART_CheckSend
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void UART_CheckSend(u8 COMx)
{
	u32 tmp_wr;	
	UART_FIFO *p;       
		
	if(USART_GetFlagStatus(COM_PORT[COMx], USART_FLAG_TXE) != RESET){
		p = &sio_tx[COMx];			
        tmp_wr = p->write;
		
		if(p->read != tmp_wr){								
			USART_SendData(COM_PORT[COMx], p->buffer[p->read++]);				
			if(p->read >= UARTDEFS_MAX_FIFO_LEN) p->read = 0;		
		}
	}
}


/*==================================================================
* Function Name  	: SET_OUT_EN
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void SET_OUT_EN(u8 state)
{
	/** 
		state?		(1) ON  : 0	(2) OFF : 1
	*/

	if(state == ON)	GPIO_ResetBits(GPIOC, nOE_WR);
	else			GPIO_SetBits(GPIOC, nOE_WR);  
}


/*==================================================================
* Function Name  	: Disable_InOutDevCsPin
* Description    	: Disable IN/OUT CS before changing values on the data port of IO
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void Disable_InOutDevCsPin(void)
{		
	//참고 :: GPIO_Write(GPIOC, PortVal_16bit); 16bit 값을 Direct Writing 할수 있음. 

	GPIO_WriteBit(GPIOB, nCS_RD_DIPSW | nCS_RD_IDSW | 
						 nCS_RD_PUSHSW, Bit_SET);	/* 1 */		

	GPIO_SetBits(GPIOC, nLED_LAT);	/* 1 */	
}


/*==================================================================
* Function Name  	: inout_sel
* Description    	: 
* Input          		: 
* Output         	: 
* Return         	: 
====================================================================*/
void inout_sel(DeviceSelType Device)
{		
	switch(Device){

		case  SEL_RD_DIPSW:		GPIO_ResetBits(GPIOB, nCS_RD_DIPSW);	break;		
		
		case  SEL_RD_IDSW:		GPIO_ResetBits(GPIOB, nCS_RD_IDSW);		break;
		
		case  SEL_RD_SDI:	GPIO_ResetBits(GPIOB, nCS_RD_PUSHSW);	break;

	
		case  SEL_WR_LED_RF_CAN:	/* 000 */
								GPIO_ResetBits(GPIOA, LED_A2 | LED_A1 | LED_A0);	
								break;
		
		case  SEL_WR_LED_TAG_TPS:	/* 001 */								
								GPIO_ResetBits(GPIOA, LED_A2 | LED_A1);									
								GPIO_SetBits(GPIOA, LED_A0);
								break;
		
		case  SEL_WR_FND1:		/* 010 */																
								GPIO_ResetBits(GPIOA, LED_A2 | LED_A0);	
								GPIO_SetBits(GPIOA, LED_A1);
								break;

		
		case  SEL_WR_FND2:		/* 011 */								
								GPIO_ResetBits(GPIOA, LED_A2);
								GPIO_SetBits(GPIOA, LED_A1 | LED_A0);								
								break;

		case  SEL_WR_FND_SEL:	/* 100 */		
								GPIO_ResetBits(GPIOA, LED_A1 | LED_A0);								
								GPIO_SetBits(GPIOA, LED_A2);
								break;		
								
		case NOT_SEL: 			break;
		
    	default:	      		break;
	}	

	// Enable 74HC238 
	if((Device >= SEL_WR_LED_RF_CAN)&&(Device <= SEL_WR_FND_SEL)){
		GPIO_ResetBits(GPIOC, nLED_LAT);	/* 0 */
	}		
}

/*==================================================================
* Function Name  : Read_Data
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
u8 Read_Data(DeviceSelType Device)
{
	u8 data;

	GPIO_InitTypeDef GPIO_InitStructure;	
		
	Disable_InOutDevCsPin();
	
  	GPIO_InitStructure.GPIO_Pin  = nDIN0|nDIN1|nDIN2|nDIN3|nDIN4|nDIN5|nDIN6|nDIN7;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);		
	
	inout_sel(Device);
	data = (u8)GPIO_ReadInputData(GPIOC) & 0x00FF;

	Disable_InOutDevCsPin();	
	
	return (data);
}

/*==================================================================
* Function Name  : Write_Data
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void Write_Data(DeviceSelType Device, u8 data)
{
	u16 WordData;
		
	GPIO_InitTypeDef GPIO_InitStructure;

	Disable_InOutDevCsPin();
	
	/* Configure Output pins as output open drain */
  	GPIO_InitStructure.GPIO_Pin = nDIN0|nDIN1|nDIN2|nDIN3|nDIN4|nDIN5|nDIN6|nDIN7;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	/* Write Data */
	WordData  = (u16)(data);
	WordData &= 0x00ff;
	GPIO_SetBits(GPIOC, WordData);
	
	WordData ^= 0x00ff;
	WordData &= 0x00ff;
	GPIO_ResetBits(GPIOC, WordData);
	
	inout_sel(Device);	/* Enable  */		  		

	Disable_InOutDevCsPin();
}

/*
	(1) 74HC245, 74HC541   nOE == LOW  (Active low)
	(2) 74HC574   CP   == Positive Edge Trigger	
*/

/*==================================================================
* Function Name 	: IsChanged_LogicInput
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/	
bool IsChanged_LogicInput(DeviceSelType Device)
{
	u32 i, FrozenCntrsLimit[8];
	u8 newByteInput, lastByteInput;
	u8 newBitInput, lastBitInput;
	bool bIsChanged = FALSE;	
	
		
	PARAM_LDI *p = &LogicInput[Device];	
		
	lastByteInput = p->BinInputOld.byte;		

	// 검지시 1로 설정하기 위해 입력 인버팅한다. 
	newByteInput   = INVBYTE(Read_Data(Device)); 	

	switch(Device){
		
		case SEL_RD_DIPSW:
		{		
			for(i = 0; i <= 7; i++)	FrozenCntrsLimit[i] = 	DEFS_DIPSW_FROZEN_TIME;				

			newByteInput 	 &= DEFS_LOGIC_IN_MASK_DIPSW;
			break;
		}	
		
		case SEL_RD_IDSW:
		{		
			newByteInput = INVBYTE(newByteInput);			
		
			for(i = 0; i <= 7; i++) FrozenCntrsLimit[i] =	DEFS_IDSW_FROZEN_TIME;		
			break;
		}	

		case SEL_RD_SDI:
		{		
			for(i = 0; i <= 3; i++)	FrozenCntrsLimit[i] = 	DEFS_FRONT_SW_FROZEN_TIME;
			for(i = 4; i <= 7; i++)	FrozenCntrsLimit[i] = 	DEFS_TPU_SENSOR_FROZEN_TIME;				

			// Button 값은 처리하지 않음, SDI만 처리함 
			// Bttton 값은 Front.c에서 직접 read 함 
			newByteInput &= 0xF0;
			
			break;
		}	
		

		default: 
		{
			for(i = 0; i <= 7; i++)	FrozenCntrsLimit[i] = 	DEFS_HW_IN_FROZEN_TIME;// 100ms		
			break;
		}	
			
	}

	for(i = 0; i<= 7; i++){
		lastBitInput = tbit(lastByteInput, i);
	    newBitInput  = tbit(newByteInput, i);
	    if(lastBitInput != newBitInput){

			p->FrozenCntrs[i]++;						
				
			if(p->FrozenCntrs[i] >= FrozenCntrsLimit[i]){
				(newBitInput)? 	sbit(p->BinInputOld.byte, i) : \
				    			cbit(p->BinInputOld.byte, i); 
				p->FrozenCntrs[i] = 0;	    
				bIsChanged = TRUE;
			}
	    }else p->FrozenCntrs[i] = 0; 
	}
	
	return(bIsChanged);
}


/*==================================================================
* Function Name 	: IsChanged_Sdi_Index()
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
bool IsChanged_Sdi_Index(u8 pointNum)
{
	u32 FrozenCntrsLimit;	
	u8 newBitInput, lastBitInput;
	bool bIsChanged = FALSE;
	
	PARAM_SDI *p = &m_SDIO.Sdi;		

	lastBitInput = p->BinInputOld[pointNum];

	switch(pointNum){			

		case _SDI_SDCARD_INSERT: 
				newBitInput = INVBIT(GPIO_ReadInputDataBit(GPIOB, nINP_SD_INSERT));	
				FrozenCntrsLimit = DEFS_HW_IN_FROZEN_TIME;
				break;		

			
		default: break;		
	}		
	    
    if(lastBitInput != newBitInput){
		p->FrozenCntrs[pointNum]++;			
		if(p->FrozenCntrs[pointNum] > FrozenCntrsLimit){
           	p->BinInputOld[pointNum] = newBitInput;
			p->FrozenCntrs[pointNum] = 0;	    
			bIsChanged = TRUE;
		}			
    }else p->FrozenCntrs[pointNum] = 0; 

	return(bIsChanged);
}


/*==================================================================
* Function Name 	:  WRITE_XOUT
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void WRITE_XOUT(DeviceSelType Device)
{
	switch(Device){		

	case SEL_WR_LED_RF_CAN:
		if(mynewcmp(&XoutOld.LedRF_CAN.byte,&Xout.LedRF_CAN.byte))	
			Write_Data(Device, XoutOld.LedRF_CAN.byte); break; 

	case SEL_WR_LED_TAG_TPS:
		if(mynewcmp(&XoutOld.LedTAG_TPS.byte,&Xout.LedTAG_TPS.byte))	
			Write_Data(Device, XoutOld.LedTAG_TPS.byte); break;

	case SEL_WR_FND_SEL:
		if(mynewcmp(&XoutOld.FndSel.byte,&Xout.FndSel.byte))	
			Write_Data(Device, XoutOld.FndSel.byte); break; 
					
	default: break; 
	
	}	
}
