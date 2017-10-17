#include "LCB.h"
#include "HAL_Lib.h"
#include "SystemConfig.h"


typedef enum
{
    LCB_INIT_LCB    = 1,   
    LCB_ERP,
    LCB_IDLE,
    LCB_ERROR,
    LCB_STATE_COUNT,
}LCB_STATE;

typedef enum{ErrGet = 0, ErrHdle, ErrSkip, ErrWaitSkipACK, ErrIDLE}  ERR_STEP;


typedef struct
{
    UCHAR       Cmd;          //对应的Cmd
    UCHAR       RxOffset;     //
    UCHAR       RxLen;
}RX_DATA_OFFSET;


typedef struct
{
    UCHAR Cmd;
    UCHAR TxLen;
    UCHAR RxLen;
}CMD_PROPERTY;


typedef struct
{
    COM_TYPE    CmdType;
    UINT32      Data;
}CMD_BUFF;


typedef struct
{
    COM_TYPE    CmdType;   //对应的Cmd种类
    UCHAR       subcommand;
}RX_DATA_PROPERTY;


//typedef enum{DctErr = 0, ErrGet, ErrHdle, ErrSkip}  ERR_STEP;

static const CMD_PROPERTY Cmd_Talbe[CMD_LEN] = 
{   //cmd,  T,  R,  Index
    //Common Command
    {0x70,  0,  0 },//cmd initial
    {0x71,  0,  0 },//get status
    {0x72,  0,  2 },//get err code
    {0x73,  0,  2 },//get version
    {0x74,  0,  0 },//calibrate
    {0x75,  2,  1 },//update program
    {0x76,  0,  0 },//skio err
    {0x77,  1,  1 },//ext cmd
    //TM Command
    {0xf0,  2,  2 },//set Rpm max
    {0xf1,  2,  0 },//set Motor Rpm
    {0xf2,  2,  0 },//set add step 
    {0xf3,  2,  0 },//set dec step
    {0xf4,  2,  0 },//set stop step
    {0xf5,  1,  0 },//set inc action
    {0xf6,  2,  0 },//set inc location
    {0xf7,  1,  0 },//set work status
    {0xf8,  0,  2 },//get roller rpm
    {0xf9,  0,  2 },//get motor rpm
    {0xfa,  0,  2 },//get inc location
    {0x91,  0,  1 },//get Driver type
    {0x90,  2,  0 },//set motor hp
    //EP/BI Command   
    {0x61,  0,  0 },//set ECB Zero
    {0x62,  2,  0 },//set ECB Location
    {0x63,  0,  2 },//get EPBI Rpm
    {0x82,  1,  0 },//set ECB Action
    {0x83,  0,  1 },//get ECB status
    {0x84,  0,  2 },//get ECB count
    {0x95,  0,  1 },//get tm in used status
    {0x65,  2,  2 },//set inductor brake PWM
    {0x88,  2,  2 },//Save Calibration Point 1 PWM
    {0x89,  2,  2 },//Save Calibration Point 2 PWM
    {0x8A,  2,  2 },//Save Calibration Point 3 PWM  
    {0x8B,  2,  2 },//Save Calibration Point 4 PWM
    {0x8C,  2,  2 },//Save Calibration Point 5 PWM
    {0x8D,  0,  2 },//Get Calibration Point 1 PWM       
    {0x8E,  0,  2 },//Get Calibration Point 2 PWM
    {0x8F,  0,  2 },//Get Calibration Point 3 PWM
    {0x90,  0,  2 },//Get Calibration Point 4 PWM  
    {0x91,  0,  2 },//Get Calibration Point 5 PWM 
    {0x7A,  1,  1 },//CmdEUPsMode
    {0x50,  1,  6 },//Get Version New
    {0x50,  4,  2 },//Save PWM
    {0x50,  2,  3 },//Read PWM
    
    //matrix retail climbmill 
    {0x8E,  0,  2 },//READ_WATT
    {0x4B,  0,  2 },//READ_SPM  
    {0x4A,  2,  0 },//SET_SPM  
    {0x8B,  0,  2 },//READ_STATUS 
    {0x8A,  1,  0 },//SET_STATUS     
    {0x6C,  1,  0 },//SET_MACHINE_TYPE
    {0x50,  3,  5 },//DG_GET_LCB_DEVICE_DATE
    {0x50,  3,  6 },//DG_GET_TWO_CONTROL_DATA
    {0x8C,  1,  0 },//DS_SET_CONTROL_DATA
};


