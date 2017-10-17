#ifndef __DIGITAL_INTER_H__
#define __DIGITAL_INTER_H__

#include "Digital_CMD.h"
#include "Global_Config.h"

#define LCB_MOTOR                   1
#define LCB_EP_BIKE                 2
#define RETAIL_CLIMBMILL        3




typedef enum
{   //Common Command
    DS_INITIAL = 0,
    DG_STATUS,
    DG_ERRCODE,
    DG_VERSION,
    DS_CALIBRATE,
    DS_UPDATE,
    DS_SKIPERR,
    DS_EXTCMD,
    //TM COMMAND
    DS_RPM_RTN,
    DS_MOT_RPM,
    DS_ADD_STEP,
    DS_DEC_STEP,
    DS_STOP_STEP,
    DS_INC_ACTION,
    DS_INC_LOCATION,
    DS_WORK_STATUS,
    DG_ROLLER_RPM,
    DG_MOT_RPM,
    DG_INC_LOCATION,
    DG_DRVE_TYPE,
    DS_MOTOR_HP,
    //EP/BI COMMAND
    DS_ECB_ZERO,
    DS_ECB_LOCATION,
    DG_EPBI_RPM,
    DS_ECB_ACTION,
    DG_ECB_STATUS,
    DG_ECB_COUNT,
    DG_GET_TM_IN_USED,
    DS_INDUCTION_PWM,
    DS_INDUCTION_CALSAVEP1,
    DS_INDUCTION_CALSAVEP2,
    DS_INDUCTION_CALSAVEP3,
    DS_INDUCTION_CALSAVEP4,
    DS_INDUCTION_CALSAVEP5,
    DG_INDUCTION_CALP1,
    DG_INDUCTION_CALP2,
    DG_INDUCTION_CALP3,
    DG_INDUCTION_CALP4,
    DG_INDUCTION_CALP5,
    DS_EUP,
    DG_NEW_VERSION,
    DS_SAVE_PWM,
    DS_READ_PWM,
    
    //matrix retail climbmill 
    DG_READ_WATT,
    DG_READ_SPM,
    DS_SET_SPM,
    DG_READ_STATUS,
    DS_SET_STATUS,
    DS_SET_MACHINE_TYEP,
    DG_GET_LCB_DEVICE_DATE,
    DG_GET_TWO_CONTROL_DATA,
    DS_SET_CONTROL_DATA,
     
    CMD_LEN,
    CMD_NONE = 0XFF,
}COM_TYPE;


typedef enum
{
    G_ERRCODE = 0,
    G_VERSION,
    G_UPDATE_PRO,
    G_EXT_CMD,
    G_RPM_RTN,
    G_ROLLER_RPM,
    G_MOTOR_RPM,
    G_INC_LOCATION,
    G_DRVE_TYPE,
    G_EPBI_RPM,
    G_ECB_STATUS,
    G_ECB_COUNT,
    G_TM_IN_USED,
    G_INDUCTION_CALP1,
    G_INDUCTION_CALP2,
    G_INDUCTION_CALP3,
    G_INDUCTION_CALP4,
    G_INDUCTION_CALP5,
    G_LCB_ERPRESPOND,
    G_SAVE_PWM,
    G_READ_PWM,

    //matrix retail climbmill 
    G_READ_WATT,
    G_READ_SPM,
    G_READ_STATUS,
    G_GET_LCB_DEVICE_DATA,
    G_GET_TWO_CONTROL_DATA,
    //以下为无规律数据获取
    G_NEW_VERSION,
    G_MCB_TYPE,
    G_STATUS1,                  //开机时赋值0X80, 下控状态
    RX_DATA_LEN,
}RX_DATA_TYPE;


#define DEFAULT_TOPTEK_MAX_INC 205*128
#define DEFAULT_TOPTEK_MIN_INC 20*128

void LCB_Initial_IO(void);
void LCB_Initial(void);
void LCB_Power_On(UCHAR bWakeUP);
UCHAR LCB_Process(void);
UINT16 LCB_Dectect_Error(void);
UINT32 LCB_Get_Data(RX_DATA_TYPE en_Index);
void LCB_Clear_Data(RX_DATA_TYPE en_Index);
void LCB_Send_Cmd(COM_TYPE CmdType, UINT32 w_Data);
void LCB_1ms_Int(void);
void LCB_Set_DriverType(UCHAR DriverType);
UCHAR LCB_Get_MCBType(void);
UINT16 LCB_Get_Version(void);
void LCB_Get_Induct(UINT16 *pw);
void LCB_Set_SpeedRef(LCB_SPEED_REF *Ref);
void LCB_Erp(void);
UCHAR Digital_GetStatus();

#endif



