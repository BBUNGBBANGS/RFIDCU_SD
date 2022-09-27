/*****************************************************************************
* (C) COPYRIGHT 2022 GREAT RAIL
*
******************************************************************************
* File Name   : main.c
* Author      : GREAT RAIL
* Version     : V1.1.3 
* Date        : 04 AUG, 2022
* Description : 
******************************************************************************
REVISION HISTORY   
	
------------------------------------------------------------------------------
	07-JUNE-2022 Great Rail #. Released Ver.1.0.0
------------------------------------------------------------------------------ 
	-. Initial file creation    	
		
******************************************************************************/
#include "Common.h"
#include "SD_Card.h"

/*****************************************************************************
*                                                                          
* 							LITERAL CONSTANTS(DEFINES)
*                    
******************************************************************************/

/*****************************************************************************
*                                                                          
* 								PUBLIC VARIABLES
*                    
******************************************************************************/

u32 		Timer[TIMn]; 	/* Time Base Counter */
UART_FIFO 	sio_tx[UARTn];	/* UART TX Buffer */
UART_FIFO 	sio_rx[UARTn];	/* UART RX Buffer */

PARAM_LDI 		LogicInput[LSI_INPUT_QTY]; 
PARAM_SDIO		m_SDIO;

uFRONT_BUTTONS	FrontBtn, FrontBtnOld;

DIP_SWITCH		DipSwitch;
PARAM_SWITCH 	m_Config;

/* Outputs */	
sGR_OUT			Xout, XoutOld;	

sTIMER_CMD 		TimerCount;

u8 MyPCBNumber;
u8 MyDeviceID;
u8 IdSwitchOld;

OP_MODE OperationMode;
u8 		GRFTypeCode;
u8 		TagMode;

bool bE2PROM_InitOK;

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

u32 BootCount = 0;

PgmEnvSTR  m_PgmEnv; 	/* CAN Tx Message */

bool 	bRFIDBootCheckTime;

bool 	bFirstBootFlag;
u32		FirstBootTimer;

u8 		rfidMinAttnValue;

//u8 test_string[] = {"COM RFU1"};

/*****************************************************************************
*                                                                          
* 								FUNCTION DEFINITIONS(LOCAL)
*                    
******************************************************************************/
static void RCC_Configuration(void);
static void NVIC_Configuration(void);
static void GPIO_Configuration(void);
static void TIM2_Configuration(void);
static void USART_Configuration(void);
static void USART_IT_Configuration(void);
static void CAN_Configuration(void);
static void CAN_FilterConfiguration(void);
//static void Disable_All_Interrupt(void);
static void InitializePorts(void);
static void InitializeVal(void);


#if 0
/*==================================================================
* Function Name  	: Disable_All_Interrupt
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
static void Disable_All_Interrupt(void)
{		
	/* Disable USARTx Receive interrupts */
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
	USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
	USART_ITConfig(UART5, USART_IT_RXNE, DISABLE);
	  
	/* Disable USARTx Transmit interrupts */
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
	USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
	USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
	USART_ITConfig(UART5, USART_IT_TXE, DISABLE);
	
	/* Disable the USARTx */
//	USART_Cmd(USART1, DISABLE);
//	USART_Cmd(USART2, DISABLE);
//	USART_Cmd(USART3, DISABLE);
//	USART_Cmd(UART4, DISABLE);
//	USART_Cmd(UART5, DISABLE);	

	/* Disable CANs */
	CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);
	CAN_ITConfig(CAN2, CAN_IT_FMP0, DISABLE);  

	/* TIM IT enable */
  	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE); 
	
	/* TIM enable counter */
  	TIM_Cmd(TIM2, DISABLE);  	
}
#endif

