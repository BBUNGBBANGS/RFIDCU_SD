#ifndef __GRF_H__
#define __GRF_H__

#include "Common.h"


/*****************************************************************************
*                                                                          
* 							LITERAL CONSTANTS(DEFINES)
*                    
******************************************************************************/
/**
	Packet Configuration
*/

#define	GRF_MAX_RX_FRAME_LEN	(256)

// (note) fv response (7 + n(max.32) + BCC + ETX) = 41byte
#define GRF_MAX_TX_FRAME_LEN	(41)

// fv max string length
#define GRF_MAX_FV_STRING_LEN	(32)

// (note) tag response (29byte)
#define GRF_RSP_AC_CMD_LEN		(29)

// ���뿪�� ��ġ �������� UID������ ���� �� 
#define GRFDEFS_HD_TAG_UID_SIZE()					(0x26)

// STEN �� ������ġ �������� UID������ ���� �� 
#define GRFDEFS_STWJ_TAG_UID_SIZE()					(0x80)


/**
	Time Configuration
*/

#define GRF_MAX_TX_LIST_SIZE		(16)

#define GRF_MIN_TX_IDLE_TIME		(5)

// (note) Apulse : 16.6ms ~ 20ms ���� @tag packet ���� : 2.5ms
#define GRF_TAG_TX_IDLE_TIME		(15)

#define GRF_MAX_TX_IDLE_TIME		(50)

#define GRF_DEFAULT_TX_IDLE_TIME	(50)


// GRF�� Ack cmd �ڵ� ���� 
#define GRFDEFS_AUTO_RSP_ACK()		(ON)

// GRF�� Ack cmd �ڵ� ���� �ֱ� 
#define GRFDEFS_DEFAULT_RSP_ACK_PERIOD_TIME		(u32)(2000)


/*****************************************************************************
*                                                                          
* 								TYPE DEFINITIONS
*                    
******************************************************************************/

typedef struct _GRF_RX_PORT
{	
 	u8 receiveFrame0[GRF_MAX_RX_FRAME_LEN];
	u8 receiveFrame1[GRF_MAX_RX_FRAME_LEN];	

	u32 ReceiveCount;
 	u32 ActualLength; 
	u32 timeout;
	
 	u8 bToggle; 
 	u8 *pReceiveFrame;

	u8	expectedCrc;	
	
}GRF_RX_PORT;

/* Packet to GRF */
typedef struct _GRFTxPacketSTR
{
    u8 data[GRF_MAX_TX_FRAME_LEN]; 
	u8 Len;
	u8 Command[2];	
	u32 tagRxNumber;
	u8	tagType;
	
}GRFTxPacketSTR;

/* Packet List to GRF */
typedef struct _GRFSendListSTR
{
	vu32 	write;
    vu32 	read;
	GRFTxPacketSTR	packet[GRF_MAX_TX_LIST_SIZE];

	bool bPrvTagSend;	/* Tag ���� ���� */
	bool bTxWaitTime;	
	
 	u32 rsTxTime;		
	
}GRFSendListSTR;


typedef struct _GRF_LINK
{	
	/* RX Variables */
	
	// UART RX LINK	
	u8 *pRx;	
	
	u32 pollTimer;
	
}GRF_LINK; 
#define GRF_LINK_LEN (sizeof(struct _GRF_LINK))


/*****************************************************************************
*                                                                          
* 								TYPE DEFINITIONS(LOCAL)
*                    
******************************************************************************/

typedef struct _GRF_PACKET_RSP
{	
	u8 Stx;
	u8 RxID;
	u8 TxID;
	u8 Len;
	u8 Command[2];
	u8 Status;	
	// 7byte
	
	u8 Data[GRF_MAX_FV_STRING_LEN+2]; /* Data[n], Bcc, ETx */

	// Tag = Total packet length 29byte 
	// F/V = Total Data length dat[32] + BCC + ETX
}GRF_PACKET_RSP; 
#define GRF_PACKET_RSP_LEN (sizeof(struct _GRF_PACKET_RSP))


/*****************************************************************************
*                                                                          
* 								PUBLIC VARIABLES
*                    
******************************************************************************/

/*****************************************************************************
*                                                                          
*							FUNCTION DEFINITIONS(EXTERN)
*                    
******************************************************************************/
void GRF_InitVal(void);
void GRF_RspAckCmd(void);
void GRF_RspTagData(TAG_TXD *pTag, u16 numTagToSend);
void GRF_RspFV(u8 *data, u16 stringLenToSend);
void GRF_Process(void);

#endif /*__GRF_H */
