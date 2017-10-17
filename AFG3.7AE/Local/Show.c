#include "Show.h"
#include "Events_Handler.h"
#include "Programs.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include "page.h"
#include "wlt2564_command.h"

extern FRAME_STAGE Frame_Stage; 
UINT8 BT_change[8];
UINT8   ModalWndExist;
extern PAGES const pages[PAGE_TOTAL];
//extern QUICK_KEY_FUNCTION CurrentSettingPage;
static  char sprintfBuf[30];
//Window_Items
extern  unsigned char NameSprintBuffer[65];

void ShowUsrName(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( NameSprintBuffer , '\0', 64 );
    NameSprintBuffer[64] = '\0';
  	memcpy(sprintfBuf, User_Get_Name(), 65);
    
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);   
}


void ShowWeightData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 by_Dat = 0 ;
    by_Dat = User_Get_Weight();
    
	if(p->_blinkCount%2==1)
	{
        if(SystemConfig_Get_Display_Unit(User_Get_UserSlot())==STANDARD)
        {
            DisplayString(p->_x,p->_y,0,"WEIGHT    LBS");
        }
        else
        {
            DisplayString(p->_x,p->_y,0,"WEIGHT    KG");
        }
		
	}
	else
	{
        if(SystemConfig_Get_Display_Unit(User_Get_UserSlot())==STANDARD)
        {
            sprintf(sprintfBuf,"WEIGHT %3dLBS",by_Dat);
        }
        else
        {
            sprintf(sprintfBuf,"WEIGHT %3dKG",by_Dat);
        }
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void ShowWeightUnit(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', 30 - 1 );
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf); 
}
void ShowAgeData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 by_Dat=0;
    by_Dat = User_Get_Age();
    sprintf(sprintfBuf,"%4u",by_Dat);
	if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void ShowTimezoneData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
//    UINT16 by_Dat=wifi_Get_Timezone();
//    sprintf(sprintfBuf,"%4u",by_Dat);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
}

void ShowSetGenderData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', 30 - 1 );
    sprintfBuf[29] = '\0';
    sprintf(sprintfBuf,"%4u",User_Get_Gender());
	if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void ShowMaleTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', 30 - 1 );
    sprintfBuf[29] = '\0';
    char *str ;
    if(User_Get_Gender())
    {
        str = String_Get_String(STR_MALE_ID);
    }
    else
    {
        str = String_Get_String(STR_FEMALE_ID);
    }
    
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);   
}
/***************************************************************************//**
* @brief :显示wifi图标
*
* Note：已将显示区域固化，调用时在该函数内需要传入参数(p->_isBlink)，其决定wifi图标
*闪烁与否，为0时，不闪烁；为1时，若p->_blinkCount=0xAA，则一直闪烁。
* @param[in]：(p->_isBlink)
*
* @param[in]：NONE
 ******************************************************************************/
#ifdef __WIFI_FUNC__
void ShowWifiIcon(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    p->_x = 0;
    p->_y = 5;
    if(WIFIIsAPMode())    
    {
        p->_string_id = 4;
    }
    else
    {
        p->_blinkCount = 0 ;
        p->_string_id = WIFISignalStrength();
    }
    if(p->_blinkCount==0)
        DisplayString(p->_x,p->_y,p->_string_id,NULL);  
    else
    {
        if(p->_blinkCount%2==0)
          DisplayString(p->_x,p->_y,p->_string_id,NULL);  
        else
          DisplayString(p->_x,p->_y,0,NULL);   
    }
}
#endif
void ShowCommonTextBox(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', 30 - 1 );
    sprintfBuf[29] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
		
	if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void ShowENGKeyValue(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat=Engmode_Get_KeyValue();
    sprintf(sprintfBuf,"%3u ",l_Dat);
	p->_z = 0 ;
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowENG0Number(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat=Engmode_Get_Number()*1111;
    sprintf(sprintfBuf,"%4u",l_Dat);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
const char table[10][16] =
{
    "0000000000000000",
    "1111111111111111",
    "2222222222222222",
    "3333333333333333",
    "4444444444444444",
    "5555555555555555",
    "6666666666666666",
    "7777777777777777",
    "8888888888888888",
    "9999999999999999",
};
void ShowENG0LedNumber(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    char str[17];
    memset(str,0,sizeof(str));
    memcpy(str,table[Engmode_Get_Number()],16);
    DisplayString(p->_x,p->_y,p->_z,str);
}


void ShowSafekeyOffLedNumber(void *pWindow)   //zsf
{
    Win_Items* p = (Win_Items*)pWindow;
    char str[17] = {"----------------"};
    //memset(str,0,sizeof(str));
    //memcpy(str,table[Engmode_Get_Number()],16);
    DisplayString(p->_x,p->_y,p->_z,str);
}

void ShowBottomTextBox(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', 30 - 1 );
    sprintfBuf[29] = '\0';
    if(ModalWndExist) return;
    
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  

}

void ShowXID(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', 30 - 1 );
    memcpy(sprintfBuf, User_Get_Xid(),14);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}

void ShowPasscode(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', 30 - 1 );
    memcpy(sprintfBuf, User_Get_Passcode(),4);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);   
}

void ShowMacAddress(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', 30 - 1 );
    char *str = "mac ";
    memcpy(sprintfBuf,str,sizeof(str));
    extern UINT8 BT_MacAddress[4];
    memcpy(&(sprintfBuf[sizeof(str)]), BT_MacAddress/*BT_Get_BTMac()*/,4);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);   
}
void ShowBTVersion(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', 30 - 1 );
    char *str = "BT ";
    memcpy(sprintfBuf,str,3);
    extern UINT8 BT_Version[8];
    extern UINT8 BT_change[8]; 
    int i=0;
    for(i=0;i<8;i++)
    {    
      BT_change[i] = BT_Version[i];
    if('.'==BT_Version[i])
      BT_change[i]='_';
    }
      
    memcpy(&(sprintfBuf[3]), BT_change,8);
//    memcpy(&(sprintfBuf[3]), BT_Version,8);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);   
}

void ShowBTUpdatePercent(void *pWindow)    //zsf
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', 30 - 1 );
    
    extern UINT8 BT_UpdatePercent;
    sprintf(sprintfBuf,"per %d",BT_UpdatePercent);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);   
}

