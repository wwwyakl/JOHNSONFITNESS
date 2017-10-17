#include "UtilitiesRtc.h"

static inline UINT8 DecimalToBCD(UINT8 by_Data);
static inline UINT8 BCDToDecimal(UINT8 by_Data);
void RTC_Send_Byte(BYTE data);
BYTE RTC_Read_Byte(void);
void RTC_Send_Data(char by_command,PBYTE pby_Data,UINT length);
void RTC_Read_Data(char by_command,PBYTE pby_Data,UINT length);

#define BURST_READ       0XBF
#define BURST_WRITE      0XBE
#define NOP_COMMAND      0X00
#define PROTECT_COMMAND  0X8E
#define PROTECT_OPEN     0X00
#define PROTECT_CLOSE    0X80
#define XT_DISABLE       0X80
#define MODE_12          0X80
//#define NULL  0

/**************************************************************************//**
 * @brief
 *   转码函数。
 *
 * @note
 *   BCD和10进制之间转码。
 *
 *****************************************************************************/

static inline UINT8 DecimalToBCD(UINT8 by_Data)
{
  return (((by_Data / 10) << 4) + (by_Data % 10));
}

static inline UINT8 BCDToDecimal(UINT8 by_Data)
{
  return((by_Data >> 4)*10 + (by_Data % 16));
}
/***********************************************/


void RTC_Send_Byte(BYTE data)
{
  for(int i=0; i < 8; i++)
  {
	HAL_RTC_Set_CLK(0);	
    if(data & 0x01)
    {
      HAL_RTC_Set_DAT(1);
    }
    else
    {
      HAL_RTC_Set_DAT(0);
    }
    data = data >> 1;
    HAL_RTC_Set_CLK(1);
    HAL_RTC_Set_CLK(1);
  }
}

BYTE RTC_Read_Byte(void)
{
  BYTE retval; 
  for(int i=0; i < 8; i++)
  {
    retval = retval >> 1;
    HAL_RTC_Set_CLK(0);
    HAL_RTC_Set_CLK(1);
    
    if(HAL_RTC_Get_Data())
    {
      retval |= 0x80;
    }
    else
    {
      retval &= ~0x80;
    }
    HAL_RTC_Set_CLK(0);
  }
  return retval;
}

void RTC_Send_Data(char by_command,PBYTE pby_Data,UINT length)
{
  HAL_RTC_Set_RST(0);
  HAL_RTC_Set_CLK(0);
  HAL_RTC_Set_RST(1);
  RTC_Send_Byte(by_command);                    //write command;
  for(int i = 0; i < length; i++)               //write data;
  {
    RTC_Send_Byte(*(pby_Data+i));
  }
  HAL_RTC_Set_RST(0);
}

void RTC_Read_Data(char by_command,PBYTE pby_Data,UINT length)
{
  HAL_RTC_Set_RST(0);
  HAL_RTC_Set_CLK(0);
  HAL_RTC_Set_RST(1);
  RTC_Send_Byte(by_command);                    //write command;
  for(int i = 0; i < length; i++)               //Read data;
  {
    *(pby_Data+i) = RTC_Read_Byte();
  }
  HAL_RTC_Set_RST(0);
}


INT8 RTC_Write_Time(DateTime_T *ptTime)
{
  BYTE buffer[7];
  if(ptTime == NULL) return RTCADDNULL;
  buffer[0] = PROTECT_OPEN;
  RTC_Send_Data(PROTECT_COMMAND,buffer,1);
  buffer[0] = DecimalToBCD(ptTime->seconds);
  buffer[1] = DecimalToBCD(ptTime->minutes);
  buffer[2] = DecimalToBCD(ptTime->hours);
  buffer[3] = DecimalToBCD(ptTime->date);
  buffer[4] = DecimalToBCD(ptTime->month);
  buffer[5] = DecimalToBCD(ptTime->day);
  buffer[6] = DecimalToBCD(ptTime->year);
  RTC_Send_Data(BURST_WRITE,buffer,sizeof(buffer));
  buffer[0] = PROTECT_CLOSE;
  RTC_Send_Data(PROTECT_COMMAND,buffer,1);
  return RTCSUCCESSFULL;
}

INT8 RTC_Read_Time(DateTime_T *ptTime)
{
  if(ptTime == NULL)  return RTCADDNULL;
  BYTE buffer[7];
  buffer[0] = PROTECT_OPEN;
  RTC_Send_Data(PROTECT_COMMAND,buffer,1);
  RTC_Read_Data(BURST_READ,buffer,sizeof(buffer));
  ptTime->seconds = BCDToDecimal(buffer[0]);
  ptTime->minutes = BCDToDecimal(buffer[1]);
  ptTime->hours = BCDToDecimal(buffer[2]);
  ptTime->date = BCDToDecimal(buffer[3]);
  ptTime->month = BCDToDecimal(buffer[4]);
  ptTime->day = BCDToDecimal(buffer[5]);
  ptTime->year = BCDToDecimal(buffer[6]);
  buffer[0] = PROTECT_CLOSE;
  RTC_Send_Data(PROTECT_COMMAND,buffer,1);
  return RTCSUCCESSFULL;
}

UINT8 Get_Age( UINT16 year, UINT8 month, UINT8 day )
{
  DateTime_T today;
  if(RTC_Read_Time( &today ) == RTCADDNULL)
    return RTCADDNULL;
  
  UINT8 age = (UINT8)((UINT16)today.year + 2000 - year);
  
  //IF SOMEHOW RTC READ TIME IS WRONG, THEN JUST USE THE YEAR
  //VALUE TO CALCULATE THE AGE.
  if(today.day == 0 && today.month == 0)
    return age;
  
  if( month > today.month ||
     ( month == today.month && day > today.day ))
    age--;
  //        age = 30;//debug by ckm rtc error
  return age;
}