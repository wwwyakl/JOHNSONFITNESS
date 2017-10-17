//----------------------------------------------------------------------------
//  Description:  This file contains functions that allow the MSP430 device to
//  access the SPI interface of the CC1100/CC2500.  There are multiple
//  instances of each function; the one to be compiled is selected by the
//  system variable TI_CC_RF_SER_INTF, defined in "TI_CC_hardware_board.h".
//
//  MSP430/CC1100-2500 Interface Code Library v1.0
//
//  K. Quiring
//  Texas Instruments, Inc.
//  July 2006
//  IAR Embedded Workbench v6.30
//----------------------------------------------------------------------------
#include "HAL_Lib.h"
#include "CC_SPI.h"
#include "CC1100_CC2500_Definition.h"

static void CC_SPI_Wait(unsigned int cycles);
static void CC_SPI_Bitbang_out(unsigned char value);
static char CC_SPI_Bitbang_in(void);


// Delay function. # of CPU cycles delayed is similar to "cycles". Specifically,
// it's ((cycles-15) % 6) + 15.  Not exact, but gives a sense of the real-time
// delay.  Also, if MCLK ~1MHz, "cycles" is similar to # of useconds delayed.
static void CC_SPI_Wait(unsigned int cycles)
{
    HAL_RF_Delay(cycles);
}
//setup SPI
void CC_SPI_Setup_SPI(void)
{
    HAL_RF_Initial_IO();
}
#ifndef DMA_SPI
// Output eight-bit value using selected bit-bang pins
static void CC_SPI_Bitbang_out(unsigned char value)
{
    char x;

    for(x = 8; x > 0; x--)
    {
        if(value & 0x80)
            HAL_RF_Set_SPI_BITBANG_SIMO(1);
        else
            HAL_RF_Set_SPI_BITBANG_SIMO(0);
        value = value << 1;

        HAL_RF_Set_SPI_BITBANG_UCLK(0);
        HAL_RF_Set_SPI_BITBANG_UCLK(0);
        HAL_RF_Set_SPI_BITBANG_UCLK(0);
        HAL_RF_Set_SPI_BITBANG_UCLK(0);
        HAL_RF_Set_SPI_BITBANG_UCLK(0);
        HAL_RF_Set_SPI_BITBANG_UCLK(1);
        HAL_RF_Set_SPI_BITBANG_UCLK(1);
        HAL_RF_Set_SPI_BITBANG_UCLK(1);
    }
}
// Input eight-bit value using selected bit-bang pins
static char CC_SPI_Bitbang_in(void)
{
    char x = 0;
    int y=0;
//    char shift = 0;
//    x = HAL_RF_Get_SPI_BITBANG_SOMI_BIT();
//    while(x > 1)
//    {
//        shift++;
//        x = x >> 1;
//    }
//    x = 0;
    for(y = 8; y > 0; y--)
    {
        HAL_RF_Set_SPI_BITBANG_UCLK(0);
        HAL_RF_Set_SPI_BITBANG_UCLK(0);
        HAL_RF_Set_SPI_BITBANG_UCLK(0);
        HAL_RF_Set_SPI_BITBANG_UCLK(0);
        HAL_RF_Set_SPI_BITBANG_UCLK(0);
        HAL_RF_Set_SPI_BITBANG_UCLK(1);
        HAL_RF_Set_SPI_BITBANG_UCLK(1);
        HAL_RF_Set_SPI_BITBANG_UCLK(1);
        
        x = x << 1;
        x = x | HAL_RF_Get_SPI_BITBANG_SOMI();
    }
    return (x);
}
#endif
UINT8 Trans_ADD_CMD[2];
void CC_SPI_Write_Reg(char addr, char value)
{
    unsigned int w_Count = ERROR_BREAK_TIME;
#ifdef DMA_SPI
    while(!HAL_DMA_Get_transferComplete()&&(w_Count--));
    
    Trans_ADD_CMD[0] = addr;
    Trans_ADD_CMD[1] = value;
    
    HAL_DMA_SPI_Transfer((uint8_t*)Trans_ADD_CMD, 2);
    int count_delay = 1000;
    while(count_delay--);
#else
    HAL_RF_Set_SPI_BITBANG_CS(0);
    while(HAL_RF_Get_SPI_BITBANG_SOMI() && (w_Count--));
    
    CC_SPI_Bitbang_out(addr);            // Send address
    CC_SPI_Bitbang_out(value);           // Send data
    
    HAL_RF_Set_SPI_BITBANG_CS(1);
#endif
}