void ShowQuickMul(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    char strbuf[4];
    
    memset( sprintfBuf , '\0', 30 );
    char *str = String_Get_String(p->_string_id);
    memset( strbuf ,'\0', 4 );
    
    UINT16 l_Dat=Engmode_Get_PROG_Mult_Times();

    sprintf(strbuf,"%d",l_Dat);
    sprintf(sprintfBuf,"%s %s",str,strbuf);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);   
}

void ShowERPWaitTime(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    char strbuf[4];
    
    memset( sprintfBuf , '\0', 30 );
    char *str = String_Get_String(p->_string_id);
    memset( strbuf ,'\0', 4 );

    UINT16 l_Dat=(SystemConfig_Get_Int(ERP_WAIT_MIN_UINT16))/60;

    sprintf(strbuf,"%d",l_Dat);
    sprintf(sprintfBuf,"%s %s",str,strbuf);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);   
}

void ShowSettedRPM(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    char strbuf[4];
    
    memset( sprintfBuf , '\0', 30 );
    char *str = String_Get_String(p->_string_id);
    memset( strbuf ,'\0', 4 );
    
    UINT16 l_Dat=Engmode_Get_PROG_Setted_RPM();

    sprintf(strbuf,"%d",l_Dat);
    sprintf(sprintfBuf,"%s %s",str,strbuf);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);   
}

void ShowCommonData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat=p->_string_id;
    sprintf(sprintfBuf,"%4u ",l_Dat);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}

void ShowENGSetRPMData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat=Engmode_Get_SetSpeedRPM();
    sprintf(sprintfBuf,"%4u",l_Dat);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}

void ShowENGLCBRPMData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat=LCB_Get_Data(G_MOTOR_RPM);
    sprintf(sprintfBuf,"%4u",l_Dat);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
/*
void ShowENGSpeedData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat=LCB_Get_Data(G_MOTOR_RPM);
    
    unsigned char by_SpeedBuff = 0;
    unsigned int Rpm_Range = 0;
    unsigned int Speed_Range = 0;
    
    if(SystemConfig_Get_Display_Unit(User_Get_UserSlot()) == STANDARD)
    {        
        Rpm_Range = SystemConfig_Get_Int(MAX_MPH_RPM_UINT16) - SystemConfig_Get_Int(MIN_MPH_RPM_UINT16);
        Speed_Range = SystemConfig_Get_Int(MAX_MPHX10_UINT16) - SystemConfig_Get_Int(MIN_MPHX10_UINT16);
    }
    else
    { 
        Rpm_Range = SystemConfig_Get_Int(MAX_KPH_RPM_UINT16) - SystemConfig_Get_Int(MIN_KPH_RPM_UINT16);
        Speed_Range = SystemConfig_Get_Int(MAX_KPHX10_UINT16) - SystemConfig_Get_Int(MIN_KPHX10_UINT16);
    }
    by_SpeedBuff = l_Dat/(Rpm_Range/Speed_Range);

    sprintf(sprintfBuf,"%4.2u",by_SpeedBuff);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
//    if(by_SpeedBuff != 0)
//        LED_14_SEG_Dot(6,1);
//    else
//        LED_14_SEG_Dot(6,0);
}
*/

void ShowENGSpeedData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat=Engmode_Get_SetSpeedRPM();
    sprintf(sprintfBuf,"%4.2u",l_Dat);
    p->_z = 0;
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}

void ShowENGInclineADCData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat=Engmode_Get_SetInclineADC();
    sprintf(sprintfBuf,"%4u",l_Dat);
    p->_z = 0;
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowENGLCBECBCount(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat=LCB_Get_Data(G_ECB_COUNT);
    sprintf(sprintfBuf,"%4u",l_Dat);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowENGSettedECBCount(void *pWindow)
{
	Win_Items* p = (Win_Items*)pWindow;
	UINT32 l_Dat;
	l_Dat =Engmode_Get_SetECBCount();
	sprintf(sprintfBuf,"%4u",l_Dat);
	DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowACCDis(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
    l_Dat = SystemConfig_Get_Long(ACCUMULATED_MILESX100_LONG)/100;
    if(SystemConfig_Get_Display_Unit(User_Get_UserSlot()))
    {
        l_Dat = (UINT32)(l_Dat*1.6);
    }
    memset(sprintfBuf,0,sizeof(sprintfBuf));
    sprintf(sprintfBuf,"%4u",l_Dat);
//    LED_SET_DOT(0);
	p->_z = 0 ;
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf); 
}

void ShowACCTime(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat = SystemConfig_Get_Long(ACCUMULATED_SECONDS_LONG)/3600;
    memset(sprintfBuf,0,sizeof(sprintfBuf));
    sprintf(sprintfBuf,"%4u",l_Dat);
//sprintf(sprintfBuf,"%3.2u ",l_Dat);
	p->_z = 0 ;
//    LED_SET_DOT(0);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf); 
}

extern const char* rscu_version[];

void ShowUCBVision(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    char strbuf[5];
    
    memset( sprintfBuf , '\0', 30 );
    char *str = String_Get_String(p->_string_id);
    memset( strbuf ,'\0', 5 );
    
    sprintf(strbuf,"%s.%s",rscu_version[0],rscu_version[1]);
    sprintf(sprintfBuf,"%s %s",str,strbuf);

    DisplayString(p->_x,p->_y,p->_z,sprintfBuf); 
}

void ShowMCBVision(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    char strbuf[4];
    
    memset( sprintfBuf , '\0', 30 );
    char *str = String_Get_String(p->_string_id);
    memset( strbuf ,'\0', 4 );
#ifdef TM
    UINT16 l_Dat= LCB_Get_Version();
#else
    UINT16 l_Dat= 0; 
#endif   
    sprintf(strbuf,"%3.2u ",l_Dat);
    sprintf(sprintfBuf,"%s %s",str,strbuf);

    DisplayString(p->_x,p->_y,p->_z,sprintfBuf); 
}

//TITLE
void ShowSpeedTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', 30 - 1 );
    sprintfBuf[29] = '\0';
    char *str ;
    if(SystemConfig_Get_Display_Unit(User_Get_UserSlot())==STANDARD)
    {
        str = "MPH";
    }
    else
    {
        str = "KPH";
    }
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);    
}
void ShowInclineTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', 30 - 1 );
    sprintfBuf[29] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowDistanceTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', 30 - 1 );
    sprintfBuf[29] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowCaloriesTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', 30 - 1 );
    sprintfBuf[29] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowAFGLOGOTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', 30 - 1 );
    sprintfBuf[29] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowResistanceTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', 30 - 1 );
    sprintfBuf[29] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowMetsTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', 30 - 1 );
    sprintfBuf[29] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowRpmTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', 30 - 1 );
    sprintfBuf[29] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);   
}
void ShowHeartTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', 30 - 1 );
    sprintfBuf[29] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowWattsTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', 30 - 1 );
    sprintfBuf[29] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowPaceTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', 30 - 1 );
    sprintfBuf[29] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowMileTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', 30 - 1 );
    sprintfBuf[29] = '\0';
    char *str;
    if(SystemConfig_Get_Display_Unit(User_Get_UserSlot())==STANDARD)
    {
        str = String_Get_String(STR_MILE_TITLE_ID);
    }
    else
    {
        str = "KM";
    }
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}

