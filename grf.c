#include "Common.h"
/*****************************************************************************
*                                                                          
* 							LITERAL CONSTANTS(DEFINES)
*                    
******************************************************************************/
// Response packet size of tag data to GRF
#define GRF_RSP_TAG_DATA_SIZE		(29)


// GRF REQUEST PACKET INDEX NUMBER
#define MK_REQ_IDX_STX 		0
#define MK_REQ_IDX_RXID		1	
#define MK_REQ_IDX_TXID		2	
#define MK_REQ_IDX_LEN 		3
#define MK_REQ_IDX_CMD1 	4 
#define MK_REQ_IDX_CMD2 	5 
#define MK_REQ_IDX_DATA     6

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
* 								LOCAL VARIABLES
*                    
******************************************************************************/

static GRF_LINK			GRF_dlk;
static GRF_RX_PORT 		GRF_Rx;
static GRFSendListSTR	GRF_Tx;


/*****************************************************************************
*                                                                          
* 								FUNCTION DEFINITIONS(LOCAL)
*                    
******************************************************************************/

static void CheckRxCmdFrom_GRF(GRF_LINK *pDlk);
static void ParseRxFrameFrom_GRF(void);
static bool GRF_IsTagRspCmd(u8 tmp_rd);
static bool GRF_IsAckCmd(u8 tmp_rd);
static void GRF_TxManager(void);
static void GRF_TxProcess(void);


