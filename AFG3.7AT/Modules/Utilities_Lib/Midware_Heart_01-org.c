#include "Midware_Heart.h"

#define HEART_TIME_BASE             60000           // 60000ms(1 minute)
#define HEART_MAX                   180             // max heart
#define HEART_MIN                   40              // min heart
#define SINGLE_SIGNAL_MIN_PERIOD    (HEART_TIME_BASE / HEART_MAX)             
#define SINGLE_SIGNAL_MAX_PERIOD    (HEART_TIME_BASE / HEART_MIN)            
#define DELAY_DECAY_MIN_TIME        5000            // 3000ms
#define BUF_MAX_SAVE                12
#define BUF_MAX_START               3

__no_init static UCHAR by_Present_HR, by_HR_Buf[BUF_MAX_SAVE], by_HR_START[BUF_MAX_START];
__no_init static UINT16 w_Signal_Period, w_delayDecayTime; 
__no_init static UINT16 wFilterTime, w_add_ms;

__no_init static UCHAR by_Present_PL, by_PL_Buf[BUF_MAX_SAVE], by_PL_START[BUF_MAX_START];
__no_init static UINT16 w_Signal_Period_PL, w_delayDecayTime_PL; 
__no_init static UINT16 wFilterTime_PL, w_add_ms_PL;

__no_init static struct 
{
    unsigned Signal_Captured   : 1;
    unsigned Signal_Lock       : 1;
    unsigned BLINK             : 2;
    unsigned High_Level        : 1;
    unsigned High_Moment       : 1;
}HR_STATUS;

__no_init static struct 
{
    unsigned Signal_Captured   : 1;
    unsigned Signal_Lock       : 1;
    unsigned BLINK             : 2;
    unsigned High_Level        : 1;
    unsigned High_Moment       : 1;
}PL_STATUS;

static UCHAR Midware_Heart_Get_AvrValue(UCHAR *address, UCHAR byCnt , UCHAR byCheck)  
{
    UCHAR byStart;
    UCHAR byEnd;
    UCHAR byBuf[30] = {0};
    UCHAR byTemp = 0;
    UCHAR compFlag = 0;
    
    byStart = byCnt / 4;
    byEnd = byCnt * 3 / 4;
    
    if(byCnt == 3) 
    {
        byStart = 1; 
        byEnd = 2;
    }
    
    if(byCnt > 30) return 0;
    for(UCHAR i = 0; i < byCnt; i++)
        byBuf[i] = address[i];
    
    if(byCnt > 0)
    {
        for(int i = 0; i < (byCnt - 1); i++) 
        {
            for(int j = 0; j < (byCnt - 1 - i); j++)
            {
                if(byBuf[j] > byBuf[j + 1]) 
                {
                    byTemp = byBuf[j];
                    byBuf[j] = byBuf[j + 1];
                    byBuf[j + 1] = byTemp;
                    compFlag = 1;
                }
            }
            if(compFlag == 0)
            {
                break;
            }
            compFlag = 0;
        }
        
        if(byCheck > 0)
        {
            if(byBuf[byCnt - 1] - byBuf[0] >= 20)
                return 0; 
            return 1 ;
        }
        else
        {
            UINT16 wSum = 0;
            for(UCHAR i = byStart; i < byEnd; i++) 
                wSum += byBuf[i];
            return wSum / (byCnt / 2);
        }
    }
	return 0;
}

static void Midware_Heart_Scan_Signal(void)    
{
    if(HR_STATUS.Signal_Lock && HR_STATUS.High_Level > 0)
        HR_STATUS.Signal_Lock = 0;
    else if(HR_STATUS.Signal_Lock == 0 && HR_STATUS.High_Level == 0)
    {
        w_Signal_Period = w_add_ms;
        w_add_ms = 0;
        HR_STATUS.Signal_Captured = 1;  
        HR_STATUS.Signal_Lock = 1;
    }
    
    if(PL_STATUS.Signal_Lock && PL_STATUS.High_Level > 0)
        PL_STATUS.Signal_Lock = 0;
    else if(PL_STATUS.Signal_Lock == 0 && PL_STATUS.High_Level == 0)
    {
        w_Signal_Period_PL = w_add_ms_PL;
        w_add_ms_PL = 0;
        PL_STATUS.Signal_Captured = 1;  
        PL_STATUS.Signal_Lock = 1;
    }
}

