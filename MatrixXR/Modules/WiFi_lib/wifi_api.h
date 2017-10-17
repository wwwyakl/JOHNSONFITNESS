/*
 * wifi_api.h
 *
 *  Created on: Jun 19, 2013
 *      Author: Brenden.Capps
 ****************************
 *
 *
 * Modified on:Apr 16,2015
 *	 By:Peter Fu 
 *
 */

#ifndef WIFI_API_H_
#define WIFI_API_H_
#include "jisdef.h"
#include "Global_Config.h"

#ifdef __WIFI_FUNC__

#undef  __WIFI_DMA__
#undef  __WIFI_RX_DMA__
#undef  __WIFI_NO_RTC__
#undef __RSCU_UPDATE__



#define __WIFI_TEST_PETER__
#ifdef __WIFI_TEST_PETER__
	//#define __CLOSE_SSL__
#endif
#define __WIFI_DMA__
//#define __WIFI_RX_DMA__
#define __WIFI_NO_RTC__
#define __RSCU_UPDATE__
#define __WIFI_RESEND_0502__


#define TIME_OUT_DELAY 5000
#define RESEND_WIFI_0502_X10MS 300
#define MESSAGE_TIME_OUT_DELAY 500
#define TIME_ACTIVE_DELAY 500
#define WIFI_UPDATE_VALID_FLAG 0xAA

/* For Dapi Version 10 Start */

typedef enum{
    DEPLOYMENT_TYPE_START,
    DEPLOYMENT_DEVELOPMENT = DEPLOYMENT_TYPE_START,
    DEPLOYMENT_INTERNAL_TESTING,
    DEPLOYMENT_QA,
    DEPLOYMENT_PRODUCTION,
    DEPLOYMENT_TYPE_END
}DEPLOYMENT_TYPE;

typedef enum{
    SOFTWARE_TYPE_START,
    SOFTWARE_TYPE_GUI = SOFTWARE_TYPE_START,
    SOFTWARE_TYPE_BOOTLOADER,
    SOFTWARE_TYPE_NON_MCU,
    SOFTWARE_TYPE_NON_SUB_MCU,
    SOFTWARE_TYPE_LCB1,
    SOFTWARE_TYPE_LCB2,
    SOFTWARE_TYPE_LCM,
    SOFTWARE_TYPE_WIFI,
    SOFTWARE_TYPE_BLUETOOTH,
    SOFTWARE_TYPE_RFID,
    SOFTWARE_TYPE_NFC,
    SOFTWARE_TYPE_END
}SOFTWARE_TYPE;

/* For Dapi Version 10 end */

typedef enum{
    WIFI_EVENT_START,
    WIFI_EVENT_IDLE= WIFI_EVENT_START,
    WIFI_EVENT_PROCESSED,
    WIFI_EVENT_OFFLINE,
    WIFI_EVENT_LOGIN,
    WIFI_EVENT_SYNC_USER,
    WIFI_EVENT_SUCCESS,
    WIFI_EVENT_FAIL,
    WIFI_EVENT_WAITTING,
    WIFI_EVENT_TIMEOUT,
    WIFI_EVENT_UPDATE_FAIL,
    WIFI_EVENT_ACTIVATED,
    WIFI_EVENT_AP_MODE,
    WIFI_SIGNAL_CHANGED,
//    WIFI_EVENT_RSCU_CHECKING,
//    WIFI_EVENT_DOWNLOADING_SOFTWARE,
//    WIFI_EVENT_DOWNLOAD_COMPLETED,
//    WIFI_EVENT_UPDATE_VALID,
    WIFI_EVENT_UPDATE_CANCELED,
    WIFI_EVENT_UPDATE_CONFIRMED,
    WIFI_EVENT_INPUT_TIMEZONE, 
    WIFI_EVENT_TIMEZONE_COMPLETE,
    WIFI_EVENT_NO_MAC, 
    WIFI_EVENT_NO_IP,
    WIFI_EVENT_WIFI_IS_OK,
    WIFI_EVENT_AVALIABLE_UPDATE,
    WIFI_EVENT_UPDATE_PROGRESS,
    WIFI_EVENT_UPDATE_COMPLETED,
    WIFI_EVENT_END
}WIFI_EVENT_HANDLE;



/*********************** Run API ************************************************************
 *
 * This defines the Run API.  Use Digital_Initial_Data_wifi at bootup and call
 * Digital_Command_Proce_wifi every time through the main loop.
 */

/* This function needs to be called at startup.  This will initialize the WiFi hardware
 * and setup the WiFi module. */
void Digital_Initial_Data_wifi(void);


/* This function should be called every time through the main loop.  Its primary job is
 * to check for incoming messages and handle them properly.  It also takes care of the
 * workout tracking and asset management features during a workout */

/*********************** End Run API ********************************************************/



/*********************** Send API ************************************************************
 *
 * The following defines the Send API used by the console software
 * to command the WiFi library.
 */

/*********************For Remote Control Start***************************************
*
*The following defines the Remote Control API used by the console software
*to command the remote port
*
*/

typedef enum
{
	WIFI_IDLE,
	WIFI_REMOTE_CONTROL,
	WIFI_SETTINGS_MODE,
	WIFI_TEST_MODE
} WIFI_REMOTE_MODE;

typedef enum
{
	WIFI_WORKOUT_IDLE,
	WIFI_WORKOUT_RUNNING,
	WIFI_WORKOUT_PAUSED,
	WIFI_WORKOUT_STOP
} WORKOUT_STATE;

/* This function should be called by the main loop whenever the user selects a program.
 * This is used to keep a remote app in sync with the console. */
void SelectProgram( PROGRAM_ID programtype );