static const RX_DATA_PROPERTY RxDataOffset[RX_DATA_LEN] =
{
    { DG_ERRCODE,           0 },     //ERROR CODE
    { DG_VERSION,           0 },     //VERSION
    { DS_UPDATE,            0 },     //UPDATE
    { DS_EXTCMD,            0 },     //EXT_CMD
    { DS_RPM_RTN,           0 },     //RPM_RTN
    { DG_ROLLER_RPM,        0 },     //ROLLER RPM
    { DG_MOT_RPM,           0 },     //MOTOR RPM
    { DG_INC_LOCATION,      0 },     //INC LOCATION
    { DG_DRVE_TYPE,         0 },     //DRIVER_TYPE
    { DG_EPBI_RPM,          0 },     //GET EPBI RPM
    { DG_ECB_STATUS,        0 },     //GET ECB STATUS
    { DG_ECB_COUNT,         0 },     //GET ECB COUNT
    { DG_GET_TM_IN_USED,    0 },     //GET TM IN USED STATUS
    { DG_INDUCTION_CALP1,   0 },     //GET CALIBRATION POINT 1 PWM
    { DG_INDUCTION_CALP2,   0 },     //GET CALIBRATION POINT 2 PWM
    { DG_INDUCTION_CALP3,   0 },     //GET CALIBRATION POINT 3 PWM   
    { DG_INDUCTION_CALP4,   0 },     //GET CALIBRATION POINT 4 PWM     
    { DG_INDUCTION_CALP5,   0 },     //GET CALIBRATION POINT 5 PWM 
    { DS_EUP,               0 },     //ERP SLEEP AND WAKEUP
    { DS_SAVE_PWM,          0 },     //SAVE PWM
    { DS_READ_PWM,          0 },     //READ PWM

    
    //matrix retail climbmill 
    { DG_READ_WATT,         0 },     //G_READ_WATT 
    { DG_READ_SPM,          0 },     //G_READ_SPM
    { DG_READ_STATUS,       0 },     //G_READ_STATUS
    { DG_GET_LCB_DEVICE_DATE,0},
    { DG_GET_TWO_CONTROL_DATA,0x07},//25
    
    { DG_NEW_VERSION,       0xF0 },     //READ NEW MCB VERSION
    { CMD_NONE,             0 },     //READ NEW MCB TYPE ACCORD COMMAND G_NEW_VERSION - 到 G_NEW_VERSION 数组中提取
    { CMD_NONE,             1 },     //STATUS1
};

#define BIT0                (0x00000001)
#define BIT1                (0x00000002)
#define BIT2                (0x00000004)
#define BIT3                (0x00000008)
#define BIT4                (0x00000010)
#define BIT5                (0x00000020)
#define BIT6                (0x00000040)
#define BIT7                (0x00000080)
#define BIT8                (0x00000100)
#define BIT9                (0x00000200)
#define BITA                (0x00000400)
#define BITB                (0x00000800)
#define BITC                (0x00001000)
#define BITD                (0x00002000)
#define BITE                (0x00004000)
#define BITF                (0x00008000)

#define OVERSPEED_D_VALUE	340 //1190
#define OVERSPEED_CONFIRMTIMES	5

#define LCB_ERP_LEAVE       0x00
#define LCB_ERP_ENTER       0xFF
#define LCB_ERP_ACK_OK      0x01
#define LCB_ERP_ACK_FAIL    0x00
#define LCB_ERP_ACK_WAIT    0xA1
#define LCB_SPEED_TIMER     1000//800
#define LCB_OVERSPEED_TIMER     200//800

#define LCB_RPM_MAX         5000
#define LCB_RPM_MIN         85
#define LCB_RPM_ADD_STEP    137//110
#define LCB_RPM_DEC_STEP    145//130

#define MCB_ADDRS           0xFF
#define NO_PAR              0
#define C_TX_BUFF_SIZE      20          //命令缓存个数
#define C_LCB_STATE_SIZE    20          //状态缓存个数
#define C_RX_LENGHT         20          //最大接收字节长度
#define C_DATA_LEN          80          //接收数据保存buff大小
#define C_KEEP_ONLINE       400//500         //ms
#define C_KEEP_ONLINE_IDLE  200//300         //ms
#define C_LOST_TIME         5000        //ms
#define C_MCB_ERROR         BIT6
#define C_MCB_INITIALING    BIT7

__no_init static UCHAR aby_Total[C_DATA_LEN];                                   //接收数据
__no_init static UCHAR by_Command_Point;
__no_init static UCHAR by_Command_End;                                          //待发送命令buff 最后一个位置
__no_init static UINT16 w_KeepOnlineTimer;
__no_init static RX_DATA_OFFSET Offset_Adress[RX_DATA_LEN];
__no_init static CMD_BUFF Cmd_Buffer[C_TX_BUFF_SIZE];                           //待发送命令buff
__no_init static CMD_RX CmdRx;
__no_init static ERR_STEP ErrStep;
__no_init static UINT16 w_ErrorCode;
__no_init static UINT16 w_CommonTimer;
__no_init static UINT16 w_SpecialTimer;
__no_init static UINT16 w_SpeedTimer;
__no_init static UINT16 w_OverSpeedCheckTimer;
__no_init static UINT16 w_LostTimer;
__no_init static UCHAR by_State_Point;
__no_init static UCHAR by_State_End;
__no_init static UCHAR bInited;                                                 //是否已完成初始化
__no_init static LCB_STATE LcbStateBuff[C_LCB_STATE_SIZE];
__no_init static UCHAR  by_DriverType;
__no_init static UCHAR  by_MCBType;
__no_init static UINT16 w_MCBVersion;
__no_init static LCB_SPEED_REF SpeedRef;
__no_init static UINT16 w_CurrentSpeed;
__no_init static UINT16 w_TargetSpeed;
__no_init static UINT16 w_InductData[5];
__no_init static struct
{
    unsigned ERP_Ack        :1;
    unsigned ErrSkip_Ack    :1;
    unsigned Cmd_Ack        :1;
}LCB;


