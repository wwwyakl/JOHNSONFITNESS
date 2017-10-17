#include "Auto_Model.h"
#include "SystemConfig.h"
#include "em_assert.h"
#include "LCB.h"
#include "Events_Handler.h"
#include "Global_Config.h"

__no_init static union
{
  struct
  {
      UINT8  key_up :1;
      UINT8  key_dn :1;
      UINT8  key_save: 1;
  }fields;
  UINT8 event;
}Model_event;

typedef enum
{
    AUTO_STATE_START = 0,
    AUTO_STATE_LANGUAGE=AUTO_STATE_START,
    AUTO_STATE_MODEL,
    AUTO_STATE_UNIT,
    AUTO_STATE_END,
}AutoMode_Machine_State;


__no_init static UINT8 g_Mode_Set_Slot;
__no_init static AutoMode_Machine_State g_AutoMode_Machine_State;
__no_init static UINT8 Display_Change;
__no_init static const MODEL_DEFAULTS *gp_ModelDefaults;
__no_init static UINT8 g_TotalModels;
__no_init static UINT8 g_ModelIndex;
__no_init static const UINT8 *gp_Language;
__no_init static UINT8 g_TotalLanguages;

extern UINT8 FlashHaveLanguageFlag;
UINT8 ShowUpGradeLanguageFlag=0;
static UINT8 ModelFindAllMatchedModels(UINT8 *p, MCB_TYPE mcbtype)
{
    UINT8 len = 0;
   
    switch(mcbtype)
    {
    case AC_LCB_JIS:            //=0x01,   //电跑下控
    case LCB1_JHT:              //=0x02,   //电跑下控
    case LCB3_JHT:              //=0x03,   //电跑下控
    case DC_LCB_JIS:            //=0x04,   //电跑JIS直流电跑
    case LCB_CD_COSTDN_JHT:     //=0x05,   //电跑下控
    case TM_LCB_PANATTA:         //=0x06,   //电跑JIS直流电跑
    case LCB1_PWN_JHT:          //=0x07,   //电跑下控
    case LCB3_CURRENT_JHT:      //=0x08,   //电跑下控
    case LCB1_PANATTA:          //=0x09,   //电跑下控
    case LCB2_A5X_REDESIGN:     //=0x0A,   //电跑下控
    case EP_0B_JIS:             //=0x0B,   //升降EP
    case BIKE_0C_JIS:           //=0x0C,   //电磁铁BIKE下控--是否有升降未知
    case DC_LCB_20_275HP_JIS:   //=0x0D,   //电跑下控
    case DC_LCB_30_325HP_JIS:   //=0x0E,   //电跑下控
    case AC_LCB_20_275HP_JIS:   //=0x0F,   //电跑下控
    case AC_LCB_30_325HP_JIS:   //=0x10,   //电跑下控
    case AD_BOARD_JIS:          //=0x11,
    case DIGITAL_ECB:           //=0x12,   //ECB下控--是否有升降未知
    case LCBA:                  //=0x13,
    case INDUCTOR_BRAKE_INCLINE: //=0x15,   //有升降电磁铁下控
    case MCB_RETAIL_CLIMBMILL://  =0X19,   //Retail Climbmill
    case ECB_JIS:               //=0xB4,   //无升降ECB下控
    case ECB_INDUCTOR_BRAKE:    //=0xB5,   //电磁铁下控--是否有升降未知
    case DCI_DAUGHTER_BOARD:    //=0xC0,
    case MCB_TOPTEK:            //=0xC1,   //电跑阳刚下控
    case MCB_DELTA:             //=0xC2,   //电跑台达下控
    case MCB_DELTA_COMMOCIAL:   //=0xC3    //电跑台达商用下控
        for(UINT8 i = 0; i < g_TotalModels; i++)
        {
            if((gp_ModelDefaults + i)->mcbType == mcbtype)
            {
                p[len] = i;
                if(len < MAX_SUPPORTED_MODELS - 1) len++;
            }
        }
        break;
    default:
        for(UINT8 i = 0; i < g_TotalModels; i++)
        {
            p[len] = i;
            if(len < MAX_SUPPORTED_MODELS - 1) len++;
        }
        break;
    }
	if(len==0)
	{
		 for(UINT8 i = 0; i < g_TotalModels-2; i++)
        {
                p[len] = i;
                if(len < MAX_SUPPORTED_MODELS - 1) len++;
        }
	}
    return len;
}

