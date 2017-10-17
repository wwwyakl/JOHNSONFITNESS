#ifndef _RF_LIB_H_
#define _RF_LIB_H_

#include "Programs.h"
#include "SystemConfig.h"
#include "HAL_Lib.h"
#include "CC_SPI.h"
#include "CC1100_CC2500_Definition.h"
#include "CC1100_CC2500.h"
#include "HAL_Lib.h"

//*****************************************************
//        The Index of the TX Buffer In Use Mode
//*****************************************************
#define TX_INUSE_ID               0
#define TX_INUSE_CHANNEL          1
#define TX_INUSE_CONSOLE_STATE    2
#define TX_INUSE_UNIT_MODE        3
#define TX_INUSE_MACHINE_MODE     4
#define TX_INUSE_RTC_MINUTE       5
#define TX_INUSE_RTC_HOUR         6
#define TX_INUSE_WORKOUT_TIME     7
#define TX_INUSE_DISTANCE         8
#define TX_INUSE_CALORIES         9
#define TX_INUSE_HEART_RATE       10
#define TX_INUSE_MAX_HEART        11
#define TX_INUSE_INCLINE          12
#define TX_INUSE_RESISTANCE       13
#define TX_INUSE_RPM              14
#define TX_INUSE_WATTS            15
#define TX_INUSE_SPEED            16
#define TX_INUSE_CURRENT_PACE     17
#define TX_INUSE_TARGET_WORKOUT_TIME  18

//*****************************************************
//        The Index of the TX Buffer In Idel
//*****************************************************
#define TX_IDLE_ID                  0
#define TX_IDLE_CHANNEL             1
#define TX_IDLE_CONSOLE_STATE       2
#define TX_IDLE_UNIT_MODE           3
#define TX_IDLE_MACHINE_MODE        4
#define TX_IDLE_RTC_MINUTE          5
#define TX_IDLE_RTC_HOUR            6
#define TX_IDLE_MAX_WORKOUT_TIME    7
#define TX_IDLE_MIN_WORKOUT_TIME    8
#define TX_IDLE_MAX_INCLINE         9
#define TX_IDLE_MIN_INCLINE         10
#define TX_IDLE_MAX_RESISTANCE      11
#define TX_IDLE_MIN_RESISTANCE      12
#define TX_IDLE_MAX_SPEED           13
#define TX_IDLE_MIN_SPEED           14
#define TX_IDLE_MAX_WEIGHT          15
#define TX_IDLE_MIN_WEIGHT          16
#define TX_IDLE_USER_COUNT          17
#define TX_IDLE_CONSOLE_SW_VERSION  18
#define TX_IDLE_RF_SW_VERSION       19
#define TX_IDLE_MACHINE_SERIAL      20
#define TX_IDLE_SCAN_CHANNEL_MASK   21
#define TX_IDLE_SCAN_CHANNEL        22
#define TX_IDLE_SYNC_ID             23
#define TX_IDLE_SYNC_CHANNEL        24

//*****************************************************
//        The Index of the TX Buffer In Finish
//*****************************************************
#define TX_FINISH_ID                   0
#define TX_FINISH_CHANNEL              1
#define TX_FINISH_CONSOLE_STATE        2
#define TX_FINISH_UNIT_MODE            3
#define TX_FINISH_MACHINE_MODE         4
#define TX_FINISH_RTC_MINUTE           5
#define TX_FINISH_RTC_HOUR             6
#define TX_FINISH_RTC_DAY              7
#define TX_FINISH_RTC_MONTH            8
#define TX_FINISH_RTC_YEAR             9
#define TX_FINISH_TOTAL_WORKOUT_TIME   10
#define TX_FINISH_TOTAL_DISTANCE       11
#define TX_FINISH_TOTAL_CALORIES       12
#define TX_FINISH_AVERAGE_HEART_RATE   13
#define TX_FINISH_AVERAGE_PACE         14

//*****************************************************
//        The Offset of the RX Buffer
//*****************************************************
#define RX_ID                          0
#define RX_CHANNEL                     1
#define RX_STB_STATE                   2
#define RX_WEIGHT                      3
#define RX_AGE                         4
#define RX_TARGET_WORKOUT_TIME         5
#define RX_STARTUP_SPEED               6
#define RX_STARTUP_INCLINE             7
#define RX_STARTUP_RESISTANCE          8
#define RX_TERRAIN_INCLINE             9
#define RX_TERRAIN_RESISTANCE          10
#define RX_RESET_DEFAULT               11
#define RX_USER_NUMBER                 12
#define RX_SPI_NUM                     13
#define RX_RF_NUM                      14
#define RX_SYNC_ID                     15
#define RX_SYNC_CHANNEL                16
//*****************************************************
//        RF State
//*****************************************************

#define RF_STATE_NORMAL      0
#define RF_STATE_SYNC          1

#define RF_TX_BEGIN          1

#define TX_PACKET_LENGTH     34
#define RX_PACKET_LENGTH     33

#define RX_ID_PACKET_LENGTH  3


#define SYNC_OFFLINE		   0
#define SYNC_STAGE_START       1
#define SYNC_PARTNERSHIP       3
#define SYNC_SCAN              5
#define SYNC_WAITING           6
#define SYNC_FINISH            7
#define SYNC_TIMEOUT           8

#define SYNC_PARTNERSHIP_OK    15


#define ID_PARTNERSHIP_DELAY_TIME    800   //800 micro seconds.
#define ID_PARTNERSHIP_GDO2_DELAY    50


#define CHANNEL_NUMBER    7

#define RF_PARTNERSHIP_ADDRESS  0xaa


