#ifndef _UtilitiesErrorcode_h_
#define _UtilitiesErrorcode_h_

#include "HAL_Lib.h"
#include "UtilitiesTimer.h"
#include "UtilitiesKeyboard.h"
#include "SystemConfig.h"
#include "UtilitiesErp.h"
#include "Programs.h"
#include "UtilitiesErp.h"



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
#define MANUAL_SWITCH           26
#define DC_OVER_CURRENT         27
#define CONTROL_FREQUENCY       28
#define CONTROL_DISCONNECTION   29
#define IR_FREQUENCY            30
#define IR_DISCONNECTION        31
#define ELECTR_OVER_CURRENT     32
#define ELECTR_DISCONNECTION    33
#define FUN_ERR                 34
#define VOER_SPEED              35
#define SPEED_SENSOR            36
#define POSITION_SENCOR         37
#define COMMUNICATION           38
#define MACHINE_TYPE_ERROR      39
#define C50_RESISTANCE_TYPE     40
#define COMMAND_ERROR           41
#define OVER_SPEED              42

#define C_UTL_STATE_SIZE    20          //×´Ì¬»º´æ¸öÊý

typedef enum
{
	Safekey_on_to_off,
	Safekey_off_to_on,
	S_NO_EVENT,
    S_ERROR_EVENT,
    S_ERROR_RELEASED,
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
    ERR_EVT_NO_SPM,
	ERR_EVT_IRSENEOR,
    ERR_EVT_CONTROL_ZONE,
    ERR_EVT_UCB_EMERGENCY,
    ERR_EVT_HANDRAIL_EMERGENCY,
	ERR_PROCESSED,
	S_EVENT_TOTAL
}ERR_EVENT;


void Err_Initial_Data(void);
ERR_EVENT ERR_Process(ReturnEventHandler flag);
char* UTL_Get_Version(void);
void Err_Initial_IO(void);
VR_KEY UTL_ERR_Set_VK(VR_KEY vk);
UINT16 UTL_Get_ErrorCode(void);
bool UTL_Get_ErrorLevel(void);
void Clearg_ErrorCode();
#endif