/*==================================================================
* Function Name  	: GRF_InitVal
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void GRF_InitVal(void)
{	
	memset((u8 *)&GRF_dlk, 0, GRF_LINK_LEN);
	memset((u8 *)&GRF_Rx, 0, sizeof(GRF_RX_PORT));
	memset((u8 *)&GRF_Tx, 0, sizeof(GRFSendListSTR));

	GRF_Rx.pReceiveFrame = &GRF_Rx.receiveFrame0[0];	
	
	GRF_dlk.pRx = NULL;	

}


/*==================================================================
* Function Name  	: GRF_IsTagRspCmd
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
static bool GRF_IsTagRspCmd(u8 tmp_rd)
{
	bool bTagCmd = FALSE;	
		
	if((GRF_Tx.packet[tmp_rd].Command[0] == 'a') &&
		(GRF_Tx.packet[tmp_rd].Command[1] == 'c') &&
		(GRF_Tx.packet[tmp_rd].Len == GRF_RSP_TAG_DATA_SIZE)){

		bTagCmd = TRUE;
	}

	return(bTagCmd);
}

/*==================================================================
* Function Name  	: GRF_IsAckCmd
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
static bool GRF_IsAckCmd(u8 tmp_rd)
{
	bool bTagCmd = FALSE;	
		
	if((GRF_Tx.packet[tmp_rd].Command[0] == 'a') &&
		(GRF_Tx.packet[tmp_rd].Command[1] == 'c')){

		bTagCmd = TRUE;
	}

	return(bTagCmd);
}


/*==================================================================
* Function Name 	: GRF_tagPrint
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
void GRF_tagPrint(u8 tmp_rd)
{
	u32 i;	
	u32 tagNum;
	s8 str[32]; 	
	u8 temp;
	u8 *data = &GRF_Tx.packet[tmp_rd].data[0];

	if(IsUseComMonitoring()){ 
		
//	if(DBG_COM2_PRINT_ASCII()){ 	
	
		//MRTC_CalculateTime();
		
		/*[0000] 02/03 12:12:14.0 :: */

		i = 0;
		{			
			tagNum = 0;
			if(GRF_Tx.packet[tmp_rd].tagRxNumber < 10000) tagNum = GRF_Tx.packet[tmp_rd].tagRxNumber;
			

			str[i++] =0x0D;
			str[i++] =0x0A;
		
			str[i++] = '['; 
			str[i++] = 'B'; 

			temp = tagNum/100; 		
			str[i++] = D2C(temp/10);	str[i++] = D2C(temp%10);

			temp = tagNum%100;
			str[i++] = D2C(temp/10);	str[i++] = D2C(temp%10); 

			str[i++] = ']';
			str[i++] = 0x20;	/* space */ 
		}

		{
			str[i++] = D2C(s_DateStructVar.Month/10);	
			str[i++] = D2C(s_DateStructVar.Month%10);	
			str[i++] = '/'; /* '/' */	

			
			str[i++] = D2C(s_DateStructVar.Day/10); 
			str[i++] = D2C(s_DateStructVar.Day%10); 
			str[i++] = 0x20;	/* space */ 	
		}

		{			
			str[i++] = D2C(s_TimeStructVar.HourHigh);	
			str[i++] = D2C(s_TimeStructVar.HourLow);	
			str[i++] = ':'; /* '/' */	

			
			str[i++] = D2C(s_TimeStructVar.MinHigh);
			str[i++] = D2C(s_TimeStructVar.MinLow);
			str[i++] = ':'; /* '/' */	

			str[i++] = D2C(s_TimeStructVar.SecHigh);
			str[i++] = D2C(s_TimeStructVar.SecLow);
			str[i++] = '.'; /* '/' */				

			str[i++] = D2C(RTC_100msCnt);	

			
			
			str[i++] = 0x20;	/* space */ 	
			

			// glass tag 지연전송 packet
			if(GRF_Tx.packet[tmp_rd].tagType == TAG_TYPE_GLASS_DELAY){	
				str[i++] = 'G'; /* '/' */	
				str[i++] = 'D'; /* '/' */	
			}
			else{
				str[i++] = ':'; /* '/' */	
				str[i++] = ':'; /* '/' */	
			}	
			
			str[i++] = 0x20;	/* space */ 	

			COMx_Write(COM_RFU2, (u8 *)&str[0], i);
		}			

		if(DBG_COM2_TAG_PRINT()){

			for(i=MK_IDX_05; i<(MK_IDX_05+14); i++){
			//for(i=MK_IDX_05; i<(MK_IDX_05+12); i++){
				str[0] = D2C(data[i]/16);
				str[1] = D2C(data[i]%16);
				str[2] = 0x20;	/* space */	
				COMx_Write(COM_RFU2, (u8 *)&str[0], 3);
			}	

		}
		else{
			for(i=0; i<(data[MK_IDX_LEN]+6); i++){
				str[0] = D2C(data[i]/16);
				str[1] = D2C(data[i]%16);
				str[2] = 0x20;	/* space */ 
				COMx_Write(COM_RFU2, (u8 *)&str[0], 3);
			}	
		}
	}
	else{				
		
		COMx_Write(COM_RFU2, (u8 *)&GRF_Tx.packet[tmp_rd].data[0], RFID_TAG_PACKET_SIZE);
	}
}	


/*==================================================================
* Function Name 	: GRF_TxManager
* Description		: 
* Input 			: 
* Output			: 
* Return			: 
====================================================================*/
static void GRF_TxManager(void)
{	
	UART_FIFO *p = &sio_tx[COM_GRF];

	u32 tmp_rd, tmp_wr;	
	u32 i, tx_len, tx_wr;
	
	tmp_rd = GRF_Tx.read;
	tmp_wr = GRF_Tx.write;	

	if(tmp_wr != tmp_rd){
		
		if(GRF_IsTagRspCmd(tmp_rd)){
			GRF_Tx.bPrvTagSend = TRUE;

			GRF_tagPrint(tmp_rd);
		}	
		
		if(GRF_IsAckCmd(tmp_rd))	RESET_TIMER(&GRF_dlk.pollTimer);	
		
		tx_len = GRF_Tx.packet[tmp_rd].Len;		
		tx_wr  = p->write;	
		
		for(i = 0; i < tx_len; i++){	

			p->buffer[tx_wr++] = GRF_Tx.packet[tmp_rd].data[i];
			
			if(tx_wr >= UARTDEFS_MAX_FIFO_LEN) tx_wr = 0;
		}	
		
		p->write = tx_wr;					

		if (++tmp_rd >= GRF_MAX_TX_LIST_SIZE) tmp_rd = 0;
		GRF_Tx.read = tmp_rd;		
				
		SET_UART_TX_IT_CONFIG(COM_GRF, ON);			
	}
}


