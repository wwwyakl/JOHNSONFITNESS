#include "Remote_workoutTracking.h"
#ifdef __REMOTE_WORKOUT_TRACKING__
#include "UtilitiesRtc.h"
#include "programs.h"
#include "SystemConfig.h"
#include "Remote_process.h"
#include "Remote_datatype.h"
#include "remote_port.h"

static UINT8 gWorkoutTrackingCount;
static RemoteWorkoutState gWorkoutState = WORKOUT_IDLE;
static DateTime_T gWorkoutTime;

static void GetWorkoutDataFitnessTestExtra(WorkoutDataFitnessTestCompleteExtra* data)
{
    UINT16 strDescriptionLength;

    data->programType = Program_Get_ProgramID( );
    memcpy(data->programUUID , Program_Get_UUID( ), 36);
    memcpy(data->programDescription , Program_Get_Description( &strDescriptionLength ) , strDescriptionLength>256?256:strDescriptionLength);
    data->fitnessTestScore =Fitness_Test_Get_Score( );
}

static void GetWorkoutDataSprint8Extra(WorkoutDataSprint8CompleteExtra* data)
{
    UINT16 strDescriptionLength;

    data->programType = Program_Get_ProgramID( );
    memcpy(data->programUUID , Program_Get_UUID( ), 36);
    memcpy(data->programDescription , Program_Get_Description( &strDescriptionLength ) , strDescriptionLength>256?256:strDescriptionLength);

    data->sprint1SweatScoreX10 = Sprint8_Get_Sweat_Score( 0 );
    data->sprint2SweatScoreX10 = Sprint8_Get_Sweat_Score( 1 );
    data->sprint3SweatScoreX10 = Sprint8_Get_Sweat_Score( 2 );
    data->sprint4SweatScoreX10 = Sprint8_Get_Sweat_Score( 3 );
    data->sprint5SweatScoreX10 = Sprint8_Get_Sweat_Score( 4 );
    data->sprint6SweatScoreX10 = Sprint8_Get_Sweat_Score( 5 );
    data->sprint7SweatScoreX10 = Sprint8_Get_Sweat_Score( 6 );
    data->sprint8SweatScoreX10 = Sprint8_Get_Sweat_Score( 7 );

    data->totalSweatScoreX10 = Sprint8_Get_Total_Sweat_Score( );
    
}

static void GetWorkoutDataCompleteEx( WorkoutDataCompleteEx* data )
{
    UINT16 strDescriptionLength;

    data->programType = Program_Get_ProgramID( );
    memcpy(data->programUUID , Program_Get_UUID( ), 36);
    memcpy(data->programDescription , Program_Get_Description( &strDescriptionLength ) , strDescriptionLength>256?256:strDescriptionLength);
    data->workoutTime = Program_Get_WorkoutTime_Elapsed( ) ;
    data->warmupTime = Program_Get_WorkoutTime_Warmup( );
    data->cooldownTime = Program_Get_WorkoutTime_Cooldown( );
    data->totalCalories = Program_Get_Calories( 0 );
    data->milesX100 = Program_Get_Distance( DATA_STANARD );
    data->averageHeartRate =  Program_Get_HR(AVERGE_HR);
    data->totalFloorsX10 = Program_Get_Floor( );
    data->totalSteps = Program_Get_Steps( );
    data->totalElevationX100 = Program_Get_Elevation()*100;
}


