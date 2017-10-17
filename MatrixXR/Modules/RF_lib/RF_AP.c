#include "RF_Lib.h"

#define FRAME_WORK_VISION 8
#define RFSAFFEKEY_OFF_DELAYTIME 2000
#define SUMMARY_DELAYTIME 2000

__no_init static union
{
    struct
    {
        UINT16 key_rf_sync :1;
        UINT16 key_reset :1;
        UINT16 key_start :1;
        UINT16 key_safetykeyoff:1;
    }fields;
    UINT16 event;
}RF_Key_Event;

__no_init static union
{
    struct
    {
        UINT16 rf_online :1;
        UINT16 rf_offline :1;
        UINT16 rf_syncing :1;
        UINT16 rf_syncfinish :1;
        UINT16 rf_synctimeout :1;
    }fields;
    UINT16 event;
}RF_Event;
__no_init static struct
{
    UINT16 _timeelapsed;
    UINT16 _distance;
    UINT16 _calories;
    UINT16 _averageHeartrate;
    UINT16 _averagepace;
}RF_Workoutdata;


typedef enum g_RFSAFETYKEY_STATUS{
        RFSAFETYKEY_STATUS_START =0,
        RFSAFETYKEY_STATUS_IDEL = RFSAFETYKEY_STATUS_START,
        RFSAFETYKEY_STATUS_OFFDELAY,
        RFSAFETYKEY_STATUS_OFFDELAYTIMEUP,
        RFSAFETYKEY_STATUS_FINISHDELAY,
        RFSAFETYKEY_STATUS_FINISHDELAYTIMEUP,
        RFSAFETYKEY_STATUS_START_TOTAL
}RFSAFETYKEY_STATUS;

extern char paTable[];
extern char paTableLen;


__no_init FRAME_STAGE g_MainStateBuf;
__no_init unsigned char RF_isEnable;
__no_init unsigned char _lostConnect;
__no_init unsigned char *ucReceive;
__no_init unsigned int *CCA_Point;
__no_init unsigned char Receive_Update;
__no_init unsigned char CCA_Detect;
__no_init UINT16  old_receive;
__no_init UINT16  new_receive;
__no_init RFSAFETYKEY_STATUS g_RFSafeyKeyStatus ;
__no_init UINT16 g_RFSafeyKeyDelayCnt ;

const unsigned char ucChannel_Data[8][3] =
{
    //  freq0 freq1 freq2
    {0x3b, 0xb1, 0x5c}, //2410MHz //Normal
    {0xb1, 0x13, 0x5d}, //2420MHz
    {0x27, 0x76, 0x5d}, //2430MHz
    {0x13, 0x3b, 0x5e}, //2450MHz
    {0x89, 0x9d, 0x5e}, //2460MHz
    {0x00, 0x80, 0x5c}, //2405MHz
    {0x3b, 0x31, 0x5f}, //2475MHz
    {0x9d, 0xd8, 0x5d}, //2440MHz
};


const unsigned char Tx_Map_Inuse_Max = 18;
const unsigned char Tx_Map_Inuse[19][2] =
{
    {0x00,  1},   //ID
    {0x01,  1},   //Channel
    {0x02,  1},   //Console State
    {0x03,  1},   //Unit mode
    {0x04,  1},   //Machine mode
    {0x05,  1},   //RTC-Minute
    {0x06,  1},   //RTC-Hours
    {0x07,  2},   //Workout time
    {0x09,  2},   //Distance data
    {0x0b,  2},   //Calories data
    {0x0d,  1},   //Current Heart rate
    {0x0e,  1},   //Max heart rate
    {0x0f,  1},   //Incline data
    {0x10,  1},   //Resistance data
    {0x11,  1},   //RPM data
    {0x12,  2},   //Power data(watts)
    {0x14,  1},   //Speed data
    {0x15,  2},   //Current pace
    {0x17,  2},   //Target workout time
};

const unsigned char Tx_Map_Idle_Max = 24;
const unsigned char Tx_Map_Idle[25][2] =
{
    {0x00,  1},   //Current ID
    {0x01,  1},   //Current Channel
    {0x02,  1},   //Console State
    {0x03,  1},   //Unit mode
    {0x04,  1},   //Machine mode
    {0x05,  1},   //RTC-Minute
    {0x06,  1},   //RTC-Hour
    {0x07,  2},   //Max target workout time
    {0x09,  2},   //Min target workout time        8
    {0x0b,  1},   //Max Incline
    {0x0c,  1},   //Min Incline
    {0x0d,  1},   //Max Resistance
    {0x0e,  1},   //Min Resistance
    {0x0f,  1},   //Max Speed
    {0x10,  1},   //Min Speed
    {0x11,  2},   //Max Weight                  15
    {0x13,  2},   //Min Weight
    {0x15,  1},   //User Count
    {0x16,  2},   //Console Sw Version
    {0x18,  1},   //RF SW Version
    {0x19,  2},   //Machine Serial
    {0x1b,  1},   //Scan Channel Mask
    {0x1c,  1},   //Scan Channel2
    {0x1d,  1},   //SYNC ID
    {0x1e,  1},   //SYNC Channel
};

const unsigned char Tx_Map_Finish_Max = 14;
const unsigned char Tx_Map_Finish[15][2] =
{
    {0x00,  1},   //Current ID
    {0x01,  1},   //Current Channel
    {0x02,  1},   //Console State
    {0x03,  1},   //Unit mode
    {0x04,  1},   //Machine mode
    {0x05,  1},   //RTC-Minute
    {0x06,  1},   //RTC-Hour                           6
    {0x07,  1},   //RTC-Day
    {0x08,  1},   //RTC-Month
    {0x09,  1},   //RTC-Year
    {0x0a,  2},   //Total workout time
    {0x0c,  2},   //Total Distance
    {0x0e,  2},   //Total Calories
    {0x10,  1},   //Average Heart Rate
    {0x11,  2},   //Average Pace
};

const unsigned char Rx_Map_Max = 16;
const unsigned char Rx_Map[17][2] =
{
    {0x00,   1},  //ID
    {0x01,   1},  //CHANNE Setup
    {0x02,   1},  //STB Status
    {0x05,   2},  //Weight
    {0x07,   1},  //Age
    {0x08,   2},  //Target workout time
    {0x0a,   1},  //Startuup speed
    {0x0b,   1},  //Startup Incline
    {0x0c,   1},  //Startup Resistance
    {0x0d,   1},  //Terrain incline
    {0x0e,   1},  //Terrain Resistance
    //0x0f ~ 0x18 // reserved
    {0x19,   1},  //Reset Factory Default
    {0x1a,   1},  //User Number
    {0x1b,   1},  //SPI Received data number count
    {0x1c,   1},  //RF Received data nuber count
    {0x1d,   1},  //SYNC ID
    {0x1e,   1},  //SYNC CHANNEL
};