void ShowCustomSegment(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    char strbuf[5];
    UINT8 l_Dat;
    
    memset( sprintfBuf , '\0', 30 );
    char *str = String_Get_String(p->_string_id);
        
    l_Dat = Program_Get_CustomSegment();
    memset( strbuf ,'\0', 5 );
    sprintf(strbuf,"%2d",l_Dat);
    
   	if(p->_blinkCount%2==1)
	{
        
        for(char i=0;strbuf[i] != '\0';i++)
            strbuf[i] = ' ';
	}
    sprintf(sprintfBuf,"%s %s",str,strbuf);
	DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
}

void ShowSetTime(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat1;
    
    if(Program_Get_ProgramID()==PS_CUSTOM_TIME || Program_Get_ProgramID()==PS_CUSTOM_HR)
    {
        float l_Dat2;
        l_Dat2 = (float)Program_Get_Time(1)/60;
        sprintf(sprintfBuf,"SET TIME%4.1f MIN",l_Dat2);
    }
    else
    {
        l_Dat1 = Program_Get_Time(1)/60;
        sprintf(sprintfBuf,"SET TIME %2u MIN",l_Dat1);
    }
    
   	if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"SET TIME    MIN");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}

void ShowSetTotalTime(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
    
    l_Dat = Program_Get_Time(1)/60;
    sprintf(sprintfBuf,"TOTALTIME %2u MIN",l_Dat);
    
    if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"TOTALTIME    MIN");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
//DATA
void ShowCountTimeData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
    l_Dat = Program_Get_Time(1);

    if(l_Dat>=6000)
    {
        l_Dat = (l_Dat/3600*100)+(l_Dat%3600/60);
    }
    else
    {
        l_Dat = (l_Dat/60*100)+(l_Dat%60);
    }
    sprintf(sprintfBuf,"%4.3u",l_Dat);
//    LED_SET_DOT(1);
   	if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
//void ShowElapsedTimeData(void *pWindow)
//{
//    Win_Items* p = (Win_Items*)pWindow;
//    UINT32 l_Dat;
//        
//    l_Dat = Program_Get_WorkoutTime_Elapsed();
//
//    if(l_Dat>=6000)
//    {
//        l_Dat = (l_Dat/3600*100)+(l_Dat%3600/60);
//    }
//    else
//    {
//        l_Dat = (l_Dat/60*100)+(l_Dat%60);
//    }
//    sprintf(sprintfBuf,"%4.4u",l_Dat);
////    LED_SET_DOT(1);
//   	if(p->_blinkCount%2==1)
//	{
//		DisplayString(p->_x,p->_y,0,"    ");
//	}
//	else
//	{
//		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
//	}
//}
//void ShowLcdElapsedTimeData(void *pWindow)
//{
//    Win_Items* p = (Win_Items*)pWindow;
//    UINT32 l_Dat;
//        
//    l_Dat = Program_Get_WorkoutTime_Elapsed();
//
//    if(l_Dat>=6000)
//    {
//        l_Dat = (l_Dat/3600*100)+(l_Dat%3600/60);
//    }
//    else
//    {
//        l_Dat = (l_Dat/60*100)+(l_Dat%60);
//    }
//    sprintf(sprintfBuf,"%4.4u",l_Dat);
////    LED_SET_DOT(1);
//   	if(p->_blinkCount%2==1)
//	{
//		DisplayString(p->_x,p->_y,0,"    ");
//	}
//	else
//	{
//		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
//	}
//}


void ShowTimeData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
//    if(Number_Get_Setting_Flag() && CurrentSettingPage==Setting_In_Set_Time)
//    {
//        l_Dat = Number_Get_Keypad()*60;  
//    }
//    else 
    {
        l_Dat = Program_Get_Time(1);
    }    

    if(l_Dat>=6000)
    {
        l_Dat = (l_Dat/3600*100)+(l_Dat%3600/60);
    }
    else
    {
        l_Dat = (l_Dat/60*100)+(l_Dat%60);
    }
