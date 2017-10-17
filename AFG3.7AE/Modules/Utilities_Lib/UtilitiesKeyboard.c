#include "UtilitiesKeyboard.h"


void KB_Flag_1ms_Int(void);
char KB_Encoder(UINT32 Value);

#define C_MAX_TIME          20000
#define C_HOLD_TIME_S       60 //0.8s
#define C_TRUE_TIME         1//5//0.1S

#define C_REPLAY_TIME_S     24
#define C_REPLAY_TIME_M     12
#define C_REPLAY_TIME_FAST  10

__no_init static const unsigned char *pTrueTable;
__no_init KEY_COMBO *pKeyComboTable;
__no_init KEY_HOLD *pKeyHoldTable;
__no_init static KEY_DIM keyDim;
__no_init static unsigned char keycombo_column;
__no_init static unsigned char keyhold_column;
__no_init static UINT16 w_KB_Time;
__no_init static UINT16 w_KB_STOP_Time;
__no_init static unsigned char Aold,Bold; 
__no_init static unsigned char Aold1,Bold1; 
__no_init static UINT8 w_KB_Flag;


void KB_Flag_1ms_Int(void)
{
	static UINT8 w_KB_Cnt = 0 ;
	if(++w_KB_Cnt>=10)
	{
		w_KB_Cnt = 0 ;
		w_KB_Flag = 1;      
		if(Hal_Get_KeyValue()!=0)
		{
			if(w_KB_Time < C_MAX_TIME)
			{
				++w_KB_Time;
			}
			else
			{
				w_KB_Time = C_HOLD_TIME_S*4+1;
			}
		}
		else
		{
			w_KB_Time = 0;
		}
	}
}
__no_init static unsigned char Switch_Value; 

void Keyboard_Initial_IO(void)
{	
	HAL_Key_Initial_IO(KEYMODE_I2C);
    Hal_Regist_ISR(MS_ISR,HAL_KeyI2C_1ms,KB_Flag_1ms_Int);
    Hal_Regist_ISR(MS_ISR,KB_Switch_IDX,CodingsWitch_1ms_Int);   
}

/**************************************************************************//**
 * @brief
 *   初始化数据，注册中断
 *
 * @note
 *   此函数功能为初始化数据，以及注册所需要的中断。此中断为1ms中断。
 *****************************************************************************/

void Keyboard_Initial_Data(void)
{
	w_KB_Time = 0;
    w_KB_STOP_Time = 0;
}

/**************************************************************************//**
 * @brief
 *   真值表转换函数。
 *
 * @note
 *   此函数功能为将真值表传递进来。
 *
 * @param[in]freq
 *   输入为真值表的值和真值表的行和列。
 *****************************************************************************/

void KeyBoard_Set_TrueTable(const unsigned char *pTable, UINT8 rows, UINT8 columns)
{
	if(pTable)
	{
		pTrueTable = pTable;
		keyDim.row = rows;
		keyDim.col = columns;
	}
	else 
	{
			
	}//TODO log an error
	
};

/**************************************************************************//**
 * @brief
 *   组合键设置函数。
 *
 * @note
 *   此函数功能为设置具体组合键。
 *
 * @param[in] 
 *   输入为组合键和列。
 *****************************************************************************/

void KeyBoard_Set_KeyComboTable(KEY_COMBO *pComboTable,UINT8 column)
{	
	if(pComboTable)
	{
		pKeyComboTable = pComboTable;
		keycombo_column = column;		
	}
	else 
	{
		
	}//TODO log an error
	

}

/**************************************************************************//**
 * @brief
 *   长按键设置函数。
 *
 * @note
 *   此函数功能为设置具体长按键。
 *
 * @param[in]
 *   输入为长按键和列。
 *****************************************************************************/

void KeyBoard_Set_KeyholdTable(KEY_HOLD *pHoldTable,UINT8 column)
{	
	if(pHoldTable)
	{
		pKeyHoldTable = pHoldTable;
		keyhold_column = column;		
	}
	else 
	{
		
	}//TODO log an error	
}

/**************************************************************************//**
 * @brief
 *   具体按键值返回函数。
 *
 * @note
 *	 此函数功能为判断具体按键时间和按键时间，返回具体按键值。
 *
 * @param[in]
 *   输入为按键方式：I2C还是矩阵.
 *
 * @return
 *   返回具体按键值。
 *****************************************************************************/

