#include "Show.h"
#include "Events_Handler.h"
#include "Programs.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include "page.h"

#define MAX_SUPPORTED_LENGTH    128

extern const char* rscu_version[3];
extern PAGES const pages[PAGE_TOTAL];
extern  unsigned char NameSprintBuffer[65];

//extern QUICK_KEY_FUNCTION CurrentSettingPage;
static  char sprintfBuf[MAX_SUPPORTED_LENGTH];
//Window_Items

void ShowUsrName(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( NameSprintBuffer , '\0', 64 );
    sprintfBuf[64] = '\0';
  	memcpy(sprintfBuf, User_Get_Name(), 65);
    
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);   
}


void ShowWeightData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 by_Dat = 0 ;
    by_Dat = User_Get_Weight();
    sprintf(sprintfBuf,"%4u",by_Dat);
	p->_z = 0 ;
	if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void ShowWeightUnit(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1);
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
	p->_z = 0 ;
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
    UINT16 by_Dat=wifi_Get_Timezone();
    sprintf(sprintfBuf,"%4u",by_Dat);
	p->_z = 0 ;
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
}

void ShowSetGenderData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    sprintfBuf[MAX_SUPPORTED_LENGTH-1] = '\0';
    sprintf(sprintfBuf,"%4u",User_Get_Gender());
	p->_z = 0 ;
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
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    sprintfBuf[MAX_SUPPORTED_LENGTH-1] = '\0';
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


void ShowCommonTextBox(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    sprintfBuf[MAX_SUPPORTED_LENGTH-1] = '\0';
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
void ShowPXTextBox(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    sprintfBuf[MAX_SUPPORTED_LENGTH-1] = '\0';
    char *str = PxString_Get_PxString(p->_string_id);
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
void ShowBottomTextBox(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    sprintfBuf[MAX_SUPPORTED_LENGTH-1] = '\0';    
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  

}

void ShowXID(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    memcpy(sprintfBuf, User_Get_Xid(),14);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}

void ShowPasscode(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    memcpy(sprintfBuf, User_Get_Passcode(),4);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);   
}

void ShowQuickMul(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat=Engmode_Get_PROG_Mult_Times();

    sprintf(sprintfBuf,"%4u ",l_Dat);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);   
}

void ShowERPWaitTime(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat=(SystemConfig_Get_Int(ERP_WAIT_MIN_UINT16))/60;

    sprintf(sprintfBuf,"%4u ",l_Dat);
	p->_z = 0 ;
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);   
}

void ShowOverSpeedValue(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT8 l_Dat=(SystemConfig_Get_Char(OVERSPEED_VALUE));

    sprintf(sprintfBuf,"%3.2u ",l_Dat);
	p->_z = 1 ;
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);   
}

void ShowCommonData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat=p->_string_id;
    sprintf(sprintfBuf,"%4u ",l_Dat);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}

void ShowENGKeyValue(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat=Engmode_Get_KeyValue();
    sprintf(sprintfBuf,"%4u ",l_Dat);
	p->_z = 0 ;
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}

void ShowENG0Number(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat=Engmode_Get_Number()*1111;
    sprintf(sprintfBuf,"%4u ",l_Dat);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}

void ShowENGSetRPMData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat=Engmode_Get_SetSpeedRPM();
    sprintf(sprintfBuf,"%4u ",l_Dat);
	p->_z = 0 ;
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}

void ShowENGLCBRPMData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat=LCB_Get_Data(G_MOTOR_RPM);
    sprintf(sprintfBuf,"%4u ",l_Dat);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowENGInclineADCData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat=Engmode_Get_SetInclineADC();
    sprintf(sprintfBuf,"%4u ",l_Dat);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowCurrentInclineADC(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat=Program_Get_Current_Incline_ADC()/128;
    sprintf(sprintfBuf,"%4u ",l_Dat);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowENGLCBECBCount(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat=LCB_Get_Data(G_ECB_COUNT);
    sprintf(sprintfBuf,"%4u ",l_Dat);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowENGSettedECBCount(void *pWindow)
{
	Win_Items* p = (Win_Items*)pWindow;
	UINT32 l_Dat;
	l_Dat =Engmode_Get_SetECBCount();
	sprintf(sprintfBuf,"%4u ",l_Dat);
	DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowACCDis(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    FLOAT l_Dat;
    l_Dat = (SystemConfig_Get_Long(ACCUMULATED_MILESX100_LONG))/100;
    if(Get_User_Unit())
    {
        l_Dat = l_Dat*1.6;
    }
    sprintf(sprintfBuf,"%3.2u ",(UINT32)(l_Dat));
	p->_z = 0 ;
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf); 
}
void ShowACCStairs(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    FLOAT l_Dat;
    l_Dat = (SystemConfig_Get_Long(ACCUMULATED_MILESX100_LONG));
    if(Get_User_Unit())
    {
        l_Dat = l_Dat*1.6;
    }
    sprintf(sprintfBuf,"%3.2u ",(UINT32)(l_Dat));
	p->_z = 0 ;
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf); 
}
void ShowACCTime(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat = (SystemConfig_Get_Long(ACCUMULATED_SECONDS_LONG))/3600;

    sprintf(sprintfBuf,"%3.2u ",l_Dat);
	p->_z = 0 ;
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf); 
}