void LCB_CmdPack(COM_TYPE CmdType, UINT32 w_Data);
void LCB_CmdProcess(void);
void LCB_Next_State(void);
void LCB_Speed_Update(void);
void LCB_Add_State(LCB_STATE State);;


void LCB_Initial(void)
{
    Digital_Cmd_Initial();
    w_CurrentSpeed = 0;
    w_TargetSpeed = 0;
    by_Command_End = 0;
    by_Command_Point = 0;
    w_KeepOnlineTimer = 0;
    w_CommonTimer = 0;
    w_SpecialTimer = 0;
    w_SpeedTimer = 0;
	w_OverSpeedCheckTimer = 0 ;
    w_LostTimer = 0;
    by_State_Point = 0;
    by_State_End = 0;
    
    memset(Cmd_Buffer, CMD_NONE, sizeof(Cmd_Buffer));
    memset(LcbStateBuff, LCB_IDLE, sizeof(LcbStateBuff));
    memset(aby_Total, 0, sizeof(aby_Total));
    
    UCHAR by_Offset = 0;
    for(UCHAR i=0; i<RX_DATA_LEN; i++)
    {
        Offset_Adress[i].RxOffset = by_Offset;
        
        if(RxDataOffset[i].CmdType != CMD_NONE)
        {
            Offset_Adress[i].RxLen = Cmd_Talbe[RxDataOffset[i].CmdType].RxLen;
        }
        else
        {
            Offset_Adress[i].RxLen = 0;
        }
        
        Offset_Adress[i].Cmd = Cmd_Talbe[RxDataOffset[i].CmdType].Cmd;
        by_Offset += Offset_Adress[i].RxLen;
    }
    
    aby_Total[Offset_Adress[G_STATUS1].RxOffset] = C_MCB_INITIALING;            //默认下控在初始化中
    aby_Total[Offset_Adress[G_DRVE_TYPE].RxOffset] = 0x5a;
}