//WWW    sprintf(sprintfBuf,"%4.3u",l_Dat);
         sprintf(sprintfBuf,"%4.4u",l_Dat);//WWW
   	if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
//	Display_Updata_Data(Display_Get_Demo());
	//		   Display_Updata_Data(false);
	//		   Display_Scan_Int();
}

void showWeekWorkout(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    MyFirst5kDat lg_Dat;
    
    memset( sprintfBuf , '\0', 30 );
    
    lg_Dat = Program_Get_MyFirst5k_WeekWorkout();
    
    if(p->_blinkCount%2==1)
        sprintf(sprintfBuf,"%s   %s   ","WEEK","WORKOUT");
    else
        sprintf(sprintfBuf,"%s %d %s %d","WEEK",lg_Dat.Week,"WORKOUT",lg_Dat.Workout);
    
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
}
void ShowSetCalories(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 w_Dat;
    char strbuf[5];
    
    memset( sprintfBuf , '\0', 30);
    char *str = String_Get_String(p->_string_id);
        
    w_Dat = Program_Get_Calories_Target();
    memset( strbuf ,'\0', 5 );
    sprintf(strbuf,"%4u",w_Dat);
    
   	if(p->_blinkCount%2==1)
	{
        
        for(char i=0;strbuf[i] != '\0';i++)
            strbuf[i] = ' ';
	}
    sprintf(sprintfBuf,"%s %s",str,strbuf);
	DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
}
void ShowSetSpeed(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    float l_Dat;
    char strbuf[4];
    
    memset( sprintfBuf , '\0', 30 - 1 );
    sprintfBuf[29] = '\0';
    char *str = String_Get_String(p->_string_id);
        
    l_Dat = Program_Get_Speed(DATA_AUTO,0);
    memset( strbuf ,'\0', 4 );
    sprintf(strbuf,"%4.1f",l_Dat/10);
    
   	if(p->_blinkCount%2==1)
	{
        memset( strbuf ,'\0', 4 );
        for(char i = 0;i < 3;i++)
            strbuf[i] = ' ';
	}
    sprintf(sprintfBuf,"%s %s",str,strbuf);
	DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
}
void ShowSetResistance(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT8 by_Dat;
    char strbuf[5];
    
    memset( sprintfBuf , '\0', 30 - 1 );
    sprintfBuf[29] = '\0';
    char *str = String_Get_String(p->_string_id);
 /*   
    by_Dat = Program_Get_Resistance(1);
    memset( strbuf ,'\0', 5 );
    sprintf(strbuf,"%2d",by_Dat);

   	if(p->_blinkCount%2==1)
	{
        
        for(char i=0;strbuf[i] != '\0';i++)
            strbuf[i] = ' ';
	}
              
    sprintf(sprintfBuf,"%s%s",str,strbuf);
	DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
              */
   UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
		
	if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	} 
}

//void ShowProgramBLData(void *pWindow)
//{
//    Win_Items* p = (Win_Items*)pWindow;
//    char by_Data;
//    memset( sprintfBuf , '\0', 30 );
//    
//    switch(Program_Get_ProgramID())
//    {
//		case PS_MANUAL:
//        by_Data = P_MANUAL;
//        p->_z = 1;
//        break;
//		case PS_DISTANCE_GOAL:
//        by_Data = P_DISTANCE;
//        p->_z = 1;
//        break;
//		case PS_FATBURN:
//        by_Data = P_FAT_BURN;
//        p->_z = 1;
//        break;
//		case PS_CALORIES_GOAL:
//        by_Data = P_CALORIES;
//        p->_z = 1;
//        break;
//		case PS_HILL_CLIMB:
//        by_Data = P_HILL_CLIMB;
//        p->_z = 1;
//        break;
//		case PS_THR_ZONE:
//        by_Data = P_TGART_HEART_RATE;
//        p->_z = 1;
//        break;
//		case PS_MYFIRST5K:
//        by_Data = P_MY_FIRST_5K;
//        p->_z = 1;
//        break;       
//        case PS_CUSTOM_TIME:
//        case PS_CUSTOM_DISTANCE:
//        by_Data = P_CUSTOM;
//        p->_z = 1;
//        break;
//		default:
//        p->_z = 0;
//        break;
//	}
//    sprintf(sprintfBuf,"%d",by_Data);
//    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
//}
void ShowLapsCircle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT8 by_Addr;
    UINT32 l_LapsCircle = 0;
    
    SetLapCircle(0);
    by_Addr = Program_Get_LapsCircle();
    if(by_Addr == 0xff)
    {
        l_LapsCircle = 0xffffffff;
    }
    else
    { 
        by_Addr++;
        for(char i=0;i < by_Addr;i++)
            l_LapsCircle |= (1<<i);
        
        if(p->_blinkCount%2==0)
            l_LapsCircle &= ~(1 << (by_Addr-1));
    }
    SetLapCircle(l_LapsCircle);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
}
void ShowSpeedData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
//    UINT8 Blinks;

    if(p->_isHidden==1 && p->_blinkCount==0)
    {
        l_Dat = Program_Get_Calories(1);
    }
    else
    {
        if(Program_Get_Status()==PS_IDLE)
        {
            l_Dat = 0;
        }
        else
        {
            Show_LEDLight(LED7_SPEED,LED_ON);
            Show_LEDLight(LED8_INCLINE,LED_OFF);
            l_Dat = Program_Get_Speed(DATA_AUTO,1);
        }
    }
    p->_z = 1;
    sprintf(sprintfBuf,"%3.2u ",l_Dat);
	
  	if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