/*==================================================================
* Function Name  	: RCC_Configuration
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
static void RCC_Configuration(void)
{	
	/*
		Clock Configuration

		HSE = 16MHz
		SYSCLK = 72MHz
		HCLK = SYSCLK
		PCLK2 = SYSCLK
		PCLK1 = SYSCLK /2 = 36MHz (max)
		TIMxCLK = SYSCLK
	*/

	#if 0
	{	
		/* max.14Mhz :: ADCCLK = PCLK2/x = 72MHz/6 = 12MHz */
	  	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 	  	
		
		/* Enable DMA1 clock */
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		
		/* Enable ADC1 clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	}
	#endif
		
	/* Enable GPIO clock */
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
  						   RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | 
  						   RCC_APB2Periph_AFIO, ENABLE);

	/* Enable USART1 Clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 

	/* Enable USART2, 3, and UART 4, 5  Clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 |
						    RCC_APB1Periph_UART4 | RCC_APB1Periph_UART5, ENABLE);

	/* Enable CAN1 and 2  Clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2, ENABLE);
	
  	/* Enable TIM2  Clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}


/*==================================================================
* Function Name  	: NVIC_Configuration
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
static void NVIC_Configuration(void)
{
  	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/* At this stage the Vector Table setting is already configured at System_stm32f10x.c - SystemInit( ) */	
	/* Set the Vector Table base location at 0x3000 :: We use IAP Code at 0x0000 */ 

	//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x3000);	

	/*===========================================================================================================================
	    NVIC_PriorityGroup   | NVIC_IRQChannelPreemptionPriority | NVIC_IRQChannelSubPriority  | Description
	 ============================================================================================================================
	   NVIC_PriorityGroup_0  |                0                  |            0-15             |   0 bits for pre-emption priority
	                         |                                   |                             |   4 bits for subpriority
	  ----------------------------------------------------------------------------------------------------------------------------
	   NVIC_PriorityGroup_1  |                0-1                |            0-7              |   1 bits for pre-emption priority
	                         |                                   |                             |   3 bits for subpriority
	  ----------------------------------------------------------------------------------------------------------------------------    
	   NVIC_PriorityGroup_2  |                0-3                |            0-3              |   2 bits for pre-emption priority
	                         |                                   |                             |   2 bits for subpriority
	  ----------------------------------------------------------------------------------------------------------------------------    
	   NVIC_PriorityGroup_3  |                0-7                |            0-1              |   3 bits for pre-emption priority
	                         |                                   |                             |   1 bits for subpriority
	  ----------------------------------------------------------------------------------------------------------------------------    
	   NVIC_PriorityGroup_4  |                0-15               |            0                |   4 bits for pre-emption priority
	                         |                                   |                             |   0 bits for subpriority                       
	 ============================================================================================================================

	 #NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority
		: 가 다를경우, 낮은 Preemption Priority 인터럽트 실행중 
		 높은 Preemption Priority 인터럽트 발생하면,
		 낮은 Preemption Priority 인터럽트는 잠시 중단하고,
		 높은 Preemption Priority 인터럽트를 실행한후, 
		 다시 중단했던 낮은 Preemptio Priority 인터럽트를 실행한다.
		 * 높은 Preemption Priority 인터럽트가 연속발생(유지) 조건이라면,
		   낮은 Preemption Priority 인터럽트는 높은 Preemption Priority 인터럽트 실행중에는
		   실행되지 않는다. 

	#NVIC_IRQChannelSubPriority
		: 가 다를경우, 낮은 SubPriority 인터럽트 실행중
		  높은 SubPriority 인터럽트 발생하면,
		  낮은 SubPriority 인터럽트 실행을 완료한후,
		  높은 SubPriority 인터럽트를 실행한다. 
	*/

	// 현재실행중인 인터럽트보다 Pre-emtion priority가 높은 인터럽트를 처리한다. 
	
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	// NVIC_IRQChannelPreemptionPriority (0)
	{
		#ifdef __PVD__
		/* Enable the PVD Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		#endif  

		/* Enable the Timer2 Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
	  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	  	NVIC_Init(&NVIC_InitStructure);  	

		NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;  
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
	
	// NVIC_IRQChannelPreemptionPriority (1)
	{	
		// SubPriority 0 to 3 (Internal Performance) 
		{
			/* DMA */				
			/* SPI/I2C ... */
		}
		
		
		{
			/* RFID */	
			NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
		  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
		  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
		  	NVIC_Init(&NVIC_InitStructure);  		
		
			/* GRF */
			NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
		  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
		  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
		  	NVIC_Init(&NVIC_InitStructure);
		}	

		{
			/* CAN Main */
			NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
		  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
		  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
		  	NVIC_Init(&NVIC_InitStructure);

			/* CAN Backup */
			NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
		  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
		  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
		  	NVIC_Init(&NVIC_InitStructure);	

		}

		{
			/* RFU1 */
			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
		  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
		  	NVIC_Init(&NVIC_InitStructure);  	

			/* RFU2 */
			NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
		  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
			NVIC_Init(&NVIC_InitStructure);  	
		}		

		/* DEBUG */
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
	  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
	  	NVIC_Init(&NVIC_InitStructure); 
		
		// End of NVIC_IRQChannelSubPriority (7)
	}
}