__no_init struct
{
    unsigned char Receive_Buffer[33];
    unsigned char Receive_APBuffer[33];
    unsigned char Transmit_Buffer[34];
    unsigned char RF_Address;
    unsigned char RF_Mode;

    unsigned int CCATimer;
    unsigned char CCADelay;
    unsigned int RRDelay;
    unsigned int ReFlushDelay;

    unsigned char RF_SYNC_Stage;
    unsigned int RF_Delay_Time;
    unsigned int RF_Sync_Timer;

    unsigned char RF_ReFlush_Step;

    unsigned char SYNC_ID;
    unsigned char SYNC_CHANNEL;

    unsigned int StartUp_Weight;
    unsigned char StartUp_Age;
    unsigned int StartUp_Workout_Time;
    unsigned char StartUp_Speed;
    unsigned char StartUp_Incline;
    unsigned char StartUp_Resistance;
    unsigned char StartUp_UserNumber;

    unsigned char Terrain_Incline;
    unsigned char Terrain_Resistance;

    unsigned Enable_Tx         : 1;
    unsigned RF_ReFlush        : 1;
    unsigned STB_Setting       : 1;
    unsigned RF_First          : 1;
    
    unsigned char SPI_Num_Buffer;
    unsigned char RF_Num_Buffer;
    unsigned long int SPI_Count;
    unsigned long int RF_Count;
    unsigned long int RF_Correct_SelfCount; //自己接收到的RF数据包,并且checksum正确的次数
    unsigned long int RF_All_SelfCount;     //自己总共接收到的RF数据包的次数

    unsigned long int RF_Target_Time;       //STB 送下来的目标时间值
    
    unsigned char RF_Run_Status;
} RF;

static signed int w_Terrain_Data=0;
static signed int w_Terrain_Data_RESISTANCE = 0;
static signed int w_Get_STB_Terrain_Data=0;

static void RF_Set_CCA_Detect_Data(unsigned char by_Data);
static void RF_Set_CCA_Point_Data(unsigned char by_Data);
static void RF_Initial_Data(void);
static void RF_Set_Receive_Buffer_Point(unsigned char *ucPoint, unsigned int *uiPoint);
static void RF_GDO2_Int_ISR(void);
static void RF_GDO0_Int_ISR(void);
//static void RF_AP_Disable_GDO0_interrupt(void);
static void RF_AP_Set_Receive_Information(unsigned char ucReceive);
static void RF_AP_Set_Channel_Information(unsigned char ucChannel);
static void RF_AP_1ms_Timerout_IntHandle(void);
static void RF_AP_Set_RF_Mode(unsigned char by_Mode);
static void RF_AP_SYNC_Main_Process(void);
static void RF_AP_Set_ReFlush_Flag(void);
static void RF_AP_RF_ReFluse_Process_2ms_IntHandle(void);
static void RF_AP_Main_Process(void);
static unsigned char RF_AP_Check_Data_Checksum(unsigned char *PBuffer, unsigned char ucLength);
static void RF_AP_Make_CheckSum(unsigned char *PBuffer, unsigned char ucLength);
static unsigned int RF_AP_Get_Data_From_Rx(unsigned char ucIndex);
static void RF_AP_Set_Data_To_Tx(RF_TX_MODE by_Mode, unsigned char ucIndex, unsigned int uiData);
static void RF_AP_Mainprocess_TX_FINISH(void);
static void RF_AP_Mainprocess_TX_INUSE(void);
static void RF_AP_Mainprocess_TX_IDEL(void);




void RF_AP_Initial_IO(void)
{  
    if(_lostConnect!=1)
    {
        RF_Event.fields.rf_offline = 1;        
    }
 	_lostConnect = 1;
    
    HAL_RF_Config_GDO0(0);
    CC_SPI_Setup_SPI();
    CC_PowerupResetCCxxxx();               // Reset CCxxxx
    writeRFSettings();                        // Write RF settings to config reg
    CC_SPI_Write_Burst_Reg(TI_CCxxx0_PATABLE, paTable, paTableLen);//Write PATABLE
    
    HAL_RF_Clear_INT_Flag(0);
    HAL_RF_Config_GDO0(1);
		
	HAL_SystemTicker_Alloc_ISR(RF_AP_1ms_Timerout_IntHandle);
	HAL_PININT_Alloc_ISR(RF_GDO0_Int_ISR);
	HAL_PININT_Alloc_ISR(RF_GDO2_Int_ISR);
}

#if 0
static void RF_AP_Disable_GDO0_interrupt(void)
{
    HAL_RF_Clear_INT_Flag(0);
    HAL_RF_Config_GDO0(0);
}
#endif
static void RF_AP_Set_Receive_Information(unsigned char ucReceive)
{
    unsigned int w_Count = ERROR_BREAK_COUNT;

    CC_SPI_Strobe(TI_CCxxx0_SIDLE);         // Set CCxxxx to IDLE mode
    unsigned char data = 0, state;

    //state = ucReceive;
    state = (ucReceive|0x80);  //receive BIT7 is "1"
    while(state != data && w_Count > 0)
    {
        CC_SPI_Write_Reg(TI_CCxxx0_ADDR, state);
        data = CC_SPI_Read_Reg(TI_CCxxx0_ADDR);
        w_Count--;
#ifdef WDT_ENABLE
        HAL_Clear_WatchDog();
#endif 
    }

    CC_SPI_Strobe(TI_CCxxx0_SFRX);          // Flush the RX FIFO
    CC_SPI_Strobe(TI_CCxxx0_SRX);           // Set CCxxxx to RX mode
}

static void RF_AP_Set_Channel_Information(unsigned char ucChannel)
{
    unsigned char data = 0, freq2;
    unsigned int w_Count = ERROR_BREAK_COUNT;
    
    
    
    char Testbuffer[40];
    memset(Testbuffer,0,sizeof(Testbuffer));
    
    if(ucChannel > CHANNEL_NUMBER)return;

    CC_SPI_Strobe(TI_CCxxx0_SIDLE);         // Set CCxxxx to IDLE mode


    freq2 = ucChannel_Data[ucChannel][0];
    
    while (data != freq2 && w_Count > 0) 
    {
        CC_SPI_Write_Reg(TI_CCxxx0_FREQ0, freq2);
        data = CC_SPI_Read_Reg(TI_CCxxx0_FREQ0);
        w_Count--;
#ifdef WDT_ENABLE
        HAL_Clear_WatchDog();
#endif         
    }

    w_Count = ERROR_BREAK_COUNT;
    freq2 = ucChannel_Data[ucChannel][1];
    data = 0;
    while (data != freq2 && w_Count > 0)
    {
        CC_SPI_Write_Reg(TI_CCxxx0_FREQ1, freq2);
        data = CC_SPI_Read_Reg(TI_CCxxx0_FREQ1);
        w_Count--;
#ifdef WDT_ENABLE
        HAL_Clear_WatchDog();
#endif
    }

    w_Count = ERROR_BREAK_COUNT;
    freq2 = ucChannel_Data[ucChannel][2];
    data = 0;
    while (data != freq2 && w_Count > 0)
    {
        CC_SPI_Write_Reg(TI_CCxxx0_FREQ2, freq2);
        data = CC_SPI_Read_Reg(TI_CCxxx0_FREQ2);
        w_Count--;
#ifdef WDT_ENABLE
        HAL_Clear_WatchDog();
#endif 
    }

    CC_SPI_Strobe(TI_CCxxx0_SFRX);          // Flush the RX FIFO
    CC_SPI_Strobe(TI_CCxxx0_SRX);           // Set CCxxxx to RX mode
}


static unsigned int RF_tag = 0;

static void RF_GDO0_Int_ISR(void)
{
    if(HAL_RF_Get_IO_IntFlag(0))  
    {
        HAL_RF_Clear_INT_Flag(0);
        RF_tag++;
        if(RF_tag>=100)//认为异常，关中断
        {
            HAL_Set_IO_IntMode(TI_CC_GDO0_PORT,TI_CC_GDO0_PIN,1,0);//disable GDO0 int
            Receive_Update = 0;
        }
        else
        {
            Receive_Update = 1;
        }
        
    }
}