char KeyBoard_Get_Code(char mode)
{	
  char by_Dat = KB_NA;
  char by_Out = KB_NA;
  char by_replay = 0;	
  //    bool by_buffer = 0;
  static char by_Replay_Time = 0;
  static char isSpeedUpLongPressed=0;
  static UINT8 w_KB_Short_Flag;
  static UINT8 w_KB_Long_Flag;
  UINT32 Key_Value;
  if(w_KB_Flag != 1)
  {
    return(KB_NONE);
  }
  w_KB_Flag = 0 ;
  
  if(mode)
  {
    Key_Value = HAL_Key_Get_Value(KEYMODE_IO);
  }
  else
  {
    Key_Value = Hal_Get_KeyValue();//HAL_Key_Get_Value(KEYMODE_I2C);
  }	
  by_Dat = KB_Encoder(Key_Value);	
  if(by_Dat == KB_SWITCH_UP || by_Dat == KB_SWITCH_DOWN)
  {
    return(by_Dat);
  }
  if(by_Dat == KB_NONE)//---0---
  {
    w_KB_Short_Flag = 0 ;
    w_KB_Long_Flag = 0 ;
    if(isSpeedUpLongPressed == 1)
    {
      isSpeedUpLongPressed =0;
      by_Out = KB_RELEASED;
      return(by_Out);
    }
    return(by_Dat);//---------------KB_NONE---------------
  }
  if((w_KB_Time >= C_TRUE_TIME || w_KB_STOP_Time >= C_TRUE_TIME)  && (w_KB_Short_Flag==0))
  {
    Beep_Set_Beeps(1,3,1);
    by_Out = by_Dat;
    w_KB_Short_Flag = 1;
  }
  
  if(HAL_Key_I2C_Get_KeyReleaseflag())
  {
    w_KB_Time = 0;
    w_KB_STOP_Time = 0;
    if(by_Dat==KB_SPEED_FAST || by_Dat==KB_QUICK_SPD_FAST)
    {
      isSpeedUpLongPressed =0;
      by_Out = KB_RELEASED;
      return(by_Out);
    }
  }
  
  for( UCHAR i = 0; i < keyhold_column; i++)
  {
    if(by_Dat == (pKeyHoldTable+i)->code1)
    {
      if((w_KB_Time >= ((pKeyHoldTable+i)->holdTime) && (w_KB_Long_Flag==0)))
      {
        by_Out = (pKeyHoldTable+i)->code;
        w_KB_Long_Flag = 1;
        if(by_Out == KB_SPEED_LONGPRESS || by_Dat == KB_QUICKSPEED_LONGPRESS)
        {
          w_KB_Time = C_HOLD_TIME_S;
          isSpeedUpLongPressed = 1;
        }
        else
        {
          Beep_Set_Beeps(1,3,1);
          isSpeedUpLongPressed =0;
        }
      }
    }
  }	
  
  if(w_KB_Time >= C_HOLD_TIME_S)//---0.8S---
  {        
    if(by_Dat ==KB_ARROW_DOWN ||by_Dat ==KB_ARROW_UP ||\
      by_Dat == KB_INCLINE_DOWN ||by_Dat == KB_INCLINE_UP || \
        by_Dat == KB_SPEED_FAST|| by_Dat == KB_SPEED_SLOW ||\
          by_Dat == KB_QUICK_SPD_FAST|| by_Dat == KB_QUICK_SPD_SLOW ||\
            by_Dat == KB_QUICK_INC_UP|| by_Dat == KB_QUICK_INC_DOWN ||\
              by_Dat == KB_RESISTANCE_DOWN || by_Dat == KB_RESISTANCE_UP) 
    {
      by_replay=C_REPLAY_TIME_M;
      if(w_KB_Time >= C_HOLD_TIME_S*4)
      {
        by_replay=C_REPLAY_TIME_FAST;
      }
      if(by_Replay_Time >= by_replay)
      {
        if(isSpeedUpLongPressed == 1)
        {
          by_Out = KB_SPEED_LONGPRESS;
        }
        else
        {
          if(by_Dat == KB_SPEED_FAST)
          {
            //          				isSpeedUpLongPressed = 1;
            by_Out = KB_SPEED_LONGPRESS;
          }
          else if(by_Dat == KB_QUICK_SPD_FAST)
          {
            //          				isSpeedUpLongPressed = 1;
            by_Out = KB_QUICKSPEED_LONGPRESS;
          }
          else				
            by_Out = by_Dat;
        }
        by_Replay_Time = 0;
      }
      else
      {
        ++by_Replay_Time;
      }
    }	
  }
  
  return(by_Out);
}