static MODEL_EVENT ModelHandleKeyUpDown(bool isUP, UINT8 *p, MCB_TYPE mcbtype)
{
    UINT8 _length = ModelFindAllMatchedModels(p, mcbtype);
	UINT8 templength;
	
    if(_length == 0)  //return MODEL_EVENT_UNSUPPORTED_MCB_FOUND; 
		templength = g_TotalModels;
	else
		templength = _length - 1;
    if(isUP)
    {
        if(g_ModelIndex < templength)
        {
            g_ModelIndex++;
            if(g_ModelIndex==0x0C)
                g_ModelIndex = 0x0E;
        }
        else
            g_ModelIndex = 0;
        g_Mode_Set_Slot = p[g_ModelIndex];
        
    }
    else
    {
        if(g_ModelIndex > 0)
        {
            g_ModelIndex--;
            if(g_ModelIndex==0x0D)
                g_ModelIndex = 0x0B;
        }
        else
            g_ModelIndex = templength;
        g_Mode_Set_Slot = p[g_ModelIndex];
        
    }
    return MODEL_EVENT_MODEL_CHANGED;
}

void Model_Initial_Para(void)
{
    Model_event.event = 0;
    g_Mode_Set_Slot = 0 ;
    Display_Change = 1;
    g_AutoMode_Machine_State = AUTO_STATE_LANGUAGE;
    g_ModelIndex = 0;
}

void Model_Initial_Data(const MODEL_DEFAULTS *pModelDefaults, UINT8 totalModels, const UINT8 *pLang, UINT8 totalLang)
{    
    if(SystemConfig_Get_Char(IS_MACHINE_CONFIGED_UCHAR)==0xAA) return;
    EFM_ASSERT(totalModels <= MAX_SUPPORTED_MODELS ||totalLang < ALL_LANGUAGE_ID);
    
    gp_ModelDefaults = pModelDefaults;
    g_TotalModels = totalModels;
    
    gp_Language = pLang;
    g_TotalLanguages = totalLang;
    Model_Initial_Para();
    
}



UINT8 Model_Get_Set_Slot(void)
{
    UINT8 _Slot = g_Mode_Set_Slot;
    if((_Slot>=11)&&(_Slot<=13))
        _Slot = 11; 
//    if(_Slot==13) _Slot = 11;
    if(_Slot==14) _Slot = 12;
    if(_Slot==15) _Slot = 13;
    if(_Slot>=16) _Slot = 14;
    return( _Slot);
}
UINT8 Model_Get_Language_Index(void)
{
    return( g_ModelIndex);
}
void Model_Set_VirtualKey(UINT16 vk)
{
    UINT16 TempKey = vk & 0xFF00;
    //if(TempKey != FRAME_STAGE_MODEL) return;
    
    TempKey = (UINT8)(vk&0x00FF);
    switch(TempKey)
    {
        case VK_INCLINEUP:
		case VK_SPEEDUP:		
		case VK_UP:		
            Model_event.fields.key_up = 1 ;
            break;
		case VK_INCLINEDOWN:
		case VK_SPEEDDOWN:		
		case VK_DOWN:
            Model_event.fields.key_dn = 1 ;
            break;
        case VK_ENTER:	
		case VK_CHANGE_DISPLAY:
        case VK_ENTER_CHANGEDIS:            
            Model_event.fields.key_save = 1 ;
            break;
        default:break;    
    }
}

