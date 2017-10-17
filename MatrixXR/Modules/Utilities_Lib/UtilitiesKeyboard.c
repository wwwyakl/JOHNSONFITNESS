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


void Keyboard_Initial_IO(void)
{	
	HAL_Key_Initial_IO(KEYMODE_I2C);
	HAL_SystemTicker_Alloc_ISR(KB_Flag_1ms_Int);
}
/**************************************************************************//**
 * @brief
 *   ��ʼ�����ݣ�ע���ж�
 *
 * @note
 *   �˺�������Ϊ��ʼ�����ݣ��Լ�ע������Ҫ���жϡ����ж�Ϊ1ms�жϡ�
 *****************************************************************************/

void Keyboard_Initial_Data(void)
{
	w_KB_Time = 0;
    w_KB_Flag = 0;
}

/**************************************************************************//**
 * @brief
 *   ��ֵ��ת��������
 *
 * @note
 *   �˺�������Ϊ����ֵ���ݽ�����
 *
 * @param[in]freq
 *   ����Ϊ��ֵ���ֵ����ֵ����к��С�
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
 *   ��ϼ����ú�����
 *
 * @note
 *   �˺�������Ϊ���þ�����ϼ���
 *
 * @param[in] 
 *   ����Ϊ��ϼ����С�
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
 *   ���������ú�����
 *
 * @note
 *   �˺�������Ϊ���þ��峤������
 *
 * @param[in]
 *   ����Ϊ���������С�
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
 *   ���尴��ֵ���غ�����
 *
 * @note
 *	 �˺�������Ϊ�жϾ��尴��ʱ��Ͱ���ʱ�䣬���ؾ��尴��ֵ��
 *
 * @param[in]
 *   ����Ϊ������ʽ��I2C���Ǿ���.
 *
 * @return
 *   ���ؾ��尴��ֵ��
 *****************************************************************************/
char KeyBoard_Get_Code(char mode)
{	
	char by_Dat = KB_NA;
	char by_Out = KB_NA;
	char by_replay = 0;	
	static char by_Replay_Time = 0;
    static char isSpeedUpLongPressed=0;
    static char isResistanceUpLongPressed=0;
		
 	static UINT8 w_KB_Short_Flag;
 	static UINT8 w_KB_Long_Flag;
	UINT32 Key_Value;
	//static UINT32 last_keydata ;
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
	/*if(last_keydata != key[current])
	{
		Keyboard_Initial_Data();
	}*/
	//last_keydata = key[current];
	
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
        if(isResistanceUpLongPressed == 1)
        {
            isResistanceUpLongPressed =0;
            by_Out = KB_RELEASED;
            return(by_Out);
		}
		return(by_Dat);//---------------KB_NONE---------------
	}
	
	if((w_KB_Time >= C_TRUE_TIME) && (w_KB_Short_Flag==0))
	{
		Beep_Set_Beeps(1,3,1);
		by_Out = by_Dat;
        w_KB_Short_Flag = 1;
	}

	if(HAL_Key_I2C_Get_KeyReleaseflag())
	{
		w_KB_Time = 0;
        if(by_Dat==KB_SPEED_FAST)
        {
            isSpeedUpLongPressed =0;
            by_Out = KB_RELEASED;
            return(by_Out);
        }
        if(by_Dat==KB_RESISTANCE_UP)
        {
            isResistanceUpLongPressed =0;
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
                if(by_Out == KB_SPEED_LONGPRESS)
                {
                    w_KB_Time = C_HOLD_TIME_S;
                    isSpeedUpLongPressed = 1;
                }
                else
                {
                    Beep_Set_Beeps(1,3,1);
                    isSpeedUpLongPressed =0;
                }
                if(by_Out == KB_RESISTANCE_LONGPRESS)
                {
                    w_KB_Time = C_HOLD_TIME_S;
                    isResistanceUpLongPressed = 1;
                }
                else
                {
                    Beep_Set_Beeps(1,3,1);
                    isResistanceUpLongPressed =0;
                }
			}
		}
	}	
    	if(w_KB_Time >= C_HOLD_TIME_S)//---0.8S---
	{        
		if(by_Dat ==KB_ARROW_DOWN ||by_Dat ==KB_ARROW_UP ||\
			by_Dat == KB_INCLINE_DOWN ||by_Dat == KB_INCLINE_UP || \
				by_Dat == KB_SPEED_FAST|| by_Dat == KB_SPEED_SLOW||\
                 by_Dat == KB_RESISTANCE_UP || by_Dat == KB_RESISTANCE_DOWN )
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
          				isSpeedUpLongPressed = 1;
          				by_Out = KB_SPEED_LONGPRESS;
          			}
					else				
		      		 by_Out = by_Dat;
              	}
//                if(isResistanceUpLongPressed == 1)
//                {
//                    by_Out = KB_RESISTANCE_LONGPRESS;
//                }
//                else
//              	{
//              		if(by_Dat == KB_RESISTANCE_UP)
//          			{
//          				isResistanceUpLongPressed = 1;
//          				by_Out = KB_RESISTANCE_LONGPRESS;
//          			}
//					else				
//		      		 by_Out = by_Dat;
//              	}
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
 *   �������벢�ж��Ƿ���ϼ����Ƿ񳤰�������
 *
 * @note
 *	 �˺�������Ϊ�������벢���ذ���ֵ��
 *
 * @param[in]
 *   ����Ϊ32λ�İ���ֵ.
 *
 * @return
 *   ���ؾ��尴����0xAAΪ�ް�����
 *****************************************************************************/

char KB_Encoder(UINT32 Value)
{	
	UINT32 Key_Value; 
	char by_Code1 = 0xAA;
	char by_Code2 = 0xAA;
	char stop_row,stop_column;
	UINT16 stop_position;
	Key_Value = Value;
	if(Key_Value == 0xAAAAAAAA)
	{
		return KB_NONE;
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
			stop_row = (i + 1)/8;
			stop_column = (i + 1)&8;
			break;
		}
	}
	stop_position = (stop_row) << 8 + stop_column;
	HAL_Set_Stop_position(stop_position);
	/**************key combo*******************/
	if(0xAA != by_Code1 && 0xAA != by_Code2)// ����������  
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
	}
	return 0xAA;
}
