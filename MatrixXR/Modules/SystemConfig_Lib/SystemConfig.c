#include "SystemConfig.h"
#include "HAL_Lib.h"



extern const UINT16 Factory_Default[];
//extern bool isFirstBoot;
const char * timezone_string[] = {
"Asia/Kabul",//1
"America/Anchorage",//2
"Asia/Kuwait",//3
"Asia/Muscat",//4
"Asia/Baghdad",//5
"America/Halifax",//6
"Australia/Darwin",//7
"Australia/Sydney",//8
"Asia/Baku",//9
"Atlantic/Azores",//10
"America/Edmonton",//11
"Atlantic/Cape_Verde",//12
"Asia/Yerevan",//13
"Australia/Adelaide",//14
"America/Costa_Rica",//15
"Asia/Almaty",//16
"America/Cuiaba",//17
"Europe/Belgrade",//18
"Europe/Belgrade",//19
"Pacific/Guadalcanal",//20
"America/Chicago",//21
"America/Monterrey",//22
"Asia/Shanghai",//23
"Africa/Nairobi",//24
"Australia/Brisbane",//25
"Europe/Minsk",//26
"America/Sao_Paulo",//27
"America/New_York",//28
"Africa/Cairo",//29
"Asia/Yekaterinburg",//30
"Pacific/Fiji",//31
"Europe/Riga",//32
"Asia/Tbilisi",//33
"Europe/London",//34
"America/Godthab",//35
"Africa/Monrovia",//36
"Europe/Istanbul",//37
"Pacific/Honolulu",//38
"Asia/Calcutta",//39
"Asia/Tehran",//40
"Asia/Jerusalem",//41
"Asia/Seoul",//42
"Atlantic/South_Georgia",//43
"America/Denver",//44
"America/Chihuahua",//45
"Asia/Rangoon",//46
"Asia/Novosibirsk",//47
"Africa/Windhoek",//48
"Asia/Kathmandu",//49
"Pacific/Auckland",//50
"America/St_Johns",//51
"Asia/Irkutsk",//52
"Asia/Krasnoyarsk",//53
"America/Santiago",//54
"America/Tijuana",//55
"Europe/Paris",//56
"Europe/Moscow",//57
"America/Argentina/Buenos_Aires",//58
"America/Bogota",//59
"America/La_Paz",//60
"Pacific/Apia",//61
"Asia/Bangkok",//62
"Asia/Singapore",//63
"Africa/Harare",//64
"Asia/Colombo",//65
"Asia/Taipei",//66
"Australia/Hobart",//67
"Asia/Tokyo",//68
"Pacific/Tongatapu",//69
"America/Indianapolis",//70
"America/Phoenix",//71
"Asia/Vladivostok",//72
"Australia/Perth",//73
"Africa/Lagos",//74
"Europe/Berlin",//75
"Asia/Tashkent",//76
"Pacific/Guam",//77
"Asia/Yakutsk",//78
};


#define ITEM_DIRTY_FLAG                            0x5A
#define UPDATEFLAG_OFFSET                          0x36  
#define CHECKSUM_OFFSET                            0x30
#define VERSION_MASETER_OFFSET                            0x40
#define VERSION_MIRROR_OFFSET                             0x200                            
#define FLASH_USER1_ADDRESS			   0x38000
#define FLASH_USER2_ADDRESS			   0x38800
#define FLASH_USER3_ADDRESS			   0x39000
#define FLASH_USER4_ADDRESS			   0x39800
#define FLASH_USER5_ADDRESS			   0x3A000
#define FLASH_MACHINE_INFO_ADDRESS	           0x3A800
#define FLASH_CUSTOM_PRG_ADDRESS                   0x3B000
#define FLASH_WORTOUT_INFO_ADDRESS                 0x3B800
#define FLASH_BOOTLOADER_FLAG_ADDRESS              0x3E000
#define FLASH_ACCUMULATED_DATA_ADDRESS             0x0FE00000


#define SIZE_OF_MACHINEINFO   1024
typedef union {
    MACHINE_INFO mfields;
    UINT8 machineInfoBuffer[SIZE_OF_MACHINEINFO];
}MACHINE_INFO_UNION;

#define SIZE_OF_USERINFO  128
typedef union {
    USERS_INFO ufields;
    UINT8 userInfoBuffer[SIZE_OF_USERINFO];
}USERS_INFO_UNION;

#define SIZE_OF_CUSTOMCONFIG  64
typedef union{
    CUSTOM_CONFIG ccfields;
    UINT8 customProgramBuffer[SIZE_OF_CUSTOMCONFIG];
}CUSTOM_CONFIG_UNION;

#define SIZE_OF_ACCUMULATEDINFO  32
typedef union{
    ACCUMULATED_DATA accfields;
    UINT8 accmulatedData[SIZE_OF_ACCUMULATEDINFO];
}ACCUMULATED_DATA_UNION;

#define SIZE_OF_WOKROUTINFO    512
typedef union{
    WORKOUT_INFO wifields;
    UINT8 workoutInfoData[SIZE_OF_WOKROUTINFO];
}WORKOUT_INFO_UNION;


__no_init union
{
  struct{
    MACHINE_INFO_UNION _machineInfo;
    USERS_INFO_UNION _userInfo[MAX_SUPPORTED_USERS];
    ACCUMULATED_DATA_UNION _accumulatedInfo;
    CUSTOM_CONFIG_UNION _customWorkouts[MAX_SUPPORTED_CUSTOM_PRGS];
  }fields;
  unsigned char g_flashBuffer[2048];
}FLASH_DATA;
 

extern const char* rscu_version[];



static UINT8 SystemConfigResetAllData(void);
static void Default_Data(bool isForcedToDefault);

/***************************************************************************//**
* @brief
 *    Search all data stored in Flash and get the latest data and copy the data to corresponding 
 *    structure.
 *
 * @param[in] addr
 *   address of the page to be searched
 *
 * @param[in] pDest
 *   The destinated structure to be written.
 *
 * @param[in] len
 *   size of the structure.
 *
 * @note ????
 *  
******************************************************************************/
static void SystemGetLatestStoredData(UINT8 *pSrc, UINT8 *pDest, UINT16 dataLen)
{
    UINT16 PAGE_SIZE = HAL_FLASH_Get_Page_Size();
    if(pSrc == NULL  || pDest == NULL || dataLen > PAGE_SIZE ) return ;
    if(dataLen == 0) return;
    UINT16 endPos = PAGE_SIZE/dataLen;
    UINT16 index = 0;
    for( index = 0; index < endPos; index++)
    {
    	if(*(pSrc + index*dataLen) == 0xFF)
    	{
            if(index == 0)    
                memset(pDest, 0xFF, dataLen);  //Flash just be erased. 
            else
            {
                HAL_FLASH_Burst_Read((UINT32)(pSrc+(index-1)*dataLen), pDest, dataLen);
            }
            break;
    	}
    }
}


/***************************************************************************//**
* @brief
 *    Store data structure, such as accumulated info, user info to the flash.  
 *    structure.
 *
 * @param[in] pSrc
 *   Pointer to the data to be written to Flash
 *
 * @param[in] pDest
 *   The destinated Flash to be written.
 *
 * @param[in] len
 *   size of the structure.
 *
 * @note
 *  
******************************************************************************/
static UINT8 SystemSaveStructedData(UINT8* pDest, UINT8 *pSrc, UINT16 dataLen,  UINT8 enableINT )
{
    UINT8 ret = 0;
    UINT16 PAGE_SIZE = HAL_FLASH_Get_Page_Size();
    if(pSrc == NULL  || pDest == NULL || dataLen > PAGE_SIZE || dataLen == 0) return ret;
    if(dataLen & 0x03 != 0) return ret;
	//减一，做防止溢出处理	
    UINT16 endPos = PAGE_SIZE/dataLen -1;
    UINT16 index = 0;
    for( index = 0; index < endPos; index++)
    {
    	if(*(pDest + index*dataLen) == 0xFF)
    	{
    	  if(HAL_FLASH_Burst_Write((UINT32)(pDest + index*dataLen), pSrc, dataLen ,enableINT))
              ret = 1;
          else
              ret = 0;
          return ret;
    	}
    }
   if(HAL_FLASH_Erase_Page((UINT32)pDest,enableINT) == false) return 0;	
   if(HAL_FLASH_Burst_Write((UINT32)pDest, pSrc, dataLen, enableINT) == false) return 0;
   ret = 1;
   return ret;
}





#define SAVE_MACHINE_CONFIG_TO_FLASH_AND_DISABLEINT \
  HAL_FLASH_Burst_Write(FLASH_MACHINE_INFO_ADDRESS, (UINT8*)(&FLASH_DATA.fields._machineInfo), sizeof(MACHINE_INFO_UNION) ,0);
#define SAVE_MACHINE_CONFIG_TO_FLASH_AND_ENABLEINT \
  HAL_FLASH_Burst_Write(FLASH_MACHINE_INFO_ADDRESS, (UINT8*)(&FLASH_DATA.fields._machineInfo), sizeof(MACHINE_INFO_UNION) ,1);

