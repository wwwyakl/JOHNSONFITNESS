#include "UtilitiesKeyInputMethod.h"
//#include <wifi_interface.h>

#define Keypad_Reset_Time 5000
static int Keypad_Temp;
static char Keypad_counter;
static unsigned int by_Times5s;
bool Keypad_issetting;
bool Keypad_5SEnable;
bool Keypad_issetting;
bool Keypad_Number_Blank;
bool Keypad_Number_Notconfirmed;

unsigned char number[10] = {0};
//unsigned char  Xid[XIDMAXLENGTH+1] = {0};
unsigned char  PassCode[5] ={0};
unsigned char Xid_counter = 0;
unsigned char PassCode_counter = 0;

/**************************************************************************//**
* @brief
*   ��ʼ������
*
* @note
*   �˺�������Ϊ��ʼ������
*
*****************************************************************************/

void Keypad_Initial_Data(void)
{
	by_Times5s = 0;
	Keypad_Temp = 0;	
	Keypad_counter = 0;
	Keypad_5SEnable = 0;
	Keypad_issetting = 0;
	Keypad_Number_Blank = 0;
	Keypad_Number_Notconfirmed = 0;
}

/**************************************************************************//**
* @brief
*   �жϺ������жϴ򿪺�1msִ��һ�Ρ�
*
* @note
*   �˺����������ڼ�ʱ�����ж�Ϊ1ms�жϡ�
*
*****************************************************************************/
void Keypad_1ms_ISR(void)
{
	if(by_Times5s >= Keypad_Reset_Time)
	{		
		by_Times5s = 0;
		Keypad_Temp = 0;
		Keypad_counter = 0;
		Keypad_issetting = 0;
		if(Keypad_5SEnable)
		{
			Keypad_5SEnable = 0;
			Keypad_Number_Notconfirmed = 1;
		}
	}
	else
	{
		by_Times5s++;
	}
	
}
/**************************************************************************//**
* @brief
*   ��������5S�Ƿ�ENTER�жϺ�����
*
* @note
*   �˺�������Ϊ5S���Ƿ���ENTERȥȷ������İ���ֵ��
*  
* @return
*   ����true or false.
*****************************************************************************/


bool KEYPAD_Get_5S(void)
{
    bool wOut = Keypad_Number_Notconfirmed;
    Keypad_Number_Notconfirmed = 0;
    return wOut;
}

/**************************************************************************//**
* @brief
*   ���ְ������뺯��
*
* @note
*   ���ݲ�ͬ�İ���ֵ���������ܣ����ò�ͬ���㷨�������Ч����ֵ
*
* @param[in] 
*   ����Ϊ����ֵ����ͬ�İ������ܡ�
*****************************************************************************/

