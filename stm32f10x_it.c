/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "Common.h"
#include "SD_Card.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */

void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		
		Timer[0]++;	
  		SDTimer_Handler();
	}
}


/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval None
  */

void RTC_IRQHandler(void)
{	
	// interrupt request per every 1sec

	/*		
	  THH = 86399 / 3600 = 23
	  TMM = (86399 % 3600) / 60 = 59 
	  TSS = (86399 % 3600) % 60 = 59
	
	  Reset RTC Counter when Time is 23:59:59
	  0x0001517F(86399) : 23:59:59
	  0x00015180(86400 == 000000) : (24:00:00 == 00:00:00)

	  0 to 86399(0x0001517F)
	  if(RTC_GetCounter() == 86400) RTC_SetCounter(0x00);
	*/	 
	
	if(RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{
	    /* Clear the RTC Second interrupt */  
	    //NVIC_ClearPendingIRQ(RTC_IRQn);

		// Clear the RTC Second interrupt	    
	    RTC_ClearITPendingBit(RTC_IT_SEC);					

		RTC_UpdateFlag 	= TRUE;
			
		RTC_100msCnt 	= 0;	
		RESET_TIMER(&TimerCount.period_rtc_100ms);		

		/* Reset RTC Counter when Time is from 23:59:59(86399) to 24:00:00(86400) */
		if(RTC_GetCounter() == RTC_SECONDS_IN_DAY)
		{
			RTC_SetCounter(0x0); 	

			// Wait until last write operation on RTC registers has finished 
		  	RTC_WaitForLastTask();

			// Increment the date 
		  	MRTC_IncrementDate();	  
		}
  }
}


/**
  * @brief  This function handles USART1 global interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
	UART_FIFO *p;
	u32 tmp_wr;

	/* RX */
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){  			
    	/* Read one byte from the receive data register */
		p = &sio_rx[0];	
		p->buffer[p->write++] = USART_ReceiveData(USART1);		
		if(p->write >= UARTDEFS_MAX_FIFO_LEN) p->write = 0;    	
  	}
	
  	/* TX */
  	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET){   
    	/* Write one byte to the transmit data register */
		p = &sio_tx[0];	
		tmp_wr = p->write;
		if(p->read != tmp_wr){	
    		USART_SendData(USART1, p->buffer[p->read++]);
			if(p->read >= UARTDEFS_MAX_FIFO_LEN) p->read = 0;		
			if(p->read == tmp_wr)USART_ITConfig(USART1, USART_IT_TXE, DISABLE);			
		}		    
  	}		
}


/**
  * @brief  This function handles USART1 global interrupt request.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{
	UART_FIFO *p;
   	u32 tmp_wr;
        
	/* RX */
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){  			
    	/* Read one byte from the receive data register */
		p = &sio_rx[1];	
		p->buffer[p->write++] = USART_ReceiveData(USART2);		
		if(p->write >= UARTDEFS_MAX_FIFO_LEN)p->write = 0;		   	
  	}
	
  	/* TX */
  	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET){   
    	/* Write one byte to the transmit data register */
		p = &sio_tx[1];	
        tmp_wr = p->write;
		if(p->read != tmp_wr){	
    		USART_SendData(USART2, p->buffer[p->read++]);
			if(p->read >= UARTDEFS_MAX_FIFO_LEN) p->read = 0;		
			if(p->read == tmp_wr)USART_ITConfig(USART2, USART_IT_TXE, DISABLE);			
		}	    
  	}		
}



/**
  * @brief  This function handles USART1 global interrupt request.
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
{
	UART_FIFO *p;
   	u32 tmp_wr;
        
	/* RX */
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){  			
    	/* Read one byte from the receive data register */
		p = &sio_rx[2];	
		p->buffer[p->write++] = USART_ReceiveData(USART3);		
		if(p->write >= UARTDEFS_MAX_FIFO_LEN)p->write = 0;	    	
  	}
	
  	/* TX */
  	if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET){   
    	/* Write one byte to the transmit data register */
		p = &sio_tx[2];	
        tmp_wr = p->write;
		if(p->read != tmp_wr){	
    		USART_SendData(USART3, p->buffer[p->read++]);
			if(p->read >= UARTDEFS_MAX_FIFO_LEN) p->read = 0;		
			if(p->read == tmp_wr)USART_ITConfig(USART3, USART_IT_TXE, DISABLE);			
		}	    
  	}		
}



