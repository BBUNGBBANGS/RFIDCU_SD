/*****************************************************************************
* Copyright(C) 2022 GREAT RAIL CO., LTD. All rights reseved
*
******************************************************************************
* @File name 	: EEPROM.C
* @Author      	: Great RAIL
* @Version      : V1.0.0
* @Date         : 12 JUN, 2022
* @Brief		: RFIDCU 
*
******************************************************************************

# REVISION HISTORY
       
	Original File : M24M01.C

	12-JUN-2022 Great Rail
		-. Initial file creation			
	
******************************************************************************/

#include "Common.h"


/*
	AT24C16 :	0x 0000 ~ 0x07FF
	- 16Kbit(16384bit / 8 = 2048byte) 0x00H ~ 0x7FFH, 16pages

		DEVICE ADDRESS(1010 $ P2 P1 P0 R/W) (Ack)
		+ ADDRESS (Ack)

		P0 = 0 ; 0x00 ~ 0xFFH
		P2P1P0 = 001 ; 0x100 ~ 0x1FFH
		...
		P2P1P0 = 001 ; 0x700 ~ 0x7FFH
		
	
	AT24C04

	- 4Kbit(4096bit / 8 = 512byte) 0x00H ~ 0x1FFH, 16pages

		DEVICE ADDRESS(1010 $ A2 A1 P0 R/W) (Ack)
		+ ADDRESS (Ack)

		P0 = 0 ; 0x00 ~ 0xFFH
		P0 = 1 ; 0x100 ~ 0x1FFH
		
	
	- 16byte page write mode
	- 400Khz
	- Self-timed write cycle (5ms max)


	FM24W256

	- 256Kbit (262144/8 = 32768byte, 32Kbyte) 0x0000 ~ 0x7FFFH

		DEVICE ADDRESS(1010 $ A2 A1 A0 R/W) (Ack)

		+ ADDRESS MSB (Ack)	
		+ ADDRESS LSB (Ack)
	
	- page 사이즈 제한 없음 
	- 1MHz
	- No Self-timed write cycle
	

	M24M01(1Mbit 1048576bit / 8 = 131072byte , 128Kbyte)

		DEVICE ADDRESS(1010 $ E2 E1 A16 R/W) (Ack)

		+ ADDRESS MSB (Ack)	
		+ ADDRESS LSB (Ack)		
		
	- 256byte page write mode
	- 1MHz
	- Self-timed write cycle (5ms max)
*/



#define MEE_SLAVE_ADDRESS7    	0xA0	

//#define MEE_PageSize			8	/* 24C01, 02 */

#define MEE_PageSize			16	/* 24C04, 08, 16 */


//Self-timed Write Cycle (5 ms max)


//MEE_Delay(30);/* 10 == 3.6us , 30 == 12us*/

#define MEE_DLY_10U		(28*2)	/* 10us */
#define MEE_DLY			(28*2)	/* 10us */
#define MEE_DLY_CLK		(14*2)	/* 5us */

#define MEE_DLY_ACK_COUNT	10

/*==================================================================
* Function Name 	: delay_for_wait
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void MEE_Delay(vu32 dly)
{
    // --dly : 위험code
    while(dly-- > 0)
    {
    	if(dly%2)GPIO_SetBits(GPIOA, SPI_DLY);
		else     GPIO_ResetBits(GPIOA, SPI_DLY);
    }
}


/*==================================================================
* Function Name 	: SET_I2C_PORT
* Description		: 
* Input 			: 
* Output		: 
* Return			: 
====================================================================*/
void SET_I2C_PORT(uint16_t GPIO_Pin, u8 state)
{
	(state)?	GPIO_SetBits(GPIOB, GPIO_Pin) : GPIO_ResetBits(GPIOB, GPIO_Pin);  
}