void Number_Set_Keypad(char by_Dat,QUICK_KEY_FUNCTION Current_FUNCTION)
{
	unsigned int data_max = 0;
	by_Times5s = 0;
	Keypad_issetting = 1;
	Keypad_5SEnable = 1;
	Keypad_Number_Notconfirmed = 0;
	
	switch(Current_FUNCTION)
	{
		case Setting_In_Set_Speed:
		{
			/* 8< x < 200 */  /* 5< x < 120 */
			Keypad_counter++;
			if(SystemConfig_Get_Display_Unit(User_Get_UserSlot())==METRIC)
			{
				data_max = SystemConfig_Get_Int(MAX_KPHX10_UINT16);
			}
			else
			{
				data_max = SystemConfig_Get_Int(MAX_MPHX10_UINT16);
			}
			if(Keypad_counter == 1)
			{
				Keypad_Temp = (by_Dat-1)*10;
			}
			else if(Keypad_counter == 2)
			{
				Keypad_Temp = Keypad_Temp + by_Dat-1;
			}
			else if(Keypad_counter == 3)
			{
				Keypad_Temp = Keypad_Temp*10 + by_Dat-1;
				if(Keypad_Temp > data_max)
				{
					Keypad_Temp = Keypad_Temp%100;				
				}
			}
			else if(Keypad_counter == 4)
			{
				Keypad_counter = 3;
				Keypad_Temp = Keypad_Temp*10 + by_Dat-1;
				Keypad_Temp = Keypad_Temp%1000;
				if(Keypad_Temp > data_max)
				{
					Keypad_Temp = Keypad_Temp%100;			
				}
			}
			break;
		}
		
		case Setting_In_Set_Resistance:
		{
			//30,1
			data_max = SystemConfig_Get_Char(MAX_RESISTANCE_UINT8);
			Keypad_counter++;
			if(Keypad_counter == 1)
			{
				Keypad_Temp = by_Dat-1;
			}
			else if(Keypad_counter == 2)
			{
				Keypad_counter = 1;
				Keypad_Temp = Keypad_Temp*10 + by_Dat-1;
				Keypad_Temp = Keypad_Temp%100;
				if(Keypad_Temp > data_max)
					Keypad_Temp = Keypad_Temp%10;
			}
			break;
		}
		
		case Setting_In_Set_Incline:
		{
			data_max = SystemConfig_Get_Int(MAX_INCLINEX10_UINT16);
			Keypad_counter++;
			if(Keypad_counter == 1)
			{
				Keypad_Temp = (by_Dat-1)*10;
			}
			else if(Keypad_counter == 2)
			{
				Keypad_Temp = Keypad_Temp + by_Dat-1;
			}
			else if(Keypad_counter == 3)
			{
				Keypad_Temp = Keypad_Temp*10 + by_Dat-1;
			}
			else if(Keypad_counter == 4)
			{
				Keypad_counter = 3;
				Keypad_Temp = Keypad_Temp*10 + by_Dat-1;
				Keypad_Temp = Keypad_Temp%1000;
				if(Keypad_Temp > data_max)
				{
					Keypad_Temp = Keypad_Temp%100;//Keypad_Temp%100;				
				}
			}
			break;
		}
		
		case Setting_In_Set_Time:		
		{
			Keypad_counter++;
			if(Keypad_counter == 1)
			{
				Keypad_Temp = (by_Dat-1)*10;
			}
			else if(Keypad_counter == 2)
			{
				Keypad_Temp = Keypad_Temp + by_Dat-1;
			}	
			else if(Keypad_counter == 3)
			{
				Keypad_counter = 2;
				Keypad_Temp = Keypad_Temp*10 + by_Dat-1;
				Keypad_Temp = Keypad_Temp%100;
			}	
			break;		
		}	
		
		case Setting_In_Set_Weight:
		{
			Keypad_counter++;
			if(Keypad_counter == 1)
			{
				Keypad_Temp = (by_Dat-1)*10;
			}
			else if(Keypad_counter == 2)
			{
				Keypad_Temp = Keypad_Temp + by_Dat-1;
			}
			else if(Keypad_counter == 3)
			{
				Keypad_Temp = Keypad_Temp*10 + by_Dat-1;
			}
			else if(Keypad_counter == 4)
			{
				Keypad_counter = 3;
				Keypad_Temp = Keypad_Temp*10 + by_Dat-1;
				Keypad_Temp = Keypad_Temp%1000;			
			}
			break;
		}
		
		case Setting_In_Set_Age:
		{
			Keypad_counter++;
			if(Keypad_counter == 1)
			{
				Keypad_Temp = (by_Dat-1)*10;
			}
			else if(Keypad_counter == 2)
			{
				Keypad_Temp = Keypad_Temp + by_Dat-1;
			}	
			else if(Keypad_counter == 3)
			{
				Keypad_counter = 2;
				Keypad_Temp = Keypad_Temp*10 + by_Dat-1;
				Keypad_Temp = Keypad_Temp%100;
			}	
			break;
		}
		
		case Setting_In_Set_TimeZone:
		{
			Keypad_counter++;
			if(Keypad_counter == 1)
			{
				Keypad_Temp = by_Dat-1;
			}
			else if(Keypad_counter == 2)
			{
				Keypad_counter = 1;
				Keypad_Temp = Keypad_Temp*10 + by_Dat-1;
				Keypad_Temp = Keypad_Temp%100;			
			}
			break;
		}
		
		case Setting_In_Set_Level:
		{
			Keypad_counter++;
			if(Keypad_counter == 1)
			{
				Keypad_Temp = by_Dat-1;
			}
			else if(Keypad_counter == 2)
			{
				Keypad_counter = 1;
				Keypad_Temp = Keypad_Temp*10 + by_Dat-1;
				Keypad_Temp = Keypad_Temp%100;			
			}
			break;
		}
		
		case Setting_In_Set_Watts:
		{
			Keypad_counter++;
			if(Keypad_counter == 1)
			{
				Keypad_Temp = (by_Dat-1)*10;
			}
			else if(Keypad_counter == 2)
			{
				Keypad_Temp = Keypad_Temp + by_Dat-1;
			}
			else if(Keypad_counter == 3)
			{
				Keypad_Temp = Keypad_Temp*10 + by_Dat-1;
			}
			else if(Keypad_counter == 4)
			{
				Keypad_counter = 3;
				Keypad_Temp = Keypad_Temp*10 + by_Dat-1;
				Keypad_Temp = Keypad_Temp%1000;			
			}
			break;
		}
		
		case Setting_In_Set_Target_HR:
		{
			Keypad_counter++;
			if(Keypad_counter == 1)
			{
				Keypad_Temp = (by_Dat-1)*10;
			}
			else if(Keypad_counter == 2)
			{
				Keypad_Temp = Keypad_Temp + by_Dat-1;
			}
			else if(Keypad_counter == 3)
			{
				Keypad_Temp = Keypad_Temp*10 + by_Dat-1;
			}
			else if(Keypad_counter == 4)
			{
				Keypad_counter = 3;
				Keypad_Temp = Keypad_Temp*10 + by_Dat-1;
				Keypad_Temp = Keypad_Temp%1000;			
			}
			break;
		}
		
//		case Setting_In_Set_XID:
//		{
//			Keypad_Number_Blank = 0;
//            Keypad_5SEnable = 0;
//			number[0] = by_Dat-1;
//			if(Xid_counter < XIDMAXLENGTH )
//			{
//				Xid[Xid_counter++] = number[0]+'0';
//			}
//			else
//			{
//				Xid[XIDMAXLENGTH-1] = number[0]+'0';
//			}
//			break;
//		}
		
//		case Setting_In_Set_Password:
//		{
//			Keypad_Number_Blank = 0;
//            Keypad_5SEnable = 0;
//			number[0] = by_Dat-1;
//			if(PassCode_counter < 4 )
//			{
//				PassCode[PassCode_counter++] = number[0]+'0';
//			}
//			else
//			{
//				PassCode[3] = number[0]+'0';
//			}
//			break;
//		}
		
		default:  
		break;
	}
}
//
//void Keypad_Delete_Number(QUICK_KEY_FUNCTION Current_FUNCTION)
//{
//    Keypad_5SEnable = 0;
//	switch(Current_FUNCTION)
//	{
//		case Setting_In_Set_XID:
//		{
//			Xid[--Xid_counter] = 0;
//            if(Xid_counter == 0)
//            {
//                Keypad_Number_Blank = 1;
//            }
//			if(Xid_counter == 0xFF)
//			{
//				Xid_counter = 0;
//			}
//			break;
//		}
//		case Setting_In_Set_Password:
//		{
//			PassCode[--PassCode_counter] = 0;
//            if(PassCode_counter == 0)
//            {
//                Keypad_Number_Blank = 1;
//            }
//			if(PassCode_counter == 0xFF)
//			{
//				PassCode_counter = 0;
//			}
//			break;
//		}
//		default:  
//		break;
//	}
//}

//void Number_Clear_Xid(void)
//{
//	Xid_counter=0;
//	memset(Xid,0,XIDMAXLENGTH);
//}

//void Number_Clear_PassCode(void)
//{ 
//	PassCode_counter=0;
//	memset(PassCode,0,4);
//}

bool Number_Get_Setting_Flag(void)
{
	return(Keypad_issetting);
}

void Number_Clear_Keypad()
{
	Keypad_Temp = 0;
	Keypad_counter = 0;
	Keypad_issetting = 0;
}

bool Keypad_XP_STATUS(void)
{
    return(Keypad_Number_Blank);
}

/**************************************************************************//**
* @brief
*   ����ֵ���غ���
*
* @return
*   ���ذ���ֵ
*****************************************************************************/


int Number_Get_Keypad(void)
{
	return Keypad_Temp;
}

/**************************************************************************//**
* @brief
*   ע���жϣ�����mainͳһ����
*
* @note
*   ÿ��ģ���ע���жϺ�������ģ������Ӧ���Ա�mainͳһ��ʼ����������©
*****************************************************************************/

void Keypad_Regist_Functions(void)
{
	Hal_Regist_ISR(MS_ISR,Keypad_ISR_IDX,Keypad_1ms_ISR);
}