/* This function should be called by the main loop whenever the user presses a button during a workout.
 * The machine should handle the button as normal (e.g., decrease speed on speed down), but it must also
 * send the key code to remote apps.*/
 /*The specil keycode is defined.
 0x50      Safekey Removed
 0x51      Safekey Inserted
 0x60      authorize control to remote apps,if Console had started workouts
 */
void KeyPressed( UINT8 keycode );

/* This function returns the current mode.  The valid modes are as follows.
0.	Idle
1.	Remote Control
2.	System Settings
3.	Test
Note that when the WiFi Mode is set to System Settings or Test the console cannot start a workout.*/
WIFI_REMOTE_MODE GetWifiMode();







/*********************For Remote Control End*************************************************/




typedef enum{
    MARIO_BOX_TYPE_START,
    MARIO_BOX_PRODUCTION= MARIO_BOX_TYPE_START,
    MARIO_BOX_TEST,
    MARIO_BOX_STAGING,
    MARIO_BOX_TYPE_END
}MARIO_BOX_TYPE;

/* This function tries to sync the network time with the cloud. */
void UpdateNetworkTime();


/* This function tries to sync the machine’s user info time with the cloud.
 * This function returns immediately.  The console software should use the
 * function CheckUserInfoUpdated() to see if data has been returned */
void UpdateUserInfo();


/* This function should only be called after a call to UpdateUserInfo.
 * It returns a 0 if still processing, a 1 if successful.  All other
 * values indicate an error. */
UINT8 CheckUserInfoUpdated();


/* This function tries to sync the machine’s info time with the cloud */
void UpdateMachineInfo();


/* This function attempts to register a new user with the cloud.
 * The code will need to use checkRegistration to see the result
 * as the function returns immediately and it may take several
 * seconds to get a result.  The passcode is 10 bytes wide and the
 * pin is 4 bytes wide.*/
//void RegisterUser( unsigned char userId, unsigned char* passcode, unsigned char* pin );


/* This function returns the result of the registration.
 * A 0 means it is still processing.
 * A 1 means success and anything else is an error.*/
//UINT8 CheckRegistration();


/* This function asks the cloud if the passcode is available.  The
 * code will need to use CheckLoginIdResult to see the result as
 * the function returns immediately and it may take several seconds
 * to get a result.  The passcode is 10 bytes wide.*/
//void CheckPasscodeUnique( unsigned char* passcode );


/* This function returns the result of the passcode unique check.
 * A 0 means it is still processing.  A 1 means success and anything
 * else is an error.*/
//UINT8 CheckPasscodeUniqueResult();


/* This function attempts to login.  The code will need to use checkLoginResult
 * to see the result as the function returns immediately and it may take several
 * seconds to get a result.  The userId is the console user id (1-4 typically).
 * The passcode is 10 bytes wide.  On success the WiFi library will update the
 * UUID and user info automatically.*/
void Login( unsigned char userId, unsigned char* passcode, unsigned char* pin );


/* This function returns the result of the login.  A 0 means it is still processing.
 * A 1 means success and anything else is an error.*/
UINT8 CheckLoginResult();


/* This function is used to let the WiFi library know the current workout state.
 * The valid states are as follows.
0.	Idle
1.	Running
2.	Pause
3.	Stop*/
void ChangeWorkoutState( WORKOUT_STATE state );


/* This function enables the Access Point mode on the WiFi module.  It also
 * sets the SSID according to the parameters input. */
//void EnableAP( char* ssid, UINT16 ssid_length );


/* This function turns the WIFI module Access Point off. */
//void DisableAP();


/* This function allows the console software to connect to a specific network.
 * The WIFI module takes care of determining the security type.  For unsecured
 * networks pass in an empty security key to this function. */
//void ConnectWIFI( const char* ssid, UINT16 ssid_length, const char* security_key, UINT16 security_key_length ) ;


/* This function will disconnect the machine from the WIFI network */
//void DisconnectWIFI();


/* This function should be called within an interrupt when data is received. */
//void ReceivedDataInterruptHandler();


/* This funciton should be used to register a machine that has not been setup.  This is most likely not needed
 * as the registration will happen through the WIFI module.
 */
//void RegisterMachine();


/* This function clears all of the users that are linked to this machine in the cloud.
 * It also resets the wifi module to access point mode.
 * This should be called when the user restores the machine to factory defaults.
 */
void RestoreWifiToFactoryDefaults();

/* This function is used to let the WiFi library know the current network state.
 * The valid states are as follows.
0.	OffLine
1.	Online
*/
//UINT8 WIFIIsOnline(void);

//UINT8 WIFIIsAPMode(void);

UINT8 WIFISignalStrength(void);

void WIFIDeleteUser(char index);

UINT8 WifiSetMarioBox(MARIO_BOX_TYPE index);

MARIO_BOX_TYPE WifiGetCurrentMarioBox(void);
MARIO_BOX_TYPE WifiGetMarioBox(UINT8 index);

void WifiEditUser (UINT8 userindex,UINT16 weight,UINT8 age,UINT8 gender);

//void wifiSetTimeZone(UCHAR timeZoneIndex);
WIFI_EVENT_HANDLE wifiGetEvent(void);
void wifiSetFirmwareUpdate(DEPLOYMENT_TYPE deploy , SOFTWARE_TYPE software);
UINT8 WiFi_Get_Update_Percent(void);
#ifdef __WIFI_TEST_PETER__
void InitTest(void);

UINT16 WIFI_Get_uartLostCounter(void);
UINT16 WIFI_Get_uartHadSent(void);
UINT16 WIFI_Get_recvResponse(void);
UINT16 WIFI_Get_setHttpDataResponse(void);
UINT16 WIFI_Get_pushHttpDataResponse(void);
#endif
/*********************************************/

#endif

/* End Send API */

#endif /* WIFI_API_H_ */