//        LED_14_SEG_Dot(6,0);
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
//        LED_14_SEG_Dot(6,1);
	}
}
void ShowLcdSpeedData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;

    if(p->_isHidden==1 && p->_blinkCount==0)
    {
        l_Dat = Program_Get_Calories(1);
    }
    else
    {
        if(Program_Get_Status()<=PS_WAIT_3S)
        {
            l_Dat = 0;
        }
        else
        {
            l_Dat = Program_Get_Speed(DATA_AUTO,1);
        }
    	if(l_Dat>=200 )
		{
	        p->_z = 2;
			l_Dat = l_Dat/10 ;		
		}
		else
			p->_z = 1;
    }
		
    sprintf(sprintfBuf,"%3.2u",l_Dat);
	
  	if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
//        p->_z = 0;
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
//        p->_z = 1;
	}
}
void ShowSetIncline(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT8 l_Dat;
    char strbuf[10];
    
    memset( sprintfBuf , '\0', 30 - 1 );
    sprintfBuf[29] = '\0';
    char *str = String_Get_String(p->_string_id);
        
    l_Dat = Program_Get_Incline(1);
    memset( strbuf ,'\0', 9 );
    sprintf(strbuf,"%3u",l_Dat);
    
   	if(p->_blinkCount%2==1)
	{
        
        for(char i=0;strbuf[i] != '\0';i++)
            strbuf[i] = ' ';
        sprintf(sprintfBuf,"%s %s",str,strbuf);
		DisplayString(p->_x,p->_y,0,sprintfBuf);
	}
	else
	{
        sprintf(sprintfBuf,"%s %s",str,strbuf);
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void ShowInclineData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
//    UINT8 Blinks;

    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8) == TREADMILL)
        l_Dat = Program_Get_Incline(1);
    else
	   {
#ifdef AE
        l_Dat = Program_Get_Incline(1);
#else
		l_Dat = Program_Get_Incline(1)/5;
#endif
		}
    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8) == TREADMILL)
    {
        sprintf(sprintfBuf,"%3.2u",l_Dat);
        p->_z = 1;
    }
    else
    {
        sprintf(sprintfBuf,"%3u",l_Dat);
//        p->_z = 0;
    }
#ifdef AE
      Show_LEDLight(LED1_RPM_EP,LED_OFF);
      Show_LEDLight(LED8_RESISTANCE_EP,LED_OFF);
#else
      Show_LEDLight(LED7_SPEED,LED_OFF);
      Show_LEDLight(LED8_INCLINE,LED_ON);
#endif
	if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
//        LED_14_SEG_Dot(6,0);
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
//        LED_14_SEG_Dot(6,1);
	}
}
void ShowIncline_LCDData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
//    UINT8 Blinks;

    l_Dat = Program_Get_Incline(1);
    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8) == TREADMILL)
    {
        sprintf(sprintfBuf,"%3.2u",l_Dat);
        p->_z = 1;
    }
    else
    {
        sprintf(sprintfBuf,"%3u",l_Dat);
#ifdef AE
        p->_z = 0; // p->_z = 0;
#endif
    }

	if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}

void ShowSelectDistance(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    
    memset( sprintfBuf , '\0', 30);
    char *str = String_Get_String(p->_string_id);
    sprintf(sprintfBuf,"%s",str);
    DisplayString(p->_x,p->_y,0,sprintfBuf);
}


void ShowSetDistance(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    float l_Dat;
    char strbuf[5];
    
    memset( sprintfBuf , '\0', 30 - 1 );
    sprintfBuf[29] = '\0';
    char *str = String_Get_String(p->_string_id);
        
    l_Dat = (float)Program_Get_Distance(DATA_STANARD)/100;
    memset( strbuf ,'\0', 5 );
    sprintf(strbuf,"%3.1f",l_Dat);
    
   	if(p->_blinkCount%2==1)
	{
        for(char i=0;strbuf[i] != '\0';i++)
            strbuf[i] = ' ';
        sprintf(sprintfBuf,"%s %s",str,strbuf);
		DisplayString(p->_x,p->_y,0,sprintfBuf);
	}
	else
	{
        sprintf(sprintfBuf,"%s %s",str,strbuf);
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}

void ShowDistanceData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
    l_Dat = Program_Get_Distance(DATA_AUTO); 
//    if(l_Dat < 1000)
//    {
        sprintf(sprintfBuf,"%4.3u",l_Dat);
////        LED_SET_DOT(2);
//    }
//    else if(l_Dat < 10000)
//    {
//        sprintf(sprintfBuf,"%4.3u",l_Dat/10);
////        LED_SET_DOT(1);
//    }
//    else
//    {
//        sprintf(sprintfBuf,"%4u",l_Dat/100);
//        LED_SET_DOT(0);
//    }
    p->_z = 1;
    if(Program_Get_Status() > PS_SETUP && Program_Get_Status() < PS_DEMO)
    {
#ifdef TM
        Show_LEDLight(LED9_DISTANCE,LED_ON);
        Show_LEDLight(LED10_HRT,LED_OFF);
#else 
        Show_LEDLight(LED2_DISTANCE_EP,LED_ON);
        Show_LEDLight(LED3_HRT_EP,LED_OFF);
#endif
    }
    if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void ShowLcdDistanceData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
    l_Dat = Program_Get_Distance(DATA_AUTO); 
    if(l_Dat < 1000)
    {
        sprintf(sprintfBuf,"%4.3u",l_Dat);
        p->_z = 2;
    }
    else if(l_Dat < 10000)
    {
        sprintf(sprintfBuf,"%4.3u",l_Dat/10);
        p->_z = 1;
    }
    else
    {
        sprintf(sprintfBuf,"%4u",l_Dat/100);
        p->_z = 0;
    }
    
    if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void ShowCaloriesData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
    l_Dat = Program_Get_Calories(1)%1000;
        
    sprintf(sprintfBuf,"%4u",l_Dat);
    if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void ShowLcdCaloriesData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
    l_Dat = Program_Get_Calories(1)%1000;
        
    sprintf(sprintfBuf,"%03u",l_Dat);
    if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void Show_Laps(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT8 by_Dat;
    
    by_Dat = Program_Get_Laps();
    sprintf(sprintfBuf,"%3u",by_Dat);
    
    if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}

//void Show_LapsCircle(void *pWindow)
//{
//    Win_Items* p = (Win_Items*)pWindow;
//    UINT8 by_Adr;
//    UINT8 by_LapsCircle = 0;
//    
//    Display_Set_LapsCircle(0);
//    by_Adr = Program_Get_LapsCircle();
//    
//    if(by_Adr == 0xFF)
//    {
//        by_LapsCircle = 0xFF;
//    }
//    else
//    {
//        by_Adr++;
//        for(char i=0;i < by_Adr;i++)
//            by_LapsCircle |= (1<<i);
//        if(p->_blinkCount%2==1)
//        {
//            by_LapsCircle &= ~(1<<(by_Adr-1));
//        }
//    }
//    Display_Set_LapsCircle(by_LapsCircle);
//    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
//}


void Show_Calories_PerHour(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
    
    l_Dat = Program_Get_CaloriesHR();
    sprintf(sprintfBuf,"%4u",l_Dat);
    
    if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}

void ShowResistanceData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
//    UINT8 Blinks;

    l_Dat = Program_Get_Resistance(1);

    sprintf(sprintfBuf,"%3u",l_Dat);
    Show_LEDLight(LED1_RPM_EP,LED_OFF);
    if(Frame_Stage != FRAME_STAGE_ENG)
    {
    Show_LEDLight(LED8_RESISTANCE_EP,LED_ON);    
    }
    p->_z = 0;
	if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void ShowMetsData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
    l_Dat = Program_Get_METs();
    sprintf(sprintfBuf,"%3.2u ",l_Dat);
    if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void ShowWattsData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
//    UINT8 Blinks;
    l_Dat = Program_Get_Watts(AUTO_WATTS ,0);
    sprintf(sprintfBuf,"%3u",l_Dat);
    if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}