static void RF_GDO2_Int_ISR(void)
{
    if(HAL_RF_Get_IO_IntFlag(1))
    {
        RF_Set_CCA_Point_Data(0x0a);
        RF_Set_CCA_Detect_Data(1);
        HAL_RF_Clear_INT_Flag(1);
    }
}

static UINT16 Counter2Ms = 0;
static UINT16 Count2SCheckUser = 0; //do it by program or main
static UINT8 RF_Check_User_Flag = 0;
static void RF_AP_1ms_Timerout_IntHandle(void)
{  
    if(!RF_isEnable) return;
    if( RFSAFETYKEY_STATUS_OFFDELAY == g_RFSafeyKeyStatus )	
     {
        if(++g_RFSafeyKeyDelayCnt >= RFSAFFEKEY_OFF_DELAYTIME)
        {
            g_RFSafeyKeyDelayCnt = 0 ;
            g_RFSafeyKeyStatus = RFSAFETYKEY_STATUS_OFFDELAYTIMEUP ;
        }
    }
    if( RFSAFETYKEY_STATUS_FINISHDELAY == g_RFSafeyKeyStatus )
    {
         if(++g_RFSafeyKeyDelayCnt >= SUMMARY_DELAYTIME)
        {
            g_RFSafeyKeyDelayCnt = 0 ;
            g_RFSafeyKeyStatus = RFSAFETYKEY_STATUS_FINISHDELAYTIMEUP ;
        }
    }
    if(Counter2Ms >= 2)
    {
//        extern unsigned char String_Get_Flag(void);
//        if(String_Get_Flag())
//            return;
        RF_AP_RF_ReFluse_Process_2ms_IntHandle();
        Counter2Ms = 0;
    }
    else
    {
        Counter2Ms++;
    } 
    
//    if(RF_tag > 2) 
//    {
//        RF_AP_Disable_GDO0_interrupt();
//    }
    RF_tag = 0;
  
    if(++Count2SCheckUser>2000)
    {
        Count2SCheckUser = 0 ;
        RF_Check_User_Flag = 1;
    }
    if(RF.RF_Delay_Time > 0)RF.RF_Delay_Time--;
    else RF.RF_Delay_Time = 0;
    if(RF.ReFlushDelay > 0)RF.ReFlushDelay--;
    else RF.ReFlushDelay = 0;
    if(RF.RRDelay > 0)RF.RRDelay--;
    else RF.RRDelay = 0;
    if(RF.CCATimer > 0)RF.CCATimer--;
    else
    {
        RF.CCATimer = 0;
        if(RF.CCADelay > 0)RF.CCADelay--;
        else
        {
            RF.CCADelay = 0;
        }
    }
    if(RF.RF_Mode == RF_STATE_SYNC)
    {
        if(RF.RF_Sync_Timer < 60000)RF.RF_Sync_Timer++;
        else RF.RF_Sync_Timer = 60000;
    }
}

static void RF_AP_Set_RF_Mode(unsigned char by_Mode)
{
    RF.RF_Mode = by_Mode;
}