#define SAVE_CUSTOM_PROGRAM_TO_FLASH_AND_DISABLEINT \
  HAL_FLASH_Burst_Write(FLASH_CUSTOM_PRG_ADDRESS, (UINT8 *)(&FLASH_DATA.fields._customWorkouts[0]), sizeof(CUSTOM_CONFIG_UNION)*4 ,0);
#define SAVE_CUSTOM_PROGRAM_TO_FLASH_AND_ENABLEINT \
  HAL_FLASH_Burst_Write(FLASH_CUSTOM_PRG_ADDRESS, (UINT8 *)(&FLASH_DATA.fields._customWorkouts[0]), sizeof(CUSTOM_CONFIG_UNION)*4 ,1);	


/***************************************************************************//**
* @brief
 *    Initialize Flash
 *    structure.
 *
 * @param[in]
 *    none
 *
 * @param[out]
 *    none
 *
 * @note
 *    This function must be called after POR.
 *  
******************************************************************************/
void SystemConfig_Initial(void)
{
    memset( FLASH_DATA.g_flashBuffer, 0, HAL_FLASH_Get_Page_Size());
    
    //Step 1, read machine config info _machineInfo 
    HAL_FLASH_Burst_Read(FLASH_MACHINE_INFO_ADDRESS, (UINT8 *)(FLASH_DATA.fields._machineInfo.machineInfoBuffer), sizeof(MACHINE_INFO));
    
    if(FLASH_DATA.fields._machineInfo.mfields.firstBootFlag != 0xA5)
    {
        //isFirstBoot = true;
        //-------------------------------------------Accumulated Data--------------------------------------------
        HAL_FLASH_Erase_Page(FLASH_ACCUMULATED_DATA_ADDRESS, 0);
        HAL_FLASH_Erase_Page(FLASH_MACHINE_INFO_ADDRESS,0);
        HAL_FLASH_Erase_Page(FLASH_USER1_ADDRESS, 0);
        HAL_FLASH_Erase_Page(FLASH_USER2_ADDRESS, 0);
        HAL_FLASH_Erase_Page(FLASH_USER3_ADDRESS, 0);
        HAL_FLASH_Erase_Page(FLASH_USER4_ADDRESS, 0);
        HAL_FLASH_Erase_Page(FLASH_USER5_ADDRESS, 0);
        HAL_FLASH_Erase_Page(FLASH_CUSTOM_PRG_ADDRESS,0);
		
		Default_Data(1);
		
		SystemSaveStructedData((UINT8 *)(FLASH_ACCUMULATED_DATA_ADDRESS ), (UINT8 *)(&FLASH_DATA.fields._accumulatedInfo),sizeof(ACCUMULATED_DATA_UNION), 0);
        SAVE_MACHINE_CONFIG_TO_FLASH_AND_DISABLEINT	
		for(UINT8 i = 0; i < MAX_SUPPORTED_USERS; i++ )
        SystemSaveStructedData((UINT8 *)(FLASH_USER1_ADDRESS + i * 0x800 ), (UINT8 *)(&FLASH_DATA.fields._userInfo[i]),sizeof(USERS_INFO_UNION), 0);
		SAVE_CUSTOM_PROGRAM_TO_FLASH_AND_DISABLEINT
			
        
    }	
    else
    {
        //Step 2, read user data into _userInfo
        for(UINT8  i = 0; i < MAX_SUPPORTED_USERS; i++ )
        {
            SystemGetLatestStoredData((UINT8 *)(FLASH_USER1_ADDRESS + i*0x800), FLASH_DATA.fields._userInfo[i].userInfoBuffer, sizeof(USERS_INFO_UNION));
        }
        
        
        //Step 3, read accumulated data
        SystemGetLatestStoredData((UINT8 *)(FLASH_ACCUMULATED_DATA_ADDRESS), FLASH_DATA.fields._accumulatedInfo.accmulatedData, sizeof(ACCUMULATED_DATA_UNION));
        
        
        //Step 4, read customized programs to _customWorkouts
        HAL_FLASH_Burst_Read(FLASH_CUSTOM_PRG_ADDRESS, FLASH_DATA.fields._customWorkouts[0].customProgramBuffer, (sizeof(CUSTOM_CONFIG_UNION))* MAX_SUPPORTED_CUSTOM_PRGS);
        
    }
}

/***************************************************************************//**
* @brief
 *    Set Machine Information
 *
 * @param[in]
 *    machineinfo
 *
 * @param[out]
 *    API returns 1 if the new machineinfo does not equal to old one. 
 *
 * @note
 *    SystemConfigProcess must be called to save machine info to flash if 1 is returned.
 *  
******************************************************************************/
UCHAR SystemConfig_Set_MachineInfo(MACHINE_INFO machineinfo)
{

    int result=memcmp(&machineinfo,&FLASH_DATA.fields._machineInfo,sizeof(MACHINE_INFO));
    UCHAR ret=0;
    if(result != 0)
    {
        memcpy(&FLASH_DATA.fields._machineInfo,&machineinfo,sizeof(MACHINE_INFO));
        ret = 1;
    }
    return ret;    
}


/***************************************************************************//**
* @brief
*    用于WIFI获取machineinfo
*
* @note
*  
******************************************************************************/
MACHINE_INFO SystemConfig_Get_MachineInfo(void)
{
    MACHINE_INFO machineinforet;
    //HAL_FLASH_Burst_Read(FLASH_MACHINE_INFO_ADDRESS, (UINT8 *)(&FLASH_DATA.fields._machineInfo.mfields), sizeof(MACHINE_INFO));
    memcpy(&machineinforet, &FLASH_DATA.fields._machineInfo.mfields, sizeof(MACHINE_INFO));
    return machineinforet;
}


/***************************************************************************//**
* @brief
*    用于WIFI初始化UserInfo
*
* @note
*  	return值：0为前后相同，不保存；1为前后两者不同，进行保存；2为USER index不在范围内
*
******************************************************************************/
UCHAR SystemConfig_Reset_User_Default(UCHAR index)
{
     int ret = 0;
    if((index > MAX_SUPPORTED_USERS)||(0 == index))return 2;

    USERS_INFO_UNION defaultUserInfo;
	memset(&defaultUserInfo, 0, sizeof(USERS_INFO_UNION));
    defaultUserInfo.ufields.userConsoleId = index;
    memset( defaultUserInfo.ufields.xid,'0', 14);
    memset(defaultUserInfo.ufields.passcode,'0',4);
    defaultUserInfo.ufields.weightStandardX100 = FLASH_DATA.fields._machineInfo.mfields.defaultWeightLBSX100;
    defaultUserInfo.ufields.age = FLASH_DATA.fields._machineInfo.mfields.defaultAge;
    defaultUserInfo.ufields.year = 15;
    defaultUserInfo.ufields.year = defaultUserInfo.ufields.year + 2000 - defaultUserInfo.ufields.age;
    defaultUserInfo.ufields.month = 4;
    defaultUserInfo.ufields.date = 23;
    defaultUserInfo.ufields.gender = 0;
    defaultUserInfo.ufields.timeStampYear = 2000;
    defaultUserInfo.ufields.timeStampMonth = 1;
    defaultUserInfo.ufields.timeStampDate = 1;
    defaultUserInfo.ufields.timeStampHour = 10;
    defaultUserInfo.ufields.timeStampMinute = 0;
    defaultUserInfo.ufields.timeStampSeconds = 0;
    memset( defaultUserInfo.ufields.name, 0, 65 );
    defaultUserInfo.ufields.displayUnit = FLASH_DATA.fields._machineInfo.mfields.unit;
    defaultUserInfo.ufields.isLoggedOn = 0;
    defaultUserInfo.ufields.isAcceptedTerms = 0;
    defaultUserInfo.ufields.isShareWorkouts = 0;
	defaultUserInfo.ufields.isSetup = 0;
    ret = memcmp(&(defaultUserInfo.ufields),&FLASH_DATA.fields._userInfo[index -1],sizeof(USERS_INFO));
    if(ret != 0)
	{
		memcpy(&FLASH_DATA.fields._userInfo[index -1],&defaultUserInfo.ufields,sizeof(USERS_INFO));
		SystemConfigProcess(SVK_SAVE_USER2FLASH,&index,sizeof(UCHAR));
		ret=1;
	}
	return ret;
}



/***************************************************************************//**
* @brief
*    用于WIFI设置UserInfo
* @params
*    input: userinfo为传入的USER信息
*    input: index为用户Slot，其中1对应User slot 0, 5对应User slot 4,及USER 4
*    output: 返回0传入参数有误或者传入用户信息和存储信息一致，返回1表示信息不
*            一致，需要调用函数进行保存。
* @note
******************************************************************************/
UCHAR SystemConfig_Set_UserInfo(USERS_INFO userinfo , UCHAR index)
{
    if((index > MAX_SUPPORTED_USERS)||(0 == index))
        return 0;

    int result=memcmp(&userinfo,&FLASH_DATA.fields._userInfo[index -1],sizeof(USERS_INFO));
    UCHAR ret=0;
    if(result != 0)
    {
        memcpy(&FLASH_DATA.fields._userInfo[index -1],&userinfo,sizeof(USERS_INFO));
        ret = 1;
    }

    return ret;

}