/*==================================================================
* Function Name  	: GRF_RspAckCmd
* Description    	: 
* Input          	:
* Output         	: 
* Return         	: 
====================================================================*/
void GRF_RspAckCmd(void)
{
	GRF_PACKET_RSP rPacket;
	GRF_PACKET_RSP *p = &rPacket;
	
 	u8 Bcc;	
	u8 *src;  
	u32 packetLen, tmp_wr, i;
	
	p->Stx  = 0x02;
	p->RxID = 0xFF;
	p->TxID = 0x01;	
	p->Len	= 3; /* BCC and ETX are not counted */	
	p->Command[0] = 'a';
	p->Command[1] = 'c';	
	p->Status	  = 0x70; /* Communication ACK */	

	Bcc  = p->RxID;	
	Bcc ^= p->TxID;
	Bcc ^= p->Len;

	src = &p->Command[0];	
	for(i = 0; i< p->Len; i++)Bcc ^= src[i];	
	
	p->Data[0] = Bcc;
	p->Data[1] = 0x03;
	
	packetLen = (4) + p->Len + (2);

	tmp_wr = GRF_Tx.write;	
	{
		memcpy((u8 *)&GRF_Tx.packet[tmp_wr].data[0] , (u8 *)&p->Stx, packetLen);
		
		GRF_Tx.packet[tmp_wr].Len = packetLen;
		GRF_Tx.packet[tmp_wr].Command[0] = p->Command[0];
		GRF_Tx.packet[tmp_wr].Command[1] = p->Command[1];
	}

	if(++tmp_wr >= GRF_MAX_TX_LIST_SIZE) tmp_wr = 0;
	
	GRF_Tx.write = tmp_wr;		
}

/*==================================================================
* Function Name  	: GRF_RspEndCmd
* Description    	: 
* Input          	:
* Output         	: 
* Return         	: 
====================================================================*/
void GRF_RspEndCmd(u8 command1, u8 command2)
{
	GRF_PACKET_RSP rPacket;
	GRF_PACKET_RSP *p = &rPacket;
	
 	u8 Bcc;	
	u8 *src;  
	u32 packetLen, tmp_wr, i;
	
	p->Stx  = 0x02;
	p->RxID = 0xFF;
	p->TxID = 0x01;	
	p->Len	= 3; /* BCC and ETX are not counted */	
	p->Command[0] = command1;
	p->Command[1] = command2;	
	p->Status	  = 0x01; /* Command 처리 정상종료 */	

	Bcc  = p->RxID;	
	Bcc ^= p->TxID;
	Bcc ^= p->Len;

	src = &p->Command[0];	
	for(i = 0; i< p->Len; i++)Bcc ^= src[i];	
	
	p->Data[0] = Bcc;
	p->Data[1] = 0x03;
	
	packetLen = (4) + p->Len + (2);

	tmp_wr = GRF_Tx.write;	
	{
		memcpy((u8 *)&GRF_Tx.packet[tmp_wr].data[0] , (u8 *)&p->Stx, packetLen);
		
		GRF_Tx.packet[tmp_wr].Len = packetLen;
		GRF_Tx.packet[tmp_wr].Command[0] = p->Command[0];
		GRF_Tx.packet[tmp_wr].Command[1] = p->Command[1];
	}

	if(++tmp_wr >= GRF_MAX_TX_LIST_SIZE) tmp_wr = 0;
	
	GRF_Tx.write = tmp_wr;		
}

						