static void RF_AP_SYNC_Main_Process(void)
{
    if(RF.RF_Mode != RF_STATE_SYNC)return;

    unsigned char ucLength;

    if(RF.RF_Sync_Timer >= 60000)
    {
        if(RF.RF_SYNC_Stage!=SYNC_TIMEOUT)
        {
            RF_Event.fields.rf_synctimeout = 1; 
        }
        RF.RF_SYNC_Stage = SYNC_TIMEOUT;
    }

    switch(RF.RF_SYNC_Stage)
    {
        case SYNC_STAGE_START:
        {
            if(RF.RF_Delay_Time == 0 && RF.RF_SYNC_Stage == SYNC_STAGE_START)
            {
                if(RF.RF_Address < CHANNEL_NUMBER)RF.RF_Address++;
                else RF.RF_Address = 0;
                RF_AP_Set_Channel_Information(RF.RF_Address);
                RF.RF_Delay_Time = 230;
            }
            if(Receive_Update)
            {
                ucLength = 33;
                if(RFReceivePacket((char *)&RF.Receive_Buffer[0], (char *)&ucLength))    // Fetch packet from CCxxxx
                {
                    if(ucLength == RX_PACKET_LENGTH && RF_AP_Check_Data_Checksum(&RF.Receive_Buffer[1], ucLength - 1))
                    {
                        for(unsigned char i = 0; i < RX_PACKET_LENGTH - 1; i++)
                        {
                            RF.Receive_APBuffer[i] = RF.Receive_Buffer[i+1];
                        }
                        unsigned char by_State = RF_AP_Get_Data_From_Rx(RX_STB_STATE);
                        if(by_State == SYNC_SCAN)
                        {
                            RF.RF_SYNC_Stage = SYNC_STAGE_START;
                        }
                        else if(by_State == SYNC_WAITING)
                        {
                            RF.SYNC_ID = RF_AP_Get_Data_From_Rx(RX_SYNC_ID);
                            RF.SYNC_CHANNEL = RF_AP_Get_Data_From_Rx(RX_SYNC_CHANNEL);
                            RF.RF_SYNC_Stage = SYNC_PARTNERSHIP;
                        }
                        else if(by_State == SYNC_TIMEOUT)
                        {
                            RF_AP_Set_Channel_Information(SystemConfig_Get_Char(RF_CHANNEL_UCHAR));
                            RF_AP_Set_Receive_Information(SystemConfig_Get_Char(RF_ADDRESS_UCHAR));
                            
                            if(RF.RF_SYNC_Stage != SYNC_TIMEOUT)
                            {
                                RF_Event.fields.rf_synctimeout = 1; 
                            }
                            RF.RF_SYNC_Stage = SYNC_TIMEOUT;
                        }
                    }
                }
                Receive_Update = 0;
                RF_AP_Set_ReFlush_Flag();
            }
            break;
        }
        case SYNC_PARTNERSHIP:
        {
            if(Receive_Update)
            {
                ucLength = 64;
                if (RFReceivePacket((char *)&RF.Receive_Buffer[0], (char *)&ucLength))    // Fetch packet from CCxxxx
                {
                    if(ucLength == RX_PACKET_LENGTH && RF_AP_Check_Data_Checksum(&RF.Receive_Buffer[1], ucLength - 1))
                    {
                        for(unsigned char i = 0; i < RX_PACKET_LENGTH - 1; i++)
                        {
                            RF.Receive_APBuffer[i] = RF.Receive_Buffer[i+1];
                        }
                        unsigned char by_State = RF_AP_Get_Data_From_Rx(RX_STB_STATE);
                        if(by_State == SYNC_WAITING)
                        {
                            RF.RF_SYNC_Stage = SYNC_PARTNERSHIP;
                            RF.SYNC_ID = RF_AP_Get_Data_From_Rx(RX_SYNC_ID);
                            RF.SYNC_CHANNEL = RF_AP_Get_Data_From_Rx(RX_SYNC_CHANNEL);
                            RF.RF_Delay_Time = 60;
                            RF.Enable_Tx = 1;
                        }
                        else if(by_State == SYNC_FINISH)
                        {
                            RF_AP_Initial_IO();
                            
                            SystemConfigProcess(SVK_SET_RF_CHANNEL,&(RF.SYNC_CHANNEL),sizeof(SVK_SET_RF_CHANNEL));
                            SystemConfigProcess(SVK_SET_RF_ADDRESS,&(RF.SYNC_ID),sizeof(SVK_SET_RF_ADDRESS));
                            SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                            //Information_Save_Data(IM_RF_CHANNEL, RF.SYNC_CHANNEL);
                            //Information_Save_Data(IM_RF_ADDRESS, RF.SYNC_ID);
                            RF_AP_Set_Channel_Information(SystemConfig_Get_Char(RF_CHANNEL_UCHAR));
                            RF_AP_Set_Receive_Information(SystemConfig_Get_Char(RF_ADDRESS_UCHAR));
                            RF_AP_Initial_Data();
                            RF_AP_Set_Data_To_Tx(TX_MODE_IDLE, TX_IDLE_ID, (SystemConfig_Get_Char(RF_ADDRESS_UCHAR)));
                            RF_AP_Set_Data_To_Tx(TX_MODE_IDLE, TX_IDLE_CHANNEL, (SystemConfig_Get_Char(RF_CHANNEL_UCHAR)));
                            RF_AP_Set_RF_Mode(RF_STATE_NORMAL);
                            
                            RF.RF_Delay_Time = 60;
                            RF.Enable_Tx = 1;
                            _lostConnect = false; 
                            if(RF.RF_SYNC_Stage != SYNC_FINISH)
                            {
                                RF_Event.fields.rf_syncfinish = 1;
                            }
                            RF.RF_SYNC_Stage = SYNC_FINISH;
                        }
                        else if(by_State == SYNC_TIMEOUT)
                        {
                            RF_AP_Set_Channel_Information(SystemConfig_Get_Char(RF_CHANNEL_UCHAR));
                            RF_AP_Set_Receive_Information(SystemConfig_Get_Char(RF_ADDRESS_UCHAR));
                            RF_AP_Set_Data_To_Tx(TX_MODE_IDLE, TX_IDLE_ID, SystemConfig_Get_Char(RF_ADDRESS_UCHAR));
                            RF_AP_Set_Data_To_Tx(TX_MODE_IDLE, TX_IDLE_CHANNEL, SystemConfig_Get_Char(RF_CHANNEL_UCHAR));
                            RF_AP_Set_RF_Mode(RF_STATE_NORMAL);
                            
                            if(RF.RF_SYNC_Stage != SYNC_TIMEOUT)
                            {
                                RF_Event.fields.rf_synctimeout = 1; 
                            }
                            RF.RF_SYNC_Stage = SYNC_TIMEOUT;
                            RF.RF_Delay_Time = 60;
                            RF.Enable_Tx = 1;
                        }
                    }
                }
                Receive_Update = 0;
                RF_AP_Set_ReFlush_Flag();
            }
            if(RF.RF_Delay_Time == 0 && RF.Enable_Tx && HAL_RF_Get_GDO2_Pin_Input_State())
            {
                RF.Transmit_Buffer[0] = TX_PACKET_LENGTH - 1;                        // Packet length
                RF.Transmit_Buffer[1] = RF_PARTNERSHIP_ADDRESS;                     // Packet address
                RF_AP_Set_Data_To_Tx(TX_MODE_IDLE, TX_IDLE_SYNC_ID, RF.SYNC_ID);
                RF_AP_Set_Data_To_Tx(TX_MODE_IDLE, TX_IDLE_SYNC_CHANNEL, RF.SYNC_CHANNEL);
                RF_AP_Make_CheckSum(&RF.Transmit_Buffer[2], TX_PACKET_LENGTH - 2);
                RFSendPacket((char *)RF.Transmit_Buffer, TX_PACKET_LENGTH);              // Send value over RF
                RF.Enable_Tx = 0;
            }
            break;
        }
        case SYNC_FINISH:
        {
            if(RF.RF_SYNC_Stage != SYNC_FINISH)
            {
                RF_Event.fields.rf_syncfinish = 1; 
            }
            RF.RF_SYNC_Stage = SYNC_FINISH;
            _lostConnect = false;
            break;
        }
        case SYNC_TIMEOUT:
        {
            if(RF.RF_SYNC_Stage != SYNC_TIMEOUT)
            {
                RF_Event.fields.rf_synctimeout = 1; 
            }
            RF.RF_SYNC_Stage = SYNC_TIMEOUT;
            break;
        }
        default:
        break;
    }
}


static void RF_AP_Set_ReFlush_Flag(void)
{
    RF.RF_ReFlush_Step = 0;
    RF.RF_ReFlush = 1;
}

static void RF_AP_RF_ReFluse_Process_2ms_IntHandle(void)
{
    if(RF.RF_ReFlush)
    {
        switch(RF.RF_ReFlush_Step)
        {
        case 0:
            CC_SPI_Strobe(TI_CCxxx0_SIDLE);
            break;
        case 1:
            CC_SPI_Strobe(TI_CCxxx0_SFRX);
            break;
        case 2:
            CC_SPI_Strobe(TI_CCxxx0_SFTX);
            break;
        case 3:
            CC_SPI_Strobe(TI_CCxxx0_SRX);
            RF.RF_ReFlush = 0;
            break;
        default:
            break;
        }
    }
    if(RF.RF_ReFlush_Step < 3)RF.RF_ReFlush_Step++;
    else RF.RF_ReFlush_Step = 0;
}


unsigned char RF_AP_Get_Rate_Precent(unsigned char ucMode)
{
    unsigned long long ulSelfError = 0;
    if(RF.RF_All_SelfCount)ulSelfError = RF.RF_Correct_SelfCount * 100 / RF.RF_All_SelfCount;

    unsigned long long ulSPISuccess = 0;
    if(RF.SPI_Count)ulSPISuccess = RF.RF_Count * 100 / RF.SPI_Count;

    if(ucMode == PERCENT_SELFERROR)return (unsigned char)ulSelfError;
    if(ucMode == RERCENT_RFLOST)return (unsigned char)ulSPISuccess;

    return 0;
}