/***************************************************************************//**
* @brief
*    用于WIFI获取UserInfo
*
* @note
* input: index 0 : GUEST, 1, USER0... 5, USER 4
*  
******************************************************************************/
USERS_INFO SystemConfig_Get_UserInfo(UCHAR index)
{

  if(index > MAX_SUPPORTED_USERS)
  {
    index = MAX_SUPPORTED_USERS;
  }

  USERS_INFO userinforet ;  
  if(index == 0)
  {
      CHAR guestName[16] = " "; 
      userinforet.userConsoleId = 0;
      for( int j = 0; j < 14; j++ )
          userinforet.xid[j] = '0';
      for( int j = 0; j < 4; j++ )
          userinforet.passcode[j] = '0';
      userinforet.weightStandardX100 = Factory_Default[FACTORY_DEFAULT_WEIGHT_STANDARD];
      userinforet.age = Factory_Default[FACTORY_DEFAULT_AGE];
      userinforet.year = 15;
      userinforet.year = userinforet.year + 2000 - userinforet.age;
      userinforet.month = 1;
      userinforet.date = 1;
      userinforet.gender = 0;
      userinforet.timeStampYear = 2000;
      userinforet.timeStampMonth = 1;
      userinforet.timeStampDate = 1;
      userinforet.timeStampHour = 10;
      userinforet.timeStampMinute = 0;
      userinforet.timeStampSeconds = 0;
      memcpy(userinforet.name, guestName, 16);
      userinforet.displayUnit = FLASH_DATA.fields._machineInfo.mfields.unit;
      userinforet.isLoggedOn = 0;
      userinforet.isAcceptedTerms = 0;
      userinforet.isShareWorkouts = 0;
      userinforet.isSetup = 0;
  }
  else
  {
      SystemGetLatestStoredData((UINT8 *)(FLASH_USER1_ADDRESS + (index-1)*0x800), FLASH_DATA.fields._userInfo[index -1].userInfoBuffer, sizeof(USERS_INFO_UNION));
      memcpy(&userinforet, &FLASH_DATA.fields._userInfo[index-1], sizeof(USERS_INFO));
  }
  return userinforet;
}

/***************************************************************************//**
* @brief
*    用于读取char类型保存内容
*
* @note
*  
******************************************************************************/
UCHAR SystemConfig_Get_Char(SYSTEM_CONFIG_TYPE type)
{
    EFM_ASSERT(type < TOTAL_ITEMS_INT8);
    UCHAR by_Dat=0;
    switch(type)
    {
    case FIRST_BOOT_FLAG_UCHAR:  
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.firstBootFlag;
        break;
	case IS_MACHINE_CONFIGED_UCHAR:
	    by_Dat=FLASH_DATA.fields._machineInfo.mfields.IsMachineConfiged;
		break;
    case MACHINE_TYPE_UINT8:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.machineType;
        break;
    case MACHINE_MODLE_UINT8:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.machineModel;
        break;
    case BRAND_ID_UINT8:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.brandId;
        break;
    case LANGUAGE_ID_UINT8:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.languageId;
        break;
    case DRIVE_TYPE_UINT8:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.driveType;
        break;
    case UNIT_FLAG_UCHAR:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.unit;
        break;
    case FIRMWARE_FORCE_UPDATE_UCHAR:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.isforceUpdate;
        break;
    case NEW_FIRMWARE_EXISTS_UCHAR:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.isFirmwareAvailable;
        break;
    case DISPLAY_MODE_UCHAR:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.displayMode;
        break;
    case FIRMWARE_MAJOR_UCHAR:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.versionMajor;
        break;
    case FIRMWARE_MINOR_UCHAR:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.versionMinor;
        break;
    case FIRMWARE_REVISION_UCHAR:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.versionRevision;
        break;
    case DEMO_MODE_FLAG_UCHAR:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.isDemoModeOn;
        break;
    case ENERGY_SAVER_FLAG_UCHAR:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.isEnergySaverModeOn;
        break;
    case BUZZER_FLAG_UCHAR:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.isBuzzerOn;
        break;
    case RTC_USED_FLAG_UCHAR:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.wasRTCPreset;
        break;
    case RTC_TIME_FORMAT_UCHAR:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.is24HoursFormat;
        break;
    case AUTO_LOGIN_FLAG_UCHAR:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.isAutoLogin;
        break;
    case AP_MODE_FLAG_UCHAR:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.isAPModeOn;
        break;
    case RF_CHANNEL_UCHAR:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.rfChannel;
        break;
    case RF_ADDRESS_UCHAR:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.rfAddress;
        break;
    case MAX_RESISTANCE_UINT8:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.maxResistance;
        break;
    case MIN_RESISTANCE_UINT8:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.minResistance;
        break;
    case CSAFE_ID_UINT8:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.csafeId;
        break;
    case CSAFE_MANUFACTURE_ID_UINT8:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.csafeManufactureId;
        break;
    case CSAFE_MODEL_ID_UINT8:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.csafeModeId;
        break;
    case CLUB_ID_UINT8:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.clubId;
        break;
    case OVERSPEED_SWITCH_UINT8	:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.overspeedswitch;
	 break;	
   case OVERSPEED_VALUE	:
    	by_Dat=FLASH_DATA.fields._machineInfo.mfields.overspeedvalue;
 	break;	
	case SHOW_ERROR_FLAG_UCHAR:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.IsShowError;
    break;  
    case CONTROL_ZONE_FLAG_UCHAR:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.controlZoneFlag;
    break; 
    case SECOND_CONTROL_ZONE_FLAG_UCHAR:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.secondControlZoneFlag;
    break; 
    case IR_SENSOR_FLAG_UCHAR:
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.irSensorFlag;
    break; 
    case RSCU_FLAG_UCHAR:
    {
        by_Dat=FLASH_DATA.fields._machineInfo.mfields.rscuFlag;
    }
    break;
	default:
        EFM_ASSERT(0);
        break;
    }
    return(by_Dat);
}

/***************************************************************************//**
* @brief
*    用于读取Int类型保存内容
*
* @note
*  
******************************************************************************/
UINT16 SystemConfig_Get_Int(SYSTEM_CONFIG_TYPE type)
{
        EFM_ASSERT(type > TOTAL_ITEMS_INT8 && type < TOTAL_ITEMS_INT16);
	UINT16 by_Dat = 0;	
	switch(type)
	{

	  case MAX_WEIGHT_STANDARD_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.maxUserWeightLbs;
		break;
	  case MIN_WEIGHT_STANDARD_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.minUserWeightLbs;
		break;
	  case MAX_WEIGHT_METRIC_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.maxUserWeightKg;
		break;
	  case MIN_WEIGHT_METRIC_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.minUserWeightKg;
		break;
	  case MAX_MPHX10_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.maxSpeedMphX10;
		break;
	  case MIN_MPHX10_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.minSpeedMphX10;
		break;
	  case MAX_KPHX10_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.maxSpeedKphX10;
		break;
	  case MIN_KPHX10_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.minSpeedKphX10;
		break;
	  case MAX_MPH_RPM_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.maxRpmMphX10;
		break;
	  case MIN_MPH_RPM_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.minRpmMphX10;
		break;
	  case MAX_KPH_RPM_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.maxRpmKphX10;
		break;
	  case MIN_KPH_RPM_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.minRpmKphX10;
		break;
	  case MAX_INCLINEX10_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.maxInclineX10;
		break;
	  case MIN_INCLINEX10_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.minInclineX10;
		break;
	  case MAX_INCLINEADC_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.maxInclineADC;
		break;
	  case MIN_INLCINEADC_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.minInclineADC;
		break;

	  case ACCUMULATED_AGING_HOURS_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.accumulatedAgingHours;
		break;	  
	  case MAX_AGE_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.MaxAge;
		break;
	  case DEFAULT_AGE_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.defaultAge;
		break;
	  case MIN_AGE_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.MinAge;
		break;
	  case DEFAULT_WEIGHTLBSX100_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.defaultWeightLBSX100;
		break;
	  case DEFAULT_WEIGHTKGX100_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.defaultWeightKGX100;
		break;
	  case DEFALUT_VOLUME_UINT8:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.defaultVolume;
		break;
	  case DEFAULT_BRIGHTNESS_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.defaulitBrightness;
		break;
//	  case DEFAULT_WORKOUT_MIN_UINT16:
//		by_Dat=WorkoutConfig->defaultWorkoutMinutes;
//		break;
//	  case MAX_WORKOUT_MIN_UINT16:
//		by_Dat=WorkoutConfig->maxWorkoutMinutes;
//		break;
//	  case DEFAULT_WARMUP_MIN_UINT16:
//		by_Dat=WorkoutConfig->defaultWarmupMinutes;
//		break;
//	  case DEFAULT_COOLDOWN_MIN_UINT16:
//		by_Dat=WorkoutConfig->defaultCooldownMinutes;
//		break;
//	  case DEFAULT_PAUSE_MIN_UINT16:
//		by_Dat=WorkoutConfig->defaultPauseMinutes;
//		break;
//	  case DEFAULT_HR_TARGET_UINT16:
//		by_Dat=WorkoutConfig->defaultTHRTarget;
//		break;
//	  case DEFAULT_PHR_TARGET_UINT16:
//		by_Dat=WorkoutConfig->defaultPTHRTarget;
//		break;
//	  case DEFAULT_WATTS_TARGET_UINT16:
//		by_Dat=WorkoutConfig->defaultWattsTarget;
//		break;
	  case CUSTOM1_WORKOUT_MIN_UINT16:
		by_Dat=FLASH_DATA.fields._customWorkouts[0].ccfields.customWorkoutTime;
		break;
	  case CUSTOM2_WORKOUT_MIN_UINT16:
		by_Dat=FLASH_DATA.fields._customWorkouts[1].ccfields.customWorkoutTime;
		break;
	  case CUSTOM3_WOKROUT_MIN_UINT16:
		by_Dat=FLASH_DATA.fields._customWorkouts[2].ccfields.customWorkoutTime;
		break;
	  case CUSTOM4_WORKOUT_MIN_UINT16:
		by_Dat=FLASH_DATA.fields._customWorkouts[3].ccfields.customWorkoutTime;
		break;
	  case DEMO_WAIT_MIN_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.demoOnWaitMinutes;
		break;
	  case ERP_WAIT_MIN_UINT16:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.erpOnWaitMinutes;
		break;
	
	  default:
                EFM_ASSERT(0);
		break;
	}
	return(by_Dat);
}