static void GetWorkoutDataEx( WorkoutDataEx* data )
{
    strcpy(data->programUUID , Program_Get_UUID( ));
    data->programType = Program_Get_ProgramID( );
    data->goalType = Program_Get_Goal_Type( );
    data->goalValueX100 = Program_Get_Goal( )*100;
    data->goalRemainingX100 = Program_Get_Goal_Remaining()*100;
    data->stageInterval = Program_Get_Stage( );
    data->stageGoalValueX100 = Program_Get_Stage_Goal()*100;
    data->stageGoalRemainingX100 = Program_Get_Stage_Goal_Remaining( )*100;
    data->workoutState = Program_Get_Status( ) -PS_WARMUP;
    switch(data->workoutState)
    {
        case 0:
        {
            data->timeInState= Program_Get_WorkoutTime_Elapsed();
        }
        break;
        case 1:
        {
            data->timeInState = Program_Get_WorkoutTime_Elapsed() - Program_Get_WorkoutTime_Warmup( )+1;
        }
        break;
        case 2:
        {
            data->timeInState =  Program_Get_WorkoutTime_Cooldown() - (Program_Get_WorkoutTime() - Program_Get_WorkoutTime_Elapsed());
        }
        break;
    }
    data->timeInStage = Program_Get_timeInStage( );

    data->lapIndex = Program_Get_Lap_Index();
    data->splitIndex = Program_Get_Split_Index();
    data->MPHX10 = Program_Get_Speed(DATA_STANARD, 0 );
    data->inclineX10 = Program_Get_Incline( 0 );
    data->averagePace = Program_Get_Pace_Average( );
    data->totalCalories = Program_Get_Calories( 0 );
    data->caloriesPerHour = Program_Get_Calories(0)*3600/Program_Get_WorkoutTime_Elapsed();
    data->metsX10 = Program_Get_METs( );
    data->watts = Program_Get_Watts(CURRENT_WATTS ,0 );
    data->milesX100 = Program_Get_Distance(DATA_STANARD);
    data->heartRate = Program_Get_HR(CURRENT_HR);
    data->averageHeartRate = Program_Get_HR(AVERGE_HR);
    data->RPM = Program_Get_RPM( );
    data->programLevel = Program_Get_WorkoutLevel(0);
    data->resistanceLevel = Program_Get_Resistance( 0 );
    data->targetHeartRate = Program_Get_HR(TARGET_HR);
    data->targetWattsX10 = Program_Get_Watts(TARGET_WATTS , 0 );
    data->floorsX10 = Program_Get_Floor();
    data->totalSteps =  Program_Get_Steps( );
    data->elevationX100 = Program_Get_Elevation()*100;
    data->VO2X10 = Program_Get_VO2( );
}


/*
 * This function fills out the structure passed with the current workout data.
 */
static void GetWorkoutData( WorkoutData* data )
{

	/* Notes on the various times.
	 *
	 * Workout Time = Total Workout Time - warm up - cool down.
	 *
	 * Stage Time Seconds = Program Stage Time.  This only applies to programs with multiple stages like
	 * the fitness test and table based programs.
	 *
	 * State Time Seconds = Current state time.  This depends on the program state which is either in
	 * warmup, workout, cool down, or pause.
	 *
	 * timeIn... variables is the elapsed time.
	 */
	
    unsigned int totalTime = Program_Get_WorkoutTime();
    unsigned int WarmupTime  = Program_Get_WorkoutTime_Warmup( );
    unsigned int CoolDownTime = Program_Get_WorkoutTime_Cooldown( );		
    unsigned int workoutTime = Program_Get_WorkoutTime_Elapsed();
    data->programType = Program_Get_ProgramID( );


    
	// 0 = Warmup, 1 = Workout, 2 = Cooldown, 3 = Pause
	data->workoutState = Program_Get_Status( )-PS_WARMUP;
        
        if(totalTime)
        {
	    data->workoutTimeSeconds = totalTime - WarmupTime - CoolDownTime;
        }
        else
        {
            data->workoutTimeSeconds = 0;
        }
	data->warmupTimeSeconds = WarmupTime;
	data->cooldownTimeSeconds = CoolDownTime;
        switch(data->workoutState)
        {
            case 0:
            {
                data->timeInStateSeconds = workoutTime+1;
            }
            break;
            case 1:
            {
                data->timeInStateSeconds = workoutTime - WarmupTime+1;
            }
            break;
            case 2:
            {
                data->timeInStateSeconds =  CoolDownTime - (totalTime - workoutTime);
            }
            break;
        }
    data->stage = Program_Get_Stage( );
    data->stageTimeSeconds = Program_Get_stageTime( );
    data->timeInStageSeconds =Program_Get_timeInStage( );

    data->mphx10 = Program_Get_Speed(DATA_STANARD,0);

    data->inclinex10 =  Program_Get_Incline( 0 );

    data->totalCalories = Program_Get_Calories(0);
    UINT32 calorieRate = data->totalCalories;
    calorieRate = (UINT32)calorieRate*3600/workoutTime;
    data->caloriesPerHour = (UINT16)calorieRate;

    data->metsX10 = Program_Get_METs( );
    data->watts = Program_Get_Watts(CURRENT_WATTS,0);

    data->milesx100 =Program_Get_Distance(DATA_STANARD);
    data->averagePace =Program_Get_Pace_Average( );
    data->heartRate = Program_Get_HR(CURRENT_HR);

    data->rpm = Program_Get_RPM( );

    data->programLevel = Program_Get_WorkoutLevel( 0 ) ;

    data->resistanceLevel = Program_Get_Resistance(0);

    data->thr = Program_Get_HR(TARGET_HR);

    data->targetWattsX10 = Program_Get_Watts(TARGET_WATTS,0);

    data->averageHeartRate = Program_Get_HR( AVERGE_HR );
    data->goal = Program_Get_Goal( );

}

