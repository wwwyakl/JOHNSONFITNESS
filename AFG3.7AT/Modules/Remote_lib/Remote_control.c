#include "remote_control.h"
#include "SystemConfig.h"
#include "page.h"
#include "UtilitiesRtc.h"
#ifdef __REMOTE_CONTROL__
#include "Bluetooth_lib.h"
#include "wlt2564_command.h"
#include "Remote_process.h"
#include "Remote_process.h"
#include "Remote_datatype.h"
#include "remote_port.h"
#include "crc.h"
extern void ERP_Clear_time(void);
__no_init UINT8 appStart;    //zsf
__no_init UINT16 appProId;    //zsf

static int ChangeWorkoutTime( ProgramChangeWorkoutTime* worktime )
{
    UINT16 WorkoutTime;
    UINT16 WarmUpTime;
    UINT16 CoolDownTime;
    UINT16 TotalTime;
    WorkoutTime = worktime->workouttime;
    WarmUpTime = worktime->warmuptime;
    CoolDownTime = worktime->cooldowntime;
    TotalTime = WorkoutTime + WarmUpTime + CoolDownTime;
    Data_Set_Time(TotalTime);
    return 0;
}
static int StartWorkout( ProgramStart* program )
{
    if((( Program_Get_Status()<PS_RESTART_321)&&(nCurrentPage != PAGE_ERP_COUNTDOWN) && (nCurrentPage != PAGE_ENG0_0_DisplayTest))||(Program_Get_Status()==PS_PAUSED))  //zsf
    {
        UINT16 inclineX10,speedX10;
        UINT16 totalTime = program->cooldownTime+program->warmupTime+program->workoutTime;
//        UINT8  resistance = program->startResistance;
        UINT8 Units = program->units;
        
        appStart = 1;    //zsf
        appProId = program->programType;    //zsf
        if (appProId == 11)    //zsf
        {
            Program_Select_ID(PS_DISTANCE_GOAL);
            Data_Set_Distance(TARGET_5KM);
        }
        else
        {
            Program_Select_ID(PS_MANUAL);    //zsf
        }
        
        Data_Set_Level(program->startLevel);
        inclineX10 = program->startInclineX10;
        
        Program_Set_Parameter(VK_SET_WORKOUT_TIME, &totalTime);
        Program_Set_Parameter(VK_SET_INCLINE , &inclineX10  );
        if(Program_Get_ProgramID() == PS_MYFIRST5K)
        {
            UINT16 MyFirst5k_Speed;
//            MyFirst5k_Speed = program->startSpeedX10+(program->startTarget<<8);
            Program_Set_Parameter(VK_SET_MYFIRST5K_SPEED, &MyFirst5k_Speed);
        }
        else
        {
            if(Program_Get_ProgramID() == PS_DISTANCE_GOAL)
            {
//                UINT16 totalDistance = program->startTarget;
//                Program_Set_Parameter(VK_SET_WORKOUT_DISTANCE, &totalDistance);
            }
            else if(Program_Get_ProgramID() == PS_CALORIES_GOAL)
            {
//                UINT16 totalCalories = program->startTarget;
//                Program_Set_Parameter(VK_SET_WORKOUT_CALORIES, &totalCalories);
            }
            else if(Program_Get_ProgramID() == PS_THR_ZONE)
            {
//                UINT16 TargetHR = program->startTarget;
//                Program_Set_Parameter(VK_SET_WORKOUT_TARGETHR, &TargetHR);
            }
            speedX10 = program->startSpeedX10;    
            Program_Set_Parameter(VK_SET_SPEED , &speedX10 );
        }
//        Program_Set_Parameter(VK_SET_RESISTANCE, &resistance);
        
        if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
        {
            if(Program_Set_VirtualKey((UINT16)VK_START|FRAME_STAGE_USER))
                return 1;
            else
            {
                nCurrentPage = PAGE_WORK_TM1;
                appStart = 1;    //zsf
                return 0;
            }
        }
        else
        {
            if(Program_Set_VirtualKey((UINT16)VK_START|FRAME_STAGE_USER))
                return 1;
            else
            {
                nCurrentPage = PAGE_WORK_BK1;
                appStart = 1;    //zsf
                return 0;
            }
        }
    }
    return 1;
}
static UINT16 SelectProgram(PROGRAM_ID programID)
{
    if(Program_Get_Status() >= PS_WARMUP && Program_Get_Status() <= PS_COOLDOWN)
        return 1;
    Program_Set_VirtualKey((UINT16)VK_UPDATE|FRAME_STAGE_PRGSETUP);
    Data_Set_SpeedOffset(0);
    Data_Set_InclineOffset(0);
    Data_Set_ResistanceOffset(0);
    if(nCurrentPage != PAGE_ERP_COUNTDOWN)
        Program_Select_ID(programID);
    ERP_Clear_time();
    Event_Clr_ChangeDisplayFlag();    
    return 0;
}
static int PauseWorkout(UINT8 status)
{
    if(status==0)
    {
        Program_Set_VirtualKey((UINT16)VK_PAUSE|FRAME_STAGE_PRGRUN);
        Beep_Set_Beeps(1,3,1);
        if(Program_Get_Status() >= PS_WARMUP && Program_Get_Status() <= PS_COOLDOWN)
            return 0;
        else
            return 2;
    }
    else
    {
        if(Program_Set_VirtualKey((UINT16)VK_START|FRAME_STAGE_PRGRUN))
            return 3;
        else
            return 0;
    }
}