void ShowTargetWattsData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
//    UINT8 Blinks;
   
    l_Dat = Program_Get_Watts(TARGET_WATTS ,1);
    sprintf(sprintfBuf,"%4u",l_Dat);
    if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void ShowPaceData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
    l_Dat = Program_Get_Pace();
    l_Dat = (l_Dat/60*100)+(l_Dat%60);
//    if(l_Dat>=10000)
//    {
//        l_Dat=l_Dat/100;
//        p->_z = 0;    
//    }
//    else
//    {
//        p->_z = 2;  
//    }
    if(l_Dat < 1000)
    {
        sprintf(sprintfBuf,"%4.3u",l_Dat);
//        LED_SET_DOT(2);
    }
    else if(l_Dat < 10000)
    {
        sprintf(sprintfBuf,"%4.3u",l_Dat/10);
//        LED_SET_DOT(1);
    }
    else
    {
        sprintf(sprintfBuf,"%4.3u",l_Dat/100);
//        LED_SET_DOT(0);
    }
   if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void ShowPace_LcdData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
    l_Dat = Program_Get_Pace();
    l_Dat = (l_Dat/60*100)+(l_Dat%60);
    if(l_Dat == 0 || l_Dat>=10000)
    {
        l_Dat=l_Dat/100;
        p->_z = 0;
    }
    else
    {
        p->_z = 1;
    }
    
    sprintf(sprintfBuf,"%4d",l_Dat);
    if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void ShowTargetHeartData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    char l_Dat;
    char strbuf[5];
    
    memset( sprintfBuf , '\0', 30);
    char *str = String_Get_String(p->_string_id);
    
    l_Dat = Program_Get_HR(TARGET_HR);
    memset( strbuf ,'\0', 5 );
    sprintf(strbuf,"%d",l_Dat);
    
   	if(p->_blinkCount%2==1)
	{
        
        for(char i=0;strbuf[i] != '\0';i++)
            strbuf[i] = ' ';
        sprintf(sprintfBuf,"%s %s",str,strbuf);
		DisplayString(p->_x,p->_y,0,sprintfBuf);
	}
	else
	{
        sprintf(sprintfBuf,"%s %s",str,strbuf);
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void ShowHeartData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat = 0;
    if(Program_Get_Status() > PS_SETUP && Program_Get_Status() < PS_DEMO)
    { 
        l_Dat = Program_Get_HR(CURRENT_HR);
        if(g_wndLED3TextBox.pfShow == ShowHeartData)
        {
            p->_z = 0;
#ifdef TM
            Show_LEDLight(LED9_DISTANCE,LED_OFF);
            Show_LEDLight(LED10_HRT,LED_ON);
#else
            Show_LEDLight(LED2_DISTANCE_EP,LED_OFF);
            Show_LEDLight(LED3_HRT_EP,LED_ON);
#endif
        }
    }
    sprintf(sprintfBuf,"%4u",l_Dat);
    if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}