/*==================================================================
* Function Name  	: GRF_RspTagData
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void GRF_RspTagData(TAG_TXD *pTag, u16 numTagToSend)
{
	GRF_PACKET_RSP rPacket;
	GRF_PACKET_RSP *p = &rPacket;
	
 	u8 Bcc;	
	u8 *src;  
	u32 packetLen, tmp_wr, i;

	// Stx ~ Status
	p->Stx  = 0x02;
	p->RxID = 0xFF;
	p->TxID = 0x01;	
	p->Len	= 0x17; /*BCC and ETX are not counted */	
	p->Command[0] = 'a';
	p->Command[1] = 'c';	
	p->Status	  = 0x01; /* Communication ACK */	

	// Option1
	p->Data[0] = 0x51;

	if(GRFTypeCode <= GRF_TYPE_WJ)	p->Data[1] = GRFDEFS_STWJ_TAG_UID_SIZE();
	else							p->Data[1] = GRFDEFS_HD_TAG_UID_SIZE();
	
	p->Data[2] = 0x01;	

	// PC to CRC  (30 00 05 64 .....CRC[0] CRC[1]
	memcpy((u8 *)&p->Data[3], (u8 *)&pTag->PC[0], 16);	
	//p->Data[3] = 0x30;
	//p->Data[4] = 0x00;		

	/* TagSum (01) */
	p->Data[19] = 0x01;
	
	/* BCC */
	{
		Bcc  = p->RxID;	
		Bcc ^= p->TxID;
		Bcc ^= p->Len;

		src = &p->Command[0];	
		for(i = 0; i< p->Len; i++)Bcc ^= src[i];	
		
		p->Data[20] = Bcc;
	}

	// ETX
	p->Data[21] = 0x03;
	
	packetLen = (4) + p->Len + (2);

	// new tag data
	if(pTag->cntTxtagSum == 0) LOG_printTAGConfirm(COM_RFU1);	

	for(i = 0; i < numTagToSend; i++){
		
		tmp_wr = GRF_Tx.write;	
		{
			memcpy((u8 *)&GRF_Tx.packet[tmp_wr].data[0] , (u8 *)&p->Stx, packetLen);
			
			GRF_Tx.packet[tmp_wr].Len = packetLen;
			GRF_Tx.packet[tmp_wr].Command[0] = p->Command[0];
			GRF_Tx.packet[tmp_wr].Command[1] = p->Command[1];
			GRF_Tx.packet[tmp_wr].tagRxNumber= pTag->tagLastRxNumber;	

			if(pTag->bNextTagCheckSend){
				GRF_Tx.packet[tmp_wr].tagType = TAG_TYPE_GLASS_DELAY;	
			}	
			else
				GRF_Tx.packet[tmp_wr].tagType = TAG_TYPE_NONE;
				
		}

		if(++tmp_wr >= GRF_MAX_TX_LIST_SIZE) tmp_wr = 0;

		//myprintf("[GRF_RspTagData] tmp_wr=%d\r\n", tmp_wr);
		
		GRF_Tx.write = tmp_wr;		
	}

	// GRF로 송신한 tag count 저장함 
	pTag->cntTxtagSum += numTagToSend;
	
}

/*==================================================================
* Function Name  	: GRF_RspFV
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void GRF_RspFV(u8 *data, u16 stringLenToSend)
{
	GRF_PACKET_RSP rPacket;
	GRF_PACKET_RSP *p = &rPacket;
	
 	u8 Bcc;	
	u8 *src;  
	u32 packetLen, tmp_wr, i;

	// Stx ~ Status
	p->Stx  = 0x02;
	p->RxID = 0xFF;
	p->TxID = 0x01;	
	p->Len	= stringLenToSend+3; /*BCC and ETX are not counted */	
	p->Command[0] = 'f';
	p->Command[1] = 'v';	
	p->Status	  = 0x01; /* Communication ACK */	

	memcpy((u8 *)&p->Data[0], (u8 *)&data[0], stringLenToSend);	
	
	/* BCC */
	{
		Bcc  = p->RxID;	
		Bcc ^= p->TxID;
		Bcc ^= p->Len;

		src = &p->Command[0];	
		for(i = 0; i< p->Len; i++)Bcc ^= src[i];	
		
		p->Data[stringLenToSend] = Bcc;
	}

	// ETX
	p->Data[stringLenToSend+1] = 0x03;
	
	packetLen = (4) + p->Len + (2);

	tmp_wr = GRF_Tx.write;	
	{
		memcpy((u8 *)&GRF_Tx.packet[tmp_wr].data[0] , (u8 *)&p->Stx, packetLen);
		
		GRF_Tx.packet[tmp_wr].Len = packetLen;
		GRF_Tx.packet[tmp_wr].Command[0] = p->Command[0];
		GRF_Tx.packet[tmp_wr].Command[1] = p->Command[1];
	}

	if(++tmp_wr >= GRF_MAX_TX_LIST_SIZE) tmp_wr = 0;

	//myprintf("[GRF_RspTagData] tmp_wr=%d\r\n", tmp_wr);
	
	GRF_Tx.write = tmp_wr;		
}