/*==================================================================
* Function Name  	: PreInitializeOutPorts
* Description    	: 
* Input          		:	 
* Output         	: 
* Return         	: 
====================================================================*/
static void PreInitializeOutPorts(void)
{	
	/* 	포트 디렉션을 출력 포트로 설정 전에 
		Output DataRegister 값 설정 (Default 출력이 1 이어야 할 때 ) 
		reset 후 port는 input floating 설정 상태 임 	
	*/
		
	Disable_InOutDevCsPin();

	GPIO_SetBits(GPIOA, SPI_nCS | SPI_SCK | SPI_MOSI | nLED_RUN);		

	// MODEM OFF
	GPIO_SetBits(GPIOA, nRFU1_ON | nRFU2_ON);		

	// Reset Strobe == 1
	GPIO_SetBits(GPIOB, nRST_ST);		

	if(RFIDDEFS_POWER_OFF_AFTER_RESET()) RFID_SET_POWER(OFF);
}

/*==================================================================
* Function Name  	: GPIO_Configuration
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
static void GPIO_Configuration(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;

	u8 PBA_SEL0;

	/**
		STM32F의 GPIO Reset Value

		(1) Input Mode(Input Floating)
		(2) BSRR, BRR 은 0, 즉 No action
		(3) BSRR, BRR -> Output Data Register : Output Data Register 초기 Value 0

		모든 I/O는 입력 모드 , 플로팅 상태이며 출력 모드로 변경시 BSRR, BRR or ODR 값을 변경하여 
		출력 모드 로 변경전 초기 출력 값을 설정한다. (참고 : BSRR, BRR 변경 시 출력 레지스터 ODR 값이 변경된다. 

		RESET후 H/W적으로 출력이 1이어야 하는 포트는, 출력 포트로 포트 디파인 전에 출력에 1을 Writing 한후 
		변경해야 한다. 
	*/	

	SET_OUT_EN(OFF);/* Disable */	

	// Output - PP (3V3D Pull UP)
	GPIO_InitStructure.GPIO_Pin 	= nOE_WR;    	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* 	포트 디렉션을 출력 포트로 설정 전에 
		Output DataRegister 값 설정 (Default 출력이 1 이어야 할 때 )*/
	{
		PreInitializeOutPorts();
	}	


	/**	
		PORT A 
	**/		
	GPIO_InitStructure.GPIO_Pin 	= LED_A0 | LED_A1 | LED_A2 | nLED_LAT;  
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);		

	// Output - PP (3V3D Pull UP)
	GPIO_InitStructure.GPIO_Pin 	= SPI_nCS | SPI_SCK | SPI_MOSI;		
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);		

	// Input - IPU (3V3D Pull UP)
	GPIO_InitStructure.GPIO_Pin 	= SPI_MISO;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	/* Configure Output pins as output open drain */
  	GPIO_InitStructure.GPIO_Pin 	= nRFU1_ON | nRFU2_ON;
  	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_OD;
  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_10MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure Output pins as output push pull */
  	GPIO_InitStructure.GPIO_Pin 	= nLED_RUN;
  	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);


	// PA15(LED_RUN), PB3(PCB_VER), PB4(DETECT_SD) GPIO
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); 
	

	/**	
		PORT B 
	**/		
	
	/* Configure Output pins as output push pull */
  	GPIO_InitStructure.GPIO_Pin 	= nRST_ST;
  	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= nSEL_SD_MEM | nCS_RD_IDSW | nCS_RD_DIPSW | nCS_RD_PUSHSW;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	
	// Input - IPU (No Pull UP)
  	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPU;
  	GPIO_InitStructure.GPIO_Pin 	= nINP_PBA_SEL0 | nINP_SD_INSERT;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	{
		PBA_SEL0 = GPIO_ReadInputDataBit(GPIOB, nINP_PBA_SEL0);

		// PB3	
		// OPEN(1)	PCB P/N A0
		// GND (0)	PCB P/N A2
		if(PBA_SEL0)	MyPCBNumber = PCB_A0;
		else			MyPCBNumber = PCB_A1;		
  	}	
	
	/**	
		PORT C 
	**/	

	GPIO_InitStructure.GPIO_Pin 	= nDIN0|nDIN1|nDIN2|nDIN3|nDIN4|nDIN5|nDIN6|nDIN7;	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin 	= nLED_LAT;  
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);			

  	GPIO_InitStructure.GPIO_Pin 	= nRFID_ON;
  	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_10MHz;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	 	
  	/* Configure CANs RX pins , CAN1(AF Remmap), CAN2(AF Default), */
	GPIO_InitStructure.GPIO_Pin = CAN1_RX | CAN2_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	
	/* Configure CANs TX pins */
	GPIO_InitStructure.GPIO_Pin = CAN1_TX | CAN2_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

  
	/* Remap CAN1 GPIOs */
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);	


	/* Configure USARTs Rx as input floating */
  	GPIO_InitStructure.GPIO_Pin = USART1_RX;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = USART2_RX;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART3_RX;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = UART4_RX;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = UART5_RX;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/* Configure USARTs Tx as alternate function push-pull */
  	GPIO_InitStructure.GPIO_Pin = USART1_TX;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART2_TX;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART3_TX;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = UART4_TX;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = UART5_TX;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
		
	//SET_I2C_PORT(I2C_SCL, 1);
	//SET_I2C_PORT(I2C_SDA, 1);	

	GPIO_InitStructure.GPIO_Pin 	= I2C_SCL | I2C_SDA;
  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;
  	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}