void ShowSetLevel(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    char l_Dat;
//    char strbuf[10];
    
    memset( sprintfBuf , '\0', 30 - 1 );
    sprintfBuf[29] = '\0';
//    char *str;
    l_Dat = Program_Get_WorkoutLevel(1);
    p->_z = 0;
   	if(p->_blinkCount%2==1)
	{
        sprintf(sprintfBuf,"L%u",l_Dat);
		DisplayString(p->_x,p->_y,0,sprintfBuf);
	}
	else
	{
        sprintf(sprintfBuf,"L%u",l_Dat);
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void ShowRpmData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
#ifdef TM
    if(Program_Get_Status()<=PS_WAIT_3S)
    { 
        l_Dat = 0;
    }
    else
    {
        l_Dat = Program_Get_RPM();
    }
#else
    l_Dat = Program_Get_RPM();
    if ( (Program_Get_Status()>PS_WAIT_3S)&&(Program_Get_Status()<PS_DEMO))
    {
        Show_LEDLight(LED8_RESISTANCE_EP,LED_OFF);
        Show_LEDLight(LED1_RPM_EP,LED_ON);
    }
#endif
#ifdef AE
      sprintf(sprintfBuf,"%3u ",l_Dat);
#else
      sprintf(sprintfBuf,"%3u",l_Dat);
#endif   
    p->_z = 0;
    if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
//matrix
void ShowMatrixData(void *pWindow)
{
  Win_Items* p = (Win_Items*)pWindow;
  PROFILE  *profile;
  UINT8 Profile[16];
  
  unsigned int i = 0 ;
  memset( sprintfBuf , '\0', 30 );
  if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8) == TREADMILL)
  {        
    if(martix_type == Show_Speed)
    {//speed
      profile =   Program_Get_Profile(SPEED_PROFILE);
      if(SystemConfig_Get_Display_Unit(User_Get_UserSlot()) == STANDARD)
      {
        for(i = 0;i <16;i++)
        {
          Profile[i] = profile->ProfileData[i];
          if(Profile[i]==0xff)
          {
            Profile[i] = 0;
          }
          else if(Profile[i] > 0 && Profile[i] <= 12)
          {
            Profile[i] = 1;
          }
          else if(Profile[i] <= 120)
          {
            Profile[i] = Profile[i]*10/120;
          }
        }
      }
      else
      {
        for(i = 0;i <16;i++)
        {
          Profile[i] = profile->ProfileData[i];
          if(Profile[i]==0xff)
          {
            Profile[i] = 0;
          }
          else if(Profile[i] < 30)
          {
            Profile[i] = 1;
          }
          else if(Profile[i] < 50)
          {
            Profile[i] = 2;
          }
          else if(Profile[i] < 80)
          {
            Profile[i] = 3;
          }
          else
          {
            Profile[i] = (Profile[i]*10/200);
          }
        }
      }
    }
    else if(martix_type == Show_Incline)
    {//---incline---
      profile =   Program_Get_Profile(INCLINE_PROFILE);
      for( i = 0;i <16;i++)
      {
        Profile[i] = profile->ProfileData[i];
        
        if(Profile[i]==0xff)
        {
          Profile[i] = 0;
        }
        else if(Profile[i] < 20 )
        {
          Profile[i] = 15;
        }
        else if(Profile[i] < 25)
        {
          Profile[i] = 25;
        }
        Profile[i] = (Profile[i]*10)/120;
      }
      
    }
  }
  else//---BK---
  {
    if(martix_type == Show_Incline)
    {//---incline---
      profile =   Program_Get_Profile(INCLINE_PROFILE);
      for( i = 0;i <16;i++)
      {
        Profile[i] = profile->ProfileData[i];
        
        if(Profile[i]<10 )
          Profile[i]=10;
        if(Profile[i]==0xff)
        {
          Profile[i] = 0;
        }
        else if(Profile[i]==145)
        {
          Profile[i]=15;
        }
        else if(Profile[i]==150)
        {
          Profile[i]=16;
        }
        else
        {
          Profile[i] = (Profile[i]*15)/150;
        }
      }
    }
    else if(martix_type == Show_Resistance)
    {//resistance
      profile =   Program_Get_Profile(RESISTANCE_PROFILE);
      
      if(SystemConfig_Get_Char(MAX_RESISTANCE_UINT8) == 30)
      {
        for( unsigned int i = 0;i <16;i++)
        {
          Profile[i] = profile->ProfileData[i];
          Profile[i] = (Profile[i])/2+1;
        }
      }
      else if(SystemConfig_Get_Char(MAX_RESISTANCE_UINT8) == 20)
      {
        for( unsigned int i = 0;i <16;i++)
        {
          Profile[i] = profile->ProfileData[i];
          if(Profile[i]==0xff)
          {
            Profile[i] = 0;
          }
          else
            Profile[i] = (Profile[i]+1)/2;
          
          //					if(Profile[i] <= 3)
          //						Profile[i]=1;
          //					else if(Profile[i] == 4 || Profile[i] == 5)
          //					{
          //						Profile[i] = 2;
          //					}
          //					else if(Profile[i] == 6 || Profile[i] == 7)
          //					{
          //						Profile[i] = 3;
          //					}	
          //					else 
          //					{
          //						Profile[i] = Profile[i]-4;
          //					}
        }
      }
      else if(SystemConfig_Get_Char(MAX_RESISTANCE_UINT8) == 10)
      {
        for( unsigned int i = 0;i <16;i++)
        {
          Profile[i] = profile->ProfileData[i];
          if(Profile[i]==0xff)
          {
            Profile[i] = 0;
          }
          else
            Profile[i] = Profile[i];
        }
      }
    }
  }
  if((nCurrentPage>=PAGE_WORK_TM1)&&(nCurrentPage<=PAGE_WORK_BK2)&&(p->_blinkCount%2!=0))
  {Profile[Program_Get_DotmatixPos()] = 0;}
  
  for(UCHAR i = 0;i < 16;i++)
  {
    if(Profile[i] <= 9)
    {
      Profile[i] = Profile[i] + '0';
    }
    else
    {
      Profile[i] = Profile[i] + 7 + '0';
    }
  }
  DisplayString(p->_x,p->_y,p->_z,(const char*)Profile);
}

//RF_ICON
void ShowRFICON(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UCHAR RF_Status = Event_Get_RF_Connected()||Event_Get_RF_Syncing();
 
    if(p->_blinkCount==0)
        DisplayString(p->_x,p->_y,RF_Status,sprintfBuf);  
    else
    {
        if(p->_blinkCount%2==0)
          DisplayString(p->_x,p->_y,RF_Status,sprintfBuf);  
        else
          DisplayString(p->_x,p->_y,0,"    "); 
    }
}