UCHAR LCB_Process(void)
{
    UCHAR by_Out = 0;
    
    switch(LcbStateBuff[by_State_Point])
    {
    case LCB_INIT_LCB:
        {
            LCB_Initial();
            w_CommonTimer = 0;
            w_SpecialTimer = 0;
            LCB_Send_Cmd(DS_INITIAL, 0);
            
            while(LCB_Get_Data(G_STATUS1) & C_MCB_INITIALING) 
            {
                LCB_CmdProcess();
                
                if(w_CommonTimer >= 300) 
                {
                    w_CommonTimer = 0;
                    LCB_Send_Cmd(DS_INITIAL, 0); 
                }
                
                if(w_SpecialTimer >= 5000) break;                               //5秒后退出 
#ifdef WDT_ENABLE
                HAL_Clear_WatchDog();
#endif       
            }
            
            LCB_Send_Cmd(DS_WORK_STATUS, 0);
            LCB_Send_Cmd(DS_WORK_STATUS, 0x01); 
            w_CommonTimer = 0;
            while(w_CommonTimer < 500)
            {
                LCB_CmdProcess();
#ifdef WDT_ENABLE
                HAL_Clear_WatchDog();
#endif  
            }
            
            //读版本号
            UINT16 wVersion = 0;
            LCB_Send_Cmd(DG_VERSION, 0);
            w_CommonTimer = 0;
            
            while(w_CommonTimer < 2000)
            {
                LCB_CmdProcess();
                if(LCB_Get_Data(G_VERSION) != 0x00) break;
#ifdef WDT_ENABLE
                HAL_Clear_WatchDog();
#endif 
            }
            
            wVersion = LCB_Get_Data(G_VERSION);
            
            if(wVersion == 0xFF00)
            {
                LCB_Send_Cmd(DG_NEW_VERSION, 0xf0);
                w_CommonTimer = 0;
                while(w_CommonTimer < 2000)
                {
                    LCB_CmdProcess();
                    if(LCB_Get_Data(G_NEW_VERSION) != 0x00) break;
                }
                
                by_MCBType = (UCHAR)LCB_Get_Data(G_MCB_TYPE);
                w_MCBVersion = LCB_Get_Data(G_NEW_VERSION);
#ifdef WDT_ENABLE
                HAL_Clear_WatchDog();
#endif 
            }
            else
            {
		by_MCBType = (wVersion&0xFF00)>>8;
		w_MCBVersion = wVersion;
            }
            
            bInited = TRUE;

            if(by_MCBType != 0x00 && (by_MCBType == ECB_INDUCTOR_BRAKE
                                      || by_MCBType == ECB_JIS
                                          || by_MCBType == BIKE_0C_JIS
                                              || by_MCBType == INDUCTOR_BRAKE_INCLINE))
            {
                memset(w_InductData, 0, sizeof(w_InductData));
                
                if(wVersion == 0xFF00)
                {
                    LCB_Clear_Data(G_READ_PWM);
                    LCB_Send_Cmd(DS_READ_PWM, 0x8100|0x0001);
                    w_CommonTimer = 0;
                    while(w_CommonTimer < 500)
                    {
                        LCB_CmdProcess();
                        w_InductData[0] = LCB_Get_Data(G_READ_PWM);
                        if(w_InductData[0] != 0x0000) break;
#ifdef WDT_ENABLE
                    HAL_Clear_WatchDog();
#endif 
                    }
                    
                    
                    LCB_Clear_Data(G_READ_PWM);
                    LCB_Send_Cmd(DS_READ_PWM, 0x8100|0x0002);
                    w_CommonTimer = 0;
                    while(w_CommonTimer < 500)
                    {
                        LCB_CmdProcess();
                        w_InductData[1] = LCB_Get_Data(G_READ_PWM);
                        if(w_InductData[1] != 0x0000) break;
 #ifdef WDT_ENABLE
                    HAL_Clear_WatchDog();
#endif 
                    }
                    
                    LCB_Clear_Data(G_READ_PWM);
                    LCB_Send_Cmd(DS_READ_PWM, 0x8100|0x0003);
                    w_CommonTimer = 0;
                    while(w_CommonTimer < 500)
                    {
                        LCB_CmdProcess();
                        w_InductData[2] = LCB_Get_Data(G_READ_PWM);
                        if(w_InductData[2] != 0x0000) break;
#ifdef WDT_ENABLE
                    HAL_Clear_WatchDog();
#endif 
                    }
                    
                    LCB_Clear_Data(G_READ_PWM);
                    LCB_Send_Cmd(DS_READ_PWM, 0x8100|0x0004);
                    w_CommonTimer = 0;
                    while(w_CommonTimer < 500)
                    {
                        LCB_CmdProcess();
                        w_InductData[3] = LCB_Get_Data(G_READ_PWM);
                        if(w_InductData[3] != 0x0000) break;
#ifdef WDT_ENABLE
                    HAL_Clear_WatchDog();
#endif 
                    }
                    
                    LCB_Clear_Data(G_READ_PWM);
                    LCB_Send_Cmd(DS_READ_PWM, 0x8100|0x0005);
                    w_CommonTimer = 0;
                    while(w_CommonTimer < 500)
                    {
                        LCB_CmdProcess();
                        w_InductData[4] = LCB_Get_Data(G_READ_PWM);
                        if(w_InductData[4] != 0x0000) break;
#ifdef WDT_ENABLE
                    HAL_Clear_WatchDog();
#endif 
                    }
                    
                    LCB_Clear_Data(G_READ_PWM);
                    
                    if(w_InductData[0] == 0 && w_InductData[1] == 0 &&
                       w_InductData[2] == 0 && w_InductData[3] == 0 &&
                           w_InductData[4] == 0)
                    {
                        w_InductData[0] = 1;
                        w_InductData[1] = 1;
                        w_InductData[2] = 1;
                        w_InductData[3] = 1;
                        w_InductData[4] = 1;
                    }
                }
                else
                {
                    LCB_Send_Cmd(DG_INDUCTION_CALP1,NO_PAR);                            //Get induction brake calibration point 1 data
                    LCB_Send_Cmd(DG_INDUCTION_CALP2,NO_PAR);                            //Get induction brake calibration point 2 data
                    LCB_Send_Cmd(DG_INDUCTION_CALP3,NO_PAR);                            //Get induction brake calibration point 3 data
                    LCB_Send_Cmd(DG_INDUCTION_CALP4,NO_PAR);                            //Get induction brake calibration point 4 data
                    LCB_Send_Cmd(DG_INDUCTION_CALP5,NO_PAR);                            //Get induction brake calibration point 5 data
                    
                    w_CommonTimer = 0;
                    while(w_CommonTimer < 3000)
                    {
                        LCB_CmdProcess();
                        w_InductData[0] = LCB_Get_Data(G_INDUCTION_CALP1);   
                        w_InductData[1] = LCB_Get_Data(G_INDUCTION_CALP2);     
                        w_InductData[2] = LCB_Get_Data(G_INDUCTION_CALP3);   
                        w_InductData[3] = LCB_Get_Data(G_INDUCTION_CALP4);   
                        w_InductData[4] = LCB_Get_Data(G_INDUCTION_CALP5); 
                        
                        if(w_InductData[0] != 0 && w_InductData[1] != 0 && 
                           w_InductData[2] != 0 && w_InductData[3] != 0 && 
                               w_InductData[4] != 0) break;
#ifdef WDT_ENABLE
                    HAL_Clear_WatchDog();
#endif  
                    }
                    
                    if(w_InductData[0] == 0 && w_InductData[1] == 0 &&
                       w_InductData[2] == 0 && w_InductData[3] == 0 &&
                           w_InductData[4] == 0)
                    {
                        w_InductData[0] = 2;
                        w_InductData[1] = 2;
                        w_InductData[2] = 2;
                        w_InductData[3] = 2;
                        w_InductData[4] = 2;
                    }
                }
            }
            else if(by_MCBType != 0x00 && (by_MCBType >= DC_LCB_20_275HP_JIS && by_MCBType <= AC_LCB_30_325HP_JIS) )
            {
				LCB_Send_Cmd(DS_MOTOR_HP,0x1234); 
				w_CommonTimer = 0;
				while(w_CommonTimer < 100)
				{
					LCB_CmdProcess();
#ifdef WDT_ENABLE
					HAL_Clear_WatchDog();
#endif  
				}
		   }
           if(SystemConfig_Get_Char(DRIVE_TYPE_UINT8) == MCB_RETAIL_CLIMBMILL )
           LCB_Send_Cmd(DS_SET_MACHINE_TYEP,0x06);
           if(by_MCBType != 0x00 && w_MCBVersion != 0x00) LCB_Next_State();        //版本号读取成功才切换
        }
        break;
    case LCB_ERP:
        LCB_Clear_Data(G_LCB_ERPRESPOND);
        LCB_Send_Cmd(DS_EUP, LCB_ERP_ENTER);
        LCB.ERP_Ack = 0;
        w_CommonTimer = 0;
        while(1)
        {
            LCB_CmdProcess();
            if(LCB.ERP_Ack)
            {
                LCB.ERP_Ack = 0;
                if(LCB_Get_Data(G_LCB_ERPRESPOND) == LCB_ERP_ACK_WAIT)
                {
                    LCB_Clear_Data(G_LCB_ERPRESPOND);
                    LCB_Send_Cmd(DS_EUP, LCB_ERP_ENTER);
                    LCB.ERP_Ack = 0;
                }
                else
                {
                    by_Out = 1;
                    break;
                }
            }
            
            if(w_CommonTimer >= 5000) 
            {
                by_Out = 2; 
                break;
            }
#ifdef WDT_ENABLE
            HAL_Clear_WatchDog();
#endif  
        }
        
        HAL_DLCB_Erp();
        LCB_Next_State();
        break;
    case LCB_ERROR:
        switch(ErrStep)
        {
        case ErrGet:
            LCB_Send_Cmd(DG_ERRCODE, NO_PAR);
            ErrStep = ErrHdle;
            w_SpecialTimer = 0;
            break;
        case ErrHdle:
            LCB_CmdProcess();
            w_ErrorCode = LCB_Get_Data(G_ERRCODE);
            if(w_ErrorCode != 0x00 || w_SpecialTimer >= 3000)
            {
                LCB_Clear_Data(G_ERRCODE);
                ErrStep = ErrSkip;
            }
            break;
        case ErrSkip:
			LCB.ErrSkip_Ack = 0;
            LCB_Send_Cmd(DS_SKIPERR, NO_PAR);
            ErrStep = ErrWaitSkipACK;
            w_SpecialTimer = 0;
			break;
		case ErrWaitSkipACK:
			LCB_CmdProcess();
			if(LCB.ErrSkip_Ack || w_SpecialTimer >= 2000)
			{
				LCB.ErrSkip_Ack = 0;
				ErrStep = ErrGet;
				LCB_Add_State(LCB_IDLE);
				LCB_Next_State();
			}
			break;
        default:
			ErrStep = ErrGet;
			LCB_Add_State(LCB_IDLE);
            LCB_Next_State();
            break;
        }
        break;
    default:
       if(bInited)
	{
		LCB_CmdProcess();

		if(w_ErrorCode == 0 && LCB_Get_Data(G_STATUS1) & BIT6)
		{
			LCB_Add_State(LCB_ERROR);
		}
	}
		
        LCB_Next_State();
        break;
    }
    
    return by_Out;
}