char Trans_Buffer[70];
void CC_SPI_Write_Burst_Reg(char addr, char *buffer, char count)
{
    unsigned int w_Count = ERROR_BREAK_TIME;
#ifdef DMA_SPI
    while(!HAL_DMA_Get_transferComplete()&&(w_Count--));
    
    Trans_Buffer[0]=addr|TI_CCxxx0_WRITE_BURST,
    memcpy(&Trans_Buffer[1],buffer,count);
    
    HAL_DMA_SPI_Transfer((uint8_t*)Trans_Buffer, count+1); 
    int count_delay = 1000;
    while(count_delay--);
#else    
    char i;
    HAL_RF_Set_SPI_BITBANG_CS(0);
    while(HAL_RF_Get_SPI_BITBANG_SOMI() && (w_Count--));

    CC_SPI_Bitbang_out(addr | TI_CCxxx0_WRITE_BURST);   // Send address
    for (i = 0; i < count; i++)
        CC_SPI_Bitbang_out(buffer[i]);     // Send data

    HAL_RF_Set_SPI_BITBANG_CS(1);
#endif
}

char CMD_ADD;
char CMD_CLK = 0x00 ;
#define MAX_RX_LENGTH 75
char Receivebuffer[MAX_RX_LENGTH];
#ifdef DMA_SPI
static char CCSPI_Get_RX_Valid_Start_Add(void)
{
    struct{
     unsigned RX_IsPreChar    :1; 
     char Pre_RX_Index ;
     char RX_Index ; 
    }RX;
    
    //RX.Pre_RX_Index = MAX_RX_LENGTH ;
    RX.RX_Index = 0 ;
    RX.RX_IsPreChar = 0 ;
    while(1)
    {
        if(Receivebuffer[RX.RX_Index]!=0x00&&RX.RX_IsPreChar)
        {    
            break;
        }
        if( Receivebuffer[RX.RX_Index] == 0x00 )
        {
            RX.RX_IsPreChar = 1 ;
        }
        RX.RX_Index++;
        if(RX.RX_Index>=MAX_RX_LENGTH)
        {
            RX.RX_Index = MAX_RX_LENGTH ;
            break;
        }
    }
    return(RX.RX_Index);
}    
#endif
char CC_SPI_Read_Reg(char addr)
{
    char x=0;
    unsigned int w_Count = ERROR_BREAK_TIME;
#ifdef DMA_SPI
    char Temp = 0;
    
    while(!HAL_DMA_Get_transferComplete()&&(w_Count--));
    
    Trans_ADD_CMD[0] = addr|TI_CCxxx0_READ_SINGLE;
    Trans_ADD_CMD[1] = 0x00;
    HAL_DMA_SPI_Transfer((uint8_t*)Trans_ADD_CMD, sizeof(Trans_ADD_CMD));
    
    memset(Receivebuffer, 0, sizeof(Receivebuffer));
    
    HAL_DMA_Set_RXBuffer_Length(MAX_RX_LENGTH);
    HAL_DMA_Receive((UINT8*)Receivebuffer,sizeof(Receivebuffer));
    
    int count_delay = 5000;
    while(count_delay--);
    
    Temp = CCSPI_Get_RX_Valid_Start_Add();
    if(Temp>=MAX_RX_LENGTH)
    {
        return(0);
    }        
    else
    {
        return(Receivebuffer[Temp]);
    }
    
#else    
    HAL_RF_Set_SPI_BITBANG_CS(0);
    while(HAL_RF_Get_SPI_BITBANG_SOMI() && (w_Count--)); //--------------------- add by liye
    
    CC_SPI_Bitbang_out(addr | TI_CCxxx0_READ_SINGLE);//Send address
    x = CC_SPI_Bitbang_in();               // Read data
    
    HAL_RF_Set_SPI_BITBANG_CS(1);
    
    return (x);
#endif
    
}

void CC_SPI_Read_Burst_Reg(char addr, char *buffer, char count)
{
    unsigned int w_Count = ERROR_BREAK_TIME;
#ifdef DMA_SPI
    char Vilad_Start_Add;
    memset(Trans_Buffer, 0, sizeof(Trans_Buffer));
    
    while(!HAL_DMA_Get_transferComplete()&&(w_Count--));
     
    Trans_Buffer[0]=addr | TI_CCxxx0_READ_BURST;
    
    HAL_DMA_SPI_Transfer((uint8_t*)Trans_Buffer, count+1);
    
    memset(Receivebuffer, 0, sizeof(Receivebuffer));
    
    HAL_DMA_Receive((UINT8*)Receivebuffer,count);
    
    int count_delay = 60000;
    while(count_delay--);
    
    if(CCSPI_Get_RX_Valid_Start_Add()>=MAX_RX_LENGTH)
    {
        memset(buffer, 0, count);
    }        
    else
    {
        memcpy(buffer,&Receivebuffer[CCSPI_Get_RX_Valid_Start_Add()],count); 
    }
    
#else 
    char i;
    
    HAL_RF_Set_SPI_BITBANG_CS(0);
    while(HAL_RF_Get_SPI_BITBANG_SOMI() && w_Count--);

    CC_SPI_Bitbang_out(addr | TI_CCxxx0_READ_BURST);    // Send address
    for (i = 0; i < count; i++)
        buffer[i] = CC_SPI_Bitbang_in();     // Read data

    HAL_RF_Set_SPI_BITBANG_CS(1);
#endif
}

