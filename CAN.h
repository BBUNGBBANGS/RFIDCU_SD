#ifndef __CAN_H__
#define __CAN_H__

#include "Common.h"

/*****************************************************************************
*                                                                          
* 							LITERAL CONSTANTS(DEFINES)
*                    
******************************************************************************/

/** 장치 ID */
#define CAN_ID_GND_MASTER        0x00
#define CAN_ID_RF4U_F            0x01
#define CAN_ID_RF4U_R            0x02
#define CAN_ID_LED_F             0x03
#define CAN_ID_LED_R             0x04 
#define CAN_ID_LBCU	      	  	 0x05
#define CAN_ID_RF4U_C            0x06
#define CAN_ID_RFIDCU            0x08


#define CAN_ID_BROADCAST         0xFF       // CAN ID : Broadcasting

/** ID Filter Configureation */
#define CAN_FilterID0_RF4U_F	  ((u32)0x02000107)
#define CAN_FilterID1_RF4U_F	  ((u32)0x0200FF07)
#define CAN_FilterMask_RF4U_F	  ((u32)0x02FFFFFF)		
	
#define CAN_FilterID0_RF4U_R	  ((u32)0x04000207)	
#define CAN_FilterID1_RF4U_R	  ((u32)0x0400FF07)
#define CAN_FilterMask_RF4U_R	  ((u32)0x04FFFFFF)	

/** ID MASKING */
#define CAN_ID_SRC_MASK          (0xFF<<16)  // CAN ID : Source ID Mask
#define CAN_ID_DST_MASK          (0xFF<<8)  // CAN ID : Destination ID Mask

/** GROUP MASKING */
#define GRP_GRF         		1		// 24
#define GRP_RF4U_F         		2		// 25
#define GRP_RF4U_R         		4		// 26

#define GRP_RFIDCU				4

#define GRP_LED_F         		8		// 27
#define GRP_LED_R         		16		// 28

#define CAN_GROUP_MASK_GRF_F	 (1<<25)	
#define CAN_GROUP_MASK_GRF_R	 (1<<26)

#define CAN_ID_MASK_RFGND		 (1<<24)	// PSDC-1007 Group Masking
#define CAN_ID_MASK_RF4U_F		 (1<<25)	// RF4U FRONT Group Masking
#define CAN_ID_MASK_RF4U_R		 (1<<26)	// RF4U REAR Group Masking
#define CAN_ID_MASK_LED_F		 (1<<27)	// LED FRONT Group Masking
#define CAN_ID_MASK_LED_R		 (1<<28)	// LED REAR Group Masking


/*----------------------------------------------------------------------------
 COMMAND
 ----------------------------------------------------------------------------*/

#define CAN1_CMD_LED_DISPALY_INFO	0x03
#define CAN1_CMD_LED_STATUS			0x04
#define CAN1_CMD_LBCU_STATUS		0x06
#define CAN1_CMD_RF_REQ_CONTROL		0x07
#define CAN1_CMD_RF_STATUS			0x08

#define CAN1_CMD_RFID_RAW_DATA_AFTER	0x0A
#define CAN1_CMD_RFID_RAW_DATA_BEFORE	0x0B

#define CAN1_CMD_TEMPERATURE_STATUS		0x0C

#define CMD_REQ_PROGRAM_VER		0xA4		/**< 프로그램 버젼 요청 */
#define RSP_REQ_PROGRAM_VER		0xF3		/**< 프로그램 버젼 요청  응답 */


/*****************************************************************************
*                                                                          
* 							TYPE DEFINITIONS
*                    
******************************************************************************/


/*==================================================================
* Structure Name 	: CANComStateSTR
* Description    	: The status of CAN port
====================================================================*/
typedef struct _CANComStateSTR
{
	bool 	bTxWaitTime;
    u32 	timeout_can_alarm;
	
    u8 rsPollMode;
    u32 rsTxTime;	
}CANComStateSTR;

/*==================================================================
* Structure Name 	: CANPacketSTR
* Description    	: The structure of CAN packet
====================================================================*/
typedef struct _CANPacketSTR
{
    u8 data[8];
    u32 address;
	
}CANPacketSTR;

/*==================================================================
* Structure Name 	: CANSendListSTR
* Description    	: The List of CAN messages for transmission
====================================================================*/
/* Only For RX */
typedef struct _CANReceiveListSTR
{
	vu32 	write;
    vu32 	read;
	CANPacketSTR	packet[CAN_MAX_LIST_SIZE];
	
}CANReceiveListSTR;


typedef struct _CANSendListSTR
{
	vu32 	write;
    vu32 	read;
	CANPacketSTR	packet[CAN_MAX_TX_LIST_SIZE];
	
}CANSendListSTR;


/*==================================================================
* Structure Name 	: CAN_PACKET
* Description    	: 
====================================================================*/
/**
통신 송수신용 CAN Packet 구조체
*/
typedef struct {
    u8 nGroup;         /**< Group Masking */
    u8 nSender;        /**< Sender ID */
    u8 nReceiver;      /**< Receiver ID */
    u8 nCmd;           /**< Command */
    u8 nParam[8];      /**< Parameter (8bytes) */
} ECAN_PACKET;


typedef struct { 
    u8 nCmd       :8;     /**< Command */     
    u8 nReceiver  :8;     /**< Receiver ID */
    u8 nSender    :8;     /**< Sender ID */
    u8 nGroup     :5;     /**< Group Masking */
    u8 AAM        :1;     /**< Auto Answer Mode */
    u8 AME        :1;     /**< Acceptance Mask Enable bit */
    u8 IDE        :1;     /**< Identifier Extension bit */
} S_ECAN_MSGID;


/**
(Union) ECAN Packet Message ID 구조체
*/
typedef union { 
	S_ECAN_MSGID	info; 
	u32				all;
} U_ECAN_MSGID;

typedef struct _S_ECAN_DATA_LINK
{	
	/* check connection 주/예비 별도 */
//	u32 Link_checktimer[2];
//	u8 	LinkDown[2];	

	/* 주/예비 통합 */
//	u32 	Link_checktimer;

	u8	LinkCheckFlag;	/* n초마다 1, 수신시 0로 설정함. */	
	u8 	LinkDown;		/* 수신시 0, n초 동안 데이터 없으면 1로 설정됨 */


	/* check new message */
	u32 timeout_can_rx;
	u8 	bIsRx[2];
	u8  tmp_bak[8];
	u8  command;	

	//u8  sender;
	u8  receiver;	
}S_ECAN_DATA_LINK;	


typedef struct _S_ECAN_GR_INFO
{	
	S_ECAN_DATA_LINK GRF_dlk;		
	
}S_ECAN_GR_INFO;	


/*****************************************************************************
*                                                                          
* 								PUBLIC VARIABLES
*                    
******************************************************************************/
/* CAN 0 and 1 */
extern S_ECAN_GR_INFO 		CAN_dlk;

extern CANSendListSTR 		CAN_tx[CANn];
extern CANReceiveListSTR	CAN_rx[CANn];

/*****************************************************************************
*                                                                          
*							FUNCTION DEFINITIONS(EXTERN)
*                    
******************************************************************************/
void CAN_InitVal(void);
bool CAN_IsBusOff(u8 CANx);
void CAN_AddTxMessage(u8 CANx, const ECAN_PACKET *pPacket);
void CAN_Send_RFIDCU_Status(void);
void CAN_RFIDCU_TestResult(void);
u8 CAN_TxProcess(u8 CANx);
bool CAN_Process(u8 CANx);
#endif /*__CAN_H */