static int SetSpeed( UINT16 speedX10, bool metric, bool keystatu)
{
    if(Program_Get_Status()==PS_RUN  || (Program_Get_Status()==PS_WARMUP) || (Program_Get_Status()==PS_COOLDOWN))    //zsf
    {    
        if(0 == keystatu)
        {
            Program_Set_Parameter(VK_SET_SPEED , &speedX10  );
        }
        else
        {
            Program_Set_Parameter(VK_LONG_SET_SPEED , &speedX10  );
        }
    }
    return 0;
}

static int SetIncline( UINT16 inclineX10,bool keystatu )
{
    if(Program_Get_Status()==PS_RUN  || (Program_Get_Status()==PS_WARMUP) || (Program_Get_Status()==PS_COOLDOWN))    //zsf
    {
        if(0 == keystatu)
        {
            Program_Set_Parameter(VK_SET_INCLINE , &inclineX10 );
        }
        else
        {
            Program_Set_Parameter(VK_LONG_SET_INCLINE , &inclineX10 );
        }
    }
    return 0;
}

static int SetResistance( UINT8 resistance,bool keystatu )
{
//    if(Program_Get_Status()==PS_RUN)    //zsf
//    {
//        if(0 == keystatu)
//        {
//            Program_Set_Parameter(VK_SET_RESISTANCE, &resistance );
//        }
//        else
//        {
//            Program_Set_Parameter(VK_LONG_SET_RESISTANCE, &resistance );   
//        }
//    }
    return 0;
}


//static int ChangeProgram( ProgramStart* program )
//{
//   return 1;
//}
//
//static int SetInclineADC( UINT16 adc )
//{
//	return 1;
//}
//
//static int SetRPM( UINT16 rpm )
//{
//	return 1;
//}

//static int SetECB( UINT16 ecb )
//{
//	return 0;
//}

//static int SetPWM( UINT16 pwm )
//{
//	return 0;
//}