/***************************************************************************//**
* @brief
*    用于读取long类型保存内容
*
* @note
*  
******************************************************************************/
UINT32 SystemConfig_Get_Long(SYSTEM_CONFIG_TYPE type)
{
    
    EFM_ASSERT(type > TOTAL_ITEMS_INT16 && type < TOTAL_ITEMS_LONG);
	UINT32 by_Dat = 0;
	
	switch(type)
	{
        case ACCUMULATED_MILESX100_LONG:
		by_Dat = FLASH_DATA.fields._accumulatedInfo.accfields.accumulatedDistanceMilesX100;
		break;
        case ACCUMULATED_SECONDS_LONG:
		by_Dat = FLASH_DATA.fields._accumulatedInfo.accfields.accumulatedTimeSeconds;
		break;
        case ACCUMULATED_LUBE_MILESX100_LONG:
		by_Dat = FLASH_DATA.fields._accumulatedInfo.accfields.accumulatedLubeMilesX100;
		break;
        case ACCUMULATED_LUBE_TIME:
        by_Dat = FLASH_DATA.fields._accumulatedInfo.accfields.accumulatedLubeTime;
        break;
        default:
        EFM_ASSERT(0);
        break;
	}
	return(by_Dat);
}

/***************************************************************************//**
* @brief
*    用于读取字符串类型保存内容
*
* @note
*  
******************************************************************************/
UCHAR* SystemConfig_Get_String(SYSTEM_CONFIG_TYPE type)
{
	EFM_ASSERT(type > TOTAL_ITEMS_LONG && type < ALL_SYSTEM_CONFIG_TYPES);
	unsigned char *by_Dat=NULL;
	switch(type)
	{
	  case CONSOLE_SERIAL_NUMBER_ARRAY:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.consoleSerialNumber;
		break;
	  case FRAME_SERIAL_NUMBER_ARRAY:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.frameSerialNumber;
		break;
	  case MARIO_BOX_ADDRESS_STRING:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.marioBox;
		break;
	  case MACHINE_UUIND_ARRAY:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.machineUUID;
		break;
	  case RTC_TIME_ZONE_STRING:
		by_Dat=FLASH_DATA.fields._machineInfo.mfields.timezone;
		break;
	  case CUSTOM1_SEGMENTS_ARRAY_SPEEDX10:
		by_Dat=FLASH_DATA.fields._customWorkouts[0].ccfields.customSpeedX10;
		break;
	  case CUSTOM2_SEGMENTS_ARRAY_SPEEDX10:
		by_Dat=FLASH_DATA.fields._customWorkouts[1].ccfields.customSpeedX10;
		break;
	  case CUSTOM3_SEGMENTS_ARRAY_SPEEDX10:
		by_Dat=FLASH_DATA.fields._customWorkouts[2].ccfields.customSpeedX10;
		break;
	  case CUSTOM4_SEGMENTS_ARRAY_SPEEDX10:
		by_Dat=FLASH_DATA.fields._customWorkouts[3].ccfields.customSpeedX10;
		break;
	  case CUSTOM1_SEGMENTS_ARRAY_INCLINEX10:
		by_Dat=FLASH_DATA.fields._customWorkouts[0].ccfields.customInclineX10;
		break;
	  case CUSTOM2_SEGMENTS_ARRAY_INCLINEX10:
		by_Dat=FLASH_DATA.fields._customWorkouts[1].ccfields.customInclineX10;
		break;
	  case CUSTOM3_SEGMENTS_ARRAY_INCLINEX10:
		by_Dat=FLASH_DATA.fields._customWorkouts[2].ccfields.customInclineX10;
		break;
	  case CUSTOM4_SEGMENTS_ARRAY_INCLINEX10:
		by_Dat=FLASH_DATA.fields._customWorkouts[3].ccfields.customInclineX10;
		break;
	  case CUSTOM1_SEGMENTS_ARRAY_RESISTANCE:
		by_Dat=FLASH_DATA.fields._customWorkouts[0].ccfields.customresistance;
		break;
	  case CUSTOM2_SEGMENTS_ARRAY_RESISTANCE:
		by_Dat=FLASH_DATA.fields._customWorkouts[1].ccfields.customresistance;
		break;
	  case CUSTOM3_SEGMENTS_ARRAY_RESISTANCE:
		by_Dat=FLASH_DATA.fields._customWorkouts[2].ccfields.customresistance;
		break;
	  case CUSTOM4_SEGMENTS_ARRAY_RESISTANCE:
		by_Dat=FLASH_DATA.fields._customWorkouts[3].ccfields.customresistance;
		break;
	  case USER1_INFO_STRING:
		by_Dat=FLASH_DATA.fields._userInfo[0].ufields.name;
		break;
	  case USER2_INFO_STRING:
		by_Dat=FLASH_DATA.fields._userInfo[1].ufields.name;
		break;
	  case USER3_INFO_STRING:
		by_Dat=FLASH_DATA.fields._userInfo[2].ufields.name;
		break;
	  case USER4_INFO_STRING:
		by_Dat=FLASH_DATA.fields._userInfo[3].ufields.name;
		break;
	  case USER5_INFO_STRING:
		by_Dat=FLASH_DATA.fields._userInfo[4].ufields.name;
		break;
//	  case STAMP_OF_GOAL_RTC:
//		by_Dat=FLASH_DATA.fields._machineInfo.mfields.//?
//		break;
	  default:
                EFM_ASSERT(0);
		break;
	}
	return(by_Dat);
}