void Midware_Heart_Initial_Data(void)
{
    by_Present_HR = 0;
    w_Signal_Period = 0;	
    HR_STATUS.Signal_Captured = 0;
    HR_STATUS.Signal_Lock = 0;
    HR_STATUS.BLINK = 0;
    HR_STATUS.High_Level = 0;
    HR_STATUS.High_Moment = 0;
    w_delayDecayTime = 0;
    wFilterTime = 0;
    w_add_ms = 0;
    
    by_Present_PL = 0;
    w_Signal_Period_PL = 0;	
    PL_STATUS.Signal_Captured = 0;
    PL_STATUS.Signal_Lock = 0;
    PL_STATUS.BLINK = 0;
    PL_STATUS.High_Level = 0;
    PL_STATUS.High_Moment = 0;
    w_delayDecayTime_PL = 0;
    wFilterTime_PL = 0;
    w_add_ms_PL = 0;
}

void Midware_Heart_1ms_Int(void)    
{   
    static UCHAR HR_HighLevelTime = 0;
    static UCHAR HR_LowLevelTime = 0;
    static UCHAR PL_HighLevelTime = 0;
    static UCHAR PL_LowLevelTime = 0;
    
    if(w_delayDecayTime > 0) 
    {
        --w_delayDecayTime;
    }
    if(wFilterTime < 0xffff) ++wFilterTime;
    if(w_add_ms < 0xffff) ++w_add_ms;
    
    if(w_delayDecayTime_PL > 0) 
    {
        --w_delayDecayTime_PL;
    }
    if(wFilterTime_PL < 0xffff) ++wFilterTime_PL;
    if(w_add_ms_PL < 0xffff) ++w_add_ms_PL;    
    
    
    if(Hal_Heart_Get_Heart_Level() > 0)      
    {
        ++HR_HighLevelTime;
        HR_LowLevelTime = 0;
        HR_STATUS.High_Moment = 1;
    }
    else
    {
        ++HR_LowLevelTime;
        HR_HighLevelTime = 0;
        HR_STATUS.High_Moment = 0;
    }
    
    if(Hal_Heart_Get_Polar_Level() > 0)     
    {
        ++PL_HighLevelTime;
        PL_LowLevelTime = 0;
        PL_STATUS.High_Moment = 1;
    }
    else
    {
        ++PL_LowLevelTime;
        PL_HighLevelTime = 0;
        if(PL_LowLevelTime>100)
        PL_STATUS.High_Moment = 0;
    }
    
    if(HR_HighLevelTime >= 50) 
    {
        HR_STATUS.High_Level = 1;
        HR_HighLevelTime = 0;
    }
    else if(HR_LowLevelTime >= 150)
    {
        HR_STATUS.High_Level = 0;
        HR_LowLevelTime = 0;
    }
    
    if(PL_HighLevelTime >= 5) 
    {
        PL_STATUS.High_Level = 1;
        PL_HighLevelTime = 0;
    }
    else if(PL_LowLevelTime >= 150) 
    {
        PL_STATUS.High_Level = 0;  
        PL_LowLevelTime = 0;
    }      
}