/*==================================================================
* Function Name  : TIM2_Configuration
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
static void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	/*	Time base configuration -----------------------------------------------------------*/
	
	/* 	PCLK1 / (Prescaler + 1)=72000000 /( 71+1) = 1,000,000;

		즉, 1초에 1,000,000번 카운터 증가됨,

		1sec per 1000,000 count
	 	1ms per 1000 count
	 	1us per  1 count  
	 	
		ex1) 1ms 마다 인터럽트 설정 하려면, AutoReload Register(TIM_Period)에
		        1000,000/1000 = 1000을 설정함. 
		        # 1ms = 1Khz ; 1000,000Hz / 1000Hz = 1000.

		ex2) 10ms, 1초의 백분의 1, 즉 1000,000 / 100	= 10,000을 설정함.

		ex3) 100ms, 1초의 십분의 1, 즉, 즉 1000,000 / 10	= 100,000을 설정함. (X)
			 
		         CNT 레지스터 크기가 16bit 임으로 100,000 설정 안됨.
		         따라서 PSC값을 71이 아닌, 719로 하면,

		      72,000,000 / (719+1) = 100,000;  1초에 10만번 카운팅
		      100ms 설정 하려면 , 100,000 / 10 = 10,000
		      TIM_Period(AutoReladRegister) 에 10,000을 설정한다.	 	


	   	# ARR	
	   	
			counter 가 0 부터 999까지 즉, 999번 증가후, 0가 될때 업데이트 이벤트 발생함.
  	     		즉 카운트 수는 총 1000개 (999 + 1(999 --> 0))
  	     		따라서 1000이 아닌, 1적은 숫자 999로 ARR(TIM_Period)의 값을 설정한다. 

  	     		ARR(Prescaler) = (TIMxCLK / TIMx counter clock) - 1  	  
  	     					 = (1Mhz / 1Khz) - 1
  	     					 = (1,000,000 / 1,000) - 1
       
	*/

	TIM_TimeBaseStructure.TIM_Prescaler = 71;      
	
	//TIM_TimeBaseStructure.TIM_Period = 1000;          
  	TIM_TimeBaseStructure.TIM_Period = 999;            	

	// Digital Filter 사용시 sampling clock x1, x2, x4 채배 설정 (TIM_IT_Update 에서는 상관없음) 
  	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    
	
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
  
  	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  	/* Prescaler configuration */
  	TIM_PrescalerConfig(TIM2, 71, TIM_PSCReloadMode_Immediate);

  	/* Eanble TIM2 counter */
  	TIM_Cmd(TIM2, ENABLE);

	/* Clears TIMx's Pending flags */
	// [NOTE] 동일한 동작 TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
	
  	/* Enable TIM IT */
  	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 
}   



