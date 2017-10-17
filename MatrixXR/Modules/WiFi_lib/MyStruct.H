#ifndef __MYSTRUCT_H
#define __MYSTRUCT_H

#include "jisdef.h"

#ifdef __WIFI_FUNC__

typedef unsigned char U8;
typedef unsigned char* UStringPtr;
typedef unsigned int U16;
typedef const unsigned char CU8;
typedef const unsigned int CU16;
typedef unsigned char* UPtr8;
typedef unsigned long U32;


extern U16 TCP_FLASH_INDEX; //Add by Jeffery
extern U32 Xml_Size;			//Add by Jeffery
extern unsigned char  FlashBufferx[];// Add by Jeffery


typedef struct
{
	U16 Incline;
	U16 Speed;
	U16 Distance;
	U16 Time;
	U16 Calories;
	U16 HR;
}SDemo_TM;	

typedef union
{
    struct
    {
        U8 LByte;
        U8 HByte;
    };
    struct
    {
        U16 Word;
    };
}Byte16;

typedef union
{
    struct
    {
        U8 LLByte;
        U8 LHByte;
        U8 HLByte;
        U8 HHByte;
    };
    struct
    {
        U16 LWord;
        U16 HWord;
    };
    struct
    {
        U32 DWord;
    };
}Byte32;

typedef union
{
    struct{
            U8 CRC_CCITT_L;
            U8 CRC_CCITT_H;
          };
    struct{
            U16 CRC_CCITT;
          };
}CRC_CCITT16;

#define DSaveDataBufferSize				(64-(1+1+2+4+2))        //		 (512-(1+1+2+4+2))   
typedef struct
{
	U8 Mark;
	U8 Type;
	U8 DataBuffer[DSaveDataBufferSize];
	U16 SerialNo;
	U32 UTC;
	CRC_CCITT16 CRC;
}ST_SaveData;


// Add by Jeffery
typedef struct
{
	    U8 MCHP_Update_flg;				// 1 byte
	    //U32 MCUHP_Update_size;			// 4 bytes
	    U8 Console_UCB_Update_flg;		// 1 byte
	    //U32 Console_UCB_Update_size;	// 4 bytes
	    U8 Console_LCB_Update_flg;		// 1 byte
	    //U32 Console_LCB_Update_size;	// 4 bytes
	    U8 XML_Save_flg;				// 1 byte	    
}MX_SaveFlag;
///////////////////////////////////////////////////

#ifndef __MAIN_VARDEF__

// Add by Jeffery
extern union
{
	unsigned char Val;
	struct
    {
		unsigned char MCUUpdate_http_IDLE : 1;
		unsigned char Console_UCB_Update : 1;
		unsigned char Console_LCB_Update : 1;
		//unsigned char Wifi_Going : 1;
    }bits;
}FWUPDATE_Status;


////////////////////////////////////////////////////

extern union
{
	unsigned char Val;
	struct
    {
		unsigned char WiFiConnect : 1;
		unsigned char WiFiError : 1;
		unsigned char NotSupportCommand : 1;
		unsigned char ResendPackage : 1;
		unsigned char BufferFull : 1;
		unsigned char DataReady : 1;
		unsigned char TCPIPConnect : 1;
		
    }bits;						
}WiFi_Status;

extern union
{
	unsigned char Val;
	struct
	{
		unsigned char Inc_Up : 1;
		unsigned char Inc_Dn : 1;
		unsigned char Spd_Up : 1;
		unsigned char Spd_Dn : 1;
	}bits;
}XmlData_Status;

extern union
{
	unsigned char Val;
	struct
    {
		unsigned char UART2TCP_Ready : 1;
		unsigned char TCP2UART_Ready : 1;
		unsigned char Save2TCP_Ready : 1;
    }bits;	
}WiFi_UART_Status;

extern union
{
	unsigned char bytes[13];
	struct
    {
	    unsigned char status;				//1
	    unsigned char TCPSendStatus;		//1
	    unsigned long Second;				//4
	    unsigned int  Command;				//2
	    unsigned char datacnt;				//1
	    unsigned char val[4];				//4
	}data;
	
}save_data;

extern union
{
	unsigned char Val;
	struct
    {
		unsigned char UARTDataNotSend : 1;
		unsigned char SaveWorkout : 1;
		unsigned char WorkoutOK : 1;	 //Add by Jeffery
		unsigned char TCPFlashEND : 1;	 //Add by Jeffery
		unsigned char TCP1stEND : 1;	 //Add by Jeffery
		unsigned char TCPEND : 1;	 //Add by Jeffery
		unsigned char WifiPermanentlyLost : 1; //Add by Jeffery
    }bits;		
}main_system;



#endif

#endif

#endif