void LCB_CmdProcess(void)
{
	static UINT8 _overspeedtimes = 0 ;
    UINT8 found_G_NEW_VERSION = 0;
    //检查是否有收到新的数据
    if(Digital_Cmd_Get_Cmd(&CmdRx))
    {
        aby_Total[Offset_Adress[G_STATUS1].RxOffset] = CmdRx.Status;
        LCB.Cmd_Ack = 1;
        w_LostTimer = 0;
        
        if(CmdRx.Cmd == 0x76)
        {
            LCB.ErrSkip_Ack = 1;
        }
        
        if(CmdRx.Cmd == 0x7A && !LCB.ERP_Ack)                                   //说明LCB有回复ERP执行状态
        {
            LCB.ERP_Ack = 1;
        }
        if((CmdRx.Cmd == 0x50)&&(CmdRx.Len>0))
        {
            if(RxDataOffset[G_NEW_VERSION].subcommand==CmdRx.Data[0])
            {
                found_G_NEW_VERSION = 1;
            }
        }
        
        for(UCHAR i=0; i<RX_DATA_LEN; i++)
        {
            if(CmdRx.Cmd == Offset_Adress[i].Cmd)
            {
                if(CmdRx.Len == Offset_Adress[i].RxLen)
                {
                    if(found_G_NEW_VERSION == 1)
                    {
                        found_G_NEW_VERSION = 0;
                        i = G_NEW_VERSION;

                    }
                    memcpy(&aby_Total[Offset_Adress[i].RxOffset], CmdRx.Data, CmdRx.Len);
                    return;
                }
            }
        }
    }
    
    
    //数据Buff发送
    if(!Digital_Cmd_Tx_Busy())
    {
        if(by_Command_Point != by_Command_End)
        {
            if(by_Command_Point < (C_TX_BUFF_SIZE-1))                           //发送下一个命令
                ++by_Command_Point;
            else
                by_Command_Point = 0;
            
            LCB_CmdPack(Cmd_Buffer[by_Command_Point].CmdType, Cmd_Buffer[by_Command_Point].Data);
            memset(&Cmd_Buffer[by_Command_Point], CMD_NONE, sizeof(Cmd_Buffer[by_Command_Point]));
        }
    }
    
    
    if(bInited)
    {
        //Speed Update
        if(w_SpeedTimer >= LCB_SPEED_TIMER)
        {
            w_SpeedTimer = 0;
            if(by_DriverType != RETAIL_CLIMBMILL)
            LCB_Speed_Update();
        }
        
        //保持通讯,并获取RPM
        if((by_Command_Point == by_Command_End && w_KeepOnlineTimer > C_KEEP_ONLINE_IDLE)   //忙时500ms插入一个,不忙时300ms插入一个
           || w_KeepOnlineTimer > C_KEEP_ONLINE)
        {
            w_KeepOnlineTimer = 0;
            if(by_DriverType == LCB_MOTOR)
            {
                LCB_Send_Cmd(DG_MOT_RPM, 0);
	            LCB_Send_Cmd(DG_INC_LOCATION, 0);			
            }
            else if( by_DriverType == LCB_EP_BIKE )
            {
                LCB_Send_Cmd(DG_EPBI_RPM, 0);
            }
            else
            {
                LCB_Send_Cmd(DG_READ_WATT, 0);
                LCB_Send_Cmd(DG_READ_SPM, 0);
                LCB_Send_Cmd(DG_READ_STATUS, 0);          
            }
        }
	UINT8 _overspeedenable = 0 ;
	if( (1 == SystemConfig_Get_Char(OVERSPEED_SWITCH_UINT8)) && (SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL) && \
		(SystemConfig_Get_Char(MACHINE_MODLE_UINT8)>=1 && SystemConfig_Get_Char(MACHINE_MODLE_UINT8)<=6 ))
	{
		_overspeedenable = 1;
	}
	if(w_OverSpeedCheckTimer>= LCB_OVERSPEED_TIMER && 1== _overspeedenable)
	{
		w_OverSpeedCheckTimer = 0 ;
        		UINT16 tempRPM =  LCB_Get_Data(G_MOTOR_RPM) ;
		UINT16 tempOverValue = SystemConfig_Get_Int(MAX_MPH_RPM_UINT16)/SystemConfig_Get_Int(MAX_MPHX10_UINT16) *SystemConfig_Get_Char(OVERSPEED_VALUE) ;
		if( tempRPM> w_CurrentSpeed &&  tempRPM - w_CurrentSpeed >= tempOverValue )
		{
			if(++_overspeedtimes >= OVERSPEED_CONFIRMTIMES)
			{
				_overspeedtimes = OVERSPEED_CONFIRMTIMES;
				Program_Set_VirtualKey(VK_SPEEDOVER);
			}
		}
		else
		{
			_overspeedtimes = 0;
		}
	}
    }
}