/**
  * @brief  This function handles USART1 global interrupt request.
  * @param  None
  * @retval None
  */
void UART4_IRQHandler(void)
{
	UART_FIFO *p;
   	u32 tmp_wr;
        
	/* RX */
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET){  			
    	/* Read one byte from the receive data register */
		p = &sio_rx[3];	
		p->buffer[p->write++] = USART_ReceiveData(UART4);		
		if(p->write >= UARTDEFS_MAX_FIFO_LEN)p->write = 0;    	
  	}
	
  	/* TX */
  	if(USART_GetITStatus(UART4, USART_IT_TXE) != RESET){   
    	/* Write one byte to the transmit data register */
		p = &sio_tx[3];	
        tmp_wr = p->write;
		if(p->read != tmp_wr){	
    		USART_SendData(UART4, p->buffer[p->read++]);
			if(p->read >= UARTDEFS_MAX_FIFO_LEN) p->read = 0;		
			if(p->read == tmp_wr)USART_ITConfig(UART4, USART_IT_TXE, DISABLE);			
		}		    
  	}		
}



/**
  * @brief  This function handles USART1 global interrupt request.
  * @param  None
  * @retval None
  */
void UART5_IRQHandler(void)
{
	UART_FIFO *p;
	u32 tmp;
        
	/* RX */
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET){			
		/* Read one byte from the receive data register */
		p = &sio_rx[4]; 
		p->buffer[p->write++] = USART_ReceiveData(UART5);		
		if(p->write >= UARTDEFS_MAX_FIFO_LEN)p->write = 0;		
	}
		
	/* TX */
	if(USART_GetITStatus(UART5, USART_IT_TXE) != RESET){   
		/* Write one byte to the transmit data register */
		p = &sio_tx[4]; 
        tmp = p->write;
		if(p->read != tmp){	
			USART_SendData(UART5, p->buffer[p->read++]);
			if(p->read >= UARTDEFS_MAX_FIFO_LEN) p->read = 0;		
			if(p->read == tmp)USART_ITConfig(UART5, USART_IT_TXE, DISABLE);			
		}			
	}		
}

/**
  * @brief  This function handles CAN1 RX0 Handler.
  * @param  None
  * @retval None
  */
void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	u32 tmp_wr;

	tmp_wr = CAN_rx[0].write;
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);	
	if((RxMessage.IDE == CAN_ID_EXT)&&(RxMessage.DLC == 8)){
		CAN_rx[0].packet[tmp_wr].address = RxMessage.ExtId;
		memcpy((u8 *)&CAN_rx[0].packet[tmp_wr].data[0], (u8 *)&RxMessage.Data[0], 8);

		if (++tmp_wr >= CAN_MAX_LIST_SIZE) tmp_wr = 0;
			
		CAN_rx[0].write = tmp_wr;
	}  
}

/**
  * @brief  This function handles CAN2 RX0 Handler.
  * @param  None
  * @retval None
  */

void CAN2_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	u32 tmp_wr;

	tmp_wr = CAN_rx[1].write;
	CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);	
	if((RxMessage.IDE == CAN_ID_EXT)&&(RxMessage.DLC == 8)){
		CAN_rx[1].packet[tmp_wr].address = RxMessage.ExtId;
		memcpy((u8 *)&CAN_rx[1].packet[tmp_wr].data[0], (u8 *)&RxMessage.Data[0], 8);

		if (++tmp_wr >= CAN_MAX_LIST_SIZE) tmp_wr = 0;
			
		CAN_rx[1].write = tmp_wr;
	}  
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
