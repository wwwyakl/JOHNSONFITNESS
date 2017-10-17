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



#define C_MEM_LENGHT 20
#define C_HRPLOAR_OFFSET 5
#define C_HRPLOAR_AVG    8
#define C_HRPOLAR_RENEW_TIME 20 
#define C_HRPOLAR_MIN    40


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

__no_init static UINT16 w_Pulse_Poalr_Count;           
__no_init static UINT16 w_Pulse_HR_Count;
__no_init static UINT16 w_Pulse_Polar_Data;
__no_init static UINT16 w_Pulse_HR_Data;
__no_init static UCHAR aby_Heart[C_MEM_LENGHT];
__no_init static UCHAR by_Heart;
__no_init static UCHAR by_Polar, by_HR;

__no_init static struct 
{
    unsigned Polar_FRIST  : 1;
    unsigned Polar_STOP   : 1;
    unsigned Polar_OK     : 1;
    unsigned Polar_BLINK  : 1;
    unsigned RENEW      : 1;
    unsigned Level:1;
}b;

static void Heart_Save_MEM_FIFO(UCHAR by_Dat)
{
    UCHAR by_Loop;
    for(by_Loop = C_MEM_LENGHT - 1 ; by_Loop > 0; by_Loop--)
    {
        aby_Heart[by_Loop] = aby_Heart[by_Loop - 1];
    }
    aby_Heart[0] = by_Dat;
}

static void Heart_Clear_MEM(void)
{
    UCHAR by_Loop;
    for(by_Loop = 0; by_Loop < C_MEM_LENGHT; by_Loop++)
    {
        aby_Heart[by_Loop] = 0;
    }
}

static UCHAR Heart_Get_Polar(void)
{
    UINT16 w_Dat;
    if(!b.Polar_OK)  return(by_Polar);
    b.Polar_OK = 0;
    if(b.Polar_STOP) return(by_Polar = 0);
    w_Dat = w_Pulse_Polar_Data;
    if(w_Dat < SINGLE_SIGNAL_MAX_PERIOD)
    {
        by_Polar = 61440 / w_Dat;
    }
    return(by_Polar);
}


static UCHAR Heart_Calculate_MEM(UCHAR *pby_Avg)
{
    UCHAR by_Num, by_Loop, by_Count = 0;
    UCHAR by_Max_Count = 0;
    UINT16 w_Max_Total = 0;
    UINT16 w_Total = 0;

    if(aby_Heart[0] == 0 && aby_Heart[1] == 0 && aby_Heart[2] == 0)
    {
        *pby_Avg = 0;
        return(C_MEM_LENGHT);
    }
    for(by_Num = 0; by_Num < C_MEM_LENGHT; by_Num++)
    {
        by_Count = 0;
        w_Total = 0;
        if(aby_Heart[by_Num] < 40) continue;
        for(by_Loop = 0; by_Loop < C_MEM_LENGHT; by_Loop++)
        {
            if( aby_Heart[by_Loop] <= (aby_Heart[by_Num] + C_HRPLOAR_OFFSET)  &&
                    aby_Heart[by_Loop] >= (aby_Heart[by_Num] - C_HRPLOAR_OFFSET)      )
            {
                ++by_Count;
                w_Total += aby_Heart[by_Loop];
            }
        }
        if(by_Count > by_Max_Count)
        {
            by_Max_Count = by_Count;
            w_Max_Total = w_Total;
        }
    }

    *pby_Avg = w_Max_Total / by_Max_Count;
    return(by_Max_Count);
}