UINT32 LCB_Get_Data(RX_DATA_TYPE en_Index)
{
    UINT32 w_Data = 0;
    
    if(en_Index == G_NEW_VERSION)
    {
        UINT16 n = aby_Total[Offset_Adress[G_NEW_VERSION].RxOffset + 1];
        if(n) n--;
        w_Data |= aby_Total[Offset_Adress[G_NEW_VERSION].RxOffset + 2 * n + 4];
        w_Data <<= 8;
        w_Data += aby_Total[Offset_Adress[G_NEW_VERSION].RxOffset + 2 * n + 5];
    }
    else if(en_Index == G_MCB_TYPE)
    {
        w_Data = aby_Total[Offset_Adress[G_NEW_VERSION].RxOffset + 2];
    }
    else
    {
        switch(Offset_Adress[en_Index].RxLen)
        {
        case 1:
            w_Data  |= aby_Total[Offset_Adress[en_Index].RxOffset];
            break;
        case 2:
            w_Data  |= aby_Total[Offset_Adress[en_Index].RxOffset];
            w_Data <<= 8;
            w_Data  += aby_Total[Offset_Adress[en_Index].RxOffset+1];
            break;
        case 3:
            w_Data   = 0;
            w_Data  |= aby_Total[Offset_Adress[en_Index].RxOffset+1];
            w_Data <<= 8;
            w_Data  += aby_Total[Offset_Adress[en_Index].RxOffset+2];
            break;
        case 5:
            w_Data   = 0;
            w_Data  |= aby_Total[Offset_Adress[en_Index].RxOffset+1];
            w_Data <<= 8;
            w_Data  += aby_Total[Offset_Adress[en_Index].RxOffset+2];
            w_Data <<= 8;
            w_Data  += aby_Total[Offset_Adress[en_Index].RxOffset+3];
            w_Data <<= 8;
            w_Data  += aby_Total[Offset_Adress[en_Index].RxOffset+4];
            break;
            case 6:
            w_Data   = 0;
            w_Data  |= aby_Total[Offset_Adress[en_Index].RxOffset+2];
            w_Data <<= 8;
            w_Data  += aby_Total[Offset_Adress[en_Index].RxOffset+3];
            w_Data <<= 8;
            w_Data  += aby_Total[Offset_Adress[en_Index].RxOffset+4];
            w_Data <<= 8;
            w_Data  += aby_Total[Offset_Adress[en_Index].RxOffset+5];
            break;
        default: break;
        }
    }
    
    return w_Data;
}

