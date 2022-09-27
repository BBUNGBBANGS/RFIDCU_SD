#include "Common.h"

/*****************************************************************************
*                                                                          
* 							LITERAL CONSTANTS(DEFINES)
*                    
******************************************************************************/
#define RFID_ALARM_FAULT_CNT	(3)		/* RFID-READER 3회이상 무응답시 통신이상 */
#define	RFID_MAX_FRAME_LEN		(256)	/* 'HR' 지령응답 236ea */
#define RFID_TIMEOUT_RX_CHAR	(10)	/* 10ms */
#define RFID_MAX_FV_STR_LEN		(GRF_MAX_FV_STRING_LEN)

#define MK_IDX_STX 		0
#define MK_IDX_RXID		1	
#define MK_IDX_TXID		2	
#define MK_IDX_LEN 		3
#define MK_IDX_COM_C1 	4 
#define MK_IDX_COM_C2 	5 
#define MK_IDX_STATUS   6
#define MK_IDX_OPTION1  7
#define MK_IDX_OPTION2  8
#define MK_IDX_TAG_SUM  9
#define MK_IDX_USER     10


#define MK_RX_IDX_AP_ATTN	(9)
#define MK_RX_IDX_MK_ATTN	(13)

#define MK_RX_IDX_MK_FV		(7)
#define MK_RX_IDX_AP_FV		(9)

/* We don't use the table below ,
if we want to use this, then we shoud change rf attn to 4db and search time to 10*/
const u8 Mk_INIT_TBL[] = 
{	
	0x14,	/* SUM */
		
	0x01, 0x00, 0x01, /* TALK MODE */
	0x03, 0x00, 0x03, /* RF ATTN  :: min(3)*/
	0x05, 0x00, 0x01, /* ANTENNA :: 1*/
	
	/* Old Frequency */
//	0x06, 0x6E, 0x28, /* START FREQ. :: 910.400Mhz  */
//	0x07, 0x71, 0x3C, /* STOP   FREQ. :: 913.600Mhz  */

	/* New Frequency */
	0x06, 0x75, 0x1E, /* START FREQ. :: 917.300Mhz	*/
	0x07, 0x78, 0x1E, /* STOP	FREQ. :: 920.300Mhz  */

	0x0A, 0x00, 0x50, /* TAG PROTOCOL TYPE :: GEN2 */
	0x0C, 0x00, 0x04, /* Search Time(4) :: send frame after 4times scan*/
	0x10, 0x00, 0x01, /* Max Antenna :: 1EA */
	0x13, 0x00, 0x80, /* Tag Send Size :: *16Byte(80) , Variables(0x020) */
	0x1D, 0x00, 0x02, /* Time Slot(Q Value) 2 */
	0x2A, 0x00, 0x01, /* Memory Bank */
	0x38, 0x00, 0x00, /* Gen2 Tag EPC PC Value */
	0x3A, 0x00, 0x28, /* R/W Mem Retry */
	0x3F, 0x00, 0x00, /* LBT Reset Time */
	0x42, 0x00, 0x01, /* AC No Tag Response */
	0x80, 0x00, 0x00, /* External Output */
	0x81, 0x00, 0x00, /* External Input */
	0x82, 0x00, 0x00, /*  */
	0x83, 0x00, 0x00, /*  */
	0x84, 0x00, 0x00  /*  */
}; 


/*****************************************************************************
*                                                                          
* 								PUBLIC VARIABLES
*                    
******************************************************************************/

TAG_CONF TagConf;

// RFIDCU --> GRF 로 송신한 최종 tag value 
TAG_TXD tagGrfOld;
TAG_TXD tagGrfBak;

TAG_TXD tagTestOld;

u32 myTagRxNumber;

// RFID-READER 상태 제어 
RFID_LINK		RFID_dlk;	

