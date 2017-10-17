#ifndef __REMOTE_CONTROL_H__
#define __REMOTE_CONTROL_H__


#include "remote_config.h"



#ifdef __REMOTE_CONTROL__
#include "JIStypes.h"
#include "remote_datatype.h"

#pragma pack( push, 1 )

typedef struct
{
	UINT16 programType;
	UINT16 workoutTime;
	UINT16 warmupTime;
	UINT16 cooldownTime;
	UINT8 units;
	UINT16 startSpeedX10;
	UINT16 startInclineX10;
//    UINT8 startResistance;
//    UINT16 startTarget;
    UINT8 startLevel;    
} ProgramStart;
typedef struct
{
    UINT16 rtcyear;
    UINT8 rtcmonth;
    UINT8 rtcday;
    UINT8 rtchour;
    UINT8 rtcmin;
    UINT8 rtcsecond;
    UINT8 rtctimeformat;
}
ProgramRTC;
typedef struct
{
    UINT8 status;
} ProgramPause;
typedef struct
{
    UINT16 speedX10;
    UINT8 metric;
    UINT8 appkeystatu;
} ProgramSetSpeed;
typedef struct
{
    UINT16 programtype;
    UINT16 workouttime;
    UINT16 warmuptime;
    UINT16 cooldowntime;
} ProgramChangeWorkoutTime;
typedef struct
{
    UINT16 inclineX10;
    UINT8 appkeystatu;
} ProgramSetIncline;
typedef struct
{
    UINT8 resistance;
    UINT8 appkeystatu;
} ProgramSetResistance;
#pragma pack( pop )

SubProcessState RemoteControlProcess(RemoteDevicesInformation *deviceinfor, UINT8* buf, UINT16 *bufLength, const  DapiGlobalVariable* dapiGlobalVariable);
RemoteResponse RemoteControlReceive(SubReceiveDataType datatype, ADDSTATESFUNC addstatefunc, DapiGlobalVariable* dapiGlobalVariable );
static int DeleteUser( UINT8 UserID );
void GetHeartrate( UINT8 CurHeartrate );
static int SetSpeed( UINT16 speedX10, bool metric, bool keystatu);

#endif

#endif