/*==================================================================
* Function Name  	: USART_Configuration
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
static void USART_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	/* USARTs configuration ------------------------------------------------------*/
  
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  

  	/* Configure USART1 */
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  	USART_InitStructure.USART_BaudRate = 115200;
  	USART_Init(USART1, &USART_InitStructure);
  
  	/* Configure USART2, 3, 4 */
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  	USART_InitStructure.USART_BaudRate = 115200;
  	USART_Init(USART2, &USART_InitStructure);
  	USART_Init(USART3, &USART_InitStructure);
  	USART_Init(UART4, &USART_InitStructure);  
  
  	/* Configure USART5 */
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  	USART_InitStructure.USART_BaudRate = 115200;
  	USART_Init(UART5, &USART_InitStructure);
  
  	
}

/*==================================================================
* Function Name  	: USART_Configuration
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
static void USART_IT_Configuration(void)
{
	/* Enable USARTx Receive interrupts */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
 	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
  
	/* Enable USARTx Transmit interrupts */
  	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
  	//USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	//USART_ITConfig(UART4, USART_IT_TXE, ENABLE);
  	//USART_ITConfig(UART5, USART_IT_TXE, ENABLE);

	/* Enable the USARTx */
 	USART_Cmd(USART1, ENABLE);
	USART_Cmd(USART2, ENABLE);
  	USART_Cmd(USART3, ENABLE);
	USART_Cmd(UART4, ENABLE);
  	USART_Cmd(UART5, ENABLE);  
}



/*==================================================================
* Function Name  	: CAN_Configuration
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
static void CAN_Configuration(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	
  	/* CAN1 and CAN2 register init */
  	CAN_DeInit(CAN1);
  	CAN_DeInit(CAN2);

  	/* Struct init*/
  	CAN_StructInit(&CAN_InitStructure);

  	/* CAN1 and CAN2  cell init */
  	CAN_InitStructure.CAN_TTCM = DISABLE;
	
  	//CAN_InitStructure.CAN_ABOM = DISABLE;
  	CAN_InitStructure.CAN_ABOM = ENABLE;	/* V1.1A */
	
  	CAN_InitStructure.CAN_AWUM = DISABLE;
	
	/* Enable the no-automatic retransmission mode */
   	CAN_InitStructure.CAN_NART = ENABLE;

  	CAN_InitStructure.CAN_RFLM = DISABLE;

	//CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_TXFP = ENABLE;
	
  	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;  
  	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
  	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
 
	#if CAN_BAUDRATE == 1000 /* 1MBps */
  		CAN_InitStructure.CAN_Prescaler =6;
	#elif CAN_BAUDRATE == 500 /* 500KBps */
  		CAN_InitStructure.CAN_Prescaler =12;
	#elif CAN_BAUDRATE == 250 /* 250KBps */
  		CAN_InitStructure.CAN_Prescaler =24;
	#elif CAN_BAUDRATE == 125 /* 125KBps */
  		CAN_InitStructure.CAN_Prescaler =48;
	#elif  CAN_BAUDRATE == 100 /* 100KBps */
  		CAN_InitStructure.CAN_Prescaler =60;
	#elif  CAN_BAUDRATE == 50 /* 50KBps */
  		CAN_InitStructure.CAN_Prescaler =120;
	#elif  CAN_BAUDRATE == 20 /* 20KBps */
  		CAN_InitStructure.CAN_Prescaler =300;
	#elif  CAN_BAUDRATE == 10 /* 10KBps */
  		CAN_InitStructure.CAN_Prescaler =600;
	#else
   
	#endif  
  
  	/*Initialize the CANs */
  	CAN_Init(CAN1, &CAN_InitStructure);
  	CAN_Init(CAN2, &CAN_InitStructure);
}