/**************************************************************************//**
 * @brief
 *   按键译码并判断是否组合键，是否长按函数。
 *
 * @note
 *	 此函数功能为按键译码并返回按键值。
 *
 * @param[in]
 *   输入为32位的按键值.
 *
 * @return
 *   返回具体按键，0xAA为无按键。
 *****************************************************************************/

char KB_Encoder(UINT32 Value)
{	
	UINT32 Key_Value,by_Out = KB_NONE; 
	char by_Code1 = 0xAA;
	char by_Code2 = 0xAA;
	char stop_row = 0;  //WWW
              char stop_column = 0;  //WWW
	UINT16 stop_position;
	Key_Value = Value;
	if(Key_Value == 0xAAAAAAAA && Switch_Value == KB_NONE)
	{
		return KB_NONE;
	}
	if(Switch_Value != KB_NONE)
    {
        by_Out = Switch_Value;
        Switch_Value = KB_NONE;
		return by_Out;
    }
	char Key_Value_Row1 = (char)(Key_Value >> 24);
	char Key_Value_Column1 = (char)((Key_Value & 0x00FF0000)>> 16);
	char Key_Value_Row2 = (char)((Key_Value & 0x0000FF00) >> 8);
	char Key_Value_Column2 = (char)((Key_Value) & 0x000000FF);
	if(Key_Value_Row1 <= keyDim.row && Key_Value_Column1 <=  keyDim.col)
	{
		by_Code1 = pTrueTable[Key_Value_Row1 * keyDim.col + Key_Value_Column1];
	}
	if(Key_Value_Row2 <= keyDim.row && Key_Value_Column2 <=  keyDim.col)
	{
		by_Code2 = pTrueTable[Key_Value_Row2 * keyDim.col + Key_Value_Column2];
	}		
	for(char i = 0;i<64;i++)
	{
		if(pTrueTable[i] == KB_STOP)
		{
			stop_row = (i)/8;
			stop_column = (i)%8;
			break;
		}
	}
    //(stop_row) << 8 + stop_column
	stop_position = (stop_row) << 8;
    stop_position = stop_position + stop_column;
	HAL_Set_Stop_position(stop_position);
	/**************key combo*******************/
	if(0xAA != by_Code1 && 0xAA != by_Code2)// 两个键按下  
	{
		for( UCHAR i = 0; i < keycombo_column; i++)	
		{
			if(by_Code1 + by_Code2 == ((pKeyComboTable+i)->code1 + (pKeyComboTable+i)->code2))
				return (pKeyComboTable+i)->code;
		}
		//TODO trace an error
	}
	else if(0xAA != by_Code2)
	{
		return by_Code2;		
	}
	else if(0xAA != by_Code1)
	{
		return by_Code1;		
	}
	else
	{
		w_KB_Time = 0;
        w_KB_STOP_Time = 0;
	}
	return 0xAA;
}

void CodingsWitch_1ms_Int(void)
{
	static unsigned char st=0;
    
	if((HAL_SwitchA_Pin_Input_State()&&HAL_SwitchB_Pin_Input_State())||(HAL_SwitchA_Pin_Input_State()==0&&HAL_SwitchB_Pin_Input_State()==0))
		st = 1;
        
	if(st)
	{
		if(HAL_SwitchA_Pin_Input_State()==0&&HAL_SwitchB_Pin_Input_State()==0) //if all the pins are low level
		{
			if(Bold && !Aold) //if B is high,mean forward
			{
				st = 0;
				Switch_Value = KB_SWITCH_UP;
			}
			else if(Aold && !Bold) //if A is high,mean backforward
            {
                st = 0;
                Switch_Value = KB_SWITCH_DOWN;
            }
		}
		else if(HAL_SwitchA_Pin_Input_State()&&HAL_SwitchB_Pin_Input_State())
		{
			if(!Bold1 && Aold1) //if B is low,mean forward
			{
				st = 0;
				Switch_Value = KB_SWITCH_UP;
			}
			else if(!Aold1 && Bold1) //if A is low,mean backforward
            {
                st = 0;
                Switch_Value = KB_SWITCH_DOWN;
            }
		}
	}
	Aold = Aold1= HAL_SwitchA_Pin_Input_State(); 
	Bold = Bold1= HAL_SwitchB_Pin_Input_State();
}

unsigned char KB_Get_Switch_Value(void)
{
    return Switch_Value;
}