static void Midware_Ploar_Interrupt(void)
{
    UCHAR  heartratePinStatus ;

    if(Hal_Heart_Get_Polar_Level())
    {
        if(w_Pulse_Poalr_Count > SINGLE_SIGNAL_MIN_PERIOD)
        {
            w_Pulse_Polar_Data = w_Pulse_Poalr_Count;
            w_Pulse_Poalr_Count = 0;
            if(b.Polar_FRIST)                  
            {
                b.Polar_OK = 1;
                if(by_Heart != 0)
                    b.Polar_BLINK = 1;
                else
                    b.Polar_BLINK = 0;
            }
            b.Polar_FRIST = 1;
            b.Polar_STOP = 0;
            Heart_Save_MEM_FIFO(Heart_Get_Polar());   //7.24
        }
        b.Level = 0;
    }
    else
    {
        b.Level = 1;
    }
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

    //HAL_INT_Regist_Pin_INT(PIN_INT_HR,Midware_Heart_Interrupt);//HAL_PININT_Alloc_ISR(Midware_Heart_Interrupt);

    
    
    #ifdef __PLOAR_HEARTRATE_DETECT__
    //HAL_PININT_Alloc_ISR(Midware_Ploar_Interrupt);
    b.Polar_OK = 0;
    b.Polar_FRIST = 0;
    b.Polar_STOP = 1;
    b.Polar_BLINK = 0;
    b.RENEW = 0;
    by_Polar = by_HR = 0;
    by_Heart = 0;
    Heart_Clear_MEM();
   

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
        static UCHAR by_Timer1 =0;
        static UCHAR by_timer2 = 0;
        static UCHAR by_timer3 = 0;
        by_timer2++;
        if(by_timer2>99)
            {
                by_timer2 = 0;
                by_timer3++;
                if(by_timer3>20)
                {
                    b.RENEW = 1;
                    by_timer3 = 0;
                }
            }
        Midware_Ploar_Interrupt( );
        if(w_Pulse_Poalr_Count > SINGLE_SIGNAL_MAX_PERIOD)
        {
            b.Polar_FRIST = 0;
            b.Polar_STOP = 1;
            b.Polar_OK = 1;
            w_Pulse_Poalr_Count = 0;
            //if(b.HR_STOP == 1)
    			//Heart_Save_MEM_FIFO(0);    //7.24
            Heart_Clear_MEM();    //zsf
        }
        else
    	{
    		++w_Pulse_Poalr_Count;
    	}
        
        if(b.Polar_BLINK)
        {
            if(by_Timer1) 
                --by_Timer1;
            else 
                b.Polar_BLINK = 0;
        }
        else 
            by_Timer1 = 250;

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
    
          UCHAR by_HR;
    UCHAR by_AVG;
	
    by_AVG = Heart_Calculate_MEM(&by_HR);

    
    if(!b.RENEW) return;  // 2 sec reflash
	
    b.RENEW = 0;
	
    if(by_HR == 0)
    {
        by_Heart = 0;
        Heart_Clear_MEM();
        return;
    }
	
	if(!b.Polar_STOP)
	{
	    if(by_AVG > 3/*C_HRPLOAR_AVG*/)
		{
			by_Heart = by_HR;
		}
	}  
    }

#endif
}
UCHAR Midware_Heart_Get_Status(void)
{
    UCHAR ret =  byHeartRateValidFlag;
    #ifdef __PLOAR_HEARTRATE_DETECT__
        ret += b.Polar_BLINK;
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
    if(!b.Polar_STOP && (by_Heart != 0))    //ZSF
        retHeartRate =by_Heart;
    #endif

    return retHeartRate;
}
UCHAR Midware_Heart_Get_Blink(void)
    {
    #ifdef __PLOAR_HEARTRATE_DETECT__
        if(b.Polar_BLINK == 1) 
            return b.Level;
        else if (b.Polar_FRIST == 0)    //zsf
    #endif
        {
             if(byHeartRateValidFlag == 4) 
                 return byHeartratePinLevelStatus>1?1:0;
        }
        
        return 0;
    }

void Midware_Heart_Regist_Functions(void) 
{ 
    HAL_INT_Regist_Pin_INT(PIN_INT_HR,Midware_Heart_Interrupt);    //zsf
}