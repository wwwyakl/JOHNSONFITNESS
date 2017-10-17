#ifndef _UtilitiesErrorcode_h_
#define _UtilitiesErrorcode_h_

#include "HAL_Lib.h"
#include "UtilitiesTimer.h"
#include "UtilitiesKeyboard.h"
#include "SystemConfig.h"

#define SAFEKEY				1
#define NO_INCODE			2
#define MOS_FAULT			3
#define SPEED_FAST		    4
#define SPEED_LOW			5
#define OVER_CURRENT		6
#define INC_NOCOUNT		    7
#define MCB_TEMP_OVER		8
#define DIGTIAL_TIMEOUT	    9
#define DIGTIAL_RX_ERROR	10
#define FALSH_WRITE		    11
#define FALSH_READ		    12		
#define CURRENT_OVER		13
#define TMEP_HIGH			14
#define VOLTAGE_LOW	 		15
#define INCLINE_NO_COUNT	16
#define ECB_NO_COUNT	 	17
#define ECB_ZERO_OEPN		18
#define CONNECT_FAIL		19
#define NO_RPM				20
#define RX_TX_ERROR			21
#define RESISTANCE_OPEN		22
#define NVFLASH_R	 		23
#define NVFLASH_W			24
#define TEMP_ERR			25

#define C_UTL_STATE_SIZE    20          //×´Ì¬»º´æ¸öÊý

typedef enum
{
	Safekey_on_to_off=0x0,
	Safekey_off_to_on,
	S_NO_EVENT,
	S_NO_INCODE , 
	S_MOS_FAULT,
	S_SPEED_FAST,
	S_SPEED_LOW,
	S_OVER_CURRENT,
	S_INC_NOCOUNT,
	S_MCB_TEMP_OVER,
	S_DIGTIAL_TIMEOUT,
	S_DIGTIAL_RX_ERROR,
	S_FALSH_WRITE,
	S_FALSH_READ,
	S_EVENT_TOTAL
}ERR_EVENT;


void Err_Initial_Data(void);
ERR_EVENT ERR_Process(UINT16 vk);
void Err_Regist_Functions(void);
char* UTL_Get_Version(void);
UINT8 UTL_GetErrState(void);



#endif