MODEL_EVENT Model_Process(void)
{
    MODEL_EVENT evt=MODEL_EVENT_NONE;
    extern     UINT8 REFRESH_WINDOW_CHANNEL;
    if(SystemConfig_Get_Char(IS_MACHINE_CONFIGED_UCHAR)==0xAA) return(evt);
    
    if(Display_Change) 
    {
        Display_Change = 0 ;
        evt = MODEL_FIRST_DISPLAY;
    }
    switch(g_AutoMode_Machine_State)
    {
    case AUTO_STATE_LANGUAGE:
        {
            if(Timer_Counter(REFRESH_WINDOW_CHANNEL,2))
            {
                Event_Refresh_Timer_Handler();
            }
            if(ShowUpGradeLanguageFlag>=100)
            {
                ShowUpGradeLanguageFlag = 0;
                g_ModelIndex = 0;
                evt = MODEL_EVENT_LANGUAGE_CHANGE;
            }
            if( Model_event.fields.key_up )
            {
                    if(g_ModelIndex <  g_TotalLanguages -1)
                        g_ModelIndex++;
                    else
                        g_ModelIndex = 0;
                    g_Mode_Set_Slot = gp_Language[g_ModelIndex];
                    evt = MODEL_EVENT_LANGUAGE_CHANGE;
            }
            else if( Model_event.fields.key_dn )
            {
                    if(g_ModelIndex > 0 )
                        g_ModelIndex--;
                    else
                        g_ModelIndex =  g_TotalLanguages -1;
                    g_Mode_Set_Slot = gp_Language[g_ModelIndex];
                    evt = MODEL_EVENT_LANGUAGE_CHANGE;
            }
            else if( Model_event.fields.key_save )
            {
                MACHINE_INFO machineinfo = SystemConfig_Get_MachineInfo();
                machineinfo.languageId=g_ModelIndex;// g_Mode_Set_Slot ;
                if((FlashHaveLanguageFlag == 1)||(g_ModelIndex == 0))
                {
                    if(SystemConfig_Set_MachineInfo(machineinfo)) 
                    {
                        SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                    }
                    String_Set_Language(g_ModelIndex);
                    g_ModelIndex = 0;
                    MCB_TYPE _type = (MCB_TYPE)LCB_Get_MCBType();
             
                    UINT8 _tempBuffer[MAX_SUPPORTED_MODELS];
                    memset(_tempBuffer,0,MAX_SUPPORTED_MODELS);
                    
                    UINT8 _length = ModelFindAllMatchedModels(_tempBuffer, _type);
                    if(_length == 0)
                        g_Mode_Set_Slot = 0;
                    else
                        g_Mode_Set_Slot = _tempBuffer[g_ModelIndex];
                    evt = MODEL_EVENT_MODEL_CHANGED;
                    g_AutoMode_Machine_State = AUTO_STATE_MODEL;  
                    Language_LanguageChecking(String_Get_LanguageIndex());
                }
                else
                {
                    ShowUpGradeLanguageFlag = 1;
                    evt = MODEL_EVENT_FLASH_HAVE_NO_LANGUAGE; 
                }
            }
        }
        break;
    case AUTO_STATE_MODEL:
        {
            MCB_TYPE _type = (MCB_TYPE)LCB_Get_MCBType();
            UINT8 _tempBuffer[MAX_SUPPORTED_MODELS];
            memset(_tempBuffer,0,MAX_SUPPORTED_MODELS);
            if(Model_event.fields.key_up)
            {
                evt =ModelHandleKeyUpDown(true, _tempBuffer, _type);
            }
            else if( Model_event.fields.key_dn )
            {
                evt =ModelHandleKeyUpDown(false,_tempBuffer, _type);
            }          
            else if( Model_event.fields.key_save )
            {
                MACHINE_INFO machineinfo = SystemConfig_Get_MachineInfo();
                machineinfo.maxSpeedMphX10 = (gp_ModelDefaults+g_Mode_Set_Slot)->maxSpeedSTDX10;
                machineinfo.minSpeedMphX10 = (gp_ModelDefaults+g_Mode_Set_Slot)->minSpeedSTDX10;
                machineinfo.maxSpeedKphX10 = (gp_ModelDefaults+g_Mode_Set_Slot)->maxSpeedKPHX10;
                machineinfo.minSpeedKphX10 = (gp_ModelDefaults+g_Mode_Set_Slot)->minSpeedKPHX10;
                machineinfo.maxRpmMphX10 = (gp_ModelDefaults+g_Mode_Set_Slot)->maxRPMSTD;
                machineinfo.minRpmMphX10 = (gp_ModelDefaults+g_Mode_Set_Slot)->minRPMSTD;
                machineinfo.maxRpmKphX10 = (gp_ModelDefaults+g_Mode_Set_Slot)->maxRPMKPH;
                machineinfo.minRpmKphX10 = (gp_ModelDefaults+g_Mode_Set_Slot)->minRPMKPH;
                machineinfo.maxInclineX10 = (gp_ModelDefaults+g_Mode_Set_Slot)->maxIncline;
                machineinfo.minInclineX10 = (gp_ModelDefaults+g_Mode_Set_Slot)->minIncline;
                machineinfo.maxInclineADC= (gp_ModelDefaults+g_Mode_Set_Slot)->maxInclineADC;
                machineinfo.minInclineADC= (gp_ModelDefaults+g_Mode_Set_Slot)->minInclineADC;
                machineinfo.maxResistance= (gp_ModelDefaults+g_Mode_Set_Slot)->maxResistance;
                machineinfo.minResistance= (gp_ModelDefaults+g_Mode_Set_Slot)->minResistance;
                machineinfo.machineType= (gp_ModelDefaults+g_Mode_Set_Slot)->machineType;
                machineinfo.maxUserWeightLbs= (gp_ModelDefaults+g_Mode_Set_Slot)->maxuserweightLBS;
				machineinfo.minUserWeightLbs= (gp_ModelDefaults+g_Mode_Set_Slot)->minuserweightLBS;		
				machineinfo.maxUserWeightKg= (gp_ModelDefaults+g_Mode_Set_Slot)->maxuserweightKG;
				machineinfo.minUserWeightKg= (gp_ModelDefaults+g_Mode_Set_Slot)->minuserweightKG;
                machineinfo.driveType= (gp_ModelDefaults+g_Mode_Set_Slot)->mcbType;
                machineinfo.machineModel= (gp_ModelDefaults+g_Mode_Set_Slot)->machineModelDAPI;
                machineinfo.brandId= (gp_ModelDefaults+g_Mode_Set_Slot)->brandid;
                
                if(SystemConfig_Set_MachineInfo(machineinfo)) 
                {
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                }
                if(SystemConfig_Get_Char(DRIVE_TYPE_UINT8) == MCB_RETAIL_CLIMBMILL )
                {
                    LCB_Send_Cmd(DS_SET_STATUS,0xA1);	
                    LCB_Send_Cmd(DS_SET_STATUS,0xB1);
                    LCB_Send_Cmd(DS_SET_MACHINE_TYEP,0x06);
	            }
                evt = MODEL_EVENT_UNIT_CHANGE;
                g_AutoMode_Machine_State = AUTO_STATE_UNIT;
                g_Mode_Set_Slot = 0 ;
                if(Get_User_Unit()==STANDARD)
                {
                    g_Mode_Set_Slot = 0;
                }
                else
                {
                    g_Mode_Set_Slot = 1;
                }
            }
            if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==TREADMILL)
            {
                LCB_Set_DriverType(LCB_MOTOR);
            }
            else if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8)==CLIMB_MILL)
            {
                LCB_Set_DriverType(RETAIL_CLIMBMILL);
            }
            else 
            {
                LCB_Set_DriverType(LCB_EP_BIKE);
            }
        }
        break;
    case AUTO_STATE_UNIT:
        {
            if( Model_event.fields.key_up || Model_event.fields.key_dn )
            {
                if(g_Mode_Set_Slot==0)
                {
                    g_Mode_Set_Slot =1 ;
                }
                else
                {
                    g_Mode_Set_Slot =0 ;
                }
                evt = MODEL_EVENT_UNIT_CHANGE;
            }
            else if( Model_event.fields.key_save )
            {
                MACHINE_INFO machineinfo = SystemConfig_Get_MachineInfo();
                machineinfo.unit= g_Mode_Set_Slot ;
                machineinfo.IsMachineConfiged= 0xAA;
//                machineinfo.IsMachineConfiged = LCB_Get_MCBType();//power on check LCB type
                if(SystemConfig_Set_MachineInfo(machineinfo)) 
                {
                    SystemConfigProcess(SVK_SAVE_MACHINEINFO2FLASH,NULL,0);
                }
                g_Mode_Set_Slot = 0 ;
                
                evt = MODEL_EVENT_COMPLETE;
            }
        }
        break;  
    default:break;
    }
    Model_event.event = 0;
    return(evt);
}
void Auto_1ms_INT(void)
{
    static UINT8 w_100ms_Time;
    w_100ms_Time++;
    if(w_100ms_Time>100)
    {
        w_100ms_Time = 0;   
        if(ShowUpGradeLanguageFlag>=1)
        {
            ShowUpGradeLanguageFlag++;
        }
    }
}
void Auto_Regist_Function(void)
{
    HAL_SystemTicker_Alloc_ISR(Auto_1ms_INT);   
}