/*==================================================================
* Function Name  	: CAN_FilterConfiguration()
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
static void CAN_FilterConfiguration(void)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	
	u32 FilterId[2];
	u32 FilterMask[2];		

	FilterId[0] 	= (u32)(CAN_GROUP_MASK_GRF_R);
	FilterMask[0]  	= (u32)(CAN_GROUP_MASK_GRF_R);	
	FilterId[1] 	= (u32)(CAN_GROUP_MASK_GRF_R);
	FilterMask[1]  	= (u32)(CAN_GROUP_MASK_GRF_R);	

	MyDeviceID = CAN_ID_RFIDCU; 	

	FilterId[0] <<= 3;
	FilterId[1] <<= 3;
	
	FilterMask[0]  <<= 3;
	FilterMask[0]   &= 0xFFFFFFF8; /* Masking LSB 2, 1, 0 :: IDE, RTR, Reserved */

	FilterMask[1]  <<= 3;
	FilterMask[1]   &= 0xFFFFFFF8; /* Masking LSB 2, 1, 0 :: IDE, RTR, Reserved */

	/* Filter Number range ------------------------------------------------
		CAN1 : 00 To 13
		CAN2 : 14 To 27
	------------------------------------------------------------------*/	
 	/* CAN1 filter 0 init */
  	CAN_FilterInitStructure.CAN_FilterNumber 		= 0;
  	CAN_FilterInitStructure.CAN_FilterMode 			= CAN_FilterMode_IdMask;
  	CAN_FilterInitStructure.CAN_FilterScale 		= CAN_FilterScale_32bit;
  	CAN_FilterInitStructure.CAN_FilterIdHigh 		= (u16)(FilterId[0] >> 16);
  	CAN_FilterInitStructure.CAN_FilterIdLow 		= (u16)FilterId[0];
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh 	= (u16)(FilterMask[0] >> 16);
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow 	= (u16)FilterMask[0];
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment= 0;
  	CAN_FilterInitStructure.CAN_FilterActivation 	= ENABLE;
  	CAN_FilterInit(&CAN_FilterInitStructure);	
	
 	/* CAN2 filter 14 init */
	CAN_FilterInitStructure.CAN_FilterNumber 	= 14;
	CAN_FilterInitStructure.CAN_FilterIdHigh 	= (u16)(FilterId[1] >> 16);
  	CAN_FilterInitStructure.CAN_FilterIdLow 	= (u16)FilterId[1];	
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= (u16)(FilterMask[1] >> 16);
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow = (u16)FilterMask[1];
	CAN_FilterInit(&CAN_FilterInitStructure);	
}


/*==================================================================
* Function Name  	: InitializePorts
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
static void InitializePorts(void)
{	
	SET_OUT_EN(OFF);/* Disable outputs */	

	PreInitializeOutPorts();

	/* Xout 초기화 */
	//InitializeXout();	

	LED_Init();		

	FND_Init();
}

