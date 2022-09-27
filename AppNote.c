/*****************************************************************************
* (C) COPYRIGHT 2022 GREAT RAIL

******************************************************************************
* Description 	: RFIDCU
* MODEL 		: RFIDCU (SG-22601), PCB P/N - PSD-RFIDCU-MAIN-A0
*
******************************************************************************
REVISION HISTORY
******************************************************************************
	NEW HISTORY (최근 Revision을 위에 기록한다.)
******************************************************************************	

	-. Original File : 1.5.6
	-. Compiler Ver. : IAR v7.7	
	-. CPU : Mainstream Connectivity line, 
	 	     Arm Cortex-M3 MCU with 256 Kbytes of Flash memory, 72 MHz CPU, 
	     	 CAN, USB 2.0 OTG
	     	 
	-. Apulse boot time 3~4sec
------------------------------------------------------------------------------
	
------------------------------------------------------------------------------

	- 	Woojin LCU제어기의 Log기능 추가된 프로그램은 
		cpu_rtc.c 의 RTCClockOutput_Enable 관련 부분 삭제해야함 
		PC.13 은 Reset Strobe 출려임 , 
		RTCClockOutput_Enable 설정시 Program halt condition에서  
		RTC Clock/64 is output 됨으로 DS1302 watchdog 의 reset 출력하지 않음 
	
	- 	MRTC_Configuration()
		FLASH_Unlock();		적용 확인 

	- 	RTC_IRQHandler() 에서 
		RTC_SetCounter(0x1)을 RTC_SetCounter(0x0)로 변경함  	
		
------------------------------------------------------------------------------
	Function Description
------------------------------------------------------------------------------

	1. MODEM1, MODEM2 
	1) Reserved 기능 - H/W 기능만 시험토록 한다. 

	2. CAN통신 연동기능 
	1) Reserved 기능 - H/W 기능만 시험토록 한다. 
	2) 우진/STEN은 CAN통신 연동 하지 않음 
	3) 현대/삼중 등 차후 필요시 적용토록 함 
------------------------------------------------------------------------------
	nn-JUNE-2022 Great Rail #. Released Ver.1.0.x
------------------------------------------------------------------------------ 

------------------------------------------------------------------------------
	02-AUG-2022 Great Rail #. Released Ver.1.1.2
------------------------------------------------------------------------------ 
	- tag99 : auto test mode
	- IsUseCANInterlock() 적용 함 
	
------------------------------------------------------------------------------
	27-JUL-2022 Great Rail #. Released Ver.1.1.1
------------------------------------------------------------------------------ 

	-. test mode에서는 통신이상시 rfid-reader reboot 실행하지 않음 
	
------------------------------------------------------------------------------
	26-JUL-2022 Great Rail #. Released Ver.1.1.0
------------------------------------------------------------------------------ 
	< 개봉역급행 2022-07-26 새벽 설치 및 사용개시 >

------------------------------------------------------------------------------
	22-JUL-2022 Great Rail #. Released Ver.1.0.9
------------------------------------------------------------------------------ 
	< 구일역 2022-07-23 새벽 설치 및 사용개시 >
	- 구일역 하선 (인턴방향 ) ID=01, Bypass and Auto resend mode , 반대편 트랙 없음
	- 구일역 상선 (서울방향 ) ID=02, Train on mode

    (note) 설치후, 토요일 (7/23 ~ 7/24) 양일간 비왔음 
    
------------------------------------------------------------------------------
	18-JUL-2022 Great Rail #. Released Ver.1.0.8
------------------------------------------------------------------------------ 
	- COM1, COM2 에 디버그 출력 추가함 

------------------------------------------------------------------------------
	15-JUL-2022 Great Rail #. Released Ver.1.0.7
------------------------------------------------------------------------------ 
	-

------------------------------------------------------------------------------
	15-JUL-2022 Great Rail #. Released Ver.1.0.6
------------------------------------------------------------------------------ 
	- Apulse Power 모드 설정기능 추가함 


------------------------------------------------------------------------------
	14-JUL-2022 Great Rail #. Released Ver.1.0.5
------------------------------------------------------------------------------ 
	- GRF 변수 초기화 버그 수정함 

------------------------------------------------------------------------------
	10-JUL-2022 Great Rail #. Released Ver.1.0.4
------------------------------------------------------------------------------ 
	-. 열차진입 / 정차 / Unknown / 대기 검지 모드 추가초안 작성중 .... 	

------------------------------------------------------------------------------
	09-JUL-2022 Great Rail #. Released Ver.1.0.3
------------------------------------------------------------------------------ 
	-. GRF 지령 응답 기능 구현 

------------------------------------------------------------------------------
	09-JUL-2022 Great Rail #. Released Ver.1.0.2
------------------------------------------------------------------------------ 
	-. 센서검지시 Tag 전송 기능 구현

------------------------------------------------------------------------------
	06-JUL-2022 Great Rail #. Released Ver.1.0.1
------------------------------------------------------------------------------ 
	-. Tag bypass 전송기능 구현	

------------------------------------------------------------------------------
	08-JUNE-2022 Great Rail #. Released Ver.1.0.0
------------------------------------------------------------------------------ 
	-. Initial file creation   

******************************************************************************/
