#ifndef __RFID_H__
#define __RFID_H__

#include "Common.h"


/*****************************************************************************
*                                                                          
* 							LITERAL CONSTANTS(DEFINES)
*                    
******************************************************************************/
#define RFID_MAX_TAG_LIST_SIZE		(8)
#define	RFID_MAX_RX_FRAME_LEN		(256)

#define RFID_MAX_ATTN_VALUE			(30)
#define RFID_MIN_ATTN_VALUE			(3)
#define RFID_AP_MIN_ATTN_VALUE		(21)

#define RFID_TAG_PACKET_SIZE		(29)

// RSSI������ ���Ե� Packet ������ 
#define RFID_RSSI_TAG_PACKET_SIZE	(31)

// (note) -60dBm ���ϴ� ���� ���� 
#define RFID_RSSI_DATA_MIN			(u16)(64536)	// -99.9dBm	
#define RFID_RSSI_DATA_MAX			(u16)(65535)	// 0dBm

#define __MK__		(0)		/* Minerva MKUM-300 */
#define __UB__		(1)		/* Ubist U9-2100 */
#define __AP__		(2)		/* Apulse A311 */


//#define __RFID_VER__	__MK__	/* Minerva */
//#define __RFID_VER__	__UB__	/* UBIST */
#define __RFID_VER__	__AP__


#if (__RFID_VER__ == __UB__)	/* UBIST */
	#define RFID_ALARM_FAULT_REBOOT_CNT		30	// UBIST(60SEC)
	#define RFID_COMMAND_SD_MODE			0x01
	#define RFID_MAX_BOOT_CNT				600		/* 60SEC */
#elseif(__RFID_VER__ == __MK__)/* MINERVA */
	#define RFID_ALARM_FAULT_REBOOT_CNT		5	// MINERVA(8SEC)
	#define RFID_COMMAND_SD_MODE			0x02
	#define RFID_MAX_BOOT_CNT				100		/* 10SEC */
#else /* __AP__ */
	#define RFID_ALARM_FAULT_REBOOT_CNT		5	// MINERVA(8SEC)
	#define RFID_COMMAND_SD_MODE			0x02
	#define RFID_MAX_BOOT_CNT				100		/* 10SEC */
#endif	


#define RFID_ALARM_FAULT_CNT	(3)		/* RFID-READER 3ȸ�̻� ������� ����̻� */
#define	RFID_MAX_FRAME_LEN		(256)	/* 'HR' �������� 236ea */
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
#define MK_IDX_30     	10
#define MK_IDX_00     	11
#define MK_IDX_05     	12
#define MK_IDX_64     	13

#define MK_IDX_BCC		27
#define MK_IDX_ETX		28


// RSSI tag data
#define MK_IDX_RSSI_LOW		27
#define MK_IDX_RSSI_HIGH	28
#define MK_IDX_RSSI_BCC		29
#define MK_IDX_RSSI_ETX		30


#define MK_RX_IDX_AP_ATTN	(9)
#define MK_RX_IDX_MK_ATTN	(13)

#define MK_RX_IDX_MK_FV		(7)
#define MK_RX_IDX_AP_FV		(9)

/*****************************************************************************
*                                                                          
* 								TYPE DEFINITIONS(LOCAL)
*                    
******************************************************************************/

/**
	TAGCU RX port handler from RFID-READER
*/
typedef struct _RFID_RX_PORT
{	
 	u8 receiveFrame0[RFID_MAX_RX_FRAME_LEN];
	u8 receiveFrame1[RFID_MAX_RX_FRAME_LEN];	

	u32 ReceiveCount;
 	u32 ActualLength; 
	u32 timeout;
	
 	u8 bToggle; 
 	u8 *pReceiveFrame;

	u8	expectedCrc;	
}RFID_RX_PORT;
#define RFID_RX_PORT_LEN (sizeof(struct _RFID_RX_PORT))

/**
	TAGCU to RFID TX PACKET STRUCTURE
*/
typedef struct _RFID_TX_PACKET
{	
	u8 Stx;
	u8 RxID;
	u8 TxID;
	u8 Len;
	u8 Command[2];
	u8 Option1;
	u8 Option2;
	u8 Data[255];
//	u8 Bcc;
//	u8 Etx;
	
}RFID_TX_PACKET; 
#define RFID_TX_PACKET_LEN (sizeof(struct _RFID_TX_PACKET))

/**
	TAGCU and RFID Communication Link Sataus
*/
typedef struct _RFID_LINK
{	
	u8 *pRx;	// UART RX LINK

	StateType NextState;
	
	u8 cntNoReply;	/* none response counter */
	u8 cntReply;	/* noraml reply counter */
	
	u8 rsp_code[2];
	u8 rspAttn;
	u8 bResponse;	

	bool bMkComLink;	/* rfid-reader communication link */	
	bool bMkLinkAlarm;
	bool bMkAttn;

	u32 systemTimeTag;	
}RFID_LINK; 
#define RFID_LINK_LEN (sizeof(struct _RFID_LINK))