/*==================================================================
* Function Name  	: InitializeVariables
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
static void InitializeVal(void)
{
	u32 i;	

	/* Time Base Counter */
	for(i = 0; i < TIMn; i++) Timer[i] = 0x0000; 	
	
	/* uart */
	for(i = 0; i< UARTn; i++){
		memset((u8 *)&sio_rx[i], 0, sizeof(UART_FIFO));
		memset((u8 *)&sio_tx[i], 0, sizeof(UART_FIFO));
	}	
	
	for(i=0; i < LSI_INPUT_QTY; i++) memset((u8 *)&LogicInput[i], 0, sizeof(PARAM_LDI));		

	memset((u8 *)&TimerCount, 0, sizeof(sTIMER_CMD));

	memset((u8 *)&DipSwitch, 0, sizeof(DIP_SWITCH));

	memset((u8 *)&m_Config, 0, sizeof(PARAM_SWITCH));		

	memset((u8 *)&m_PgmEnv, 0, sizeof(PgmEnvSTR));

	XoutOld.LedRF_CAN.byte 	= Xout.LedRF_CAN.byte 	= __led_all_off;
	XoutOld.LedTAG_TPS.byte = Xout.LedTAG_TPS.byte 	= __led_all_off;
	XoutOld.FndSel.byte	    = Xout.FndSel.byte   =  __fnd_all_off;
		
	MyPCBNumber = 0;
	MyDeviceID	= 0;
	IdSwitchOld	= 0;
	
	OperationMode = OP_MODE_NORMAL;
	bE2PROM_InitOK = FALSE;

	BootCount = 0;
	bRFIDBootCheckTime = FALSE;

	GRFTypeCode = GRF_TYPE_ST;
	
	LOG_InitVal();

	App_InitVal();

	CAN_InitVal();

	MRTC_InitVal();

	RFU_InitVal();

	GRF_InitVal();

	RFID_InitVal();

	TPU_InitVal();
	
}

/*==================================================================
* Function Name 	:  main
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
int main(void)
{		
	vu32 i;	
	u32 timeout_boot; 
	u8 tmp;
	
	
  	/*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
    */ 

  	/* RCC configuration */
	RCC_Configuration();
	  
	/* NVIC configuration */
	NVIC_Configuration();
	
#ifdef __PVD__	
		/* Configure the PVD Level to 2.7V */
		PWR_PVDLevelConfig(PWR_PVDLevel_2V7);
		
		/* Enable the PVD Output */
		PWR_PVDCmd(ENABLE); 