char TEST_CMD[4] ;
char CC_SPI_Read_Status(char addr)
{
    unsigned int w_Count = ERROR_BREAK_TIME;
    
#ifdef DMA_SPI 
    while(!HAL_DMA_Get_transferComplete()&&(w_Count--));
    
    Trans_ADD_CMD[0] = addr|TI_CCxxx0_READ_BURST;
    Trans_ADD_CMD[1] = 0x00;
    HAL_DMA_SPI_Transfer((uint8_t*)Trans_ADD_CMD, sizeof(Trans_ADD_CMD));
    memset(Receivebuffer, 0, sizeof(Receivebuffer));
    
    HAL_DMA_Set_RXBuffer_Length(75);
    
    HAL_DMA_Receive((UINT8*)Receivebuffer,sizeof(Receivebuffer));
    
    int count_delay = 1000;
    while(count_delay--);
    
    if(CCSPI_Get_RX_Valid_Start_Add()>=MAX_RX_LENGTH)
    {
        return(0);
    }        
    else
    {
        return(Receivebuffer[CCSPI_Get_RX_Valid_Start_Add()]);
    }
#else 
    char x=0;
    HAL_RF_Set_SPI_BITBANG_CS(0);
    while(HAL_RF_Get_SPI_BITBANG_SOMI() && w_Count--);
    
    CC_SPI_Bitbang_out(addr | TI_CCxxx0_READ_BURST);      // Send address
    x = CC_SPI_Bitbang_in();               // Read data
    
    HAL_RF_Set_SPI_BITBANG_CS(1);
    return x;
#endif
    
}

void CC_SPI_Strobe(char strobe)
{
    unsigned int w_Count = ERROR_BREAK_COUNT;
#ifdef DMA_SPI 
    while(!HAL_DMA_Get_transferComplete()&&(w_Count--));
    
    CMD_ADD = strobe ;
    
    HAL_DMA_SPI_Transfer((uint8_t*)(&CMD_ADD), 1);
#else
    
    HAL_RF_Set_SPI_BITBANG_CS(0);
    while(HAL_RF_Get_SPI_BITBANG_SOMI() && w_Count--);

    CC_SPI_Bitbang_out(strobe);            // Send strobe

    HAL_RF_Set_SPI_BITBANG_CS(1);
#endif
}


void CC_PowerupResetCCxxxx(void)
{
    unsigned int w_Count = ERROR_BREAK_TIME;
#ifdef DMA_SPI 
    // Sec. 27.1 of CC1100 datasheet
    //HAL_RF_Set_SPI_BITBANG_CS(1);

    CC_SPI_Wait(30);

    //HAL_RF_Set_SPI_BITBANG_CS(0);

    CC_SPI_Wait(30);

    //HAL_RF_Set_SPI_BITBANG_CS(1);

    CC_SPI_Wait(45);
    
    while(!HAL_DMA_Get_transferComplete()&&(w_Count--));
    
    CMD_ADD = TI_CCxxx0_SRES ; 
    HAL_DMA_SPI_Transfer((uint8_t*)(&CMD_ADD), 1); 
#else
    // Sec. 27.1 of CC1100 datasheet
    HAL_RF_Set_SPI_BITBANG_CS(1);

    CC_SPI_Wait(30);

    HAL_RF_Set_SPI_BITBANG_CS(0);

    CC_SPI_Wait(30);

    HAL_RF_Set_SPI_BITBANG_CS(1);

    CC_SPI_Wait(45);
    
    HAL_RF_Set_SPI_BITBANG_CS(0);
    while(HAL_RF_Get_SPI_BITBANG_SOMI() && w_Count--);

    CC_SPI_Bitbang_out(TI_CCxxx0_SRES);

    w_Count = ERROR_BREAK_TIME;
    while(HAL_RF_Get_SPI_BITBANG_SOMI() && w_Count--);

    HAL_RF_Set_SPI_BITBANG_CS(1);
#endif
}

