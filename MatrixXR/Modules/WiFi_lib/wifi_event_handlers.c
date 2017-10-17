/*
 * wifi_event_handlers.c
 *
 *  Created on: May 23, 2013
 *      Author: brenden.capps
 */
#include "Jisdef.h"
#ifdef __WIFI_FUNC__
#include "Global_Config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "UtilitiesRtc.h"
#include "programs.h"
#include "wifi_event_handlers.h"
#include "wifi_interface.h"
#include "SystemConfig.h"
#include "System_config.h"

#ifdef __WIFI_NO_RTC__
__no_init static DateTime_T gNetworkTime;
#endif

static bool workout_paused = false;
//NEED_FIX

/* This function is called when a remote app wants to start a program.
 * If there is not a workout running, the software should start the
 * machine and run a manual workout using the warmup, cooldown, and workout times
 * to compute the length of the workout.
 */
int StartWorkout( WIFIDataProgramStart* program )
{
  if( Program_Get_Status() == PS_IDLE)
  {
        UINT16 inclineX10,speedX10;
        UINT8   resistance = program->startLevel;
        inclineX10 = program->startInclineX10;
        speedX10 = program->startSpeedX10;
        Program_Set_Parameter(VK_SET_INCLINE , &inclineX10  );
        Program_Set_Parameter(VK_SET_SPEED , &speedX10 );
        Program_Set_Parameter(VK_SET_RESISTANCE, &resistance);
        //Program_set

        Program_Set_VirtualKey(VK_START);
	  //Data_Set_Start( 1 );
	  workout_paused = false;
	  return 0;
  }
  return 1;
}

/*
 * This function is restarts a paused workout.  Restart the workout if in pause mode.
 */
int RestartWorkout()
{
	if( Program_Get_Status() == PS_PAUSED)
	{
		if( workout_paused )
			workout_paused = false;
		return 0;
	}
  return 1;
}

/*
 * This function stops the workout.
 */
int StopWorkout()
{
 //Event_Set_Virtual_key(VK_RESET);
//			nCurrentPage = PAGE_SELECTUSER;
//            Frame_Stage = FRAME_STAGE_USER;
            //Event_Set_Virtual_key(VK_RESET);
    if(Program_Get_Status() != PS_COMPLETED)
	  return    Event_Set_Virtual_key(VK_RESET);//Program_Set_VirtualKey( VK_STOP );
}

/*
 * This function pauses the currently running workout.
 */
int PauseWorkout()
{
    if(Program_Set_VirtualKey( VK_PAUSE))
    {
        if( !workout_paused )
    		workout_paused = true;
        return 0;
    }
    else
        return 1;
}

/*
 * This function changes the treadmill speed
 */
int SetSpeed( UINT16 speedX10, bool metric )
{

  Program_Set_Parameter(VK_SET_SPEED , &speedX10  );
  // TODO: Set the speed of the treadmill here.
  return 0;
}

/*
 * This function changes the treamdill or EP incline.
 */
   UINT16 inclineValue = 0;//(inclineX10>>4)/10;
int SetIncline( UINT16 inclineX10 )
{
//   inclineValue = (inclineX10>>8);
   inclineValue = ((inclineX10<<8)&0xff00) | ((inclineX10>>8) &0x00ff);
    Program_Set_Parameter(VK_SET_INCLINE , &inclineValue );

  // TODO: Set the incline here.
  return 0;
}

/*
 * This function changes the resistance level for the bike / EP.
 */
int SetResistance( UINT8 resistance )
{
    Program_Set_Parameter(VK_SET_RESISTANCE, &resistance );
  // TODO: Set the resistance here.
  return 0;
}

/*
 * This function changes the workout type and workout time for the current workout.
 * Also the machine should adjust to the start speed, start incline, and start level.
 */
int ChangeProgram( WIFIDataProgramStart* program )
{
 // if( Data_In_Workout() )
   {
	  // TODO: Update workout here.
	  //return 0;
   }
   return 1;

}

/*
 * This function is used to set the raw ADC value.  This should only be called in test mode.
 */
int SetInclineADC( UINT16 adc )
{
	//if( adc >= machineConfig->minInclineADC && adc <= machineConfig->maxInclineADC )
	{

		// TODO: Set incline ADC here.
		//Incline_Set_ADC( adc );//sean_0618
		//return 0;
	}
	return 1;
}