void LCB_Clear_Data(RX_DATA_TYPE en_Index)
{
    memset(&aby_Total[Offset_Adress[en_Index].RxOffset], 0, Offset_Adress[en_Index].RxLen);
}

void LCB_Send_Cmd(COM_TYPE CmdType, UINT32 w_Data)
{
    if(CmdType == DS_MOT_RPM)                                                   //如果是发送Speed的话,把Speed存入Buff中
    {
        w_TargetSpeed = w_Data;
    }
    else
    {
        if(CmdType == DS_INC_LOCATION)
        {
            if(by_MCBType == MCB_DELTA_COMMOCIAL || by_MCBType == MCB_TOPTEK || by_MCBType == MCB_DELTA) 
            {
                if(w_Data > DEFAULT_TOPTEK_MAX_INC)
                {
                    w_Data = DEFAULT_TOPTEK_MAX_INC;
                }
                
                if(w_Data < DEFAULT_TOPTEK_MIN_INC)
                {
                    w_Data = DEFAULT_TOPTEK_MIN_INC;
                }
                
                //w_Data = w_Data * 128;
                w_Data |=0x8000;
            }
        }
        
        //待发送Buff中已有相同命令,则丢弃
        for(UCHAR i=0; i<C_TX_BUFF_SIZE; i++)
        {
            if(Cmd_Buffer[i].CmdType == CmdType && Cmd_Buffer[i].Data == w_Data)
            {
                return;
            }
        }
        
        //存入Buff中
        if(by_Command_End < (C_TX_BUFF_SIZE-1))
            by_Command_End++;
        else
            by_Command_End = 0;
        
        
        Cmd_Buffer[by_Command_End].CmdType  = CmdType;
        Cmd_Buffer[by_Command_End].Data = w_Data;
    }
}

UINT16 LCB_Dectect_Error(void)
{
    UINT16 wOut = w_ErrorCode;
    w_ErrorCode = 0;
    
    if(w_LostTimer >= C_LOST_TIME && wOut == 0)
    {
        wOut = 0x04A0;
    }
    
    return wOut;
}

void LCB_CmdPack(COM_TYPE CmdType, UINT32 w_Data)
{
    CMD_TX CmdTx;
    memset(&CmdTx, 0, sizeof(CmdTx));
    CmdTx.Cmd = Cmd_Talbe[CmdType].Cmd;
    CmdTx.Adress = MCB_ADDRS;
    CmdTx.Len = Cmd_Talbe[CmdType].TxLen;
    
    if(CmdTx.Len)
    {
        for(UCHAR i=0; i<CmdTx.Len; i++)
        {
            CmdTx.Data[i] = (UCHAR)(w_Data >> ((CmdTx.Len - 1 - i) * 8));       //仅可发送0-4个byte的数据
        }
    }
    
    Digital_Cmd_Send_Cmd(&CmdTx);
}

void LCB_1ms_Int(void)
{
    Digital_Cmd_1ms_Int();
    if(w_OverSpeedCheckTimer < 0xFFFF) w_OverSpeedCheckTimer++;
    if(w_KeepOnlineTimer < 0xFFFF) w_KeepOnlineTimer++;
    if(w_CommonTimer < 0xFFFF) w_CommonTimer++;
    if(w_SpecialTimer < 0xFFFF) w_SpecialTimer++;
    if(w_SpeedTimer < 0xFFFF) w_SpeedTimer++;
    if(w_LostTimer < 0xFFFF) w_LostTimer++;
}

void LCB_Add_State(LCB_STATE State)
{
    if(by_State_End < (C_LCB_STATE_SIZE-1))
        by_State_End++;
    else
        by_State_End = 0;
    
    LcbStateBuff[by_State_End] = State;
}

void LCB_Next_State(void)
{
    LcbStateBuff[by_State_Point] = LCB_IDLE;
    if(by_State_Point != by_State_End)
    {
        if(by_State_Point < (C_LCB_STATE_SIZE-1))                               //发送下一个命令
            ++by_State_Point;
        else
            by_State_Point = 0;
    } 
}