/*==================================================================
* Function Name 	:SET_SDA_DIR
* Description		: 
* Input 			: 
* Output		: 
* Return			: 
====================================================================*/
static void SET_SDA_DIR(u8 Direction)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if(Direction == DIR_IN){

		GPIO_InitStructure.GPIO_Pin 	= I2C_SDA;
	  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;		
	  	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;
	  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
	else{/* DIR_OUT */
		GPIO_InitStructure.GPIO_Pin 	= I2C_SDA;
	  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;		
	  	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
}


/*==================================================================
* Function Name 	:MEE_GenerateSTART
* Description		: 
* Input 			: 
* Output		: 
* Return			: 
====================================================================*/
void MEE_GenerateSTART(void)
{
	SET_I2C_PORT(I2C_SDA, 1);
	SET_I2C_PORT(I2C_SCL, 1);
	
	MEE_Delay(MEE_DLY);
	
	SET_I2C_PORT(I2C_SDA, 0);	MEE_Delay(MEE_DLY);
	
	SET_I2C_PORT(I2C_SCL, 0);	MEE_Delay(MEE_DLY);
}


/*==================================================================
* Function Name 	:MEE_GenerateSTOP
* Description		: 
* Input 			: 
* Output		: 
* Return			: 
====================================================================*/
void MEE_GenerateSTOP(void)
{
	MEE_Delay(MEE_DLY);
	
	SET_I2C_PORT(I2C_SDA, 0);	MEE_Delay(MEE_DLY);
	
	SET_I2C_PORT(I2C_SCL, 1);	MEE_Delay(MEE_DLY);	
	
	SET_I2C_PORT(I2C_SDA, 1);	MEE_Delay(MEE_DLY);
}


/*==================================================================
* Function Name 	:MEE_CheckACK
* Description		: 
* Input 			: 
* Output		: 
* Return			: 
====================================================================*/
bool MEE_CheckACK(void)
{
	vu32 nDelay = 1;
	
	SET_I2C_PORT(I2C_SDA, 1);
	
	SET_SDA_DIR(DIR_IN);	/* Input */

	while(GPIO_ReadInputDataBit(GPIOB, I2C_SDA) 
               && (nDelay < MEE_DLY_ACK_COUNT)){
		
		nDelay++;		MEE_Delay(MEE_DLY);/* 10us * 10 = 100us */
	}

	MEE_Delay(MEE_DLY);
	SET_SDA_DIR(DIR_OUT);	/* Output */
	
	SET_I2C_PORT(I2C_SDA, 0);	MEE_Delay(MEE_DLY);
	
	SET_I2C_PORT(I2C_SCL, 1);	MEE_Delay(MEE_DLY);	

	SET_I2C_PORT(I2C_SCL, 0);

	if(nDelay < MEE_DLY_ACK_COUNT)	return(TRUE);
	else							return(FALSE);

}


/*==================================================================
* Function Name 	:MEE_NoACK
* Description		: 
* Input 			: 
* Output		: 
* Return			: 
====================================================================*/
void MEE_NoACK(void)
{
	vu32 nDelay = 1;

	SET_SDA_DIR(DIR_OUT);	/* Output */
	
	SET_I2C_PORT(I2C_SDA, 1);	MEE_Delay(MEE_DLY);
	
	SET_I2C_PORT(I2C_SCL, 1);	MEE_Delay(MEE_DLY);	
	
	SET_I2C_PORT(I2C_SCL, 0);	MEE_Delay(MEE_DLY);		
}


/*==================================================================
* Function Name 	:MEE_SendData
* Description		: 
* Input 			: 
* Output		: 
* Return			: 
====================================================================*/
bool MEE_SendData(u8 data)
{
	u32 i;

	for(i = 0; i < 8; i++){
		
		if((data & 0x80) == 0x80)	SET_I2C_PORT(I2C_SDA, 1);
		else 						SET_I2C_PORT(I2C_SDA, 0);	

		data <<= 1;

		MEE_Delay(MEE_DLY_CLK);
		
		SET_I2C_PORT(I2C_SCL, 1);	MEE_Delay(MEE_DLY_CLK);	
	
		SET_I2C_PORT(I2C_SCL, 0);	MEE_Delay(MEE_DLY_CLK);			
	}

	return(MEE_CheckACK());
}

/*==================================================================
* Function Name 	:MEE_SendDataNoAck
* Description		: 
* Input 			: 
* Output		: 
* Return			: 
====================================================================*/
void MEE_SendDataNoAck(u8 data)
{
	u32 i;

	for(i = 0; i < 8; i++){
		
		if((data & 0x80) == 0x80)	SET_I2C_PORT(I2C_SDA, 1);
		else 						SET_I2C_PORT(I2C_SDA, 0);	

		data <<= 1;

		MEE_Delay(MEE_DLY_CLK);
		
		SET_I2C_PORT(I2C_SCL, 1);	MEE_Delay(MEE_DLY_CLK);	
	
		SET_I2C_PORT(I2C_SCL, 0);	MEE_Delay(MEE_DLY_CLK);			
	}
	MEE_NoACK();
	//return(MEE_CheckACK());
}

/*==================================================================
* Function Name 	:MEE_ReadData
* Description		: 
* Input 			: 
* Output		: 
* Return			: 
====================================================================*/
u8 MEE_ReadData(void)
{
	u32 i;
	u8 rData;
	
	SET_SDA_DIR(DIR_IN);	/* Input */

	for(i = 0; i < 8; i++){

		rData <<= 1;

		SET_I2C_PORT(I2C_SCL, 1);	MEE_Delay(MEE_DLY_CLK);	

		if(GPIO_ReadInputDataBit(GPIOB, I2C_SDA)) rData |= 0x01;
		else									  rData &= 0xFE;
		
		SET_I2C_PORT(I2C_SCL, 0);		
	}

	MEE_NoACK();
	
	return(rData);
}


/*==================================================================
* Function Name 	:MEE_ReadData
* Description		: 
* Input 			: 
* Output		: 
* Return			: 
====================================================================*/
u8 MEE_ReadDataAck(void)
{
	u32 i;
	u8 rData;
	
	SET_SDA_DIR(DIR_IN);	/* Input */

	for(i = 0; i < 8; i++){

		rData <<= 1;

		SET_I2C_PORT(I2C_SCL, 1);	MEE_Delay(MEE_DLY_CLK);	

		if(GPIO_ReadInputDataBit(GPIOB, I2C_SDA)) rData |= 0x01;
		else									  rData &= 0xFE;
		
		SET_I2C_PORT(I2C_SCL, 0);		
	}

	MEE_CheckACK();
	
	return(rData);
}

/*==================================================================
* Function Name 	:MEE_ByteWrite
* Description		: 
* Input 			: 
* Output		: 
* Return			: 
====================================================================*/
void MEE_ByteWrite(u32 WriteAddr, u8* pBuffer)
{
	u8 addr;
	u8 BlockNumber;
	/*	M24M01 1Mbit, 128K x 8bit 
	
		0x	0000 ~ 0x  FFFF  : I2C1_SLAVE_ADDRESS7 0xA0
		0x10000 ~ 0x1FFFF  : I2C1_SLAVE_ADDRESS7 0xA2	
	*/		

	SET_SDA_DIR(DIR_OUT);	/* Data 출력 */

	MEE_GenerateSTART();	/* Start */	

	if(WriteAddr > 0xff){
		BlockNumber = WriteAddr / 0x100;		
		BlockNumber <<= 1;
		BlockNumber &= 0x0E;
	}
	else BlockNumber = 0x00;
	
	addr = MEE_SLAVE_ADDRESS7 | BlockNumber;
	MEE_SendData(addr);	/* Device Address */ 

	addr = (u8)((WriteAddr) & 0xff);
    
    MEE_SendData(addr);	/* 하위 어드레스 */      

	MEE_SendData(*pBuffer);	/* Data */      

	MEE_GenerateSTOP();
}

/*==================================================================
* Function Name 	:MEE_ByteRead
* Description		: 
* Input 			: 
* Output		: 
* Return			: 
====================================================================*/
u8 MEE_ByteRead(u32 Address)
{
	u8 addr;
	u8 BlockNumber;
	u8 rData;
	
	/* AT24C16 */
	
	SET_SDA_DIR(DIR_OUT);	/* Data 출력 */

	MEE_GenerateSTART();	/* Start */	

	
	if(Address > 0xff){
		BlockNumber = Address / 0x100;		
		BlockNumber <<= 1;
		BlockNumber &= 0x0E;
	}
	else BlockNumber = 0x00;
	
	addr = MEE_SLAVE_ADDRESS7 | BlockNumber;
	
	MEE_SendData(addr); /* Device Address */	

	addr = (u8)((Address) & 0xff);
    
    MEE_SendData(addr);	/* 하위 어드레스 */      

	MEE_GenerateSTOP();

	MEE_GenerateSTART();	/* Start */	
	
	
	addr = MEE_SLAVE_ADDRESS7 | BlockNumber;	
	
	MEE_SendData(addr|0x01);	/* Device Address (R)*/
	
	rData = MEE_ReadData();

	MEE_GenerateSTOP();	

	MEE_Delay(MEE_DLY);	
    return(rData);
}

/*==================================================================
* Function Name 	:MEE_PageWrite
* Description		: 
* Input 			: 
* Output		: 
* Return			: 
====================================================================*/
void MEE_PageWrite(u32 WriteAddr, u8* pBuffer, u16 NumByteToWrite)
{
	u8 BlockNumber, addr;
	u16 NumOfSingle, count;
	
	/*	M24M01 1Mbit, 128K x 8bit , 최대 page size 256
	
		0x	0000 ~ 0x  FFFF  : I2C1_SLAVE_ADDRESS7 0xA0
		0x10000 ~ 0x1FFFF  : I2C1_SLAVE_ADDRESS7 0xA2	
	*/		
		
	/* 최대 page size 256 */
  	NumOfSingle = NumByteToWrite % (MEE_PageSize+1);
	
	SET_SDA_DIR(DIR_OUT);	/* Data 출력 */

	MEE_GenerateSTART();	/* Start */	

	if(WriteAddr > 0xff){
		BlockNumber = WriteAddr / 0x100;		
		BlockNumber <<= 1;
		BlockNumber &= 0x0E;
	}
	else BlockNumber = 0x00;
	
	addr = MEE_SLAVE_ADDRESS7 | BlockNumber;

	MEE_SendData(addr);	/* Device Address */ 

	addr = (u8)((WriteAddr) & 0xff);
    
    MEE_SendData(addr);	/* 하위 어드레스 */      

	for(count = 0; count < NumOfSingle; count++){
		//MEE_SendDataNoAck(pBuffer[count]);	/* Data */   
		MEE_SendData(pBuffer[count]);	/* Data */   
	}

	MEE_GenerateSTOP();
}


/*==================================================================
* Function Name 	:MEE_PageRead
* Description		: 
* Input 			: 
* Output		: 
* Return			: 
====================================================================*/
void MEE_PageRead(u32 Address, u8 *pBuffer, u16 NumByteToRead)
{
	u8 addr;
	u8 BlockNumber;
	u8 rData;
	u16 NumOfSingle, count;
	
	/*	M24M01 1Mbit, 128K x 8bit 
	
		0x	0000 ~ 0x  FFFF  : I2C1_SLAVE_ADDRESS7 0xA0
		0x10000 ~ 0x1FFFF  : I2C1_SLAVE_ADDRESS7 0xA2	
	*/		
	/* 최대 page size 256 */
  	NumOfSingle = NumByteToRead % (MEE_PageSize+1);
	
	SET_SDA_DIR(DIR_OUT);	/* Data 출력 */

	MEE_GenerateSTART();	/* Start */	

	/* AT24C16BN */
	if(Address > 0xff){
		BlockNumber = Address / 0x100;		
		BlockNumber <<= 1;
		BlockNumber &= 0x0E;
	}
	else BlockNumber = 0x00;
	
	addr = MEE_SLAVE_ADDRESS7 | BlockNumber;	
	
	MEE_SendData(addr);	/* Device Address */ 

	addr = (u8)((Address) & 0xff);
    
    MEE_SendData(addr);	/* 하위 어드레스 */      

	MEE_GenerateSTOP();

	MEE_GenerateSTART();	/* Start */	

	addr = MEE_SLAVE_ADDRESS7 | BlockNumber;	
	MEE_SendData(addr|0x01);	/* Device Address (R)*/
	
	for(count = 0; count < NumOfSingle; count++){
		if(count == (NumOfSingle -1)){
			rData = MEE_ReadData();			
		}
		else{
			rData = MEE_ReadDataAck();
		}
		
		pBuffer[count]= rData;
	}

	MEE_GenerateSTOP();	  
}