/*
 * This function is used to set the raw RPM value for the treadmill.  This should only be called in
 * test mode.
 */
int SetRPM( UINT16 rpm )
{
	//if( !Data_In_Workout() && rpm >= machineConfig->minKPH_RPM && rpm <= machineConfig->maxKPH_RPM )
	{

		// TODO: Set RPM here.
		//return 0;
	}
	return 1;

}

/*
 * This function is used to set the raw ECB counts.  This is useful at times during certain
 * fitness tests so may be called during remote control or test mode.
 */
int SetECB( UINT16 ecb )
{

	// TODO: Set raw ECB counts here.
	return 0;
}

/*
 * This function is used to set the raw PWM value for induction brakes.  This is useful at times
 * during certain fitness tests so may be called during remote control or test mode.
 */
int SetPWM( UINT16 pwm )
{

	// TODO: Set raw PWM here.
	return 0;
}

/*
 * This function is used to simulate a user pressing a key.
 */
int PressKey( UINT8 key )
{
	// TODO: Simulate key press.
	return 0;
}

/*
 * This function is used to.....
 */
int Setup_Workout_Passport( UINT16 workout_time)
{
    
	// TODO: Simulate key press.
    
	return 0;
}

/*
 * This function is used to.....
 */
UINT16 Goal_Seconds = 0;
UINT16 Goal_Seconds1 = 0;
int Set_Goal_Passport( UINT16 Seconds)
{
	// TODO: Simulate key press.
    Goal_Seconds1 = Seconds;
//Goal_Seconds = (Seconds<<8)&0xff00;
	Goal_Seconds = ((Seconds<<8)&0xff00) | ((Seconds>>8) &0x00ff);
	  Program_Set_Parameter(VK_SET_WORKOUT_TIME , &Goal_Seconds );
	return 1;
}

/*
 * This function fills out the structure passed with the current workout data.
 */
void GetWorkoutDataRemote( WIFIWorkoutDataRemote* data )
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
	 /*

	int totalTime = Data_Get_Time(DT_TARGET);
	int workoutTime = Data_Get_Time(DT_TIME);


	// TODO: Fill this out with actual data.
	data->programType = 0;

	// 0 = Warmup, 1 = Workout, 2 = Cooldown, 3 = Pause
	data->workoutState = 1;

	// Stage time is used for table based programs, intervals, and fit test.  If this
	// is a manual workout, THR, VA, or constant watts the stage time = state time.
	data->stage = 0;

	data->stageTimeSeconds = totalTime;
	data->workoutTimeSeconds = totalTime;
	data->warmupTimeSeconds = 0;
	data->cooldownTimeSeconds = 0;

	data->timeInStateSeconds = workoutTime;
	data->timeInStageSeconds = workoutTime;

	data->mphx10 = Program_Get_Speed();
	
	data->inclinex10 = Program_Get_Incline() * 10 ;

	data->totalCalories = Program_Get_Calories();
	UINT32 calorieRate = (UINT32)Program_Get_Calories() * 60 * 60 / 10000;
	data->caloriesPerHour = (UINT16)calorieRate;

	// TODO: Get Mets and Watts calculations for cardio.  The calculations below are for a treadmill.
	UINT32 metersPerMinute =
		( (UINT32)data->mphx10 * 1609 / 60 + 5 ) / 10;
	if( data->mphx10 <= 40 )
		data->metsX10 = (UINT16)( ( (UINT32)35000 + 2680 * data->mphx10 + 4828 * data->mphx10 * data->inclinex10 / 100 ) / 3500 );
	else
		data->metsX10 = (UINT16)( ( (UINT32)35000 + 5360 * data->mphx10 + 2414 * data->mphx10 * data->inclinex10 / 100 ) / 3500 );
	data->watts = data->caloriesPerHour * 10 / 42;

	//UINT32 meters = Data_Get_Distance(DG_NORMAL);
	UINT32 milesX1000 = Program_Get_Distance( );// meters * 1000 / 1609;
	data->milesx100 = (UINT16)(( milesX1000 + 5 ) / 10 );
	data->averagePace = milesX1000 / workoutTime / 1000;
	data->heartRate = Program_Get_HR(CURRENT_HR);

	// TODO: Compute get RPM for cardio machine.
	data->rpm = 0;

	data->programLevel = Program_Get_Level();

	// TODO: Get resistance level for cardio.
	data->resistanceLevel = Program_Get_Resistance();

	// TODO: Get THR for Target Heart rate Program.
	data->thr = Program_Get_HR(TARGET_HR);

	// TODO: Get Target Watts for Target Heart rate Program.
	data->targetWattsX10 = Program_Get_Watts(TARGET_WATTS,0);

	// TODO: Get goal for calorie and distance goal programs.
	data->goal = 0;

	// TODO: Get next speed, incline, and resistance from programs.
	data->nextSpeed = -1;
	data->nextIncline = -1;
	data->nextResistance = -1;
*/
}
void GetWorkoutDataFitnessTestExtra(WIFIWorkoutDataFitnessTestCompleteExtra* data)
{
    UINT16 strDescriptionLength;

    data->programType = Program_Get_ProgramID( );
    memcpy(data->programUUID , Program_Get_UUID( ), 36);
    memcpy(data->programDescription , Program_Get_Description( &strDescriptionLength ) , strDescriptionLength>256?256:strDescriptionLength);
    data->fitnessTestScore =Fitness_Test_Get_Score( );
}