void ShowUCBVision(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    sprintf(sprintfBuf, "%s.%s",rscu_version[0],rscu_version[1]);    
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf); 
}

void ShowMCBVision(void *pWindow)
{
	Win_Items* p = (Win_Items*)pWindow;

	UINT16 l_Dat= LCB_Get_Version();
	UINT8 _major = (UINT8)((l_Dat&0xFF00)>>8);
	UINT8 _minor = (UINT8)(l_Dat&0x00FF);

	if(_minor<10)
	{
		sprintf(sprintfBuf,"%d.00%d",_major,_minor);
	}
	else if(_minor<100)
	{
		sprintf(sprintfBuf,"%d.0%d",_major,_minor);
	}
	else
	{
		sprintf(sprintfBuf,"%d.%d",_major,_minor);
	}
	
	DisplayString(p->_x,p->_y,p->_z,sprintfBuf); 
}
void ShowMCBType(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    MCB_TYPE _type = (MCB_TYPE)LCB_Get_MCBType();
    if(_type&0xF0)
    {
        sprintf(sprintfBuf,"0x%x",_type);
    }
    else
    {
        sprintf(sprintfBuf,"0x0%x",_type);
    }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf); 
}

void ShowACCAgingHours(void *pWindow)
{
	Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat = (SystemConfig_Get_Int(ACCUMULATED_AGING_HOURS_UINT16));
    p->_z = 0 ;
    sprintf(sprintfBuf,"%3.2u ",l_Dat);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf); 
}
//TITLE
void ShowSpeedTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    sprintfBuf[MAX_SUPPORTED_LENGTH-1] = '\0';
    char *str ;
    if(Get_User_Unit()==STANDARD)
    {
        str = String_Get_String(STR_MPH_ID);//"MPH";
    }
    else
    {
        str = String_Get_String(STR_KPH_ID);//"KPH";
    }
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);    
}
void ShowInclineTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    sprintfBuf[MAX_SUPPORTED_LENGTH-1] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowDistanceTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    sprintfBuf[MAX_SUPPORTED_LENGTH-1] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowCaloriesTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    sprintfBuf[MAX_SUPPORTED_LENGTH-1] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowResistanceTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    sprintfBuf[MAX_SUPPORTED_LENGTH-1] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowMetsTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    sprintfBuf[MAX_SUPPORTED_LENGTH-1] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowSPMTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    sprintfBuf[MAX_SUPPORTED_LENGTH-1] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowHeightTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    sprintfBuf[MAX_SUPPORTED_LENGTH-1] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowRpmTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    sprintfBuf[MAX_SUPPORTED_LENGTH-1] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);   
}
void ShowLevelTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    sprintfBuf[MAX_SUPPORTED_LENGTH-1] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);   
}
void ShowStairsTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    sprintfBuf[MAX_SUPPORTED_LENGTH-1] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);   
}

void ShowHeartTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    sprintfBuf[MAX_SUPPORTED_LENGTH-1] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowWattsTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    sprintfBuf[MAX_SUPPORTED_LENGTH-1] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowPaceTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    sprintfBuf[MAX_SUPPORTED_LENGTH-1] = '\0';
    char *str = String_Get_String(p->_string_id);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}
void ShowMileTitle(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    sprintfBuf[MAX_SUPPORTED_LENGTH-1] = '\0';
    char *str;
    if(Get_User_Unit()==STANDARD)
    {
        str = String_Get_String(STR_MILE_ID);
    }
    else
    {
        //str = "KM";
        str = String_Get_String(STR_KM_ID);
    }
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);  
}