static void GetWorkoutDataComplete( WorkoutDataComplete* data )
{

    unsigned int totalTime = Program_Get_WorkoutTime();
    unsigned int WarmupTime  = Program_Get_WorkoutTime_Warmup( );
    unsigned int CoolDownTime = Program_Get_WorkoutTime_Cooldown( );		
     unsigned int workoutTime = Program_Get_WorkoutTime_Elapsed();

    if( workoutTime <= WarmupTime )
    {
    	data->warmupTimeSeconds = workoutTime;
    	data->workoutTimeSeconds = 0;
    	data->cooldownTimeSeconds = 0;
    }
    else
    {
            data->warmupTimeSeconds = WarmupTime;
            if((totalTime - workoutTime) <= (CoolDownTime) )
            {
                data->cooldownTimeSeconds = CoolDownTime - (totalTime - workoutTime);
                data->workoutTimeSeconds = workoutTime - WarmupTime - data->cooldownTimeSeconds;
            }
            else
            {
                data->workoutTimeSeconds = workoutTime - WarmupTime;			
                data->cooldownTimeSeconds = 0;
            }
    }

	data->milesx100 =Program_Get_Distance(DATA_STANARD);
	data->averageHeartRate = Program_Get_HR(AVERGE_HR);
	data->totalCalories = Program_Get_Calories( 0 );
	data->programType = Program_Get_ProgramID( );

}