/***************************************************************************//**
* @brief
 *    System Configure Virtual Key processor
 *
 * @param[in]
 *    vk, Virtual Key.
 *
 * @param[in]
 *    *p, the virtual key parameter.
 *
  * @param[in]
 *    size, the size of the virtual key parameter.
 *
   * @param[out]
 *    1 means that the virtual key executed successfully while 0 mean virtual key execution failed.
 *
 * @note
 *    SystemConfigProcess must be called to save machine info to flash if 1 is returned.
 *  
******************************************************************************/
UINT8  SystemConfigProcess(SYSTEM_VIRTUAL_KEY vk, void *p, UINT16 size)
{
      UINT8 ret = 1;  // default as successful.
	switch(vk)
	{
	  case SVK_NONE:
		break;
	  case SVK_RESETDATA:
//		if( (*(UCHAR*)p) == 1 )
			ret = SystemConfigResetAllData();
		break;
	  case SVK_TOGGLE_FIRSTBOOT:
                if(size != sizeof(UCHAR))
                { 
                    ret = 0;
                    return ret;
                }
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.firstBootFlag),p,sizeof(UCHAR));
		break;
		  case SVK_TOGGLE_OVERSPEEDFLAG:
                if(size != sizeof(UCHAR))
                { 
                    ret = 0;
                    return ret;
                }
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.overspeedswitch),p,sizeof(UCHAR));
		break;	
 		
	  case SVK_TOGGLE_IS_MACHINE_CONFIGED:
			if(size != sizeof(UCHAR)) return 0;
			memcpy(&(FLASH_DATA.fields._machineInfo.mfields.IsMachineConfiged),p,sizeof(UCHAR));
			break;
	  case SVK_MACHINETYPE_UP:
		break;
	  case SVK_MACHINETYPE_DOWN:
		break;
	  case SVK_SET_MACHINETYPE:
                if(size != sizeof(UINT8)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.machineType),p,sizeof(UINT8));
		break;
	  case SVK_SET_MACHINEMODEL:
                if(size != sizeof(UINT8)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.machineModel),p,sizeof(UINT8));
		break;
	  case SVK_BRANDID_UP:
		break;
	  case SVK_BRANDID_DOWN:
		break;
	  case SVK_LANGUAGEID_UP:
		break;
	  case SVK_LANGUAGEID_DOWN:
		break;
	  case SVK_SET_LANGUAGEID:
                if(size != sizeof(UINT8)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.languageId),p,sizeof(UINT8));
	  break;
        case SVK_TOGGLE_UNIT:
                if(size != sizeof(UINT8)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.unit),p,sizeof(UINT8));
		break;
	  case SVK_DRIVETYPE_UP:
		break;
	  case SVK_DRIVETYPE_DOWN:
		break;
	  case SVK_SET_DRIVETYPE:
                if(size != sizeof(UINT8)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.driveType),p,sizeof(UINT8));
		break;
	  case SVK_MAXWEIGHT_UP:
		break;
	  case SVK_MAXWEIGHT_DOWN:
		break;
	  case SVK_DISPLAYMODE_UP:
		break;
	  case SVK_DISPLAYMODE_DOWN:
		break;
	  case SVK_FIRMWARE_MAJOR_UP:
		break;
	  case SVK_FIRMWARE_MAJOR_DOWN:
		break;
	  case SVK_FIRMWARE_MINOR_UP:
		break;
	  case SVK_FIRMWARE_MINOR_DOWN:
		break;
	  case SVK_FIRMWARE_REV_UP:
		break;
	  case SVK_FIRMWARE_REV_DOWN:
		break;
	  case SVK_MAXSPEED_UP:
		break;
	  case SVK_MAXSPEED_DOWN:
		break;
	  case SVK_MAXSPEED_CONFIRM:
		break;
	  case SVK_MINSPEED_UP:
		break;
	  case SVK_MINSPEED_DOWN:
		break;
	  case SVK_MINSPEED_CONFIRM:
		break;
	  case SVK_MAXRPM_UP:
		break;
	  case SVK_MAXRPM_DOWN:
		break;
	  case SVK_MAXRPM_CONFIRM:
		break;
	  case SVK_MINRPM_UP:
		break;
	  case SVK_MINRPM_DOWN:
		break;
	  case SVK_MINRPM_CONFIRM:			
		break;
	  case SVK_MAXINCLINE_UP:
		break;
	  case SVK_MAXINCLINE_DOWN:
		break;
	  case SVK_MININCLINE_UP:
		break;
	  case SVK_MININCLINE_DOWN:
		break;
	  case SVK_MAXINCLINE_ADC_UP:
		break;
	  case SVK_MAXINCLINE_ADC_DOWN:
		break;
	  case SVK_MININCLINE_ADC_UP:
		break;
	  case SVK_MININCLINE_ADC_DOWN:
		break;
	case SVK_TOGGLE_OVERSPEEDVALUE:
            if(size != sizeof(UINT8))
            { 
                ret = 0;
                return ret;
            }
	memcpy(&(FLASH_DATA.fields._machineInfo.mfields.overspeedvalue),p,sizeof(UINT16));
	break;	
	  case SVK_SET_MAX_WEIGHT_STANDARD:
                if(size != sizeof(UINT16)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.maxUserWeightLbs),p,sizeof(UINT16));
		break;
	  case SVK_SET_MIN_WEIGHT_STANDARD:
                if(size != sizeof(UINT16)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.minUserWeightLbs),p,sizeof(UINT16));
		break;
	  case SVK_SET_MAX_WEIGHT_METRIC:
                if(size != sizeof(UINT16)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.maxUserWeightKg),p,sizeof(UINT16));
		break;
	  case SVK_SET_MIN_WEIGHT_METRIC:
                if(size != sizeof(UINT16)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.minUserWeightKg),p,sizeof(UINT16));
		break;
	  case SVK_SET_MAX_MPHX10:
                if(size != sizeof(UINT16)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.maxSpeedMphX10),p,sizeof(UINT16));
		break;
	  case SVK_SET_MIN_MPHX10:
                if(size != sizeof(UINT16)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.minSpeedMphX10),p,sizeof(UINT16));
		break;
	  case SVK_SET_MAX_KPHX10:
                if(size != sizeof(UINT16)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.maxSpeedKphX10),p,sizeof(UINT16));
		break;
	  case SVK_SET_MIN_KPHX10:
                if(size != sizeof(UINT16)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.minSpeedKphX10),p,sizeof(UINT16));
		break;
	  case SVK_SET_MAX_MPH_RPM:
                if(size != sizeof(UINT16)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.maxRpmMphX10),p,sizeof(UINT16));
		break;
	  case SVK_SET_MIN_MPH_RPM:
                if(size != sizeof(UINT16)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.minRpmMphX10),p,sizeof(UINT16));
		break;
	  case SVK_SET_MAX_KPH_RPM:
                if(size != sizeof(UINT16)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.maxRpmKphX10),p,sizeof(UINT16));
		break;
	  case SVK_SET_MIN_KPH_RPM:
                if(size != sizeof(UINT16)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.minRpmKphX10),p,sizeof(UINT16));
		break;
	  case SVK_SET_MAX_INCLINEX10:
                if(size != sizeof(UINT16)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.maxInclineX10),p,sizeof(UINT16));
		break;
	  case SVK_SET_MIN_INCLINEX10:
                if(size != sizeof(UINT16)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.minInclineX10),p,sizeof(UINT16));
		break;
	  case SVK_SET_MAX_INCLINEADC:
                if(size != sizeof(UINT16)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.maxInclineADC),p,sizeof(UINT16));
		break;
	  case SVK_SET_MIN_INLCINEADC:
                if(size != sizeof(UINT16)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.minInclineADC),p,sizeof(UINT16));
		break;
	  case SVK_SET_MAX_RESISTANCE:
                if(size != sizeof(UINT8)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.maxResistance),p,sizeof(UINT8));
		break;
	  case SVK_SET_MIN_RESISTANCE:
                if(size != sizeof(UINT8)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.minResistance),p,sizeof(UINT8));
		break;
	  case SVK_NUM0:
		break;
	  case SVK_NUM1:
		break;
	  case SVK_NUM2:
		break;
	  case SVK_NUM3:
		break;
	  case SVK_NUM4:
		break;
	  case SVK_NUM5:
		break;
	  case SVK_NUM6:
		break;
	  case SVK_NUM7:
		break;
	  case SVK_NUM8:
		break;
	  case SVK_NUM9:
		break;
	  case SVK_TOGGLE_DEMO:
                if(size != sizeof(UINT8)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.isEnergySaverModeOn),p,sizeof(UINT8));
		break;
	  case SVK_TOGGLE_ERP:
                if(size != sizeof(UINT8)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.isEnergySaverModeOn),p,sizeof(UINT8));
		break;
	  case SVK_TOGGLE_BUZZER:
                if(size != sizeof(UINT8)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.isBuzzerOn),p,sizeof(UINT8));
		break;
	  case SVK_TOGGLE_RTC:
                if(size != sizeof(UINT8)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.wasRTCPreset),p,sizeof(UINT8));
		break;
	  case SVK_TOGGLE_TIMEFORMAT:
                if(size != sizeof(UINT8)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.is24HoursFormat),p,sizeof(UINT8));
		break;
	  case SVK_DEFAULT_VOL_UP:
		break;
	  case SVK_DEFAULT_VOL_DOWN:
		break;
	  case SVK_CONSOLE_SERIAL_UP:
		break;
	  case SVK_CONSOLE_SERIAL_DOWN:
		break;
	  case SVK_CONSOLE_SERIAL_NEXT:
		break;
	  case SVK_FRAME_SERIAL_UP:
		break;
	  case SVK_FRAME_SERIAL_DOWN:
		break;
	  case SVK_FRAME_SERIAL_NEXT:
		break;
	  case SVK_CSAFEID_UP:
		break;
	  case SVK_CSAFEID_DOWN:
		break;
	  case SVK_CSAFEID_CONFIRM:
		break;
	  case SVK_CSAFEMANUFACTUREID_UP:
		break;
	  case SVK_CSAFEMANUFACTUREID_DOWN:
		break;
	  case SVK_CSAFEMANUFACTUREID_CONFIRM:
		break;
	  case SVK_CSAFEMODELID_UP:
		break;
	  case SVK_CSAFEMODELID_DOWN:
		break;
	  case SVK_CSAFEMODELID_CONFIRM:
		break;
	  case SVK_CLUBID_UP:
		break;
	  case SVK_CLUBID_DOWN:
		break;
	  case SVK_CLUBID_CONFIRM:
		break;
	  case SVK_TOGGLE_AUTOLOGIN:
                if(size != sizeof(UINT8)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.isAutoLogin),p,sizeof(UINT8));
		break;
	  case SVK_TOGGLE_APMODE:
                if(size != sizeof(UINT8)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.isAPModeOn),p,sizeof(UINT8));
		break;
	  case SVK_DEFAULT_WORKOUT_TIME_UP:
		break;
	  case SVK_DEFAULT_WORKOUT_TIME_DOWN:
		break;
	  case SVK_DEFAULT_WORKOUT_TIME_CONFIRM:
		break;
          case SVK_ADD_ACCUMULATED_MILESX100:
            {
                if(size != sizeof(UINT32)) return 0;
                memcpy(&(FLASH_DATA.fields._accumulatedInfo.accfields.accumulatedDistanceMilesX100),p,sizeof(UINT32));
                break;
            }
	 case SVK_ACCAGING_HOURS:
	 {
		 if(size != sizeof(UINT16)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.accumulatedAgingHours),p,sizeof(UINT16));
	 }
	 break;
	  case SVK_ADD_ACCUMULATED_SECONDS:
		{
                    if(size != sizeof(UINT32)) return 0;
                    memcpy(&(FLASH_DATA.fields._accumulatedInfo.accfields.accumulatedTimeSeconds),p,sizeof(UINT32));
                    break;
		}
	  case SVK_ADD_ACCUMULATED_LUBE_MILESX100:
		{
                    if(size != sizeof(UINT32)) return 0;
                    memcpy(&(FLASH_DATA.fields._accumulatedInfo.accfields.accumulatedLubeMilesX100),p,sizeof(UINT32));
                    break;
		}
      case SVK_ADD_ACCUMULATED_LUBE_TIME:
		{
                    if(size != sizeof(UINT32)) return 0;
                    memcpy(&(FLASH_DATA.fields._accumulatedInfo.accfields.accumulatedLubeTime),p,sizeof(UINT32));
                    break;
		}
	  case SVK_SET_RF_CHANNEL:
                if(size != sizeof(UINT8)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.rfChannel),p,sizeof(UCHAR));
		break;
	  case SVK_SET_RF_ADDRESS:
                if(size != sizeof(UINT8)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.rfAddress),p,sizeof(UCHAR));
		break;
	  case SVK_SET_ERP_TIMES:
                if(size != sizeof(UINT16)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.erpOnWaitMinutes),p,sizeof(UINT16));
		break;
	  case SVK_SET_DEMO_TIMES:
                if(size != sizeof(UINT16)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.demoOnWaitMinutes),p,sizeof(UINT16));
		break;
	  case SVK_SAVE_USER2FLASH:
		{
                    if(size != sizeof(UINT8)) return 0;
                    UINT8 index = *((UINT8 *)p);
                    if((index == 0) || (index > MAX_SUPPORTED_USERS))
                        ret = 0;
                    else
                    {
                        SystemSaveStructedData((UINT8 *)(FLASH_USER1_ADDRESS + (index-1)*0x800), (UINT8 *)(&FLASH_DATA.fields._userInfo[index-1]), sizeof(USERS_INFO_UNION),1);
                        ret = 1;
                    }
		}
		break;
	 case SVK_SAVE_MACHINEINFO2FLASH:
	 	HAL_FLASH_Erase_Page(FLASH_MACHINE_INFO_ADDRESS, 1);
	 	SAVE_MACHINE_CONFIG_TO_FLASH_AND_ENABLEINT
		ret = 1;
	 	break;
	 case SVK_SAVE_ACCUMULATEDDATA2FLASH:
	 	if(SystemSaveStructedData((UINT8 *)(FLASH_ACCUMULATED_DATA_ADDRESS), (UINT8 *)(&FLASH_DATA.fields._accumulatedInfo),sizeof(ACCUMULATED_DATA_UNION), 1))
                    ret = 1;
                else
                    ret = 0;
	 	break;
	 case SVK_SAVE_CUSTOMPRG2FLASH:
	    HAL_FLASH_Erase_Page(FLASH_CUSTOM_PRG_ADDRESS, 1);
	 	SAVE_CUSTOM_PROGRAM_TO_FLASH_AND_ENABLEINT
		ret = 1;
	 	break;
	 case SVK_TOGGLE_SHOW_ERROR:
                if(size != sizeof(UINT8)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.IsShowError),p,sizeof(UINT8));
        break;
     case SVK_CONTROL_ZONE:
                if(size != sizeof(UINT8)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.controlZoneFlag),p,sizeof(UINT8));
        break;
     case SVK_SECOND_CONTROL_ZONE:
                if(size != sizeof(UINT8)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.secondControlZoneFlag),p,sizeof(UINT8));
        break;
     case SVK_IR_SENSOR:
                if(size != sizeof(UINT8)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.irSensorFlag),p,sizeof(UINT8));
        break;
        case SVK_RSCU:
        {
               if(size != sizeof(UINT8)) return 0;
		memcpy(&(FLASH_DATA.fields._machineInfo.mfields.rscuFlag),p,sizeof(UINT8));
        }
	  default:
		break;
	}
	return ret;
}