/*==================================================================
* Function Name  	: GRF_TxProcess
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
static void GRF_TxProcess(void)
{
	u32 tmp_rd, tmp_wr;	
	u32 timeout_tx_wait;
	
	tmp_rd = GRF_Tx.read;
	tmp_wr = GRF_Tx.write;		
	
	if(GRF_Tx.bTxWaitTime){

		// No Data, then Default = 50ms
		timeout_tx_wait = GRF_MAX_TX_IDLE_TIME;
		
		if(CHK_TOV(GRF_Tx.rsTxTime, timeout_tx_wait)){
			
			GRF_Tx.bPrvTagSend = FALSE;	
			GRF_Tx.bTxWaitTime = FALSE;			
		}	
		else if(tmp_wr != tmp_rd){ /* New Tx data ? */		

			// Tag data 연속 전송시 최소 (20ms) 대기시간 갖는다.
			// else (5ms)			
			timeout_tx_wait = GRF_MIN_TX_IDLE_TIME;

			// Prev. Data == TagData?
			if(GRF_Tx.bPrvTagSend){

				// New Data == TagData?	(20ms) delay
				if(GRF_IsTagRspCmd(tmp_rd)) timeout_tx_wait = GRF_TAG_TX_IDLE_TIME;	
			}
			

			if(CHK_TOV(GRF_Tx.rsTxTime, timeout_tx_wait)){
			
				GRF_Tx.bPrvTagSend = FALSE;	
				GRF_Tx.bTxWaitTime = FALSE;			
			}				
		}
	}	

	// New Tx Data?
	if(!GRF_Tx.bTxWaitTime && (tmp_wr != tmp_rd)){	

		GRF_TxManager();	
		
		GRF_Tx.bTxWaitTime = TRUE;
		RESET_TIMER(&GRF_Tx.rsTxTime);			
	}		    	
}