//DATA
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
    sprintf(sprintfBuf,"%4.3u",l_Dat);
   	if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void ShowSpeedData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
    if(p->_isHidden==1 && p->_blinkCount==0)
    {
        l_Dat = Program_Get_Calories(1);
    }
    else
    {
        l_Dat = Program_Get_Speed(DATA_AUTO,1);
    }
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
void ShowErrorCode(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat=UTL_Get_ErrorCode();
    sprintf(sprintfBuf,"ErrorCode:%04X ",l_Dat);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);   
}
void ShowInclineData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
    l_Dat = Program_Get_Incline(1);
    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8) == TREADMILL)
    {
        sprintf(sprintfBuf,"%3.2u ",l_Dat);
        p->_z = 1;
    }
    else
    {
        sprintf(sprintfBuf,"%4u",l_Dat);
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
void ShowDistanceData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
    l_Dat = Program_Get_Distance(DATA_AUTO); 
    sprintf(sprintfBuf,"%4.3u",l_Dat);
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
    l_Dat = Program_Get_Calories(1);
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
    l_Dat = Program_Get_Resistance(1);

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
void ShowMetsData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
    p->_z = 1 ;
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
void ShowSPMData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
    p->_z = 0 ;
    l_Dat = Program_Get_RPM();
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

void ShowHeightData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
    p->_z = 0 ;
    l_Dat = Program_Get_Height();
    sprintf(sprintfBuf,"%4u ",l_Dat);
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
    l_Dat = Program_Get_Watts(AUTO_WATTS ,1);
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

void ShowTargetWattsData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;   
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
    if(l_Dat>=10000)
    {
        l_Dat=l_Dat/100;
        p->_z = 0;    
    }
    else
    {
        p->_z = 2;  
    }
    sprintf(sprintfBuf,"%4.3u",l_Dat);
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
    UINT32 l_Dat;

    l_Dat = Program_Get_HR(TARGET_HR);

    sprintf(sprintfBuf,"%4u ",l_Dat);
  if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void ShowHeartData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
    l_Dat = Program_Get_HR(CURRENT_HR);
    sprintf(sprintfBuf,"%4u ",l_Dat);
  if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void ShowLevelData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;

    l_Dat = Program_Get_WorkoutLevel(1);
    sprintf(sprintfBuf,"%4u ",l_Dat);
   if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}
void ShowEngMaxLevelData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;

    l_Dat = Engmode_Get_MaxLevel();
    sprintf(sprintfBuf,"%4u ",l_Dat);
    if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}

void ShowEngSetControlZoneData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT8 l_Dat;
    l_Dat = Engmode_Get_ControlZoneData();
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
void ShowEng1StControlZoneData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat;

    l_Dat = Engmode_Get_1StControlZoneData();
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
void ShowEng2NdControlZoneData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat;

    l_Dat = Engmode_Get_2NdControlZoneData();
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
void ShowEngRealControlZoneData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT16 l_Dat;

    l_Dat = Engmode_Get_RealControlZoneData();
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
void ShowEngIrSensorData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;

    l_Dat = LCB_Get_Data(G_GET_LCB_DEVICE_DATA);
    l_Dat = (l_Dat&0x00ff);
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
void ShowClimbMillLevelData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;

    l_Dat = Program_Get_Resistance(1);
    sprintf(sprintfBuf,"%4u ",l_Dat);
   if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}



void ShowStairsData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;

    l_Dat = Program_Get_Stairs();
    sprintf(sprintfBuf,"%4u ",l_Dat);
   if(p->_blinkCount%2==1)
	{
		DisplayString(p->_x,p->_y,0,"    ");
	}
	else
	{
		DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
	}
}

