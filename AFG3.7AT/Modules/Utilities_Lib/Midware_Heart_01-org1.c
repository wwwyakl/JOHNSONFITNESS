#include "Midware_Heart.h"



#define HEART_TIME_BASE             60000           // 60000ms(1 minute)
#define HEART_MAX                   180             // max heart
#define HEART_MIN                   40              // min heart
#define SINGLE_SIGNAL_MIN_PERIOD    (HEART_TIME_BASE / HEART_MAX)             
#define SINGLE_SIGNAL_MAX_PERIOD    (HEART_TIME_BASE / HEART_MIN)            
#define DELAY_DECAY_MIN_TIME        5000 
#define PL_CLEAR_BUF_TIME           1000 
#define BUF_MAX_SAVE                12
#define BUF_MAX_START               3
#define BUF_MAX_START_PL            4

#define __PLOAR_HEARTRATE_DETECT__




#define MIN_HIGH_LEVEL_TIME 50
#define MIN_LOW_LEVEL_TIME  200
#define MAX_SIGNAL_NUMBER 30
#define MAX_VALID_TIME (HEART_TIME_BASE/HEART_MIN)


__no_init static UCHAR  byPresentHeartrate, by_Present_PL;
__no_init static UINT16  signalPeriodArrary[MAX_SIGNAL_NUMBER];
__no_init static UCHAR  bySignalPeriodCounter = 0;
__no_init static UCHAR  bySignalPeroidCounterOverflow = 0;
__no_init static UCHAR  byHeartRatePinLastStatus = 0;
__no_init static UCHAR  byHeartRateValidFlag = 0;
__no_init static UCHAR  byHeartratePinLevelStatus = 0;
__no_init static UINT16 heartrateAliveTime = 0;
__no_init static UINT16 heartrateAutoComeTime = 0;
__no_init static UCHAR overturnCounter = 0;


#ifdef __PLOAR_HEARTRATE_DETECT__

__no_init static UCHAR by_Present_PL, by_PL_Buf[BUF_MAX_SAVE], by_PL_START[BUF_MAX_START];
__no_init static UINT16 w_Signal_Period_PL, w_delayDecayTime_PL; 
__no_init static UINT16  w_add_ms_PL;
__no_init static UCHAR overturnCounterPL = 0;


__no_init static struct 
{
    unsigned Signal_Captured   : 1;
    unsigned Signal_Lock       : 1;
    unsigned BLINK             : 2;
    unsigned High_Level        : 1;
    unsigned High_Moment       : 1;
}PL_STATUS;