static void RF_AP_Main_Process(void)
{
    if(RF.RF_Mode != RF_STATE_NORMAL)return;

    unsigned char ucLength;
    if(Receive_Update)
    {
        ucLength = 33;
        char RF_Buffer = RFReceivePacket((char *)&RF.Receive_Buffer[0], (char *)&ucLength);
        if(RF_Buffer == 0x80 || RF_Buffer == 0x00)     // Fetch packet from CCxxxx
        {
            if(RF.RF_First)
            {
                RF.RF_Correct_SelfCount = 0;
                RF.RF_All_SelfCount = 0;
            }
            
            RF.RF_All_SelfCount++;
            
            if(RF_Buffer == 0x80 && ucLength == RX_PACKET_LENGTH && RF_AP_Check_Data_Checksum(&RF.Receive_Buffer[1], ucLength - 1))
            {
                if(_lostConnect!=0)
                {
                    RF_Event.fields.rf_online = 1; 
                }
                _lostConnect = 0;
                RF.RF_Correct_SelfCount++;
                
                //                for(unsigned char i = 0; i < RX_PACKET_LENGTH-1;i++)
                //                {
                //                    RF.Receive_APBuffer[i] = RF.Receive_Buffer[i+1];
                //                }
                unsigned char ucBuffer1 = 0,ucBuffer2 = 0;
                //                ucBuffer1 = RF_AP_Get_Data_From_Rx(RX_SPI_NUM);
                //                ucBuffer2 = RF_AP_Get_Data_From_Rx(RX_RF_NUM);
                ucBuffer1 = RF.Receive_Buffer[0x1b+1];
                ucBuffer2 = RF.Receive_Buffer[0x1c+1];
                if(RF.RF_First)
                {
                    RF.SPI_Num_Buffer = ucBuffer1;
                    RF.RF_Num_Buffer = ucBuffer2;
                    RF.SPI_Count = 0;
                    RF.RF_Count = 0;
                }
                else
                {
                    //                    RF.SPI_Count = RF.SPI_Count + (unsigned char)(ucBuffer1 - RF.SPI_Num_Buffer);
                    //                    RF.RF_Count = RF.RF_Count + (unsigned char)(ucBuffer2 - RF.RF_Num_Buffer);
                    if(ucBuffer1>=RF.SPI_Num_Buffer) RF.SPI_Count = RF.SPI_Count + (unsigned char)(ucBuffer1 - RF.SPI_Num_Buffer);
                    else RF.SPI_Count = RF.SPI_Count + (unsigned char)(ucBuffer1 + 0xff - RF.SPI_Num_Buffer);
                    if(ucBuffer2>=RF.RF_Num_Buffer) RF.RF_Count = RF.RF_Count + (unsigned char)(ucBuffer2 - RF.RF_Num_Buffer);
                    else RF.RF_Count = RF.RF_Count + (unsigned char)(ucBuffer2 + 0xff - RF.RF_Num_Buffer);
                    if(ucBuffer1 != RF.SPI_Num_Buffer)
                    {
                        for(unsigned char i = 0; i < RX_PACKET_LENGTH-1;i++)
                        {
                            RF.Receive_APBuffer[i] = RF.Receive_Buffer[i+1];
                        }
                        
                        unsigned char by_State = RF_AP_Get_Data_From_Rx(RX_STB_STATE);
                        if(by_State == D_STATE_SETTING)
                        {
                            RF.STB_Setting = 1;
                            RF.StartUp_Weight = RF_AP_Get_Data_From_Rx(RX_WEIGHT);
                            RF.StartUp_Age = RF_AP_Get_Data_From_Rx(RX_AGE);
                            
                            RF.StartUp_Workout_Time = RF_AP_Get_Data_From_Rx(RX_TARGET_WORKOUT_TIME);
                            RF.StartUp_Speed = RF_AP_Get_Data_From_Rx(RX_STARTUP_SPEED);
                            RF.StartUp_Incline = RF_AP_Get_Data_From_Rx(RX_STARTUP_INCLINE);
                            RF.StartUp_Resistance = RF_AP_Get_Data_From_Rx(RX_STARTUP_RESISTANCE);
                            RF.StartUp_UserNumber = RF_AP_Get_Data_From_Rx(RX_USER_NUMBER);
                        }
                        if(by_State == D_STATE_IN_USE)
                        {
                            RF.RF_Target_Time = RF_AP_Get_Data_From_Rx(RX_TARGET_WORKOUT_TIME);
                            RF.Terrain_Incline = RF_AP_Get_Data_From_Rx(RX_TERRAIN_INCLINE);
                            RF.Terrain_Resistance = RF_AP_Get_Data_From_Rx(RX_TERRAIN_RESISTANCE);
                        }
                        RF.Enable_Tx = 1;
                        RF.CCADelay = 50;
                    }
                    RF.SPI_Num_Buffer = ucBuffer1;
                    RF.RF_Num_Buffer = ucBuffer2;
                }
                RF.RF_First = 0;
            }
            else
            {
                RF.Enable_Tx = 1;
                RF.CCADelay = 50;
            }
            RF.RRDelay = 1000;
        }
        if(RF.RRDelay == 0)
        {
          RF_AP_Initial_IO();
          RF_AP_Set_Channel_Information(SystemConfig_Get_Char(RF_CHANNEL_UCHAR));
          RF_AP_Set_Receive_Information(SystemConfig_Get_Char(RF_ADDRESS_UCHAR));
        }
        RF.ReFlushDelay = 3000;
        Receive_Update = 0;
        RF_AP_Set_ReFlush_Flag();
    }
    else if(RF.ReFlushDelay == 0)
    {
        RF_AP_Set_ReFlush_Flag();
        RF_AP_Initial_IO();
        RF_AP_Set_Channel_Information(SystemConfig_Get_Char(RF_CHANNEL_UCHAR));
        RF_AP_Set_Receive_Information(SystemConfig_Get_Char(RF_ADDRESS_UCHAR));
        RF.ReFlushDelay = 3000;
        if(_lostConnect!=1)
        {
            RF_Event.fields.rf_offline = 1;
        }
        _lostConnect = 1;
    }

    if(RF.CCADelay == 0 && RF.Enable_Tx && HAL_RF_Get_GDO2_Pin_Input_State())
    {
        RF.Enable_Tx = 0;
        RF.Transmit_Buffer[0] = TX_PACKET_LENGTH - 1;                        // Packet length
        RF.Transmit_Buffer[1] = SystemConfig_Get_Char(RF_ADDRESS_UCHAR);;                     // Packet address
        RF_AP_Make_CheckSum(&RF.Transmit_Buffer[2], TX_PACKET_LENGTH - 2);
        RFSendPacket((char *)RF.Transmit_Buffer, TX_PACKET_LENGTH);              // Send value over RF
    }
}
static void RF_AP_Make_CheckSum(unsigned char *PBuffer, unsigned char ucLength)
{
    unsigned char Sum = 0;
    unsigned char i;
    for(i = 0; i < ucLength - 1; i++)
    {
        Sum = Sum + *(PBuffer + i);
    }
    *(PBuffer + i) = Sum;
}

static unsigned char RF_AP_Check_Data_Checksum(unsigned char *PBuffer, unsigned char ucLength)
{
    unsigned char Sum = 0;
    unsigned char i;
    for(i = 0; i < ucLength - 1; i++)
    {
        Sum = Sum + *(PBuffer + i);
    }
    if(Sum == *(PBuffer + i))
    {
        return 1;
    }
    return 0;
}

//Transform the Data from Rx Buffer to variable
static unsigned int RF_AP_Get_Data_From_Rx(unsigned char ucIndex)
{
    unsigned int uiOut = 0;

    if(ucIndex <= Rx_Map_Max)
    {
        if(Rx_Map[ucIndex][1] == 2)
        {
            uiOut = RF.Receive_APBuffer[Rx_Map[ucIndex][0]];
            uiOut = uiOut + (((unsigned int)(RF.Receive_APBuffer[Rx_Map[ucIndex][0] + 1])) << 8);
        }
        else
        {
            uiOut = RF.Receive_APBuffer[Rx_Map[ucIndex][0]];
        }
    }
    return uiOut;
}

static void RF_AP_Set_Data_To_Tx(RF_TX_MODE by_Mode, unsigned char ucIndex, unsigned int uiData)
{
    unsigned char by_Max = 0;
    const unsigned char *Point;
    switch(by_Mode)
    {
    case TX_MODE_INUSE:
        by_Max = Tx_Map_Inuse_Max;
        Point = &Tx_Map_Inuse[0][0];
        break;
    case TX_MODE_FINISH:
        by_Max = Tx_Map_Finish_Max;
        Point = &Tx_Map_Finish[0][0];
        break;
    case TX_MODE_IDLE:
        by_Max = Tx_Map_Idle_Max;
        Point = &Tx_Map_Idle[0][0];
        break;
    default:
        break;
    }

    if(ucIndex <= by_Max)
    {
        if(*(Point + ucIndex * 2 + 1) == 2)
        {
            RF.Transmit_Buffer[*(Point+ucIndex*2) + 2] = (unsigned char)uiData;
            RF.Transmit_Buffer[*(Point + ucIndex*2) + 3] = (unsigned char)(uiData >> 8);
        }
        else
        {
            RF.Transmit_Buffer[*(Point + ucIndex*2) + 2] = (unsigned char)(uiData);
        }
    }
}