void ShowRpmData(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT32 l_Dat;
    l_Dat = Program_Get_RPM();
    sprintf(sprintfBuf,"%4u ",l_Dat);
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
    UINT8 Profile[12];
    UINT8 HightProfile[35];
    UINT8 count=0;

    unsigned int i = 0 ;
    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    if(SystemConfig_Get_Char(MACHINE_TYPE_UINT8) == TREADMILL)
    {        
		if(martix_type == Show_Speed)
        {//speed
            profile =   Program_Get_Profile(SPEED_PROFILE);
			if(Get_User_Unit() == STANDARD)
            {
                for(i = 0;i <12;i++)
                {
                    Profile[i] = profile->ProfileData[i];
                    
                    if(Profile[i] >= 0 && Profile[i] < 100)
                    {
                        Profile[i] = Profile[i]/10+1;
                    }
                    else if(Profile[i] == 125)
                    {
                        Profile[i] = 16;
                    }
                    else
                    {
                        Profile[i] = ((Profile[i]-100)/5 + 11);
                    }
                }
            }
            else
            {
                for(i = 0;i <12;i++)
                {
                    Profile[i] = profile->ProfileData[i];
                    
                    if(Profile[i] >= 0 && Profile[i] < 30)
                    {
                        Profile[i] = 1;
                    }
                    else if(Profile[i] >= 30 && Profile[i] < 50)
                    {
                        Profile[i] = 2;
                    }
                    else if(Profile[i] >= 50 && Profile[i] < 80)
                    {
                        Profile[i] = 3;
                    }
                    else
                    {
                        Profile[i] = (Profile[i]/10-4);
                    }
                }
            }
        }
        else if(martix_type == Show_Incline)
        {//---incline---
            profile =   Program_Get_Profile(INCLINE_PROFILE);
            for( i = 0;i <12;i++)
            {
                Profile[i] = profile->ProfileData[i];
                
                if(Profile[i]<10 )
                    Profile[i]=10;
                if(Profile[i]==145)
				{
					Profile[i]=15;
				}
				else if(Profile[i]==SystemConfig_Get_Int(MAX_INCLINEX10_UINT16))
				{
					Profile[i]=16;
				}
				else
				{
					Profile[i] = (Profile[i]*15)/SystemConfig_Get_Int(MAX_INCLINEX10_UINT16);
				}
            }
			
        }
    }
    else//---BK---
    {
        if(martix_type == Show_Incline)
        {//---incline---
            profile =   Program_Get_Profile(INCLINE_PROFILE);
			for( i = 0;i <12;i++)
            {
                Profile[i] = profile->ProfileData[i];
//                
//                if(Profile[i]<10 )
//                    Profile[i]=10;
//                if(Profile[i] == 95)
//				{
//					Profile[i]=15;
//				}
//				else if(Profile[i] == 100)
//				{
//					Profile[i] = 16;
//				}
//				else
//				{
//					Profile[i] = (Profile[i]*15)/150;
//				}
                if(Profile[i] < 50)
                {
                    Profile[i] = (Profile[i]*15)/150 + 1;
                }
                else if(Profile[i] >= 50 && Profile[i] < 100)
                {
                    Profile[i] = (Profile[i])/5 - 4;
                }
                else if(Profile[i] == 100)
                {
                    Profile[i] = 16;
                }
            }
        }
        else if(martix_type == Show_Resistance)
        {//resistance
            profile =   Program_Get_Profile(RESISTANCE_PROFILE);
			UINT8 profileOffsetX10 = 1600/SystemConfig_Get_Char(MAX_RESISTANCE_UINT8);
            UINT16 Profile_buff[12];
            for(unsigned int i = 0;i <12;i++)
            {
                Profile[i] = profile->ProfileData[i];
//                Profile[i] *= profileOffsetX10;
//                if(Profile[i]%10>=5)
//                {
//                    Profile[i] = Profile[i]/10+1;
//                }
//                else
//                {
//                    Profile[i] = Profile[i]/10;
//                }
                Profile_buff[i] = Profile[i]*profileOffsetX10;
                if(Profile_buff[i]%100>=50)
                {
                    Profile[i] = Profile_buff[i]/100+1;
                }
                else
                {
                    Profile[i] = Profile_buff[i]/100;
                }
            }
//			if(SystemConfig_Get_Char(MAX_RESISTANCE_UINT8) == 30)
//			{
//				for( unsigned int i = 0;i <12;i++)
//				{
//					Profile[i] = profile->ProfileData[i];
//					Profile[i] = (Profile[i])/2+1;
//				}
//			}
//			else if(SystemConfig_Get_Char(MAX_RESISTANCE_UINT8) == 20)
//			{
//				for( unsigned int i = 0;i <12;i++)
//				{
//					Profile[i] = profile->ProfileData[i];
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
//				}
//			}
//            else if(SystemConfig_Get_Char(MAX_RESISTANCE_UINT8) == 25)
//			{
//				for( unsigned int i = 0;i <12;i++)
//				{
//					Profile[i] = profile->ProfileData[i];
//					if(Profile[i] <= 18)
//					{
//						Profile[i]=((Profile[i]-1)/2)+1;
//					}
//					else
//					{
//						Profile[i] = Profile[i]-9;
//					}
//				}
//			}
        }
        else if((martix_type == Show_Hight)||(martix_type == Show_Run_Hight))
        {
            profile =   Program_Get_Profile(HIGHT_PROFILE);
            for( unsigned int i = 0;i <35;i++)
            {
                HightProfile[i] = profile->ProfileData[i];
            }
        }
//        else if((martix_type == Show_Run_Hight))
//        {
//            profile =   Program_Get_Profile(HIGHT_PROFILE);
//            for( unsigned int i = 0;i <35;i++)
//            {
//                HightProfile[i] = profile->ProfileData[i];
//            }
//        }
    }
		
    if((nCurrentPage>=PAGE_WORK_TM1)&&(nCurrentPage<=PAGE_WORK_CLIMBMILL2)&&(p->_blinkCount%2!=0))
    {
        Profile[Program_Get_DotmatixPos()] = 0;
	}
    
//	for(UCHAR i = 0;i < 12;i++)
//	{
//		if(Profile[i] <= 9)
//		{
//			Profile[i] = Profile[i] + '0';
//		}
//		else
//		{
//			Profile[i] = Profile[i] + 7 + '0';
//		}
//	}
    if(martix_type == Show_Hight)
    {
        for(UINT i = 0;i < 35;i++)
        {
            if(HightProfile[i] <= 9)
            {
                HightProfile[i] = HightProfile[i] + '0';
            }
            else
            {
                HightProfile[i] = HightProfile[i] + 7 + '0';
            }
            if(HightProfile[i]>'0')
            {
                count++;   
            }
        }
        
//        if(count>30)
//            DisplayString(p->_x,p->_y,3,HightProfile);
//        else
            DisplayString(p->_x,p->_y,2,HightProfile);
    }
    else if(martix_type == Show_Run_Hight)
    {
        for(UINT i = 0;i < 35;i++)
        {
            if(HightProfile[i] <= 10)
            {
                HightProfile[i] = HightProfile[i] + '0';
            }
//            else
//            {
//                HightProfile[i] = HightProfile[i] + 7 + '0';
//            }
        }
        if((HightProfile[10]=='0')&&(HightProfile[17]=='0'))
            DisplayString(p->_x,p->_y,3,HightProfile);
        else
            DisplayString(p->_x,p->_y,4,HightProfile);
    }
    else
    {
        for(UCHAR i = 0;i < 12;i++)
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
        DisplayString(p->_x,p->_y,p->_z,Profile);
    }
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
void ShowWifiIcon(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT8 TempId = p->_string_id;
    UINT8 BlinkTime = p->_blinkCount;
    if(WIFIIsAPMode())    
    {
        TempId = 4;
    }
    else
    {
        BlinkTime = 0 ;
        TempId = WIFISignalStrength();
    }
    
    if(Event_Get_DisFlag_AllOn())
    {
        TempId = 4;
        BlinkTime = 2;
    }
    else if(Event_Get_DisFlag_AllOff())
    {
        BlinkTime = 1;
    }
    
    if(BlinkTime%2==1)
       DisplayString(p->_x,p->_y,0,NULL);   
    else
       DisplayString(p->_x,p->_y,TempId,NULL);
}
//RF_ICON
void ShowRFICON(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    UINT8 BlinkTime = p->_blinkCount;
    UCHAR RF_Status = RF_Get_Connected()||Event_Get_RF_Syncing();
 
    if(Event_Get_DisFlag_AllOn())
    {
        RF_Status = 1;
        BlinkTime = 2;
    }
    else if(Event_Get_DisFlag_AllOff())
    {
        BlinkTime = 1;
    }
    
    if(BlinkTime%2==0)
      DisplayString(p->_x,p->_y,RF_Status,sprintfBuf);  
    else
      DisplayString(p->_x,p->_y,0,"    ");
}

void ShowWiFiUpdateProcess(void *pWindow)
{
	Win_Items* p = (Win_Items*)pWindow;
	UINT32 l_Dat = 50;
	UINT8 Percent_Buffer[5];
    memset( Percent_Buffer , '\0', sizeof(Percent_Buffer));
	l_Dat = WiFi_Get_Update_Percent();
	sprintf(Percent_Buffer,"%4u%",l_Dat);

    memset( sprintfBuf , '\0', MAX_SUPPORTED_LENGTH-1  );
    sprintfBuf[MAX_SUPPORTED_LENGTH-1] = '\0';
    char *str = String_Get_String(STR_DOWNLOADING_ID);
    UCHAR stlen = strlen( str );
    for( int i = 0; i < stlen; i++ ){ sprintfBuf[i] = str[i]; }
		
	strncpy(sprintfBuf+strlen(sprintfBuf),Percent_Buffer,MAX_SUPPORTED_LENGTH-1)	;

	DisplayString(p->_x,p->_y,p->_z,sprintfBuf);
}
void ShowUpGradeLanguagePackage(void *pWindow)
{
    Win_Items* p = (Win_Items*)pWindow;
    char l_buff[]= "please upgrade language package";
  	memcpy(sprintfBuf, l_buff, 32);
    DisplayString(p->_x,p->_y,p->_z,sprintfBuf);   
}