#endif

	/* GPIO configuration */
	GPIO_Configuration();		

	/* Initialize variables */
	InitializeVal();
	
	/* Initialize Ports */
	InitializePorts(); 

	/* Enables outputs */
	SET_OUT_EN(ON);/* Enable */		
	
	/* Timer2 configuration :: 1ms timer */
	TIM2_Configuration();
	
	/* USARTs configuration */
	USART_Configuration();	
  
  	/* Reset WatchDog */
	Clear_WatchDog();	

	/* CANs Configuration */
	CAN_Configuration(); 	
		
	/* CANs ID Filter Configuration */
	CAN_FilterConfiguration();	
	
	/* Reset WatchDog */
	Clear_WatchDog();			

	/* Enable USART Interrupt */
	USART_IT_Configuration();	

	Toggle_RunLED();
		
	/* Enable CANs Rx Interrrupt */
  	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE); 
	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);

	/* Send 'Restart Message' via CANs  */		
	m_PgmEnv.Alarm.Restart = 1;	
	
	if(IsUseCANInterlock()){
		CAN_Send_RFIDCU_Status();
	}	
	
	{	
		MRTC_Init();		

		SPI_FLASH_Init();	
		
		LOG_GetRecPos();		
	} 

	tmp = KEY_BootRead();
	if(tmp != KEYDEFS_NULL){
		if(tmp == KEYDEFS_UP){
			myputsln("#[UP BTN] AP POWER MODE");
			FND_Output4Char(FND4_AP_POWER_MODE);				

			E2P_Test();			

			TagConf.ApulsePowerMode = ON;					
			LoadSetting(_reset);
			
			E2P_SaveApPowrMode();		
		}
		else if(tmp == KEYDEFS_DN){
			myputsln("#[DN BTN] MK POWER MODE");
			FND_Output4Char(FND4_MK_POWER_MODE);

			E2P_Test();			

			TagConf.ApulsePowerMode = OFF;		
				
			LoadSetting(_reset);					
			E2P_SaveApPowrMode();		
		}
		else{
			tmp = KEYDEFS_NULL;
		}

		if(tmp != KEYDEFS_NULL){			
			RESET_TIMER(&TimerCount.period_500ms);
			RESET_TIMER(&TimerCount.period_10ms);	
			while (!CHK_TOV(TimerCount.period_500ms, 500)){				
				if(CHK_TOV(TimerCount.period_10ms, DEFS_TIME_10MS)){
					RESET_TIMER(&TimerCount.period_10ms);			
					LED_Update();		Clear_WatchDog();		
				}									
				FND_ChangeDigit();		Console_CheckSendData();		
		    }	
		}
	}

	FND_FWVer();	

	myputsln(" ");
	myputsln("#SOFTGEAR Co.,LTD");	
	myprintf("#PCB :: PSD-RFIDCU-MAIN-A%d \r\n", MyPCBNumber);	
	myputs("#F/W :: RFIDCU APP. :: ");	
	myprintf("Ver. %s", FW_VER_STR);	
	myprintf(" :: %s \r\n", FW_DATE_STR);		
	myputsln("#START");

	RESET_TIMER(&TimerCount.period_500ms);
	RESET_TIMER(&TimerCount.period_10ms);	
	while (!CHK_TOV(TimerCount.period_500ms, 500)){
		
		if(CHK_TOV(TimerCount.period_10ms, DEFS_TIME_10MS)){
			RESET_TIMER(&TimerCount.period_10ms);			

			LED_Update();
			
			Clear_WatchDog();		
		}				
			
		FND_ChangeDigit();
		
		Console_CheckSendData();		
    }		

	
	Toggle_RunLED();
	{
		E2P_Test();			
		
		DefaultSetting(_reset);

		LoadSetting(_check);
	}		
	Clear_WatchDog();	/* Reset WatchDog */

	SysInit();	

	/* Turn on RUN LED */
	Toggle_RunLED();				

  	bRFIDBootCheckTime = TRUE;	
  	
	RFID_Init();

	/* Send 'Restart Message' via CANs  */		
	m_PgmEnv.Alarm.Restart = 1;	
	CAN_RFIDCU_TestResult();	

	MRTC_DisplayDataAndTime(_reset);
		
	RESET_TIMER(&TimerCount.period_2sec);
	RESET_TIMER(&TimerCount.period_500ms);
	RESET_TIMER(&TimerCount.period_10ms);
	RESET_TIMER(&TimerCount.period_10ms_tpu);
	RESET_TIMER(&TimerCount.period_rtc_100ms);
	RESET_TIMER(&TimerCount.com_link_checktime);
	RESET_TIMER(&TimerCount.FndDisTime);

	/* SD Card Init */
	SD_Card_Init();
	
	/* Reset WatchDog */
	Clear_WatchDog();		
	
	BootCount 	 = 0;	
	timeout_boot = GetTickCount(TIMER0);	


	FND_MenuNone();		
		
	//COMx_Write(COM_RFU1, test_string, mystrlen(test_string));

	bFirstBootFlag = TRUE;	
	RESET_TIMER(&FirstBootTimer);	
	
	LOG_printEvent(COM_RFU1, LOG_EVENT_RESET, 0);

	LOG_printEvent(COM_RFU2, LOG_EVENT_RESET, 0);
	
	/* ==========================
		Start Main Loop 
	=============================*/	
	while(1){		

	
		/* print boot count if RFID is enabled	*/	
		if(bRFIDBootCheckTime){
			if(IS_TIMEOUT(&timeout_boot, 100)){			 

				Toggle_RunLED();
			
				/* increment the boot count every 100ms */
				/* 10sec */
				if(++BootCount >= RFID_MAX_BOOT_CNT){ 
					bRFIDBootCheckTime = FALSE;
					m_PgmEnv.Alarm.Restart = 0;
					myputsln("[RFID] LINK FAIL");					
				}else{
					/* print the boot count every 1sec */
					tmp = (u8)(BootCount % 10); 				
					if(tmp == 0){ 
						tmp = (u8)(BootCount / 10);
						myprintf("[RFID] BOOT COUNT %02d\r\n", tmp);						
					}
				}
			}			
		}	
		

		AppMain();		
		
		/* 115200bps: 1character per 87us,  512bytes * 87 = 45ms   */		
		Console_CheckSendData();

		COMx_CheckSendData(COM_RFU1);

		COMx_CheckSendData(COM_RFU2);
		
		/* Reset WatchDog every 16us */
    	Clear_WatchDog();
	}		
}