/***************************************************************************//**
* @brief
 *    Set Machine Information
 *
 * @param[in]
 *    machineinfo
 *
 * @param[out]
 *    API returns 1 if the new machineinfo does not equal to old one. 
 *
 * @note
 *    SystemConfigProcess must be called to save machine info to flash if 1 is returned.
 *  
******************************************************************************/
void SystemConfigRestoreToFactory(void)
{
    if(HAL_FLASH_Erase_Page(FLASH_ACCUMULATED_DATA_ADDRESS, 1)  == false) return;
    if(HAL_FLASH_Erase_Page(FLASH_CUSTOM_PRG_ADDRESS, 1) == false) return;
	if(HAL_FLASH_Erase_Page(FLASH_USER1_ADDRESS, 1) == false) return;
    if(HAL_FLASH_Erase_Page(FLASH_USER2_ADDRESS, 1) == false) return;
    if(HAL_FLASH_Erase_Page(FLASH_USER3_ADDRESS, 1) == false) return;
    if(HAL_FLASH_Erase_Page(FLASH_USER4_ADDRESS, 1) == false) return;
    if(HAL_FLASH_Erase_Page(FLASH_USER5_ADDRESS, 1) == false) return;
	
	Default_Data(0);
	
	SystemSaveStructedData((UINT8 *)(FLASH_ACCUMULATED_DATA_ADDRESS ), (UINT8 *)(&FLASH_DATA.fields._accumulatedInfo),sizeof(ACCUMULATED_DATA_UNION), 1);
	SAVE_MACHINE_CONFIG_TO_FLASH_AND_ENABLEINT	
	for(UINT8 i = 0; i < MAX_SUPPORTED_USERS; i++ )
    SystemSaveStructedData((UINT8 *)(FLASH_USER1_ADDRESS + i * 0x800 ), (UINT8 *)(&FLASH_DATA.fields._userInfo[i]),sizeof(USERS_INFO_UNION), 1);
	SAVE_CUSTOM_PROGRAM_TO_FLASH_AND_ENABLEINT
}

/***************************************************************************//**
* @brief
*    SystemConfig参数参数完全初始化，恢复到第一次烧完程序状态，用于ENG8
*
* @note
*   用于SystemConfigProcess的VK中
******************************************************************************/
UINT8 SystemConfigResetAllData(void)
{
    return HAL_FLASH_Erase_Page(FLASH_MACHINE_INFO_ADDRESS, 1);
}


/***************************************************************************//**
* @brief
*   返回最大支持的用户数量， DAPI 10.0
*
* @note
*   
******************************************************************************/
char SystemConfig_Get_MaxUserNumber(void)
{
	return(MAX_SUPPORTED_USERS);
}

/***************************************************************************//**
* @brief
*   返回系统配置默认字符串， DAPI 10.0
*
* @note
*  
******************************************************************************/
SystemSettingType SystemConfig_Get_DefaultString(SystemSettingString  index)
{
    return SystemSettingArray[index];
}


/***************************************************************************//**
* @brief
*   返回DAPI协议版本信息， DAPI 10.0
*
* @note
*  
******************************************************************************/
UCHAR SystemConfig_Get_Dapi_Version(void)
{
    return WIFI_DAPI_VERSION;
}

/***************************************************************************//**
* @brief
*   返回仪表软体版本信息
*
* @note
*  
******************************************************************************/
UINT8 SystemConfig_Get_Console_Version(char *pVersion)
{
    
    UINT8 strLength;
    
    strncpy(pVersion, rscu_version[0],sizeof(pVersion) );
    strLength = strlen(rscu_version[0]);
    strncpy((pVersion+strLength),rscu_version[1],sizeof(pVersion+strLength));
    strLength += strlen(rscu_version[1]);

    return strLength;
}
/***************************************************************************//**
* @brief
*   返回库文件版本
*
* @note
*  
******************************************************************************/
UINT8 SystemConfig_Get_Version_Code(UINT8 *buffer)
{
    UINT8 string[8] = SYSTEMCONFIG_VISION_CODE;
    memcpy(buffer,string,sizeof(string));
    return(strlen(SYSTEMCONFIG_VISION_CODE));
}
/***************************************************************************//**
* @brief
*  更新Update Flag位置信息
*
* @note
*  1个字节，0x01, wifi update, 0x81 usb update
*  
******************************************************************************/
UINT8 SystemConfig_Set_UpdateFlag(UINT8 flag)
{
    EFM_ASSERT(flag == USB_UPDATE_AVAILABLE || flag == WIFI_UPDATE_AVAILABLE);
    UINT8 Temp[128] = {0};
    HAL_FLASH_Burst_Read(FLASH_BOOTLOADER_FLAG_ADDRESS, Temp,  128);
    if(HAL_FLASH_Erase_Page(FLASH_BOOTLOADER_FLAG_ADDRESS,1) == false) return 0;
    Temp[UPDATEFLAG_OFFSET] = flag;
    if(HAL_FLASH_Burst_Write(FLASH_BOOTLOADER_FLAG_ADDRESS,Temp,128,1) == false) return 0;
    else 
        return 1;
}