static void RF_Set_Receive_Buffer_Point(unsigned char *ucPoint, unsigned int *uiPoint)
{
    ucReceive = ucPoint;
    CCA_Point = uiPoint;
}

static void RF_Initial_Data(void)
{
    Receive_Update = 0;
    CCA_Detect = 0;
}

static void RF_Set_CCA_Point_Data(unsigned char by_Data)
{
    *(CCA_Point) = by_Data;
}

static void RF_Set_CCA_Detect_Data(unsigned char by_Data)
{
    CCA_Detect = by_Data;
}

void RF_AP_SYNC_Process_Start(void)
{
    RF_AP_Set_RF_Mode(RF_STATE_SYNC);

    RF.RF_First = 1;
    RF.RF_SYNC_Stage = SYNC_STAGE_START;
    RF.RF_Address = 0;
    RF.RF_Delay_Time = 230;
    RF.RF_Sync_Timer = 0;
    RF_AP_Set_Channel_Information(RF.RF_Address);
    RF_AP_Set_Receive_Information(RF_PARTNERSHIP_ADDRESS);
}

static void RF_AP_Mainprocess_TX_IDEL(void)
{
   
    RF.RF_Run_Status = 0 ;
    
    RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_ID,SystemConfig_Get_Char(RF_ADDRESS_UCHAR));
    RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_CHANNEL,SystemConfig_Get_Char(RF_CHANNEL_UCHAR));
    RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_CONSOLE_STATE,D_STATE_IDLE);
    RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_UNIT_MODE,Get_User_Unit());
    RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_MACHINE_MODE,SystemConfig_Get_Char(MACHINE_TYPE_UINT8));
    RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_MAX_WORKOUT_TIME,99*60);
    RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_MIN_WORKOUT_TIME,5*60);
    RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_MAX_INCLINE,(SystemConfig_Get_Int(MAX_INCLINEX10_UINT16)/5));
    RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_MIN_INCLINE,(SystemConfig_Get_Int(MIN_INCLINEX10_UINT16)/5));
    RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_MAX_RESISTANCE,SystemConfig_Get_Char(MAX_RESISTANCE_UINT8));
    RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_MIN_RESISTANCE,SystemConfig_Get_Char(MIN_RESISTANCE_UINT8));//0);
    
    if(Get_User_Unit()== STANDARD)//Metric
    {
        RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_MAX_SPEED,SystemConfig_Get_Int(MAX_MPHX10_UINT16));//D_MAX_SPEED_MPHX10);//120);
        RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_MIN_SPEED,SystemConfig_Get_Int(MIN_MPHX10_UINT16));//5);
        
        RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_MAX_WEIGHT,SystemConfig_Get_Int(MAX_WEIGHT_STANDARD_UINT16));//148);
        RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_MIN_WEIGHT,SystemConfig_Get_Int(MIN_WEIGHT_STANDARD_UINT16));//23);
    }
    else//Standard
    {
        RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_MAX_SPEED,SystemConfig_Get_Int(MAX_KPHX10_UINT16));//D_MAX_SPEED_MPHX10);//120);
        RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_MIN_SPEED,SystemConfig_Get_Int(MIN_KPHX10_UINT16));//5);
        
        RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_MAX_WEIGHT,SystemConfig_Get_Int(MAX_WEIGHT_METRIC_UINT16));//D_MAX_USER_WEIGHT_LBS);//325);
        RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_MIN_WEIGHT,SystemConfig_Get_Int(MIN_WEIGHT_METRIC_UINT16));//50);
    }
    
//    unsigned char mix  ;
//    //mix  = USer_Get_MaxUser();;
//    mix  = 1 ;
//    mix = mix<<4;
//    mix = mix|0x04;
    RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_USER_COUNT,USer_Get_MaxUser());
    // "mix" high 4bit is current user number   low 4bit is total user numbers
    // for example 0x12 means  user1 total two users
    RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_CONSOLE_SW_VERSION,FRAME_WORK_VISION);//console version
    RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_RF_SW_VERSION,0);
    RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_SCAN_CHANNEL_MASK,0);
    RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_SCAN_CHANNEL,0);
    RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_SYNC_ID,0);
    RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_SYNC_CHANNEL,0);
    RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_CONSOLE_STATE,D_STATE_IDLE);
    
    RF_AP_SYNC_Main_Process();//---SYNC_TIMEOUT---SYNC_FINISH---
    
    RF_AP_Main_Process();
   
   
    //以下是RF在Program为IDLE时的状态切换。
    new_receive= RF_AP_Get_STB_StartUp_Data(D_STARTUP_DATA_USERNUMBER);
    
    if( (old_receive !=  new_receive) && RF_Check_User_Flag )
    {
        unsigned int TempData = RF_AP_Get_STB_StartUp_Data(D_STARTUP_DATA_USERNUMBER);;
        /* only run here when user is changed */
        RF_Check_User_Flag = 0;
        old_receive = new_receive;
        USER_Set_UserSlot(TempData);
        Beep_Set_Beeps(1,3,1);
    }
    
}