void Midware_Heart_Process(void)
{
    static UCHAR HR_NUM = 0;
    static UCHAR HRsignFail = 0;
    
    static UCHAR PL_NUM = 0;
    static UCHAR PLsignFail = 0;
    
    Midware_Heart_Scan_Signal();
    if(HR_STATUS.Signal_Captured)
    {
        HR_STATUS.Signal_Captured =  0;
        if(w_Signal_Period <= SINGLE_SIGNAL_MAX_PERIOD && w_Signal_Period >= SINGLE_SIGNAL_MIN_PERIOD)
        {
            UCHAR byTemp = HEART_TIME_BASE / w_Signal_Period;
            if(HR_STATUS.BLINK == 2)
            {
                //if(byTemp < 60 + by_Present_HR && byTemp + 60 > by_Present_HR)
                {
                    by_HR_Buf[HR_NUM] = byTemp;
                    if(++HR_NUM >= BUF_MAX_SAVE) 
                    {
                        HR_NUM = 0;
                    }
                    by_Present_HR = Midware_Heart_Get_AvrValue(by_HR_Buf, BUF_MAX_SAVE, 0);
                    if(by_Present_HR > 0) w_delayDecayTime = HEART_TIME_BASE / by_Present_HR + DELAY_DECAY_MIN_TIME;
                }
            }
            else
            {
                HR_STATUS.BLINK = 1;
                if(byTemp > 0) w_delayDecayTime = HEART_TIME_BASE / byTemp + DELAY_DECAY_MIN_TIME;
                by_HR_START[HR_NUM] = byTemp;
                if(++HR_NUM >= BUF_MAX_START || HRsignFail == 1) 
                {
                    if(HR_NUM >= BUF_MAX_START) HR_NUM = 0;
                    if(Midware_Heart_Get_AvrValue(by_HR_START, BUF_MAX_START, 1) == 0) 
                    {
                        HRsignFail = 1;
                    }
                    else
                    {
                        HRsignFail = 0;
                        by_Present_HR = Midware_Heart_Get_AvrValue(by_HR_START, BUF_MAX_START, 0);
                        if(by_Present_HR > 140)  
                        {
                            by_Present_HR = by_Present_HR - 50;
                        }  
                        for(UCHAR i = 0; i < BUF_MAX_SAVE; i++)
                        {
                            by_HR_Buf[i] = by_Present_HR;
                        }
                        HR_NUM = 0;
                        if(by_Present_HR > 0) w_delayDecayTime = HEART_TIME_BASE / by_Present_HR + DELAY_DECAY_MIN_TIME;
                        HR_STATUS.BLINK = 2;
                    }
                }
            }
        }
    }
    else 
    {
        if(w_delayDecayTime == 0 && HR_STATUS.BLINK > 0) 
        {
            by_Present_HR = 0;
            HR_STATUS.BLINK = 0;
            HR_NUM = 0;
        }  
    }  
    
    if(PL_STATUS.Signal_Captured)
    {
        PL_STATUS.Signal_Captured =  0;
        if(w_Signal_Period_PL <= SINGLE_SIGNAL_MAX_PERIOD && w_Signal_Period_PL >= SINGLE_SIGNAL_MIN_PERIOD)
        {
            UCHAR byTemp = HEART_TIME_BASE / w_Signal_Period_PL;
            if(PL_STATUS.BLINK == 2)
            {
                //if(byTemp < 60 + by_Present_PL && byTemp + 60 > by_Present_PL)
                {
                    by_PL_Buf[PL_NUM] = byTemp;
                    if(++PL_NUM >= BUF_MAX_SAVE) 
                    {
                        PL_NUM = 0;
                    }
                    by_Present_PL = Midware_Heart_Get_AvrValue(by_PL_Buf, BUF_MAX_SAVE, 0);
                    if(by_Present_PL > 0) w_delayDecayTime_PL = HEART_TIME_BASE / by_Present_PL + DELAY_DECAY_MIN_TIME;
                }
            }
            else
            {
                PL_STATUS.BLINK = 1;
                if(byTemp > 0) w_delayDecayTime_PL = HEART_TIME_BASE / byTemp + DELAY_DECAY_MIN_TIME;
                by_PL_START[PL_NUM] = byTemp;
                if(++PL_NUM >= BUF_MAX_START || PLsignFail == 1) 
                {
                    if(PL_NUM >= BUF_MAX_START) PL_NUM = 0;
                    if(Midware_Heart_Get_AvrValue(by_PL_START, BUF_MAX_START, 1) == 0) 
                    {
                        PLsignFail = 1;
                    }
                    else
                    {
                        by_Present_PL = Midware_Heart_Get_AvrValue(by_PL_START, BUF_MAX_START, 0);
                        PLsignFail = 0;
                        for(UCHAR i = 0; i < BUF_MAX_SAVE; i++)
                            by_PL_Buf[i] = by_Present_PL;
                        PL_NUM = 0;
                        if(by_Present_PL > 0) w_delayDecayTime_PL = HEART_TIME_BASE / by_Present_PL + DELAY_DECAY_MIN_TIME;
                        PL_STATUS.BLINK = 2;
                    }
                }
            }
        }
    }
    else 
    {
        if(w_delayDecayTime_PL == 0 && PL_STATUS.BLINK > 0) 
        {
            by_Present_PL = 0;
            PL_STATUS.BLINK = 0;
            PL_NUM = 0;
        }     
    }           
}

UCHAR Midware_Heart_Get_Status(void)
{
    return (HR_STATUS.BLINK | PL_STATUS.BLINK);
}

UCHAR Midware_Heart_Get_Blink(void)
{
    if(PL_STATUS.BLINK == 2) 
        return PL_STATUS.High_Moment;
    else if(HR_STATUS.BLINK == 2) 
        return HR_STATUS.High_Moment;
    
    return 0;
}

UCHAR Midware_Heart_Get_Heart(void) 
{   
    if(PL_STATUS.BLINK == 2)  return by_Present_PL;
    else if(HR_STATUS.BLINK == 2) return by_Present_HR;
    return 0;
}