/** 
	Tag Info. (Send To GRF, record to memory) 
*/
typedef struct _TAG_DATA
{	
	// Tag Data 16 Byte
	u8 PC[2];
	u8 Sync[2];	
	u8 SN[2];
	u8 RouteNo;
	u8 CarNo;
	u8 OrgNo;
	u8 CarType;
	u8 CarCount;
	u8 Maker;
	u8 Reserved[2];

	//u8 crc[2];
	u8 TagConvCode;
	u8 TagConvData;	

	
	// �±� ���� Ƚ�� 
	u16 cntTagSum;
	
	// ó�� ������ �ð� 
	u32 fisrtReadTimeTag;		
	
	// ������ ������ �ð� -> 1�� 30�� ����� ���� 
	u32 lastReadTimeTag;			
	
	// ��ȿ�±� Ȯ�� 
	bool bCnfmReady;	

	// �ҷ��±� Ȯ�� 
	bool bCnfmNotMyTag;

	// ��Ż�±� Ȯ�� 
	bool bCnfmMetalTag;		
}TAG_DATA; 
#define TAG_DATA_LEN (sizeof(struct _TAG_DATA))

/** 
	Tag Read Buffer
*/
typedef struct _TAG_BUF
{
	vu32 		write;
    vu32 		read;	

	TAG_DATA	packet[RFID_MAX_TAG_LIST_SIZE];	
	
}TAG_BUF;
#define TAG_BUF_LEN (sizeof(struct _TAG_BUF))


/**
	RFID-TAG Notify
*/
typedef struct _TAG_TXD
{	
	// Tag Data 16 Byte
	u8 PC[2];
	u8 Sync[2];	
	u8 SN[2];
	u8 RouteNo;
	u8 CarNo;
	u8 OrgNo;
	u8 CarType;
	u8 CarCount;
	u8 Maker;
	u8 Reserved[2];	
	u8 TagConvCode;// crc[0]
	u8 TagConvData;// crc[1]	

	// dummy data
	u8 tagSum;
	u8 rssi_low;
	u8 rssi_high;	
	// u8 tagBCC
	// u8 tagETX
	
	// tag���� �����ϴ� �ð� 
	bool bTagOnTime;	
	u32	 tagOnTimeout;	

	// �ű� ID1, ID2 ���Ž� rx number ���� 
	u32 tagFirstRxNumber;

	// ���� ID1, ID2 ���Ž� rx number ���� 
	u32 tagLastRxNumber;
		
	// �±� ���� Ƚ�� 
	u16 cntTagSum;	

	// �ٸ��±� ���� count (1st glass, 1st metal, 2nd metal, 2nd glass)
	u16 cntTagDiffPos;

	// �±� ���Ž� ���� ���� ��ġ 
	TRAIN_POSITION tagRxPos;	
	
	// ó�� ������ �ð� 
	u32 firstReadTimeTag;
	
	// ������ ������ �ð� 
	u32 lastReadTimeTag;	

	// ��Ż�±� ����, ��ȿ�±� Ȯ�� 
	bool bCnfmMetalTag;	

	// train_none ���� ������ n�� ���� train_enter ��Ȯ�� 
	bool bNotMytag;

	bool bTrnNoneMark;	

	/** 
		TX Variables
	*/

	// 1st glass tag ��������� �������� �ʰ�,  �������� tag ��Ȯ���� �۽��Ѵ�. 
	bool bNextTagCheckSend;
	
	// �±� �۽� Ƚ�� 
	u32 cntTxtagSum;

	// �±� ������ ��� ������ ī��Ʈ, 50ms�̳� �±� ����Ž�  
	u32 firstResendCount;

	// 1���� ������ 
	u32 DelayResendCount;
	
	u32 tagResendTimeout;
	
}TAG_TXD; 
#define TAG_TXD_LEN (sizeof(struct _TAG_TXD))

/**
	TAG Config (16byte) read/write from EEPROM
*/
typedef struct _TAG_CONF
{	
	u8 romTagConf;
	u8 tagTest;	
	u8 tagBypass;
	u8 rfidAttn;
	
	u8 rfidBeep;
	u8 ApulsePowerMode; /* Apulse Power Setting 1~10 -> 21~30 */
	u8 trnOnCfrm;		/* ���� on  Ȯ�� 0.1s default 1.0 */
	u8 trnOffCfrm; 		/* ���� off Ȯ�� 0.1s default 0.6 */

	u8 resByte[8];
	
}TAG_CONF; 
#define TAG_CONF_LEN (sizeof(struct _TAG_CONF))

/*****************************************************************************
*                                                                          
* 								PUBLIC VARIABLES
*                    
******************************************************************************/
extern RFID_LINK RFID_dlk;	

extern bool bUseTag99testmode;
extern bool bAutoTagTestRun;

extern bool bUseRSSI;
extern bool bTagWithRssi;
extern u16  tagRssiValue;


extern TAG_CONF TagConf;
extern TAG_TXD 	tagGrfOld;
extern TAG_TXD tagGrfBak;

extern TAG_TXD 	tagTestOld;
extern u32 myTagRxNumber;


/*****************************************************************************
*                                                                          
*							FUNCTION DEFINITIONS(EXTERN)
*                    
******************************************************************************/
void RFID_SET_POWER(u8 state);
bool RFID_IsAttnValueOkay(u8 attn);
u8 RFID_GetRspAttn(void);

void Mk_BeginReadTag(void);
void Mk_ReadFirmWareVer(void);
void RFID_InitVal(void);
void RFID_Reset(RFID_LINK *pDlk);
bool RFID_Init(void);
bool RFID_ChkTagInfo(u8 *data);
bool RFID_IsAlive(void);
bool RFID_IsRun(void);
void RFID_SetAttn(u8 command);
void RFID_ClearTagGrfOld(void);
void RFID_ClearTagTestOld(void);
void RFID_Process(void);
void RFID_tagTest(u8 *pRcvTag);

#endif /*__RFID_H */