void LCB_Power_On(UCHAR bWakeUP)
{
    by_DriverType = LCB_MOTOR;
    by_MCBType = 0;
    w_MCBVersion = 0;
    w_ErrorCode = 0;
    ErrStep = ErrGet;
    
    memset(&SpeedRef, 0, sizeof(SpeedRef));
    memset(&LCB, 0, sizeof(LCB));
    memset(w_InductData, 0, sizeof(w_InductData));
    
    SpeedRef.MaxSpeed = LCB_RPM_MAX;
    SpeedRef.AddStep = LCB_RPM_ADD_STEP;
    SpeedRef.DecStep = LCB_RPM_DEC_STEP;
    
    
    if(bWakeUP)
    {
        HAL_DLCB_WakeUp_LCB();
        
        LCB_Initial();
        bInited = FALSE;
        LCB_Clear_Data(G_LCB_ERPRESPOND);
        LCB_Send_Cmd(DS_EUP, LCB_ERP_LEAVE);
        LCB.ERP_Ack = 0;
        LCB.Cmd_Ack = 0;
        w_CommonTimer = 0;
        while(1)
        {
            LCB_CmdProcess();
            
            if(LCB.ERP_Ack || LCB.Cmd_Ack)
            {
                LCB.ERP_Ack = 0;
                LCB.Cmd_Ack = 0;
                if(LCB_Get_Data(G_LCB_ERPRESPOND) == LCB_ERP_ACK_WAIT)
                {
                    LCB_Clear_Data(G_LCB_ERPRESPOND);
                    LCB_Send_Cmd(DS_EUP, LCB_ERP_LEAVE);
                    LCB.ERP_Ack = 0;
                }
                else
                {
                    break;
                }
            }
            
            if(w_CommonTimer >= 5000) 
                break;
 #ifdef WDT_ENABLE
            HAL_Clear_WatchDog();
#endif  
        }
    }
    
    LCB_Add_State(LCB_INIT_LCB);
}

void LCB_Set_DriverType(UCHAR DriverType)
{
    by_DriverType = DriverType;
}

void LCB_Speed_Update(void)
{
    UINT16 wTargetSpeed = w_TargetSpeed & 0x7FFF;
    
    if(wTargetSpeed > w_CurrentSpeed)
    {
        if((w_CurrentSpeed + SpeedRef.AddStep) <= wTargetSpeed)
        {
            w_CurrentSpeed += SpeedRef.AddStep;
        }
        else
        {
            w_CurrentSpeed = wTargetSpeed;
        }
        
        if(w_CurrentSpeed < LCB_RPM_MIN)  w_CurrentSpeed = LCB_RPM_MIN;
    }
    else if(wTargetSpeed < w_CurrentSpeed)
    {
        if((w_CurrentSpeed - wTargetSpeed) >= SpeedRef.DecStep)
        {
            w_CurrentSpeed -= SpeedRef.DecStep;
        }
        else
        {
            w_CurrentSpeed = wTargetSpeed;
        }
        
        if(w_CurrentSpeed < LCB_RPM_MIN)  w_CurrentSpeed = 0;
    }
    else
    {
        w_CurrentSpeed = wTargetSpeed;
    }
    
    
    UINT16 wSendSpeed = 0;
    
    if(w_TargetSpeed & 0x8000)
    {
        wSendSpeed = w_CurrentSpeed & 0x8000;
    }
    else
    {
        wSendSpeed = w_CurrentSpeed;
    }
    
    
    COM_TYPE CmdType = DS_MOT_RPM;
    
    //待发送Buff中已有相同命令,则丢弃
    for(UCHAR i = 0; i < C_TX_BUFF_SIZE; i++)
    {
        if(Cmd_Buffer[i].CmdType == CmdType && Cmd_Buffer[i].Data == wSendSpeed)
        {
            return;
        }
    }
    
    //存入Buff中
    if(by_Command_End < (C_TX_BUFF_SIZE - 1))
        by_Command_End++;
    else
        by_Command_End = 0;
    
    
    Cmd_Buffer[by_Command_End].CmdType  = CmdType;
    Cmd_Buffer[by_Command_End].Data = wSendSpeed;
}

UCHAR LCB_Get_MCBType(void)
{
    return by_MCBType;
}

UINT16 LCB_Get_Version(void)
{
    return w_MCBVersion;
}

void LCB_Set_SpeedRef(LCB_SPEED_REF *Ref)
{
    memcpy(&SpeedRef, (void*)Ref, sizeof(SpeedRef));
}

void LCB_Get_Induct(UINT16 *pw)
{
    for(UCHAR i=0; i<5; i++)
    {
        pw[i] = w_InductData[i];
    }
}

void LCB_Erp(void)
{
	memset(LcbStateBuff, LCB_IDLE, sizeof(LcbStateBuff));
	by_State_End = 0;
	by_State_Point = 0;
    LCB_Add_State(LCB_ERP);
}

void LCB_Initial_IO(void)
{
	HAL_DLCB_Initial_IO();
	HAL_SystemTicker_Alloc_ISR(LCB_1ms_Int);
	HAL_Digital_Regist_ISR(Digital_Cmd_Tx_Int,Digital_Cmd_Rx_Int);
}
UCHAR Digital_GetStatus()
{
    return CmdRx.Status;
}