static void RF_AP_Mainprocess_TX_INUSE(void)
{
    if(!RF.RF_Run_Status && RF.STB_Setting ==1)
    {
        RF.RF_SYNC_Stage = 0 ;
        RF.RF_Sync_Timer = 0 ;
        
        RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_ID,SystemConfig_Get_Char(RF_ADDRESS_UCHAR));
        RF_AP_Set_Data_To_Tx(TX_MODE_IDLE,TX_IDLE_CHANNEL,SystemConfig_Get_Char(RF_CHANNEL_UCHAR));
        RF_AP_Set_RF_Mode(RF_STATE_NORMAL);
          
        RF.RF_Run_Status = 1 ;

        /*RF connected的时候才去同步数据*/
        if(_lostConnect==0)
        {
            RF.STB_Setting = 0;
            
            UINT32 TempData = 0 ;
            TempData=RF_AP_Get_STB_StartUp_Data(D_STARTUP_DATA_WORKOUTTIME);
            Program_Set_Parameter(VK_SET_WORKOUT_TIME,&TempData);    

            TempData = RF_AP_Get_STB_StartUp_Data(D_STARTUP_DATA_INCLINE)*5;
            Program_Set_Parameter(VK_SET_INCLINE,&TempData);
            
            TempData=RF_AP_Get_STB_StartUp_Data(D_STARTUP_DATA_SPEED);
            Program_Set_Parameter(VK_SET_SPEED,&TempData);
            
            TempData=RF_AP_Get_STB_StartUp_Data(D_STARTUP_DATA_RESISTANCE);
            Program_Set_Parameter(VK_SET_RESISTANCE,&TempData); 
        }
    }
    if(RF.RF_Run_Status)   
    {
        RF_AP_Main_Process();//for updata  Packet length address.....
        RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_ID,SystemConfig_Get_Char(RF_ADDRESS_UCHAR));
        RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_CHANNEL,SystemConfig_Get_Char(RF_CHANNEL_UCHAR));
        
        if(Program_Get_Status()==PS_RUN)
            RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_CONSOLE_STATE,D_STATE_IN_USE);
        else 
            RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_CONSOLE_STATE,D_STATE_PAUSE);
        
        RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_UNIT_MODE,Get_User_Unit());
        RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_MACHINE_MODE,SystemConfig_Get_Char(MACHINE_TYPE_UINT8));
        RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_WORKOUT_TIME,Program_Get_WorkoutTime_Remaining());
        RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_DISTANCE,Program_Get_Distance(DATA_AUTO));
        RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_CALORIES,Program_Get_Calories(1));
        RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_HEART_RATE,Program_Get_HR(CURRENT_HR));
        RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_SPEED,Program_Get_Speed(DATA_AUTO,0));
        RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_TARGET_WORKOUT_TIME,Program_Get_WorkoutTime());
        
        
        RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_INCLINE,Program_Get_Incline(0)/10);
		UINT32 l_Dat = Program_Get_Pace();
    	l_Dat = (l_Dat/60*100)+(l_Dat%60);		
        RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_CURRENT_PACE,l_Dat);
        RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_RESISTANCE,Program_Get_Resistance(0));
        RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_RPM,Program_Get_RPM());
        RF_AP_Set_Data_To_Tx(TX_MODE_INUSE,TX_INUSE_WATTS,Program_Get_Watts(CURRENT_WATTS,0));
        
        if(_lostConnect==0 && RF_Event.event==0 )
        {
            if(w_Terrain_Data != RF_AP_Get_STB_Terrain_Data(MODE_TERRAIN_INCLINE))
            {
                signed int Temp_RF_Incline = RF_AP_Get_STB_Terrain_Data(MODE_TERRAIN_INCLINE);
                w_Terrain_Data = Program_Get_Incline(1) + (Temp_RF_Incline - w_Terrain_Data)/5;
                Program_Set_Parameter(VK_SET_INCLINE,&w_Terrain_Data);
                w_Terrain_Data = RF_AP_Get_STB_Terrain_Data(MODE_TERRAIN_INCLINE);
            }
            
            w_Get_STB_Terrain_Data=(signed int)RF_AP_Get_STB_Terrain_Data(MODE_TERRAIN_RESISTANCE);
            if((w_Get_STB_Terrain_Data<=SystemConfig_Get_Char(MAX_RESISTANCE_UINT8)) && (w_Get_STB_Terrain_Data>SystemConfig_Get_Char(MIN_RESISTANCE_UINT8)))
            {
                if(w_Terrain_Data_RESISTANCE != w_Get_STB_Terrain_Data)
                {
                    signed int Temp_RF_Res_Data = w_Get_STB_Terrain_Data - w_Terrain_Data_RESISTANCE;
                    w_Terrain_Data_RESISTANCE = Program_Get_Resistance(1)+Temp_RF_Res_Data;
                    Program_Set_Parameter(VK_SET_RESISTANCE,&w_Terrain_Data_RESISTANCE); 
                    w_Terrain_Data_RESISTANCE = w_Get_STB_Terrain_Data;
                }
            }
        }
    }
}
static void RF_AP_Mainprocess_TX_FINISH(void)
{
    RF_AP_Main_Process();
    RF_AP_Set_Data_To_Tx(TX_MODE_FINISH,TX_FINISH_ID,SystemConfig_Get_Char(RF_ADDRESS_UCHAR));
    RF_AP_Set_Data_To_Tx(TX_MODE_FINISH,TX_FINISH_CHANNEL,SystemConfig_Get_Char(RF_CHANNEL_UCHAR));
    RF_AP_Set_Data_To_Tx(TX_MODE_FINISH,TX_FINISH_CONSOLE_STATE,D_STATE_FINISH);
    RF_AP_Set_Data_To_Tx(TX_MODE_FINISH,TX_FINISH_UNIT_MODE,Get_User_Unit());
    RF_AP_Set_Data_To_Tx(TX_MODE_FINISH,TX_FINISH_MACHINE_MODE,SystemConfig_Get_Char(MACHINE_TYPE_UINT8));
    
    RF_AP_Set_Data_To_Tx(TX_MODE_FINISH,TX_FINISH_TOTAL_WORKOUT_TIME,RF_Workoutdata._timeelapsed);
    RF_AP_Set_Data_To_Tx(TX_MODE_FINISH,TX_FINISH_TOTAL_DISTANCE,RF_Workoutdata._distance);
    RF_AP_Set_Data_To_Tx(TX_MODE_FINISH,TX_FINISH_TOTAL_CALORIES,RF_Workoutdata._calories);
    RF_AP_Set_Data_To_Tx(TX_MODE_FINISH,TX_FINISH_AVERAGE_HEART_RATE,RF_Workoutdata._averageHeartrate);    
    RF_AP_Set_Data_To_Tx(TX_MODE_FINISH,TX_FINISH_AVERAGE_PACE,RF_Workoutdata._averagepace);
}    


RF_EVENT Get_RF_Events(void)
{
    RF_EVENT ret_evt = RF_EVENT_NONE;
    if(RF_Event.fields.rf_online == 1)
    {
        ret_evt = RF_ONLINE;
    }
    if(RF_Event.fields.rf_offline == 1)
    {
        ret_evt = RF_OFFLINE;
    }
    if(RF_Event.fields.rf_syncing == 1)
    {
        ret_evt = RF_SYNC_ING;
    }
    if(RF_Event.fields.rf_syncfinish == 1)
    {
        ret_evt = RF_SYNC_FINISH;
    }
    if(RF_Event.fields.rf_synctimeout == 1)
    {
        ret_evt = RF_SYNC_TIMEOUT;
    }
    RF_Event.event = 0 ;
    
    return(ret_evt);
}

static void RF_RefreshWorkoutData(void)
{
    if( g_RFSafeyKeyStatus == RFSAFETYKEY_STATUS_IDEL )
    {
        RF_Workoutdata._timeelapsed = Program_Get_WorkoutTime_Elapsed() ;
        RF_Workoutdata._distance = Program_Get_Distance(DATA_AUTO) ;
        RF_Workoutdata._calories = Program_Get_Calories(1) ;
        RF_Workoutdata._averageHeartrate = Program_Get_HR(AVERGE_HR);
        RF_Workoutdata._averagepace = Program_Get_Pace_Average();
    }
}

