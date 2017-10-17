#ifndef _EERom_H__
#define _EERom_H__
#include  "SPI_Flash.h"

#define C_FAIL      0
#define C_PASS      1
#define EE_MD5   8
#define EE_MD5   8

#define _ParameterMax             38 //==>目前最大储存数 (随储存数增减)

#define EE_BlockMask              0
#define EE_CheckSum               2
#define EE_ParameterNo            4
#define EE_BlockIDSize            6
#define EE_DataStart              6

#define   _NumberOfBlock_     8
#define   _Sector_BlockSize_  1024
typedef union{
  struct {
    unsigned short CheckNo ;                      // 2          // save number FF ==> NULL 0xAA51-0xAA54
    unsigned short CheckSum ;                     // 2    x     // 0xFFFF - Total sum = XXXX
    unsigned short MLength ;                      // 2  
    unsigned short MemoryCheck ;                  // 2
    unsigned char MD5[32];
    unsigned long RSCU_LastReadAddress;
  } Parameter ;
  unsigned char Block[_Sector_BlockSize_] ;
} EEMEMORY ;

extern EEMEMORY EEMemory;

unsigned char EEROM_GetMemoryErrorStatus(void) ;
unsigned char EEROM_Initial(void) ;
unsigned char EEROM_WriteParameter(void) ;

unsigned char EEROM_ExportEEMemoryData(unsigned short Addr ,unsigned char *ExportData) ; // Return = 0 is END
unsigned char EEROM_InportEEMemoryData( unsigned short Addr, unsigned char InportData ) ; // Return = 0 is END
void EEROM_ParameterDefault(unsigned char by_D) ;


#endif