#define D_STATE_IDLE            0x01
#define D_STATE_IN_USE          0x02
#define D_STATE_PAUSE           0x03
#define D_STATE_FINISH          0x04
#define D_STATE_SETTING         0x0a


typedef enum
{
    D_STARTUP_DATA_WEIGHT=0,
    D_STARTUP_DATA_AGE,
    D_STARTUP_DATA_WORKOUTTIME,
    D_STARTUP_DATA_SPEED,
    D_STARTUP_DATA_INCLINE,
	D_STARTUP_DATA_RESISTANCE,
    D_STARTUP_DATA_USERNUMBER,
	D_STARTUP_DATA_ALL_DATA
}RF_D_STARTUP_DATA;

typedef enum
{
    MODE_TERRAIN_INCLINE = 0,
    MODE_TERRAIN_RESISTANCE,
    MODE_TERRAIN_ALL_TYPE
}RF_TERRAIN_TYPE;


#define PERCENT_SELFERROR    0
#define RERCENT_RFLOST       1

typedef enum
{
    TX_MODE_INUSE = 0,
    TX_MODE_FINISH,
    TX_MODE_IDLE,
    TX_MODE_ALL
}RF_TX_MODE;


typedef enum
{
    RF_NO_EVENT = 0 ,
    RF_ONLINE,
    RF_OFFLINE,
    RF_SYNC_ING,
    RF_SYNC_FINISH,
    RF_SYNC_TIMEOUT,
	RF_TERRAIN_CHANGED,
    RF_RESISTANCE_CHANGED,
    RF_TERRAIN_RES_CHANGED,
	RF_USER_CHANGED,
    RF_SYNC_DATA_WHEN_STARTUP,
	RF_ALL_EVENT
}RF_EVENT;

/***************************************************************************//**
* @brief
*    RF_EVENT RF_AP_Process(void);
*
* @note
*    RF main loop
*    ����ʱ��ö��RF_EVENT�����£�
*    RF_ONLINE----------------------RF����
*    RF_OFFLINE---------------------RF����
*    RF_SYNC_ING--------------------RFͬ����
*    RF_SYNC_FINISH-----------------RFͬ�����
*    RF_SYNC_TIMEOUT----------------RFͬ����ʱ
*    RF_TERRAIN_CHANGED-------------RF�¶ȸı�
*    RF_RESISTANCE_CHANGED----------RF�����ı�
*    RF_USER_CHANGED----------------RF��ǰuser�����ı�
*    RF_SYNC_DATA_WHEN_STARTUP------RF�˶���ʼʱ����Ҫ�Ǳ�ͬ��RF���õ��˶���Ϣ
*    RF_NO_EVENT-----------------���¼�
* @param[in]��none
* @param[out]��RF_EVENT  
******************************************************************************/
RF_EVENT RF_AP_Process(void);
/***************************************************************************//**
* @brief
*    RF_AP_Initial_Data
*
* @note
*    ��ʼ��RF��һЩ�Ĵ���
*
* @param[in]��none
* @param[out]��none  
******************************************************************************/
void RF_AP_Initial_Data(void);
/***************************************************************************//**
* @brief
*    RF_AP_SYNC_Process_Start
*    ����SYNC�����󣬵��øú���ʵ���Ǳ���passport��ͬ��
* @note
*    
*
* @param[in]��none
* @param[out]��none  
******************************************************************************/
void RF_AP_SYNC_Process_Start(void);
/***************************************************************************//**
* @brief
*    RF_AP_Get_STB_StartUp_Data
*    ��ȡpassport������
* @note
*    D_STARTUP_DATA_WEIGHT-----------��ȡpassport���õ����أ�����ʹ�ã�
*    D_STARTUP_DATA_AGE--------------��ȡpassport���õ����䣨����ʹ�ã�
*    D_STARTUP_DATA_WORKOUTTIME------��ȡpassport���õ��˶�ʱ��
*    D_STARTUP_DATA_SPEED------------��ȡpassport���õ��ٶ�
*    D_STARTUP_DATA_INCLINE----------��ȡpassport���õ�����
*	 D_STARTUP_DATA_RESISTANCE-------��ȡpassport���õ�����
*    D_STARTUP_DATA_USERNUMBER-------��ȡpassport���õĵ�ǰ�û�
*
* @param[in]��RF_D_STARTUP_DATA
* @param[out]��passport���õ������Ϣ 
******************************************************************************/
unsigned int RF_AP_Get_STB_StartUp_Data(RF_D_STARTUP_DATA by_Mode);
/***************************************************************************//**
* @brief
*    RF_AP_Get_STB_Terrain_Data
*    ��ȡpassportӰƬ�е��Ƶı仯��Ӧ���������������ı仯
* @note
*    MODE_TERRAIN_INCLINE-----------��ȡ������Ϣ
*    MODE_TERRAIN_RESISTANCE--------��ȡ������Ϣ
*
* @param[in]��RF_TERRAIN_TYPE
* @param[out]���������������ı仯���� 
******************************************************************************/
unsigned int RF_AP_Get_STB_Terrain_Data(RF_TERRAIN_TYPE by_Mode);

/***************************************************************************//**
* @brief
*    RF_Registe_Function
*    ע��RF�õ���������
* @note
*    RF_AP_1ms_Timerout_IntHandle
*    RF_GDO0_Int_ISR
*    RF_GDO2_Int_ISR
* @param[in]��none
* @param[out]��none  
******************************************************************************/
void RF_Regist_Functions(void);


void RF_Set_VirtualKey(UINT16 vk);
#endif