void Default_Data(bool isForcedToDefault)
{
	MACHINE_INFO_UNION machineInfo;
    machineInfo.mfields = SystemConfig_Get_MachineInfo();
	//-------------------------------------------Accumulated Data--------------------------------------------
	FLASH_DATA.fields._accumulatedInfo.accfields.isAccumulatedDataDirty = ITEM_DIRTY_FLAG;
	FLASH_DATA.fields._accumulatedInfo.accfields.accumulatedDistanceMilesX100=0;
	FLASH_DATA.fields._accumulatedInfo.accfields.accumulatedTimeSeconds=0;
	FLASH_DATA.fields._accumulatedInfo.accfields.accumulatedLubeMilesX100=0;
    FLASH_DATA.fields._accumulatedInfo.accfields.accumulatedLubeTime = 0;
	//--------------------------------------Machine Info---------------------------------------------------------
    
    memcpy(&(FLASH_DATA.fields._machineInfo),&machineInfo,sizeof(MACHINE_INFO_UNION));
    if(isForcedToDefault)
    {
        FLASH_DATA.fields._machineInfo.mfields.machineType=Factory_Default[FACTORY_DEFAULT_MACHINE_TYPE];
        FLASH_DATA.fields._machineInfo.mfields.machineModel=Factory_Default[FACTORY_DEFAULT_MACHINE_MODEL];
        FLASH_DATA.fields._machineInfo.mfields.consoletype=Factory_Default[FACTORY_DEFAULT_CONSOLE_TYPE];
        FLASH_DATA.fields._machineInfo.mfields.brandId=Factory_Default[FACTORY_DEFAULT_BRAND_ID];
        FLASH_DATA.fields._machineInfo.mfields.languageId=Factory_Default[FACTORY_LANGUAGE];
        FLASH_DATA.fields._machineInfo.mfields.unit=Factory_Default[FACTORY_UNIT];
        FLASH_DATA.fields._machineInfo.mfields.isEnergySaverModeOn=Factory_Default[FACTORY_DEFAULT_ENERGY_SAVER_ON];
        FLASH_DATA.fields._machineInfo.mfields.IsMachineConfiged = 0;
        FLASH_DATA.fields._machineInfo.mfields.driveType=Factory_Default[FACTORY_DRIVE_TYPE];
        FLASH_DATA.fields._machineInfo.mfields.displayMode=Factory_Default[FACTORY_DEFAULT_DISPLAY_MODE];
        FLASH_DATA.fields._machineInfo.mfields.versionMajor=Factory_Default[FACTORY_DEFAULT_VERSION_MAJOR];
        FLASH_DATA.fields._machineInfo.mfields.versionMinor=Factory_Default[FACTORY_DEFAULT_VERSION_MINOR];
        FLASH_DATA.fields._machineInfo.mfields.versionRevision=Factory_Default[FACTORY_DEFAULT_VERSION_REVISION];
        FLASH_DATA.fields._machineInfo.mfields.isforceUpdate=Factory_Default[FACTORY_DEFAULT_FORCE_UPDATE];
        FLASH_DATA.fields._machineInfo.mfields.isFirmwareAvailable=Factory_Default[FACTORY_DEFAULT_FIRMWARE_AVAILANLE];
        FLASH_DATA.fields._machineInfo.mfields.isDemoModeOn=Factory_Default[FACTORY_DEFAULT_DEMO_MODE_ON];
        FLASH_DATA.fields._machineInfo.mfields.isBuzzerOn=Factory_Default[FACTORY_DEFAULT_BUZZER_ON];
        FLASH_DATA.fields._machineInfo.mfields.wasRTCPreset=Factory_Default[FACTORY_DEFAULT_RTC_PRESET];
        FLASH_DATA.fields._machineInfo.mfields.is24HoursFormat=Factory_Default[FACTORY_DEFAULT_24HOURS_FORMAT];
        FLASH_DATA.fields._machineInfo.mfields.accumulatedAgingHours=Factory_Default[FACTORY_DEFAULT_ACCUMULATED_AGING_HOURS];
        FLASH_DATA.fields._machineInfo.mfields.defaultVolume=Factory_Default[FACTORY_DEFAULT_VOLUME];
        FLASH_DATA.fields._machineInfo.mfields.defaulitBrightness=Factory_Default[FACTORY_DEFAULT_BRIGHTNESS];
        FLASH_DATA.fields._machineInfo.mfields.MaxAge=Factory_Default[FACTORY_DEFAULT_MAX_AGE];
        FLASH_DATA.fields._machineInfo.mfields.defaultAge=Factory_Default[FACTORY_DEFAULT_AGE];
        FLASH_DATA.fields._machineInfo.mfields.MinAge=Factory_Default[FACTORY_DEFAULT_MIN_AGE];
        FLASH_DATA.fields._machineInfo.mfields.defaultWeightLBSX100=Factory_Default[FACTORY_DEFAULT_WEIGHT_STANDARD];
        FLASH_DATA.fields._machineInfo.mfields.defaultWeightKGX100=Factory_Default[FACTORY_DEFAULT_WEIGHT_METRIC];
        FLASH_DATA.fields._machineInfo.mfields.csafeId=Factory_Default[FACTORY_DEFAULT_CSAFE_ID];
        FLASH_DATA.fields._machineInfo.mfields.csafeManufactureId=Factory_Default[FACTORY_DEFAULT_CSAFE_MANU_FACTURE_ID];
        FLASH_DATA.fields._machineInfo.mfields.csafeModeId=Factory_Default[FACTORY_DEFAULT_CSAFE_MODE_ID];
        FLASH_DATA.fields._machineInfo.mfields.clubId=Factory_Default[FACTORY_DEFAULT_CLUB_ID];
        FLASH_DATA.fields._machineInfo.mfields.overspeedswitch=Factory_Default[FACTORY_DEFFAULT_OVERSPEED_SWITCH];
        FLASH_DATA.fields._machineInfo.mfields.overspeedvalue=Factory_Default[FACTORY_DEFFAULT_OVERSPEEDVALUE];
        FLASH_DATA.fields._machineInfo.mfields.isAutoLogin=Factory_Default[FACTORY_DEFAULT_AUTO_LOGIN];
        FLASH_DATA.fields._machineInfo.mfields.isAPModeOn=Factory_Default[FACTORY_DEFAULT_AP_MODE_ON];
        FLASH_DATA.fields._machineInfo.mfields.rfChannel=Factory_Default[FACTORY_DEFAULT_RF_CHANNEL];
        FLASH_DATA.fields._machineInfo.mfields.rfAddress=Factory_Default[FACTORY_DEFAULT_RF_ADDRESS];
        FLASH_DATA.fields._machineInfo.mfields.maxUserWeightLbs=Factory_Default[FACTORY_DEFAULT_MAX_USER_WEIGHT_LBS];
        FLASH_DATA.fields._machineInfo.mfields.minUserWeightLbs=Factory_Default[FACTORY_DEFAULT_MIN_USER_WEIGHT_LBS];
        FLASH_DATA.fields._machineInfo.mfields.maxUserWeightKg=Factory_Default[FACTORY_DEFAULT_MAX_USER_WEIGHT_KG];
        FLASH_DATA.fields._machineInfo.mfields.minUserWeightKg=Factory_Default[FACTORY_DEFAULT_MIN_USER_WEIGHT_KG];
        FLASH_DATA.fields._machineInfo.mfields.maxSpeedMphX10=Factory_Default[FACTORY_DEFAULT_MAX_SPEED_MPHX10];
        FLASH_DATA.fields._machineInfo.mfields.minSpeedMphX10=Factory_Default[FACTORY_DEFAULT_MIN_SPEED_MPHX10];
        FLASH_DATA.fields._machineInfo.mfields.maxSpeedKphX10=Factory_Default[FACTORY_DEFAULT_MAX_SPEED_KPHX10];
        FLASH_DATA.fields._machineInfo.mfields.minSpeedKphX10=Factory_Default[FACTORY_DEFAULT_MIN_SPEED_KPHX10];
        FLASH_DATA.fields._machineInfo.mfields.maxRpmMphX10=Factory_Default[FACTORY_DEFAULT_MAX_RPM_MPHX10];
        FLASH_DATA.fields._machineInfo.mfields.minRpmMphX10=Factory_Default[FACTORY_DEFAULT_MIN_RPM_MPHX10];
        FLASH_DATA.fields._machineInfo.mfields.maxRpmKphX10=Factory_Default[FACTORY_DEFAULT_MAX_RPM_KPHX10];
        FLASH_DATA.fields._machineInfo.mfields.minRpmKphX10=Factory_Default[FACTORY_DEFAULT_MIN_RPM_KPHX10];
        FLASH_DATA.fields._machineInfo.mfields.maxInclineX10=Factory_Default[FACTORY_DEFAULT_MAX_INCLINEX10];
//        FLASH_DATA.fields._machineInfo.mfields.maxInclineX10=120;
        FLASH_DATA.fields._machineInfo.mfields.minInclineX10=Factory_Default[FACTORY_DEFAULT_MIN_INCLINEX10];
        FLASH_DATA.fields._machineInfo.mfields.maxInclineADC=Factory_Default[FACTORY_DEFAULT_MAX_INCLINE_ADC];
        FLASH_DATA.fields._machineInfo.mfields.minInclineADC=Factory_Default[FACTORY_DEFAULT_MIN_INCLINE_ADC];
        FLASH_DATA.fields._machineInfo.mfields.maxResistance=Factory_Default[FACTORY_DEFAULT_MAX_RESISTANCE];
        FLASH_DATA.fields._machineInfo.mfields.minResistance=Factory_Default[FACTORY_DEFAULT_MIN_RESISTANCE];
        FLASH_DATA.fields._machineInfo.mfields.erpOnWaitMinutes=Factory_Default[FACTORY_DEFAULT_ERP_WAITING_TIME];
        FLASH_DATA.fields._machineInfo.mfields.demoOnWaitMinutes = Factory_Default[FACTORY_DEFAULT_DEMO_WAIT_TIME];
        FLASH_DATA.fields._machineInfo.mfields.lastLoggedUser = 0;
        FLASH_DATA.fields._machineInfo.mfields.defaultGender = MALE ;
				
        FLASH_DATA.fields._machineInfo.mfields.targetHR =Factory_Default[FACTORY_DEFAULT_TARGETHR];
        
        FLASH_DATA.fields._machineInfo.mfields.IsShowError = Factory_Default[FACTORY_ERROR_CODE_OFF];
        FLASH_DATA.fields._machineInfo.mfields.controlZoneFlag = Factory_Default[FACTORY_CONTROL_ZONE_ON];
        FLASH_DATA.fields._machineInfo.mfields.secondControlZoneFlag = Factory_Default[FACTORY_SECOND_CONTROL_ZONE_ON];
        FLASH_DATA.fields._machineInfo.mfields.irSensorFlag = Factory_Default[FACTORY_IR_SENSOR_ON];
        FLASH_DATA.fields._machineInfo.mfields.rscuFlag = 0XA5;
        FLASH_DATA.fields._machineInfo.mfields.firstBootFlag = 0xA5;
    }
    SystemSettingType systemsettings = SystemSettingArray[SETTING_DEFAULT_SERIAL_NUMBER];
    memset( FLASH_DATA.fields._machineInfo.mfields.consoleSerialNumber, 0, systemsettings.StringMaxLength );
    strncpy(FLASH_DATA.fields._machineInfo.mfields.consoleSerialNumber , systemsettings.strings ,systemsettings.StringMaxLength);
    systemsettings = SystemSettingArray[SETTING_DEFAULT_FRAME_NUMBER];
    memset( FLASH_DATA.fields._machineInfo.mfields.frameSerialNumber, 0, systemsettings.StringMaxLength );
    strncpy(FLASH_DATA.fields._machineInfo.mfields.frameSerialNumber , systemsettings.strings,systemsettings.StringMaxLength );            
    systemsettings = SystemSettingArray[SETTING_MARIOX_PRODUCTION_ADDRESS];
    memset( FLASH_DATA.fields._machineInfo.mfields.marioBox, 0, systemsettings.StringMaxLength );
    strncpy(FLASH_DATA.fields._machineInfo.mfields.marioBox , systemsettings.strings,systemsettings.StringMaxLength );
    if(isForcedToDefault)
    {
    systemsettings = SystemSettingArray[SETTING_DEFAULT_MACHINE_UUID];
    memset( FLASH_DATA.fields._machineInfo.mfields.machineUUID, 0, systemsettings.StringMaxLength );
    strncpy(FLASH_DATA.fields._machineInfo.mfields.machineUUID , systemsettings.strings,systemsettings.StringMaxLength );
    }
    systemsettings = SystemSettingArray[SETTING_DEFAULT_TIME_ZONE];
    memset( FLASH_DATA.fields._machineInfo.mfields.lastUpdateTimezone, 0, systemsettings.StringMaxLength );
    strncpy(FLASH_DATA.fields._machineInfo.mfields.lastUpdateTimezone , systemsettings.strings,systemsettings.StringMaxLength );
    memset( FLASH_DATA.fields._machineInfo.mfields.timezone, 0, systemsettings.StringMaxLength );
    strncpy(FLASH_DATA.fields._machineInfo.mfields.timezone , systemsettings.strings,systemsettings.StringMaxLength );
    FLASH_DATA.fields._machineInfo.mfields.lastUpdateYear = 2014;
    FLASH_DATA.fields._machineInfo.mfields.lastUpdateMonth = 1;
    FLASH_DATA.fields._machineInfo.mfields.lastUpdateDay = 1;
    FLASH_DATA.fields._machineInfo.mfields.lastUpdateHour = 1;
    FLASH_DATA.fields._machineInfo.mfields.lastUpdateMinute = 1;
    FLASH_DATA.fields._machineInfo.mfields.lastUpdateSecond  =1;
    FLASH_DATA.fields._machineInfo.mfields.consoleType = 1;                            //must be modified
    FLASH_DATA.fields._machineInfo.mfields.maxUserNumber = SystemConfig_Get_MaxUserNumber();
    FLASH_DATA.fields._machineInfo.mfields.headphoneInsertions = 0;
    FLASH_DATA.fields._machineInfo.mfields.headphoneRemovals = 0;
	FLASH_DATA.fields._machineInfo.mfields.rfChannel=Factory_Default[FACTORY_DEFAULT_RF_CHANNEL];
    FLASH_DATA.fields._machineInfo.mfields.rfAddress=Factory_Default[FACTORY_DEFAULT_RF_ADDRESS];

	//--------------------------------------------USERS------------------------------------------------------    
	for(UINT8 i = 0; i < MAX_SUPPORTED_USERS; i++ )
	{
		FLASH_DATA.fields._userInfo[i].ufields.isUserDataDirty = ITEM_DIRTY_FLAG;
		FLASH_DATA.fields._userInfo[i].ufields.userConsoleId = i;
		for( int j = 0; j < 14; j++ )
			FLASH_DATA.fields._userInfo[i].ufields.xid[j] = '0';
		for( int j = 0; j < 4; j++ )
			FLASH_DATA.fields._userInfo[i].ufields.passcode[j] = '0';
		FLASH_DATA.fields._userInfo[i].ufields.weightStandardX100 = Factory_Default[FACTORY_DEFAULT_WEIGHT_STANDARD];
		FLASH_DATA.fields._userInfo[i].ufields.age = Factory_Default[FACTORY_DEFAULT_AGE];
		FLASH_DATA.fields._userInfo[i].ufields.year = 15;
		FLASH_DATA.fields._userInfo[i].ufields.year = FLASH_DATA.fields._userInfo[i].ufields.year + 2000 - FLASH_DATA.fields._userInfo[i].ufields.age;
		FLASH_DATA.fields._userInfo[i].ufields.month = 4;
		FLASH_DATA.fields._userInfo[i].ufields.date = 23;
		FLASH_DATA.fields._userInfo[i].ufields.gender = FLASH_DATA.fields._machineInfo.mfields.defaultGender;
		FLASH_DATA.fields._userInfo[i].ufields.timeStampYear = 2000;
		FLASH_DATA.fields._userInfo[i].ufields.timeStampMonth = 1;
		FLASH_DATA.fields._userInfo[i].ufields.timeStampDate = 1;
		FLASH_DATA.fields._userInfo[i].ufields.timeStampHour = 10;
		FLASH_DATA.fields._userInfo[i].ufields.timeStampMinute = 0;
		FLASH_DATA.fields._userInfo[i].ufields.timeStampSeconds = 0;
		memset( FLASH_DATA.fields._userInfo[i].ufields.name, 0, 65 );
		FLASH_DATA.fields._userInfo[i].ufields.displayUnit = FLASH_DATA.fields._machineInfo.mfields.unit;
		FLASH_DATA.fields._userInfo[i].ufields.isLoggedOn = 0;
		FLASH_DATA.fields._userInfo[i].ufields.isAcceptedTerms = 0;
		FLASH_DATA.fields._userInfo[i].ufields.isShareWorkouts = 0;
		FLASH_DATA.fields._userInfo[i].ufields.isSetup = 0;
	}
	//--------------------------------------Customized Programs-------------------------------------------------
	for(UINT8 i = 0; i < MAX_SUPPORTED_CUSTOM_PRGS; i++)
	{
		FLASH_DATA.fields._customWorkouts[i].ccfields.customWorkoutTime = D_CUSTOM_TIME;
		for(UINT8 j = 0; j < MAX_CUSTOM_SEGMENTS; j++)
		{
			FLASH_DATA.fields._customWorkouts[i].ccfields.customSpeedX10[j] = D_CUSTOM_SPEED;
			FLASH_DATA.fields._customWorkouts[i].ccfields.customInclineX10[j] = D_CUSTOM_INCLINE;
			FLASH_DATA.fields._customWorkouts[i].ccfields.customresistance[j] = D_CUSTOM_RESISTANCE;
		}
	}    
}

UINT32 SystemConfig_Get_FirstBoot_ADDRESS(void)
{
    UINT32 ShiftAdd = &FLASH_DATA.fields._machineInfo.mfields.machinedataEnd - &FLASH_DATA.fields._machineInfo.mfields.firstBootFlag;
    UINT32 TempAdd= FLASH_MACHINE_INFO_ADDRESS + (sizeof(MACHINE_INFO) - ShiftAdd - 1 );
    return(TempAdd);
}