static void Heart_Inter_Scan_Signal(void)    
{
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
static UCHAR Heart_Get_AvrValue(UCHAR *address, UCHAR byCnt , UCHAR byCheck)  
{
    UCHAR byStart;
    UCHAR byEnd;
    UCHAR byBuf[30] = {0};
    UCHAR byTemp = 0;
    UCHAR compFlag = 0;
    
	if(byCnt % 4 == 0) 
	{
    byStart = byCnt / 4;
    byEnd = byCnt * 3 / 4;
    }
	else if(byCnt % 3 == 0) 
    {
        byStart = byCnt / 3;
		byEnd = byCnt * 2 / 3;    
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
		    UCHAR err = 30;
		    if(byCnt == 4) err = 15;
			else if(byCnt == 6) err = 25;
            if(byBuf[byCnt - 1] - byBuf[0] >= err)
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





#endif


static void Midware_Heart_Interrupt(void)
{
    UCHAR  heartratePinStatus ;

    if(HAL_Get_IO_IntFlag(HEART_PIN))
    {
        HAL_Clear_IO_IntFlag(HEART_PIN);
        heartratePinStatus = GPIO_PinInGet(HEART_PORT, HEART_PIN);
        if(heartratePinStatus > 0)
        {
            byHeartratePinLevelStatus =1;
        }
        else
        {
            byHeartratePinLevelStatus =2;
        }

        if(heartratePinStatus != byHeartRatePinLastStatus )
        {
            overturnCounter++;
            heartrateAliveTime = 0;
            byHeartRatePinLastStatus = heartratePinStatus;
            if(byHeartRateValidFlag == 0)
            {
                byHeartRateValidFlag = 2;
            }
        }
    }
}


void Midware_Heart_Initial_Data(void)
{
    byPresentHeartrate = 0;
    bySignalPeriodCounter = 0;
    bySignalPeroidCounterOverflow = 0;
    byHeartRatePinLastStatus = 0;
    byHeartRateValidFlag = 0;
    byHeartratePinLevelStatus = 0;
    heartrateAutoComeTime = 0;
    overturnCounter = 0;
    for(int i=0;i<MAX_SIGNAL_NUMBER;i++)
    {
        signalPeriodArrary[i] = 0;
    }

    HAL_INT_Regist_Pin_INT(PIN_INT_HR,Midware_Heart_Interrupt);//HAL_PININT_Alloc_ISR(Midware_Heart_Interrupt);   //zsf
    
    #ifdef __PLOAR_HEARTRATE_DETECT__
    overturnCounterPL = 0;
    by_Present_PL = 0;
    w_Signal_Period_PL = 0;	
    PL_STATUS.Signal_Captured = 0;
    PL_STATUS.Signal_Lock = 0;
    PL_STATUS.BLINK = 0;
    PL_STATUS.High_Level = 0;
    PL_STATUS.High_Moment = 0;
    w_delayDecayTime_PL = 0;    
    w_add_ms_PL = 0;
    #endif
}

void Midware_Heart_1ms_Int(void)
{
    static UCHAR heartrateHighLevelTime=0;
    static UCHAR heartrateLowLevelTime = 0;
    static UCHAR heartrateValidFlag = 0;
    static UINT16 heartrateKeepTime = 0;
    

    heartrateAutoComeTime++;
    heartrateAliveTime ++;
    if(heartrateAliveTime > MAX_VALID_TIME*byHeartRateValidFlag)
    {
        byHeartRateValidFlag = 0;
        heartrateAliveTime = MAX_VALID_TIME*byHeartRateValidFlag;
    }
    heartrateKeepTime++;
    if(byHeartratePinLevelStatus == 1)
    {
    	//frist high level come set flag,counter high level time
        heartrateHighLevelTime++;
        heartrateLowLevelTime = 0;
        if(heartrateHighLevelTime > MIN_HIGH_LEVEL_TIME)
        {
            heartrateValidFlag = 1;
        }

	// next high level come,add keep time to buffer.
        if(heartrateValidFlag == 2)
        {
            heartrateValidFlag = 0;
            if(heartrateKeepTime <MAX_VALID_TIME)
            {
                if((byHeartRateValidFlag == 2)&&(bySignalPeriodCounter == 0))
                {
                    signalPeriodArrary[bySignalPeriodCounter++] =857;
                    bySignalPeriodCounter = bySignalPeriodCounter%MAX_SIGNAL_NUMBER;
                    signalPeriodArrary[bySignalPeriodCounter++] =857;
                    bySignalPeriodCounter = bySignalPeriodCounter%MAX_SIGNAL_NUMBER;
                    signalPeriodArrary[bySignalPeriodCounter++] =857;
                    bySignalPeriodCounter = bySignalPeriodCounter%MAX_SIGNAL_NUMBER;
                    signalPeriodArrary[bySignalPeriodCounter++] =857;
                    bySignalPeriodCounter = bySignalPeriodCounter%MAX_SIGNAL_NUMBER;
                    signalPeriodArrary[bySignalPeriodCounter++] =857;
                    bySignalPeriodCounter = bySignalPeriodCounter%MAX_SIGNAL_NUMBER;
                }
                signalPeriodArrary[bySignalPeriodCounter++] = heartrateKeepTime;
                byHeartRateValidFlag = 4;
                heartrateAutoComeTime = 0;
                if(bySignalPeriodCounter >=  MAX_SIGNAL_NUMBER)
                {
                    bySignalPeriodCounter = 0;
                    bySignalPeroidCounterOverflow = 1;
                }
            }            
            heartrateKeepTime = 0;
        }
    }
    else
    {
        heartrateHighLevelTime = 0;
        heartrateLowLevelTime ++;
        if((heartrateLowLevelTime > MIN_LOW_LEVEL_TIME)&&(heartrateValidFlag == 1))
        {
            heartrateValidFlag = 2;
        }
    }


#ifdef __PLOAR_HEARTRATE_DETECT__
    {
        static UCHAR PL_HighLevelTime = 0;
        static UCHAR PL_LowLevelTime = 0;


        if(w_delayDecayTime_PL > 0) 
        {
            --w_delayDecayTime_PL;
        }
        
        if(w_add_ms_PL < 0xffff) ++w_add_ms_PL;    

        if(GPIO_PinInGet(POLAR_PORT, POLAR_PIN) > 0)     
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
        if(PL_HighLevelTime >= 5) 
        {
            PL_STATUS.High_Level = 1;
            PL_HighLevelTime = 0;
        }
        else if(PL_LowLevelTime >= 150) 
        {
            if((PL_STATUS.High_Level ==  1)&&(overturnCounterPL<10))
            {
                overturnCounterPL++;
            }
            PL_STATUS.High_Level = 0;  
            PL_LowLevelTime = 0;
        }   
    }
#endif
    
}
void Midware_Heart_Process(void)
{
	static UCHAR lastHeartrateValidFlag = 0;

	UINT32 heartrateTimeSum = 0;
	UINT16 averageHeartrateTime = 0;
	UCHAR  i=0;
       UCHAR  tempCounter;


     if((bySignalPeroidCounterOverflow == 1)||(bySignalPeriodCounter>0))
    {
        if(bySignalPeriodCounter>0)
        {
           tempCounter = bySignalPeriodCounter -1;
        }
        else
        {
            tempCounter = MAX_SIGNAL_NUMBER-1;
        }
        
        if((signalPeriodArrary[tempCounter]*2<heartrateAutoComeTime)&&(overturnCounter>3))
        {
            signalPeriodArrary[bySignalPeriodCounter++] = signalPeriodArrary[tempCounter];
            if(bySignalPeriodCounter>= MAX_SIGNAL_NUMBER)
            {
                bySignalPeroidCounterOverflow = 1;
                bySignalPeriodCounter = 0;
            }
            overturnCounter = 0;
            heartrateAutoComeTime = 0;
        }
    }

	//calc normal heart rate
	if(bySignalPeroidCounterOverflow == 1)
	{
		for(i = 0;i<MAX_SIGNAL_NUMBER;i++)
		{
			heartrateTimeSum += signalPeriodArrary[i];
		}
		
	}
	else
	{
		for(i = 0;i<bySignalPeriodCounter;i++)
		{
			heartrateTimeSum += signalPeriodArrary[i];
		}
	}
	if(heartrateTimeSum > 0)
	{
		averageHeartrateTime = heartrateTimeSum/i;
		byPresentHeartrate = HEART_TIME_BASE/averageHeartrateTime ;

		//lost signal  
		if(lastHeartrateValidFlag> byHeartRateValidFlag )
		{
                for(i=0;i<MAX_SIGNAL_NUMBER;i++)
                {
                    signalPeriodArrary[i] = 0;
                }
                bySignalPeroidCounterOverflow = 0;
                bySignalPeriodCounter = 0;
                lastHeartrateValidFlag = byHeartRateValidFlag;
                byHeartratePinLevelStatus = 0;
		}
		else
		{
		    lastHeartrateValidFlag = byHeartRateValidFlag;
		}

	}
	else
	{
		byPresentHeartrate = 0;
	}

    
#ifdef __PLOAR_HEARTRATE_DETECT__
    {
    
        static UCHAR PL_NUM = 0;
        static UCHAR PLsignFail = 0;
        Heart_Inter_Scan_Signal();

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
                        by_Present_PL = Heart_Get_AvrValue(by_PL_Buf, BUF_MAX_SAVE, 0);
                        if(by_Present_PL > 0)
                        {
                            w_delayDecayTime_PL = HEART_TIME_BASE / by_Present_PL + PL_CLEAR_BUF_TIME;
                        }
                    }
                }
                else
                {
                    PL_STATUS.BLINK = 1;
                    if(byTemp > 0) 
                    {
                        w_delayDecayTime_PL = HEART_TIME_BASE / byTemp + PL_CLEAR_BUF_TIME;
                    }
                    by_PL_START[PL_NUM] = byTemp;
                    if(++PL_NUM >= BUF_MAX_START_PL || PLsignFail == 1) 
                    {
                        if(PL_NUM >= BUF_MAX_START_PL) PL_NUM = 0;

                        if(Heart_Get_AvrValue(by_PL_START, BUF_MAX_START_PL, 1) == 0) 
                        {
                            PLsignFail = 1;
                        }
                        else
                        {
                            by_Present_PL = Heart_Get_AvrValue(by_PL_START, BUF_MAX_START_PL, 0);
                            PLsignFail = 0;
                            for(UCHAR i = 0; i < BUF_MAX_SAVE; i++)
                            {
                                by_PL_Buf[i] = by_Present_PL;
                            }
                            PL_NUM = 0;
                            if(by_Present_PL > 0) 
                            {
                                w_delayDecayTime_PL = HEART_TIME_BASE / by_Present_PL + PL_CLEAR_BUF_TIME;
                            }
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
                overturnCounterPL = 0;
                by_Present_PL = 0;
                PL_STATUS.BLINK = 0;
                PL_NUM = 0;
                PLsignFail = 0;
            }     
        }            
    }
#endif
}
UCHAR Midware_Heart_Get_Status(void)
{
    UCHAR ret =  byHeartRateValidFlag;
    #ifdef __PLOAR_HEARTRATE_DETECT__
        ret += PL_STATUS.BLINK; 
    #endif
    
    return (ret>0?1:0);
}
UCHAR Midware_Heart_Get_Heart(void)
{
    UCHAR retHeartRate = 0;
    if(byPresentHeartrate> HEART_MAX)
    {
        retHeartRate = HEART_MAX;
    }
    else
    {
        retHeartRate = byPresentHeartrate;
    }

    #ifdef __PLOAR_HEARTRATE_DETECT__
        if((PL_STATUS.BLINK == 2)&&(overturnCounterPL>4))
        {
            retHeartRate = by_Present_PL;
        }
    #endif

    return retHeartRate;
}
UCHAR Midware_Heart_Get_Blink(void)
    {
    #ifdef __PLOAR_HEARTRATE_DETECT__
        if(PL_STATUS.BLINK == 2) 
            return PL_STATUS.High_Moment;
        else 
    #endif
        {
             if(byHeartRateValidFlag == 4) 
                return (byHeartratePinLevelStatus - 1);
        }
        
        return 0;
    }