/*==================================================================
* Function Name  	: ParseRxFrameFrom_GRF
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
static void ParseRxFrameFrom_GRF(void)
{
	u8 	COMx = COM_GRF;	
	GRF_RX_PORT *p = &GRF_Rx;	

	u8 	tmp_sio_buf[UARTDEFS_MAX_FIFO_LEN];		
	u8	*pBytesToParse, *src_addr, newByte; 
	u32 tmp_wr, tmp_rd, tmp_cnt, numBytesToParse;	

	/*copies global variables to local variables */
	u32 ReceiveCount = p->ReceiveCount;
	u32 ActualLength = p->ActualLength;
	u32 timeout 	 = p->timeout;
	
	u8 bToggle 		 = p->bToggle;
	u8 *pReceiveFrame= p->pReceiveFrame;
	u8 expectedCrc 	 = p->expectedCrc;		
	

	tmp_wr 	= sio_rx[COMx].write;
	tmp_rd 	= sio_rx[COMx].read;
	src_addr= &sio_rx[COMx].buffer[0];	

	
	if(tmp_wr == tmp_rd) return;

	if(tmp_wr > tmp_rd){
		numBytesToParse = tmp_wr - tmp_rd;	
		memcpy((u8 *)&tmp_sio_buf[0], (u8 *)src_addr+tmp_rd, numBytesToParse);
		
	}else{
		tmp_cnt = UARTDEFS_MAX_FIFO_LEN - tmp_rd;
		numBytesToParse =  tmp_cnt + tmp_wr;
		memcpy((u8 *)&tmp_sio_buf[0], (u8 *)src_addr+tmp_rd, tmp_cnt);
		memcpy((u8 *)&tmp_sio_buf[tmp_cnt], (u8 *)src_addr, tmp_wr);		
	}

	pBytesToParse = &tmp_sio_buf[0];
	tmp_cnt = numBytesToParse;

	/* Check the time out of character Interval */
	if(ReceiveCount > 0){
		if(m_Elapsed(TIMER0, timeout) >= TIMEOUT_SERIAL_RX_CHAR){
			ReceiveCount = 0;	
		}else{
			timeout = GetTickCount(TIMER0);
		}		
	}		
		
	while(numBytesToParse)
	{		
		numBytesToParse--;				
		newByte = *pBytesToParse++;					
		
		pReceiveFrame[ReceiveCount++] = newByte;		
			
    	switch(ReceiveCount){
			case(1):  /* First Sync byte */
      		{
        		if (newByte != 0x02){
					
        			ReceiveCount = 0;    			
          		}else{ 
          		
	          		timeout = GetTickCount(TIMER0);					
        		}
        		break;
      		}

			case(2): /* Receiver ID */
      		{
        		if (newByte != 0x01){
					
          			ReceiveCount = 0;           
        		}else{
        			expectedCrc = 0x01;// set a crc to the initial value 
        		}	
        		break;
        	}	
			
        	case(3): /* TX - ID */
      		{				
        		if (newByte != 0xff){
          			ReceiveCount = 0;           
        		}else{
        			expectedCrc ^= newByte;							
        		}	
        		break;
        	}


			case(4):/* LEN */
			{									
				expectedCrc ^= newByte;
				ActualLength = 4 + newByte + 1;/* STX~LEN(4), BCC~EXT(1) */
				
        		break;				
			}									
			
        	default:
        	{   
				/* end of Block ? */				
        		if(ReceiveCount == ActualLength){          			        		
					/* BCC */									 
					if(expectedCrc != newByte){
						ReceiveCount = 0;
				
						break;											
					}
        		}	
        		else if(ReceiveCount == (ActualLength+1)){
					
					ReceiveCount = 0;
					
					/* ETX */
					if( newByte == 0x03){

						if(GRF_dlk.pRx == NULL){
							GRF_dlk.pRx = pReceiveFrame;

							bToggle ^= 0x01;
							
							(bToggle)?	(pReceiveFrame = &p->receiveFrame1[0]) :                     
		    						 	(pReceiveFrame = &p->receiveFrame0[0]);	
						}			
					}else{
						ReceiveCount = 0;											
					}
        		}
				else{        		
					expectedCrc ^= newByte;
        		}	
				
				break;
			}			
        }		
	}
	
	tmp_cnt -= numBytesToParse;
	tmp_rd += tmp_cnt;
	if(tmp_rd >= UARTDEFS_MAX_FIFO_LEN) tmp_rd -= UARTDEFS_MAX_FIFO_LEN;
	
	sio_rx[COMx].read = tmp_rd;	

	/*copies local variables to global variables*/
	p->ReceiveCount =  ReceiveCount;
	p->ActualLength = ActualLength;
	p->timeout 		= timeout;
	
	p->bToggle 		= bToggle;
	p->pReceiveFrame= pReceiveFrame;	
	p->expectedCrc 	= expectedCrc;	
}