void ShowRTCYear(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
	DateTime_T*  time =RTCGetSettedTimeData() ;
	if(p->_blinkCount%2==1)
	{
            DisplayString(p->_x,p->_y,0,"SET YEAR      ");
	}
	else
	{
        sprintf(sprintfBuf,"SET YEAR 20%2d",time->year);
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}

void ShowRTCMonth(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
	DateTime_T*  time =RTCGetSettedTimeData() ;
	if(p->_blinkCount%2==1)
	{
            DisplayString(p->_x,p->_y,0,"SET MONTH    ");
	}
	else
	{
         sprintf(sprintfBuf,"SET MONTH %3d",time->month);
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}

void ShowRTCDate(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
	DateTime_T*  time =RTCGetSettedTimeData() ;
	if(p->_blinkCount%2==1)
	{
            DisplayString(p->_x,p->_y,0,"SET DATE    ");
	}
	else
	{
         sprintf(sprintfBuf,"SET DATE %3d",time->date);
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}

void ShowRTCHour(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
	DateTime_T*  time =RTCGetSettedTimeData() ;
	if(p->_blinkCount%2==1)
	{
            DisplayString(p->_x,p->_y,0,"SET HOUR    ");
	}
	else
	{
         sprintf(sprintfBuf,"SET HOUR %3d",time->hours);
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void ShowRTCMinute(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
	DateTime_T*  time =RTCGetSettedTimeData() ;
	if(p->_blinkCount%2==1)
	{
            DisplayString(p->_x,p->_y,0,"SET MINUTE    ");
	}
	else
	{
         sprintf(sprintfBuf,"SET MINUTE %3d",time->minutes);
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void ShowRTCAMPM(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
	memset( sprintfBuf , '\0', 30 - 1 );
    sprintfBuf[29] = '\0';
	char *str ;
	if(p->_blinkCount%2==1)
	{
		str = "SET AM OR PM   ";
	}
	else
	{
		if(RTCGetPMAMInfo() )
		{
			str = "SET AM OR PM PM";
		}
		else
		{
			str = "SET AM OR PM AM";
		}
	}
    for( int i = 0; i < strlen( str ); i++ ){ sprintfBuf[i] = str[i]; }
		
	DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
}


void Show_LEDLight(LedType Index,LED_STATUS ON_OFF)
{
    LED_Light(Index, ON_OFF);
}
void Show_ProgramLED(PROGRAM_ID ProgramId)
{
    LedType l_LED_Program;
#ifdef TM
    //Clear
    LED_Light(LED1_PRO_MANUAL, LED_OFF);
    LED_Light(LED2_PRO_WEIGHT_LOSS, LED_OFF);
    LED_Light(LED3_PRO_INTERVALS, LED_OFF);
    LED_Light(LED13_PRO_DISTANCE, LED_OFF);
    LED_Light(LED14_PRO_CALORIES, LED_OFF);
    //ON
    if(ProgramId == PS_MANUAL)
        l_LED_Program = LED1_PRO_MANUAL;
    else if(ProgramId == PS_WEIGHT_LOSS)
        l_LED_Program = LED2_PRO_WEIGHT_LOSS;
    else if(ProgramId == PS_INTERVALS)
        l_LED_Program = LED3_PRO_INTERVALS;
    else if(ProgramId == PS_DISTANCE_GOAL)
        l_LED_Program = LED13_PRO_DISTANCE;
    else if(ProgramId == PS_CALORIES_GOAL)
        l_LED_Program = LED14_PRO_CALORIES;
    LED_Light(l_LED_Program, LED_ON);
#else
    //Clear
    LED_Light(LED11_PRO_MANUAL_EP, LED_OFF);
    LED_Light(LED9_PRO_WEIGHT_LOSS_EP, LED_OFF);
    LED_Light(LED14_PRO_INTERVALS_EP, LED_OFF);
    LED_Light(LED12_PRO_DISTANCE_EP, LED_OFF);
    LED_Light(LED7_PRO_CALORIES_EP, LED_OFF);
    //ON
    if(ProgramId == PS_MANUAL)
        l_LED_Program = LED11_PRO_MANUAL_EP;
    else if(ProgramId == PS_WEIGHT_LOSS)
        l_LED_Program = LED9_PRO_WEIGHT_LOSS_EP;
    else if(ProgramId == PS_INTERVALS)
        l_LED_Program = LED14_PRO_INTERVALS_EP;
    else if(ProgramId == PS_DISTANCE_GOAL)
        l_LED_Program = LED12_PRO_DISTANCE_EP;
    else if(ProgramId == PS_CALORIES_GOAL)
        l_LED_Program = LED7_PRO_CALORIES_EP;
    LED_Light(l_LED_Program, LED_ON);
#endif 
}
void Show_ClearLedLight(void)
{
    LED_Light(LED1_PRO_MANUAL, LED_OFF);
    LED_Light(LED2_PRO_WEIGHT_LOSS, LED_OFF);
    LED_Light(LED3_PRO_INTERVALS, LED_OFF);
    LED_Light(LED5_TIME, LED_OFF);
    LED_Light(LED6_CALORIES, LED_OFF);
    LED_Light(LED7_SPEED, LED_OFF);
    LED_Light(LED8_INCLINE, LED_OFF);
    LED_Light(LED9_DISTANCE, LED_OFF);
    LED_Light(LED10_HRT, LED_OFF);
//    LED_Light(LED11_FAN, LED_OFF);
//    LED_Light(LED12_BT, LED_OFF);
    LED_Light(LED12_PRO_DISTANCE_EP, LED_OFF);
    LED_Light(LED11_PRO_MANUAL_EP, LED_OFF);
    LED_Light(LED13_PRO_DISTANCE, LED_OFF);
    LED_Light(LED14_PRO_CALORIES, LED_OFF);
    LED_Light(LED15_CHANGE_DISPLAY, LED_OFF);
}