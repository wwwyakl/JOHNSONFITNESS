/*
 * wifi_event_handlers.h
 *
 *  Created on: May 23, 2013
 *      Author: brenden.capps
 */

#ifndef WIFI_EVENT_HANDLERS_H_
#define WIFI_EVENT_HANDLERS_H_

#include "jisdef.h"
#include <Digital_wifi.h>
#include "wifi_api.h"
#include "UtilitiesRtc.h"
#include <wifi_interface.h>

/* These functions implement the Event Handler API.
 * These are called from within the WiFi library when
 * certain events happen.  These need to be filled out
 * to glue the WiFi library to the console software
 * These may be different across projects
 */

/* This function is called when a valid start workout is sent to the console.
 * The console software should start a workout based on the parameters passed.
 * If the parameters are invalid or the workout is already in progress this
 * function should return 0.  Otherwise it should return a 1.
 */
int StartWorkout( WIFIDataProgramStart* program );

/* This function is called when a valid restart workout is sent to the console.
 * The console software should restart the workout.  If the workout is not paused
 * this function should return 0 otherwise it should return a 1.
 */
int RestartWorkout();

/*
 * This function is called when a valid stop workout is sent to the console.
 * The console software should stop any workout in progress immediately.
 */
int StopWorkout();

/* This function is called when a valid pause workout is sent to the console.
 * The console software should pause the workout.  If the workout is already
 * paused this function should return 0 otherwise it should return a 1.
 */
int PauseWorkout();

/* This function changes the treadmill speed.  The console should adjust any
 * program it is running accordingly.  The console should also beep.
 */
int SetSpeed( UINT16 speedX10, bool metric );

/* This function changes the incline during a workout.  The console should adjust
 * any program it is running accordingly.  The console should also beep if the new
 * incline is different.
 */
int SetIncline( UINT16 inclineX10 );

/* This function changes the resistance level during a workout.  The console should
 * adjust any program it is running accordingly.  The console should also beep if
 * the new incline is different.
 */
int SetResistance( UINT8 resistance );

/*This function is used to change the workout time of the running program.
 * The console should adjust its workout properties.
 */
int ChangeProgram( WIFIDataProgramStart* program );

/* This function is used to set the raw incline ADC.  The console software should
 * verify that the number is within range.
 */
int SetInclineADC( UINT16 adc );

/* This function is used to set the Treadmill RPM.  The console software should verify
 * that the number is within range.
 */
int SetRPM( UINT16 rpm );

/* This function is called to set the raw ECB counts.  There are some programs that need
 * to use this during a workout.  Because this places the program off the Watts table this
 * command passes the watts to display.  All other data should be based off of this number.
 */
int SetECB( UINT16 ecb );

/* This function is called to set the raw PWM value.  There are some programs that need to
 * use this during a workout.  Because this places the program off the Watts table this command
 * passes the watts to display.  All other data should be based off of this number.
 */
int SetPWM( UINT16 pwm );

/* This is used to simulate a key press.
 */
int PressKey( UINT8 key );
/*
 * This function is used to.....
 */
int Setup_Workout_Passport( UINT16 workout_time);

/*
 * This function is used to.....
 */
int Set_Goal_Passport( UINT16 Seconds);

/* This function is called when it is time to collect workout data for workout tracking.
 * The console software just needs to fill out the structure passed to this function
 * with the current values.
 */
void GetWorkoutData( WIFIWorkoutData* data );


/* This function is called when it is time to collect workout data for workout tracking.
 * The console software just needs to fill out the structure passed to this function
 * with the current values.
 */
void GetWorkoutDataRemote( WIFIWorkoutDataRemote* data );

/* This function is called when the clock needs to be updated due to a network time request.
 */
bool UpdateRTCTime( WIFIDataRegion* time, int offset );

/* This function is called to get the workout parameters to pass to the cloud for workout tracking.
 * The console software needs to fill in the structure with the correct values.
 */
void GetStartWorkoutData( WIFIDataExternalProgramStart* data );

/* This function is called at the end of the workout to get the workout total values.
 */
void GetWorkoutDataComplete( WIFIWorkoutDataComplete* data );

/* This function is called when the WIFI library determines that the user info needs
 * to be synced.  This happens when the console joins a WiFi network. */
void SyncingUserInfo();

/* This function is called when the user info has been synced. */
void SyncUserInfoComplete();

/* This function is called when a remote app selects a program.  If the program is
 * known the console should jump to the program setup otherwise it should sit in
 * a state waiting for Go to be pressed. */
int ProgramSelected( UINT16 program_id );

/* This function is called when the WIFI mode changes.  The valid modes are as follows.
0.	Idle
1.	Remote Control
2.	System Settings
3.	Test
Note that when the WiFi Mode is set to System Settings or Test the console cannot start a workout
 */
void WIFIModeChanged( WIFI_MODE mode );


/* This function is called when a remote device attempts to update
 * the PWM calibration.
 */
#if 0
void UpdatePWMCalibration( PWM_CAL_POINT calPoint[5] );
#endif
/* This function is called when a remote device changes the current user.
 */
void OnChangedUser( UINT8 userId );

/* This function is called when a remote device wants to change the emachine state.
 * Typically this requires the user to confirm the change by pressing a button.
 */
bool GrantRemoteAccess();


#ifdef __WIFI_NO_RTC__
 DateTime_T ReadNetworkTime(UINT16 offset);
#endif

void GetWorkoutDataEx( WIFIWorkoutDataEx* data );
void GetWorkoutDataCompleteEx( WIFIWorkoutDataCompleteEx* data );
void GetWorkoutDataSprint8Extra(WIFIWorkoutDataSprint8CompleteExtra* data);
void GetWorkoutDataFitnessTestExtra(WIFIWorkoutDataFitnessTestCompleteExtra* data);


#endif /* WIFI_EVENT_HANDLERS_H_ */