/*==================================================================
* Function Name  	: CheckRxCmdFrom_GRF
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
static void CheckRxCmdFrom_GRF(GRF_LINK *pDlk)
{	
	u8 *pData = &pDlk->pRx[MK_REQ_IDX_STX];
	u8 temp;

	
	/*
		1) Attn 설정지령 : N/A -> RFIDCU의 menu에서 설정함 
		2) HR, register 읽기 지령 : N/A
		3) Shutdown 지령 : 처리하지 않음 
		   (note) RFIDCU가 RFID리더기를 자체적으로 관리한다. 
		          또한 리더기 자체 진단에 재부팅 기능 있음 
		4) Tag Read Begin 지령 수신시 RFID리더기 Alive 상태면 AckCmd 회신한다.          
	*/
	
	// RSP :: STX RXID TXID Length command1 command2 option1 option2 data[n]...BCC ETX
	if((pData[MK_REQ_IDX_CMD1] == 'F') && (pData[MK_REQ_IDX_CMD1] == 'V')){		
		/* Firmware version */

		if(RFID_IsAlive())Mk_ReadFirmWareVer();
	}	
	else if((pData[MK_REQ_IDX_CMD1] == 'A') && (pData[MK_REQ_IDX_CMD1] == 'C')){		
		/* AC Request */
		
		if(RFID_IsAlive()) 	GRF_RspAckCmd();
	}
	else if((pData[MK_REQ_IDX_CMD1] == 'H') && (pData[MK_REQ_IDX_CMD1] == 'S')){		
		/* Hardware Setup * : ex) Attn 설정 */
		// ex) 02 01 FF 08 (48 53) (00) 00 01 (주소03) 00 (설정값03) EC 03 

		if(pData[MK_REQ_IDX_DATA] == 8){			
			if((pData[MK_REQ_IDX_DATA+3] == 0x03)&&	(pData[MK_REQ_IDX_DATA+4] == 0x00)){

				temp = pData[MK_REQ_IDX_DATA+5];
				if(RFID_IsAttnValueOkay(temp)){					
					
					if(RFID_IsAlive()){

						GRF_RspEndCmd('h','s');

						if(TagConf.rfidAttn != temp){
							TagConf.rfidAttn = temp;
							E2P_SaveRfidAttn();	

							if(TagConf.rfidAttn != RFID_GetRspAttn()){
								RFID_SetAttn(_reset);	
							}
						}						
					}	
				}
			}
		}
	}	
	else if((pData[MK_REQ_IDX_CMD1] == 'H') && (pData[MK_REQ_IDX_CMD1] == 'R')){		
		/* Hardware Read : ex) Register read */

		// no response !!!
	}	
	else if((pData[MK_REQ_IDX_CMD1] == 'S') && (pData[MK_REQ_IDX_CMD1] == 'D')){		
		/* ShutDown Request */
		
		GRF_RspEndCmd('s','d');

		// no action !!!
	}	
	else if((pData[MK_REQ_IDX_CMD1] == 'F') && (pData[MK_REQ_IDX_CMD1] == 'D')){		
		/* Factory Default */
		
		GRF_RspEndCmd('f','d');

		// no action !!!
	}		
	
	pDlk->pRx = NULL;
}

/*==================================================================
* Function Name  	: GRF_Process
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void GRF_Process(void)
{	
	u8	COMx = COM_GRF; 
	u32 tmp_rd, tmp_wr; 	
	GRF_LINK *pDlk = &GRF_dlk;

	if(!IS_OP(OP_MODE_HW_TEST)){
		
		/* RX */
		tmp_rd = sio_rx[COMx].read;
		tmp_wr = sio_rx[COMx].write;
			
		if(tmp_rd != tmp_wr)ParseRxFrameFrom_GRF();
			
		if(pDlk->pRx != NULL)CheckRxCmdFrom_GRF(pDlk);				
			
		/* every 2sec since the last */		
		if(GRFDEFS_AUTO_RSP_ACK()){
			
			if(CHK_TOV(pDlk->pollTimer, GRFDEFS_DEFAULT_RSP_ACK_PERIOD_TIME)){
				if(RFID_IsAlive()) 	GRF_RspAckCmd();
				else				RESET_TIMER(&pDlk->pollTimer);
			}	
		}

		// 바이패스 모드 아니면 재전송 실행 한다. 
		if(!TAG_IsBypassMode()){

			if(TagMode == TAG_MODE_AUTO_RESEND) 		TAG_AutoResend();
			else{

				if(TagMode >= TAG_MODE_TRAIN_INTERLOCK)TAG_TrainOnResend();
			}							
		}	
		
		/* TX */
		GRF_TxProcess();	
	}		
}
