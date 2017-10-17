#ifndef _DIGITAL_CMD
#define _DIGITAL_CMD

//#include "includes.h"
#include "HAL_Lib.h"

#define RX_BUFFER_MAX	128
#define TX_BUFFER_MAX	128


typedef struct
{
    UCHAR       Cmd;
    UCHAR       Status;             //Status command
    UCHAR       Len;                //Length of data bytes £¬max.255
    UCHAR       Data[RX_BUFFER_MAX];//Data bytes£¬max.255Bytes
}CMD_RX;

typedef struct
{
    UCHAR       Cmd;
    UCHAR       Adress;             //0xFF used for only one receiver condition
    UCHAR       Len;                //Length of data bytes £¬max.255
    UCHAR       Data[TX_BUFFER_MAX];//Data bytes£¬max.255Bytes
}CMD_TX;


void Digital_Cmd_Initial(void);
UCHAR Digital_Cmd_Get_Cmd(CMD_RX *CmdRx);
void Digital_Cmd_Send_Cmd(CMD_TX *CmdTx);
void Digital_Cmd_TxBuff(UCHAR by_Len);
UCHAR Digital_Cmd_Tx_Busy(void);
void Digital_Cmd_Tx_Int(void);
void Digital_Cmd_Rx_Int(void);
UCHAR Digital_Cmd_CRC8_Check(UCHAR *pby_Data, UCHAR by_Length);
void Digital_Cmd_1ms_Int(void);


#endif