void RemoteChangeWorkotState(RemoteWorkoutState state, ADDSTATESFUNC addstatefunc, DapiGlobalVariable* dapiGlobalVariable, UINT8 *deviceMac )
{
   if( ( gWorkoutState==WORKOUT_PAUSED||gWorkoutState == WORKOUT_IDLE || gWorkoutState == WORKOUT_STOP ) && state == WORKOUT_RUNNING )
    {
        RTC_Read_Time( & gWorkoutTime );
        memset( &(dapiGlobalVariable->dataInfor.workoutInfor) , 0 , sizeof(WorkoutInfor) );            
    }
   else if( ( gWorkoutState == WORKOUT_RUNNING || gWorkoutState == WORKOUT_PAUSED ) && state == WORKOUT_STOP )
    {
    
       MACHINE_INFO machineConfig = SystemConfig_Get_MachineInfo( );
       SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );
       if( SystemConfig_Get_Dapi_Version( )>9)
       {
           WorkoutDataEx *workoutdata = &(dapiGlobalVariable->dataInfor.workoutInfor.workouData.WorkdataTrackEx);
           GetWorkoutDataEx(workoutdata );
           memcpy( workoutdata->userExternalId, &(dapiGlobalVariable->dataInfor.workoutInfor._UserExternalID[1]), 36 );
           memcpy( workoutdata->macAddress, deviceMac, 6 );
           memcpy(workoutdata->MachineID,machineConfig.machineUUID, Settings.StringMaxLength);
           workoutdata->workoutStartYear = gWorkoutTime.year ;
           workoutdata->workoutStartYear = workoutdata->workoutStartYear+ 2000;
           workoutdata->workoutStartMonth=gWorkoutTime.month;
           workoutdata->workoutStartDay= gWorkoutTime.date;
           workoutdata->workoutStartHour= gWorkoutTime.hours;
           workoutdata->workoutStartMinute= gWorkoutTime.minutes;
           workoutdata->workoutStartSecond= gWorkoutTime.seconds;   
           WorkoutDataCompleteEx *workoutdataComplete = &(dapiGlobalVariable->dataInfor.workoutInfor.wokoutCompleteData.wkDataCompleteEx);
           GetWorkoutDataCompleteEx( workoutdataComplete );
           memcpy( workoutdataComplete->userExternalId, &(dapiGlobalVariable->dataInfor.workoutInfor._UserExternalID[1]), 36 );
           memcpy( workoutdataComplete->macAddress, deviceMac, 6 );
           memcpy(workoutdataComplete->MachineID,machineConfig.machineUUID, Settings.StringMaxLength);
           workoutdataComplete->workoutStartYear = gWorkoutTime.year ;
           workoutdataComplete->workoutStartYear = workoutdataComplete->workoutStartYear + 2000;
           workoutdataComplete->workoutStartMonth= (UINT8)gWorkoutTime.month;
           workoutdataComplete->workoutStartDay= (UINT8)gWorkoutTime.date;
           workoutdataComplete->workoutStartHour= (UINT8)gWorkoutTime.hours;
           workoutdataComplete->workoutStartMinute= (UINT8)gWorkoutTime.minutes;
           workoutdataComplete->workoutStartSecond= (UINT8)gWorkoutTime.seconds;           
           if( Program_Get_ProgramID( ) == PS_SPRINT8 )
           {
               WorkoutDataSprint8CompleteExtra *sprint8 = &(dapiGlobalVariable->dataInfor.workoutInfor.programExtraInfor.completeSprint8Data);
               memcpy( sprint8->userExternalId, &(dapiGlobalVariable->dataInfor.workoutInfor._UserExternalID[1]), 36 );
               memcpy( sprint8->macAddress, deviceMac, 6 );
               memcpy(sprint8->MachineID,machineConfig.machineUUID, Settings.StringMaxLength);
               sprint8->workoutStartYear = gWorkoutTime.year ;
               sprint8->workoutStartYear = sprint8->workoutStartYear + 2000;
               sprint8->workoutStartMonth= (UINT8)gWorkoutTime.month;
               sprint8->workoutStartDay= (UINT8)gWorkoutTime.date;
               sprint8->workoutStartHour= (UINT8)gWorkoutTime.hours;
               sprint8->workoutStartMinute= (UINT8)gWorkoutTime.minutes;
               sprint8->workoutStartSecond= (UINT8)gWorkoutTime.seconds;
               GetWorkoutDataSprint8Extra(sprint8);
           }
           if( Program_Get_ProgramID( ) == PS_FITNESS_TEST )
           {
               WorkoutDataFitnessTestCompleteExtra *fitness =  &(dapiGlobalVariable->dataInfor.workoutInfor.programExtraInfor.completeFitnessTestData);
               memcpy( fitness->userExternalId, &(dapiGlobalVariable->dataInfor.workoutInfor._UserExternalID[1]), 36 );
               memcpy( fitness->macAddress, deviceMac, 6 );
               memcpy(fitness->MachineID,machineConfig.machineUUID, Settings.StringMaxLength);
               fitness->workoutStartYear = gWorkoutTime.year ;
               fitness->workoutStartYear = fitness->workoutStartYear + 2000;
               fitness->workoutStartMonth= (UINT8)gWorkoutTime.month;
               fitness->workoutStartDay= (UINT8)gWorkoutTime.date;
               fitness->workoutStartHour= (UINT8)gWorkoutTime.hours;
               fitness->workoutStartMinute= (UINT8)gWorkoutTime.minutes;
               fitness->workoutStartSecond= (UINT8)gWorkoutTime.seconds;
               GetWorkoutDataFitnessTestExtra(fitness);
           }
          addstatefunc((REMOTE_PROPERTY_WORKOUTTRACKING<<8)|REMOTE_WORKOUT_TRACKING_COMPLETE_EX, 0, 0, NULL);
          addstatefunc(RCMD_ASSET_UPDATE_MACHINE_STATE, 0, 0, NULL);
       }
       else
       {
           WorkoutData *workoutdata = &(dapiGlobalVariable->dataInfor.workoutInfor.workouData.WorkdataTrack);
           memcpy( workoutdata->userExternalId, &(dapiGlobalVariable->dataInfor.workoutInfor._UserExternalID[1]), 36 );             
           memcpy( workoutdata->macAddress, deviceMac, 6 );
           memcpy(workoutdata->MachineID,machineConfig.machineUUID,Settings.StringMaxLength );
           workoutdata->year = gWorkoutTime.year;
           workoutdata->year = workoutdata->year+ 2000;
           workoutdata->month = (UINT8)gWorkoutTime.month;
           workoutdata->day = (UINT8)gWorkoutTime.date;
           workoutdata->hour = (UINT8)gWorkoutTime.hours;
           workoutdata->minute = (UINT8)gWorkoutTime.minutes;
           workoutdata->second = (UINT8)gWorkoutTime.seconds;
           GetWorkoutData( workoutdata );
           WorkoutDataComplete *workoutdataComplete = &(dapiGlobalVariable->dataInfor.workoutInfor.wokoutCompleteData.wkDataComplete);
           memcpy( workoutdataComplete->userExternalId, &(dapiGlobalVariable->dataInfor.workoutInfor._UserExternalID[1]), 36 );                
           memcpy( workoutdataComplete->macAddress,  deviceMac, 6 );
           memcpy( workoutdataComplete->MachineID, machineConfig.machineUUID, Settings.StringMaxLength);
           workoutdataComplete->year = gWorkoutTime.year ;
           workoutdataComplete->year  = workoutdataComplete->year+2000;
           workoutdataComplete->month = (UINT8)gWorkoutTime.month;
           workoutdataComplete->day = (UINT8)gWorkoutTime.date;
           workoutdataComplete->hour = (UINT8)gWorkoutTime.hours;
           workoutdataComplete->minute = (UINT8)gWorkoutTime.minutes;
           workoutdataComplete->second = (UINT8)gWorkoutTime.seconds;
           GetWorkoutDataComplete( workoutdataComplete );           
           addstatefunc((REMOTE_PROPERTY_WORKOUTTRACKING<<8)|REMOTE_WORKOUT_TRACKING_WORKOUT_COMPLETE, 0, 0, NULL);
           
       }
    }
   gWorkoutState = state;

}