void GetWorkoutDataSprint8Extra(WIFIWorkoutDataSprint8CompleteExtra* data)
{
    UINT16 strDescriptionLength;

    data->programType = Program_Get_ProgramID( );
    memcpy(data->programUUID , Program_Get_UUID( ), 36);
    memcpy(data->programDescription , Program_Get_Description( &strDescriptionLength ) , strDescriptionLength>256?256:strDescriptionLength);
//
//    for(int i = 0;i<8;i++)
//    {
//        UINT16* p = &(data->sprint1SweatScoreX10);
//        *(p+i) = Sprint8_Get_Sweat_Score( i );
//    }
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

void GetWorkoutDataCompleteEx( WIFIWorkoutDataCompleteEx* data )
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


void GetWorkoutDataEx( WIFIWorkoutDataEx* data )
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
void GetWorkoutData( WIFIWorkoutData* data )
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

void GetWorkoutDataComplete( WIFIWorkoutDataComplete* data )
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

void GetStartWorkoutData( WIFIDataExternalProgramStart* data )
{
	//UINT8 userId = 0;
	data->programType = 1;
	data->workoutTime = 2;
	data->warmupTime = 3;
	data->cooldownTime = 4;
	data->startMPHX10 = 5;
	data->startInclineX10 = 6;
	data->startLevel = 7;
}


/*
 * This function updates the time based on the input.
 */
bool UpdateRTCTime( WIFIDataRegion* time, int offset )
{
	DateTime_T now;
       // MACHINE_INFO machineConfig = SystemConfig_Get_MachineInfo( );
	if(
			 time->hour > 23 ||
			 time->minute > 59 ||
			 time->second > 59 ||
			 time->month > 12 ||
			 time->day > 31 ||
			time->year < 2013
	)
		return false;
	switch( time->month )
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		if( time->day > 31 )
			return false;
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		if( time->day > 30 )
			return false;
		break;
	case 2:
		if(
				( time->year % 100 == 0 && time->year % 400 == 0 ) ||
				( time->year % 4 == 0 && time->year % 100 != 0 )
		)
		{
			if( time->day > 29 )
				return false;
		}
		else if( time->day > 28 )
			return false;
		break;
	}
	if( time->minute > 59 )
		return false;


	if( offset > 0 )
	{
		time->second = ( time->second + offset );
		if( time->second >= 60 )
		{
			time->second = time->second % 60;
			time->minute++;
			if( time->minute == 60 )
			{
				time->minute = 0;
				time->hour++;
				if( time->hour == 24 )
				{
					time->hour = 0;
					time->day++;
					switch( time->month )
					{
					case 1:
					case 3:
					case 5:
					case 7:
					case 8:
					case 10:
					case 12:
						if( time->day == 32 )
						{
							time->day = 1;
							time->month++;
							if( time->month == 13 )
							{
								time->month = 1;
								time->year++;
							}
						}
						break;
					case 4:
					case 6:
					case 9:
					case 11:
						if( time->day == 31 )
						{
							time->day = 1;
							time->month++;
						}
						break;
					case 2:
						if(
							( time->year % 100 == 0 && time->year % 400 == 0 ) ||
							( time->year % 4 == 0 && time->year % 100 != 0 )
						)
						{
							if( time->day == 30 )
							{
								time->month++;
								time->day = 1;
							}
						}
						else if( time->day == 29 )
						{
							time->month++;
							time->day = 1;
						}
						break;
					}
				}
			}
		}
	}

	now.date = time->day;
	now.month = time->month;
	now.year = time->year - 2000;
	now.hours = time->hour;
	now.minutes = time->minute;
	now.seconds = time->second;

#ifdef __WIFI_NO_RTC__
    memset(&gNetworkTime,0,sizeof(DateTime_T));
    memcpy(&gNetworkTime,&now,sizeof(DateTime_T));
#else
	RTC_Write_Time( &now );
#endif
#if 0
	memset( machineConfig.timezone, 0, 65 );
	memcpy( machineConfig.timezone, time->timeZone, 64 );
	machineConfig.is24HoursFormat = time->timeFormat;
	if( SystemConfig_Set_MachineInfo( machineConfig))
        {
            SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH, NULL,0);
        }
 #endif
	return true;
}
#ifdef __WIFI_NO_RTC__
 DateTime_T ReadNetworkTime(UINT16 offset)
{
    //WIFIDataRegion* time;
    UCHAR timeFormat = SystemConfig_Get_Char(RTC_TIME_FORMAT_UCHAR);

    gNetworkTime.seconds += offset%60; 
    if(gNetworkTime.seconds>=60)
    {
        gNetworkTime.seconds = gNetworkTime.seconds%60; 
        gNetworkTime.minutes++;
    }
    gNetworkTime.minutes += (offset/60)%60 ;
    if(gNetworkTime.minutes>=60)
    {
        gNetworkTime.minutes = gNetworkTime.minutes%60; 
        gNetworkTime.hours++;
    }
    gNetworkTime.hours +=offset/3600;
    if(gNetworkTime.hours>=24)
    {
        gNetworkTime.hours = gNetworkTime.hours%24; 
        gNetworkTime.date++;
    }
    switch(gNetworkTime.month)
    {
    	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		if(gNetworkTime.date>31)
            {
                gNetworkTime.month ++;
                gNetworkTime.date = gNetworkTime.date%31;
            }
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		if(gNetworkTime.date>30)
            {
                gNetworkTime.month ++;
                gNetworkTime.date = gNetworkTime.date%30;
            }
		break;
	case 2:
		if(
		( gNetworkTime.year % 100 == 0 && gNetworkTime.year % 400 == 0 ) ||
		( gNetworkTime.year % 4 == 0 && gNetworkTime.year % 100 != 0 )
		)
		
		if(gNetworkTime.date>29)
            {
                gNetworkTime.month ++;
                gNetworkTime.date = gNetworkTime.date%29;
            }		
		else if(gNetworkTime.date>28)
            {
                gNetworkTime.month ++;
                gNetworkTime.date = gNetworkTime.date%28;
            }
		break;
    }
   // time->day = gNetworkTime.date;
    if( gNetworkTime.month>12)
    {
        gNetworkTime.month = gNetworkTime.month%12;
        gNetworkTime.year ++;
    }
    //time->month = gNetworkTime.month;
    //time->year = gNetworkTime.year ;
    //time->hour = gNetworkTime.hours;
    //time->minute = gNetworkTime.minutes;
    //time->second = gNetworkTime.seconds;
    //memset( time->timeZone, 0, 65 );
    //memcpy( time->timeZone, machineConfig.timezone, 64 );
    //time->timeFormat = timeFormat;
   // UpdateRTCTime( time, 0 );
    SetNetworkDelay(0);
    
    return gNetworkTime;
}
#endif

void SyncingUserInfo()
{

}


void SyncUserInfoComplete()
{

}


int ProgramSelected( UINT16 program_id )
{
	return 1;
}


void WIFIModeChanged( WIFI_MODE mode )
{

}
/*
void UpdatePWMCalibration( PWM_CAL_POINT calPoint[5] )
{
}
*/
void OnChangedUser( UINT8 userId )
{
}

bool GrantRemoteAccess()
{
	return true;
}


#endif