static int PressKey( UINT8 key )
{
	return 0;
}
static int DeleteUser( UINT8 UserID )
{
	return 0;
}
void GetHeartrate( UINT8 CurHeartrate )
{
	wlt2564_Set_HeartData(CurHeartrate);
}
static UINT8 SetUser(UINT8 slot)
{
    UINT8 ret = 1;
    if(nCurrentPage != PAGE_WORK_TM1 && nCurrentPage != PAGE_WORK_TM2)
    {
        if( slot < SystemConfig_Get_MaxUserNumber()+1)
        {
            if(Program_Get_Status() == PS_COMPLETED)
            {
                Program_Set_VirtualKey((UINT16)VK_RESET | FRAME_STAGE_PRGRUN);
            }
            User_Set_UserSlot( slot );
            User_Set_VirtualKey((UINT16)VK_UPDATE|FRAME_STAGE_USER);
            ret = 0;
        }
    }
    BT_Set_SyncFlag(true);
    return ret;
}
SubProcessState RemoteControlProcess(RemoteDevicesInformation *deviceinfor, PUINT8 buf, UINT16 *bufLength, const  DapiGlobalVariable* dapiGlobalVariable)
{
    SubProcessState ret = SUB_PROCESS_STATE_NOT_SUPPORTED;
    UINT8 flag = deviceinfor->currentState>>8;
    UINT8 command = (UINT8)(deviceinfor->currentState&0x00ff);
    MACHINE_INFO machineConfig=SystemConfig_Get_MachineInfo( );     
    SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
    RemoteHeader *remoteHeader =  (RemoteHeader*)buf;


    remoteHeader->syncWord = 0xAA55;
    remoteHeader->lingoId = DAPI_LINGO_REMOTE;
    remoteHeader->messageId = command;
    remoteHeader->dataSize= 0;
    remoteHeader->checksum= 0;
    remoteHeader->transactionId = deviceinfor->transactionId;
    if((flag & REMOTE_PROPERTY_CONTROL) == 0)
    {
        return ret;
    }
    
    switch(command)
    {
        case CONTROL_REMOTE_ACKNOWLEDGMENT:
        {
            RemoteDataAck *data =( RemoteDataAck *) (buf + sizeof(RemoteHeader));
		    remoteHeader->dataSize= sizeof(RemoteDataAck);
            data->messageType=deviceinfor->argscounter;
//			if(deviceinfor->argscounter == 0x10)
//			{
//				remoteHeader->lingoId = 0x02;
//			}
            data->error=deviceinfor->cmdParameter;
            deviceinfor->reSend = 0xA5;
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        case CONTROL_SET_CURRENT_USER:
        {
            UINT8 *data = buf+sizeof(RemoteHeader);
            *data = deviceinfor->cmdParameter;
            remoteHeader->dataSize= sizeof(UINT8);
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        case CONTROL_START:
        {
            ProgramStart *data =( ProgramStart *) (buf + sizeof(RemoteHeader));
		    remoteHeader->dataSize= sizeof(ProgramStart);
            data->programType=Program_Get_ProgramID();
            data->workoutTime=Program_Get_WorkoutTime()-Program_Get_WorkoutTime_Warmup()-Program_Get_WorkoutTime_Cooldown();
            data->warmupTime=Program_Get_WorkoutTime_Warmup();
            data->cooldownTime=Program_Get_WorkoutTime_Cooldown();
            data->units=SystemConfig_Get_Display_Unit(User_Get_UserSlot());
            data->startSpeedX10=Program_Get_Speed(DATA_AUTO,0);
            data->startInclineX10=Program_Get_Incline(0);
            data->startLevel=Program_Get_WorkoutLevel(0); 
//            data->startTarget=Program_Get_WorkoutTime();
//            data->startResistance=Program_Get_Resistance(0);
            deviceinfor->reSend = 0xA5;
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
            
            appStart = 0;    //zsf
        }
        break;
        case CONTROL_PAUSE:
        {
            ProgramPause *data =( ProgramPause *) (buf + sizeof(RemoteHeader));
            remoteHeader->dataSize= sizeof(ProgramPause);
            data->status = deviceinfor->cmdParameter;
            deviceinfor->reSend = 0xA5;
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
//        case CONTROL_RESTART:
//        {
//            ProgramPause *data =( ProgramPause *) (buf + sizeof(RemoteHeader));
//            *bufLength = sizeof(RemoteHeader) + sizeof(ProgramPause);
//            data->status = 0x01;
//            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
//        }
//        break;
        case CONTROL_CHANGE_PROGRAM:
        {
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        case CONTROL_SET_SPEED:
        {
            UINT16 *data = (UINT16 *)buf+sizeof(RemoteHeader);
            *data = deviceinfor->cmdParameter;
            remoteHeader->dataSize= sizeof(UINT16);
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        case CONTROL_SET_INCLINE:
        {
            UINT16 *data = (UINT16 *)buf+sizeof(RemoteHeader);
            *data = deviceinfor->cmdParameter;
            remoteHeader->dataSize= sizeof(UINT16);
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        case CONTROL_SET_RESISTANCE:
        {
            UINT8 *data = (UINT8*)buf+sizeof(RemoteHeader);
            *data = deviceinfor->cmdParameter;
            remoteHeader->dataSize= sizeof(UINT16);
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        case CONTROL_SET_RAW_ECB:
        {
            UINT16 *data =(UINT16 *) buf+sizeof(RemoteHeader);
            *data = deviceinfor->cmdParameter;
            remoteHeader->dataSize= sizeof(UINT16);
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        case CONTROL_SET_RAW_PWM:
        {
            UINT16 *data = (UINT16 *)buf+sizeof(RemoteHeader);
            *data = deviceinfor->cmdParameter;
		    remoteHeader->dataSize= sizeof(UINT16);
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        case CONTROL_SELECT_PROGRAM:
        {
            UINT16 *data =(UINT16*) buf+sizeof(RemoteHeader);
            *data = deviceinfor->cmdParameter;
		    remoteHeader->dataSize= sizeof(UINT16);
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        case CONTROL_KEY_PRESS:
        {
            UINT8 *data = (UINT8*)buf+sizeof(RemoteHeader);
            *data = deviceinfor->cmdParameter;
		    remoteHeader->dataSize= sizeof(UINT8);
                    deviceinfor->reSend = 0xA5;
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        case CONTROL_GET_NETWORK_TIME:
        {
		    remoteHeader->dataSize= sizeof(RemoteHeader);
            deviceinfor->reSend = 0xA5;
            ret = SUB_PROCESS_STATE_DIRECT_CONNECT;
        }
        break;
        default:
        break;
        
    }
	if(remoteHeader->dataSize>0)
		remoteHeader->checksum=GenerateCRC_CCITT((buf+sizeof(RemoteHeader)) , remoteHeader->dataSize);
		
    *bufLength=sizeof(RemoteHeader)+remoteHeader->dataSize;
    return ret;
}

RemoteResponse RemoteControlReceive(SubReceiveDataType datatype, ADDSTATESFUNC addstatefunc, DapiGlobalVariable* dapiGlobalVariable )
{
    RemoteResponse ret = REMOTE_RESPONSE_START;
    ret = REMOTE_RESPONSE_SUCCESFUL;
    switch(datatype.command)
    {
        case CONTROL_REMOTE_ACKNOWLEDGMENT:
        {
            RemoteDataAck *reponse =( RemoteDataAck *)datatype.data;		
            if(reponse->error)
			{
                ret = REMOTE_RESPONSE_START;
	            addstatefunc(REMOTE_PROPERTY_GENERAL<<8|reponse->messageType, 0, 0, NULL );
			}
        }
        break;
        case CONTROL_SET_CURRENT_USER:
        {
            UINT8 *response = datatype.data;
            addstatefunc(RCMD_CONTROL_ACKNOWLEDGMENT,datatype.command,SetUser( *response),NULL);
            ERP_Clear_time();
        }
        break;
        case CONTROL_START:
        {
            ProgramStart *response =(ProgramStart *) datatype.data;            
            addstatefunc(RCMD_CONTROL_ACKNOWLEDGMENT,datatype.command,StartWorkout(response),NULL);
        }
        break;
        case CONTROL_PAUSE:
        {            
            ProgramPause *response =(ProgramPause *) datatype.data;            
            addstatefunc(RCMD_CONTROL_ACKNOWLEDGMENT,datatype.command,PauseWorkout(response->status),NULL);
        }
        break;
        //        case CONTROL_RESTART:
        //        {
        //            addstatefunc(RCMD_CONTROL_ACKNOWLEDGMENT,1,PauseWorkout(),NULL);
        //        }
        //        break;
        case CONTROL_CHANGE_PROGRAM:
        {
//            ProgramStart *response =(ProgramStart *) datatype.data;   
//            addstatefunc(RCMD_CONTROL_ACKNOWLEDGMENT,datatype.command,StartWorkout(response),NULL);
            ProgramChangeWorkoutTime *response =(ProgramChangeWorkoutTime *) datatype.data;
//            addstatefunc(RCMD_CONTROL_ACKNOWLEDGMENT,datatype.command,ChangeWorkoutTime(response),NULL);
            ChangeWorkoutTime(response);
        }
        break;
        case CONTROL_SET_SPEED:
        {
            ProgramSetSpeed *response = (ProgramSetSpeed *)datatype.data;
            
            addstatefunc(RCMD_CONTROL_ACKNOWLEDGMENT,datatype.command,SetSpeed(response->speedX10,response->metric,response->appkeystatu),NULL);
        }
        break;
        case CONTROL_SET_INCLINE:
        {
            ProgramSetIncline *response = (ProgramSetIncline *)datatype.data;
            addstatefunc(RCMD_CONTROL_ACKNOWLEDGMENT,datatype.command,SetIncline(response->inclineX10,response->appkeystatu),NULL);
        }
        break;
        case CONTROL_SET_RESISTANCE:
        {
            ProgramSetResistance *response = (ProgramSetResistance *)datatype.data;
            addstatefunc(RCMD_CONTROL_ACKNOWLEDGMENT,datatype.command,SetResistance(response->resistance,response->appkeystatu),NULL);
        }
        break;
//        case CONTROL_SET_RAW_ECB:
//        {
//            UINT16 *response = (UINT16*)datatype.data;
//            addstatefunc(RCMD_CONTROL_ACKNOWLEDGMENT,datatype.command,SetECB( *response),NULL);
//        }
//        break;
//        case CONTROL_SET_RAW_PWM:
//        {
//            UINT16 *response = ( UINT16 *)datatype.data;
//            addstatefunc(RCMD_CONTROL_ACKNOWLEDGMENT,datatype.command,SetPWM( *response ),NULL);
//        }
        break;
        case CONTROL_SELECT_PROGRAM:
        {
            UINT16 *response = ( UINT16 *)datatype.data;
            addstatefunc(RCMD_CONTROL_ACKNOWLEDGMENT,datatype.command,SelectProgram((PROGRAM_ID)*response),NULL);
            
            if(Program_Get_ProgramID()==PS_MYFIRST5K)
            {
                UINT8 userSlot = User_Get_UserSlot();
                BluetoothSetMyFirst5K(userSlot);
            }
            if(Program_Get_ProgramID()==PS_CUSTOM || Program_Get_ProgramID()==PS_CUSTOM_DISTANCE || Program_Get_ProgramID()==PS_CUSTOM_TIME)
            {
//                if(CUSTOM_SYNC.FLAG == 1)
                    BluetoothSetCustom();
            }
            
        }
        break;
        case CONTROL_KEY_PRESS:
        {
            UINT16 *response = (UINT16 *)datatype.data;
            addstatefunc(RCMD_CONTROL_ACKNOWLEDGMENT,datatype.command,PressKey( *response ),NULL);
        }
        case CONTROL_DELETE_USER:
        {
            UINT16 *response = (UINT16 *)datatype.data;
            addstatefunc(RCMD_CONTROL_ACKNOWLEDGMENT,datatype.command,DeleteUser( *response ),NULL);
        }
        case CONTROL_GET_HEARTRATE:
        {
            UINT16 *response = (UINT16 *)datatype.data;
//            addstatefunc(RCMD_CONTROL_ACKNOWLEDGMENT,datatype.command,GetHeartrate( *response ),NULL);
            GetHeartrate( *response );
        }
        break;
        case CONTROL_GET_NETWORK_TIME:
        {
            DateTime_T APP_RTC_TIME;
            ProgramRTC *response = (ProgramRTC *)datatype.data;
            
            APP_RTC_TIME.year = response->rtcyear-2000;
            APP_RTC_TIME.date = response->rtcday;
            APP_RTC_TIME.month = response->rtcmonth;
            APP_RTC_TIME.hours = response->rtchour;
            APP_RTC_TIME.minutes = response->rtcmin;
            APP_RTC_TIME.seconds = response->rtcsecond;
            
            RTC_Write_Time(&APP_RTC_TIME);
        }
        break;
        default:
        break;
        
    }
    return ret;
}

#endif