SubProcessState RemoteWorkoutProcess(RemoteDevicesInformation *deviceinfor, UINT8* buf, UINT16 *bufLength, const  DapiGlobalVariable* dapiGlobalVariable)
{
    SubProcessState ret = SUB_PROCESS_STATE_NOT_SUPPORTED;
//    UINT8 flag = deviceinfor->currentState>>8;
    UINT8 command = (UINT8)(deviceinfor->currentState&0x00ff);
    MACHINE_INFO machineConfig=SystemConfig_Get_MachineInfo( );
    SystemSettingType Settings = SystemConfig_Get_DefaultString( SETTING_DEFAULT_MACHINE_UUID );


    RemoteHeader *remoteHeader =  (RemoteHeader*)buf;


    remoteHeader->syncWord = 0xAA55;
    remoteHeader->lingoId = DAPI_LINGO_WORKOUT_TRACKING;
    remoteHeader->messageId = command;


    switch(command)
    {
        case REMOTE_WORKOUT_TRACKING_WORKOUT_DATA:
        {            
            switch(deviceinfor->currentSubState)
            {
                case 0:
                {
                    WorkoutData *requestdata =(WorkoutData*)( buf+ sizeof(RemoteHeader)); 
                    memcpy(requestdata->userExternalId, dapiGlobalVariable->dataInfor.workoutInfor._UserExternalID+1, 36 );
                    memcpy( requestdata->macAddress, deviceinfor->deviceMacAddress, 6 );
                    memcpy(requestdata->MachineID,machineConfig.machineUUID, Settings.StringMaxLength);
                    requestdata->year = gWorkoutTime.year;
                    requestdata->year = requestdata->year+ 2000;
                    requestdata->month = gWorkoutTime.month;
                    requestdata->day = gWorkoutTime.date;
                    requestdata->hour = gWorkoutTime.hours;
                    requestdata->minute = gWorkoutTime.minutes;
                    requestdata->second = gWorkoutTime.seconds;
                    GetWorkoutData( requestdata );
                    ret = SUB_PROCESS_STATE_NEXT_SUBSTATE;
                }
                break;
                case 1:
                {
                    if((gWorkoutTrackingCount + 2) * sizeof(WorkoutData) > deviceinfor->deviceBufSize)
                    {
                        ret = SUB_PROCESS_STATE_NEXT_STATE;
                    }
                    else
                    {
                        ret = SUB_PROCESS_STATE_JUMP_STATE;
                    }
                }
                break;
            }
        }
        break;
        case REMOTE_WORKOUT_TRACKING_WORKOUT_COMPLETE:
        {
            switch(deviceinfor->currentSubState)
            {
                case 0:
                {
                    WorkoutData *requestdata =(WorkoutData*)( buf+ sizeof(RemoteHeader)); 
                    memcpy(requestdata,&(dapiGlobalVariable->dataInfor.workoutInfor.workouData.WorkdataTrack), sizeof(WorkoutData));
                    remoteHeader->messageId = REMOTE_WORKOUT_TRACKING_WORKOUT_DATA;
                    ret = SUB_PROCESS_STATE_NEXT_SUBSTATE;
                }
                break;
                case 1:
                {
                    WorkoutDataComplete *requestdata = (WorkoutDataComplete*)(buf+ sizeof(RemoteHeader)); 
                    memcpy(requestdata,&(dapiGlobalVariable->dataInfor.workoutInfor.wokoutCompleteData.wkDataComplete), sizeof(WorkoutDataComplete));
                    ret = SUB_PROCESS_STATE_NEXT_SUBSTATE;
                }
                break;
                case 2:
                {
                     ret = SUB_PROCESS_STATE_NEXT_STATE;
                }
                break;
            }
        }
        break;
        case REMOTE_WORKOUT_TRACKING_DATA_EX:
        {            
            switch(deviceinfor->currentSubState)
            {
                case 0:
                {
                    WorkoutDataEx *requestdata = (WorkoutDataEx*)(buf+ sizeof(RemoteHeader)); 
                    memcpy(requestdata->userExternalId, dapiGlobalVariable->dataInfor.workoutInfor._UserExternalID+1, 36 );
                    memcpy( requestdata->macAddress, deviceinfor->deviceMacAddress, 6 );
                    memcpy(requestdata->MachineID,machineConfig.machineUUID, Settings.StringMaxLength);
                    requestdata->workoutStartYear= gWorkoutTime.year;
                    requestdata->workoutStartYear = requestdata->workoutStartYear+ 2000;
                    requestdata->workoutStartMonth= gWorkoutTime.month;
                    requestdata->workoutStartDay= gWorkoutTime.date;
                    requestdata->workoutStartHour= gWorkoutTime.hours;
                    requestdata->workoutStartMinute= gWorkoutTime.minutes;
                    requestdata->workoutStartSecond= gWorkoutTime.seconds;
                    GetWorkoutDataEx( requestdata );
                    ret = SUB_PROCESS_STATE_NEXT_SUBSTATE;
                }
                break;
                case 1:
                {
                    if((gWorkoutTrackingCount + 2) * sizeof(WorkoutDataEx) > deviceinfor->deviceBufSize)
                    {
                        ret = SUB_PROCESS_STATE_NEXT_STATE;
                    }
                    else
                    {
                        ret = SUB_PROCESS_STATE_JUMP_STATE;
                    }
                }
                break;
            }
        }
        break;
        case REMOTE_WORKOUT_TRACKING_COMPLETE_EX:
        {
            switch(deviceinfor->currentSubState)
            {
                case 0:
                {
                    WorkoutDataEx *requestdata = (WorkoutDataEx*)(buf+ sizeof(RemoteHeader)); 
                    memcpy(requestdata,&(dapiGlobalVariable->dataInfor.workoutInfor.workouData.WorkdataTrackEx), sizeof(WorkoutDataEx));
                    remoteHeader->messageId = REMOTE_WORKOUT_TRACKING_DATA_EX;
                    ret = SUB_PROCESS_STATE_NEXT_SUBSTATE;
                }
                break;
                case 1:
                {
                    if(dapiGlobalVariable->dataInfor.workoutInfor.wokoutCompleteData.wkDataCompleteEx.programType == PS_SPRINT8)
                    {
                        WorkoutDataSprint8CompleteExtra *requestdata =(WorkoutDataSprint8CompleteExtra*)( buf+ sizeof(RemoteHeader)); 
                        memcpy(requestdata,&(dapiGlobalVariable->dataInfor.workoutInfor.programExtraInfor.completeSprint8Data), sizeof(WorkoutDataSprint8CompleteExtra));
                        remoteHeader->messageId = REMOTE_WORKOUT_TRACKING_SPRINT8_COMPLETE_EXTRA;
                        ret = SUB_PROCESS_STATE_NEXT_SUBSTATE;
                        return ret;
                    }
                    else if(dapiGlobalVariable->dataInfor.workoutInfor.wokoutCompleteData.wkDataCompleteEx.programType == PS_FITNESS_TEST)
                    {
                        WorkoutDataFitnessTestCompleteExtra *requestdata =(WorkoutDataFitnessTestCompleteExtra*)( buf+ sizeof(RemoteHeader)); 
                        memcpy(requestdata, &(dapiGlobalVariable->dataInfor.workoutInfor.programExtraInfor.completeFitnessTestData), sizeof(WorkoutDataFitnessTestCompleteExtra));
                        remoteHeader->messageId = REMOTE_WORKOUT_TRACKING_FITNESS_COMPLETE_EXTRA;
                        ret = SUB_PROCESS_STATE_NEXT_SUBSTATE;
                        return ret;
                    }
                }
                //break;
                case 2:
                {
                    WorkoutDataCompleteEx *requestdata = (WorkoutDataCompleteEx*)(buf+ sizeof(RemoteHeader)); 
                    memcpy(requestdata ,&( dapiGlobalVariable->dataInfor.workoutInfor.wokoutCompleteData.wkDataCompleteEx), sizeof(WorkoutDataCompleteEx));
                    ret = SUB_PROCESS_STATE_NEXT_SUBSTATE;
                }
                break;
                case 3:
                {
                     ret = SUB_PROCESS_STATE_NEXT_STATE;
                }
                break;
            }
        }
        break;
        default:
                break;
    }
    return ret;
}




RemoteResponse RemoteWorkoutReceive(SubReceiveDataType datatype, ADDSTATESFUNC addstatefunc, DapiGlobalVariable* dapiGlobalVariable )
{
    RemoteResponse ret = REMOTE_RESPONSE_START;

    switch(datatype.command)
    {
        case REMOTE_WORKOUT_TRACKING_ACKNOWLEDGMENT:
        {
            RemoteDataAck *ack = (RemoteDataAck*)datatype.data;
            switch( ack->messageType )
            {
                case REMOTE_WORKOUT_TRACKING_WORKOUT_DATA:
                {
                }
                break;
                case REMOTE_WORKOUT_TRACKING_WORKOUT_COMPLETE:
                {
                }
                break;
                case REMOTE_WORKOUT_TRACKING_DATA_EX:
                {
                }
                break;
                case REMOTE_WORKOUT_TRACKING_COMPLETE_EX:
                {
                }
                break;
                case REMOTE_WORKOUT_TRACKING_SPRINT8_COMPLETE_EXTRA:
                {
                }
                break;
                case REMOTE_WORKOUT_TRACKING_FITNESS_COMPLETE_EXTRA:
                {
                }
                break;
            }
        }
        break;

        default:
                break;
    }

    return ret;
}

#endif