//RF interface 
RF_EVENT RF_AP_Process(ReturnEventHandler flag)
{	
    RF_EVENT g_RF_Events = RF_EVENT_NONE;
    if(!RF_isEnable) 
    {
        _lostConnect = 1;
        return g_RF_Events;
    }


    PROGRAM_STATUS _tempStaus = Program_Get_Status();
    if( RFSAFETYKEY_STATUS_OFFDELAY == g_RFSafeyKeyStatus )
    {
      _tempStaus = PS_RUN ;
    }
    if( RFSAFETYKEY_STATUS_OFFDELAYTIMEUP == g_RFSafeyKeyStatus )
    {
        g_RFSafeyKeyStatus = RFSAFETYKEY_STATUS_FINISHDELAY ;
        g_RFSafeyKeyDelayCnt = 0 ;
        _tempStaus = PS_COMPLETED ;
    }
    if( RFSAFETYKEY_STATUS_FINISHDELAY == g_RFSafeyKeyStatus )
    {
            _tempStaus = PS_COMPLETED ; 
    }
    if( RFSAFETYKEY_STATUS_FINISHDELAYTIMEUP == g_RFSafeyKeyStatus )
    {
        g_RFSafeyKeyStatus = RFSAFETYKEY_STATUS_IDEL ;
    }
    switch( _tempStaus )
    {
        case PS_IDLE:
        case PS_SELECTPRG:
        case PS_SETUP:
        {
            if(RF_Key_Event.fields.key_rf_sync == 1)
            {
                RF_AP_SYNC_Process_Start();
                RF_Event.fields.rf_syncing = 1; 
            }
            RF_Key_Event.event = 0 ;
            memset(&RF_Workoutdata,0,sizeof( RF_Workoutdata ) );
            RF_AP_Mainprocess_TX_IDEL();
        }
        break;
        
        case PS_WARMUP:
        case PS_COOLDOWN:
        case PS_PAUSED:
        case PS_RUN:
        {
            RF_Key_Event.event = 0 ;
            RF_AP_Mainprocess_TX_INUSE();
            RF_RefreshWorkoutData();
        }
        break;
        case PS_COMPLETED:
       {
            RF_AP_Mainprocess_TX_FINISH();
       }
       break;
        default:
        break;
    }
    if(flag < RET_PROCESS_DISPLAY)
		RF_Event.event = 0 ;
    
    if(RF_Event.fields.rf_online == 1)
    {
        g_RF_Events = RF_ONLINE;
    }
    if(RF_Event.fields.rf_offline == 1)
    {
        g_RF_Events = RF_OFFLINE;
    }
    if(RF_Event.fields.rf_syncing == 1)
    {
        g_RF_Events = RF_SYNC_ING;
    }
    if(RF_Event.fields.rf_syncfinish == 1)
    {
        g_RF_Events = RF_SYNC_FINISH;
    }
    if(RF_Event.fields.rf_synctimeout == 1)
    {
        g_RF_Events = RF_SYNC_TIMEOUT;
    }
    RF_Event.event = 0 ;
    return(g_RF_Events);
}

void RF_AP_Initial_Data(void)
{
    RF_AP_Set_Channel_Information(SystemConfig_Get_Char(RF_CHANNEL_UCHAR));
    RF_AP_Set_Receive_Information(SystemConfig_Get_Char(RF_ADDRESS_UCHAR));
    RF_Initial_Data();
    RF_Set_Receive_Buffer_Point(&RF.Receive_Buffer[0], &RF.CCATimer);
   
    memset(RF.Receive_Buffer,0,RX_PACKET_LENGTH);
    memset(RF.Receive_APBuffer,0,RX_PACKET_LENGTH);
    memset(RF.Transmit_Buffer,0,RX_PACKET_LENGTH);

    RF.Transmit_Buffer[0] = TX_PACKET_LENGTH - 1;
    RF.Transmit_Buffer[1] = SystemConfig_Get_Char(RF_ADDRESS_UCHAR);
    //RF.Transmit_Buffer[1] = Information_Get_Data(IM_RF_ADDRESS);
    RF.Enable_Tx = 0;
    
    RF.CCATimer = 3000;
    RF.CCADelay = 0;
    RF.RRDelay = 1000;
    RF.ReFlushDelay = 0;
    
    RF.RF_SYNC_Stage = SYNC_OFFLINE;
    RF.RF_Mode = RF_STATE_NORMAL;
    RF_Event.fields.rf_offline = 1; 
    
    RF.SYNC_ID = 0;
    RF.SYNC_CHANNEL = 0;
    
    RF.STB_Setting = 0;
    
    RF.Terrain_Incline = 0;
    RF.Terrain_Resistance = 0;
    
    RF.RF_First = 1;
    RF.SPI_Num_Buffer = 0;
    RF.RF_Num_Buffer = 0;
    RF.SPI_Count = 0;
    RF.RF_Count = 0;
    RF.RF_Correct_SelfCount = 0;
    RF.RF_All_SelfCount = 0;
    
    RF.StartUp_UserNumber = 0xff; // Guest
    
    RF.RF_Target_Time = 0xffff;
    RF.RF_Run_Status = 0 ;
    _lostConnect = 1 ;
    RF_Key_Event.event = 0 ;
    RF_Event.event = 0 ;
    RF_Event.fields.rf_offline = 1;
	new_receive = 0xff;
	old_receive = 0xff;
	RF_isEnable = 1;
    g_RFSafeyKeyStatus = RFSAFETYKEY_STATUS_IDEL ;
    g_RFSafeyKeyDelayCnt = 0 ;
    memset(&RF_Workoutdata,0,sizeof( RF_Workoutdata ) );

}

unsigned int RF_AP_Get_STB_StartUp_Data(RF_D_STARTUP_DATA by_Mode)
{
    unsigned int w_Out;
    switch(by_Mode)
    {
        case D_STARTUP_DATA_WEIGHT:
        w_Out = RF.StartUp_Weight;
        break;
        case D_STARTUP_DATA_AGE:
        w_Out = RF.StartUp_Age;
        break;
        case D_STARTUP_DATA_WORKOUTTIME:
        w_Out = RF.StartUp_Workout_Time;
        break;
        case D_STARTUP_DATA_SPEED:
        w_Out = RF.StartUp_Speed;
        break;
        case D_STARTUP_DATA_INCLINE:
        w_Out = RF.StartUp_Incline;
        break;
        case D_STARTUP_DATA_RESISTANCE:
        w_Out = RF.StartUp_Resistance;
        break;
        case D_STARTUP_DATA_USERNUMBER:
        w_Out = RF.StartUp_UserNumber;
        break;
        default:
        w_Out = 0;
        break;
    }
    return w_Out;
}

unsigned int RF_AP_Get_STB_Terrain_Data(RF_TERRAIN_TYPE by_Mode)
{
    unsigned int w_Out;
    switch(by_Mode)
    {
    case MODE_TERRAIN_INCLINE:
        w_Out = RF.Terrain_Incline;
        break;
    case MODE_TERRAIN_RESISTANCE:
        w_Out = RF.Terrain_Resistance;
        break;
    default:
        w_Out = 0;
    }
    return w_Out;
}

VR_KEY RF_Set_VirtualKey(VR_KEY vk)
{
    VR_KEY ret = vk;
    //ret = VK_PROCESSED ;
    if( ret == VK_SAFETY_KEY_OFF )
    {
        RF_Key_Event.fields.key_safetykeyoff = 1 ;
        if( RF.RF_Run_Status )
        {
          g_RFSafeyKeyStatus =  RFSAFETYKEY_STATUS_OFFDELAY ;
          g_RFSafeyKeyDelayCnt = 0 ;
        }
    }   
    if( ret == VK_RF_SYNC )
    {
        RF_Key_Event.fields.key_rf_sync = 1;
    }
    return ret;
}


bool RF_Get_Connected(void)
{
 if( DigitalGetRFstatus())
   return 1;
 
#ifdef RF_MODULE_EXIST_
	return(_lostConnect==1?0:1);
#else
	return 0;
#endif
}

void RF_Set_RF_Enable(bool isEnable)
{
	RF_isEnable = isEnable;
	if(RF_isEnable)
	{
		RF_AP_Initial_IO();		
	}
	else
	{
		HAL_RF_Clear_INT_Flag(0);
    	HAL_RF_Config_GDO0(0);
		HAL_RF_Clear_INT_Flag(1);
    	HAL_RF_Config_GDO0(1);
	}
}