bool bUseTag99testmode;
bool bAutoTagTestRun;
bool bUseRSSI;
bool bTagWithRssi;
u16  tagRssiValue;

/*****************************************************************************
*                                                                          
* 								LOCAL VARIABLES
*                    
******************************************************************************/

// tx packet structure to rfide-reader
static RFID_TX_PACKET	MkTxPacket;	

// rx packet handler, from rfid-reader
static RFID_RX_PORT 	RFID_Rx;


/**
	Tag data handling variables
*/




/*****************************************************************************
*                                                                          
* 								FUNCTION DEFINITIONS(LOCAL)
*                    
******************************************************************************/		
static bool RFID_ChkRxCmd(RFID_LINK *pDlk);


/*==================================================================
* Function Name  	: RFID_InitVal
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void RFID_InitVal(void)
{		

}


/*==================================================================
* Function Name  	: Mk_write
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void Mk_write(RFID_TX_PACKET *pMk)
{

}

// STX RXID TXID Length command1 command2 option1 option2 data[n]...BCC ETX


/*==================================================================
* Function Name  	: Mk_ReadFirmWareVer
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void Mk_ReadFirmWareVer(void)
{	

}


/*==================================================================
* Function Name  	: Mk_AttnSetup
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void Mk_AttnSetup(void)
{	

}


/*==================================================================
* Function Name  	: Mk_BeginReadTag
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void Mk_BeginReadTag(void)
{	

}


/*==================================================================
* Function Name  	: Mk_HardwareRead
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void Mk_HardwareRead(void)
{	

}

/*==================================================================
* Function Name  	: Mk_FinishReadTag
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void Mk_FinishReadTag(void)
{

}

/*==================================================================
* Function Name  	: Mk_SystemReset
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void Mk_SystemReset(void)
{

}


/*==================================================================
* Function Name  	: Mk_Close
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void Mk_Close(void)
{			

}

/*==================================================================
* Function Name  	: RFID_SET_POWER
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void RFID_SET_POWER(u8 state)
{	

}


/*==================================================================
* Function Name  	: RFID_GetRspAttn
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
u8 RFID_GetRspAttn(void)
{

}

/*==================================================================
* Function Name  	: RFID_IsAttnValueOkay
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
bool RFID_IsAttnValueOkay(u8 attn)
{

}

/*==================================================================
* Function Name  	: RFID_ParseRxFrame
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
static void RFID_ParseRxFrame(void)
{

}


/*==================================================================
* Function Name  	: RFID_Reset
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void RFID_Reset(RFID_LINK *pDlk)
{

}


/*==================================================================
* Function Name  	: RFID_Init
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
bool RFID_Init(void)
{	

}

/*==================================================================
* Function Name  	: RFID_ChkTagInfo
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
bool RFID_ChkTagInfo(u8 *data)
{

}

/*==================================================================
* Function Name  	: RFID_tagTest
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void RFID_tagTest(u8 *pRcvTag)
{

}

/*==================================================================
* Function Name  	: RFID_ChkRxCmd
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
static bool RFID_ChkRxCmd(RFID_LINK *pDlk)
{	

}


/*==================================================================
* Function Name  	: RFID_IsAlive
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
bool RFID_IsAlive(void)
{

}


/*==================================================================
* Function Name  	: RFID_IsAlive
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
bool RFID_IsRun(void)
{

}

/*==================================================================
* Function Name  	: RFID_SetAttn
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void RFID_SetAttn(u8 command)
{

}


/*==================================================================
* Function Name  	: RFID_ClearTagGrfOld
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void RFID_ClearTagGrfOld(void)
{
}


/*==================================================================
* Function Name  	: RFID_ClearTagGrfOld
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void RFID_ClearTagTestOld(void)
{

}

/*==================================================================
* Function Name  	: RFID_Process
* Description    	: 
* Input          	: 
* Output         	: 
* Return         	: 
====================================================================*/
void RFID_Process(void)
{	

}
