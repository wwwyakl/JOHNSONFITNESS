#include  "EERom.h"
//==============================================================================

#define   _Sector0_Block0_  _EEROM_BaseAddress_
#define   _Sector0_Block1_  _Sector0_Block0_+_Sector_BlockSize_
#define   _Sector0_Block2_  _Sector0_Block1_+_Sector_BlockSize_
#define   _Sector0_Block3_  _Sector0_Block2_+_Sector_BlockSize_
#define   _Sector1_Block0_  _Sector0_Block3_+_Sector_BlockSize_
#define   _Sector1_Block1_  _Sector1_Block0_+_Sector_BlockSize_
#define   _Sector1_Block2_  _Sector1_Block1_+_Sector_BlockSize_
#define   _Sector1_Block3_  _Sector1_Block2_+_Sector_BlockSize_

#define   _Sector0_  _Sector0_Block0_/_Sector_BlockSize_
#define   _Sector1_  _Sector1_Block0_/_Sector_BlockSize_

#define   _BlockIndex_        0xAA51
#define   _Sector0_StartNo_   1
#define   _Sector1_StartNo_   5
// Must be match 4 byte
__no_init EEMEMORY EEMemory;




unsigned short EEROM_CalculatorChecksum( unsigned char *Dptr, unsigned short size ) ;
unsigned char EEROM_CheckEEMemoryDataStatus( unsigned long Maddr, unsigned short size, unsigned short CheckData );

unsigned char EEROM_CheckSectorErase( unsigned long SetcorAddr ) ;
unsigned char EEROM_VerifyData( unsigned long WriteAddress, unsigned char *Dptr, unsigned short size ) ;
unsigned char EEROM_GetMemoryErrorStatus(void) ;

unsigned char NowMemoryBlockNo ;
unsigned char MemoryStatus ;     // 0xAA = empty , 0x55 = OK
unsigned char MemoryErrorStatus ;
#define   _Empty_           0xAA
#define   _NoEmpty_         0x55
#define   _WriteError_      0xAA
#define   _WriteOK_         0x55


//------------------------------------------------------------------------------
unsigned char EEROM_Initial(void) 
{
  unsigned short CheckData ;
  unsigned short CheckLength ;
  unsigned short CheckSum ;
  unsigned long NowAddress ;
  unsigned char i ;
  //
  HAL_SPI_Flash_CS(Bit_SET);
  HAL_SPI_Flash_CLK(Bit_RESET);
  HAL_SPI_Flash_DI(Bit_SET);
  #ifdef      _Clear_All_
  SPI_Flash_SectorErase(_Sector0_) ;
  
  SPI_Flash_SectorErase(_Sector1_) ;
  #endif
  MemoryErrorStatus = _WriteOK_ ;
  NowMemoryBlockNo = 0 ;
  MemoryStatus = _Empty_ ; 
  // Sector 0
  // Check Block number 0~3
  for( i = 0 ; i < _NumberOfBlock_ ;i++ )
      {
      #ifdef WDT_ENABLE
      HAL_Clear_WatchDog();
#endif
      // Calculator Memory Address  
      if( i != 0 )
          NowAddress = _Sector0_Block0_+i * _Sector_BlockSize_  ;
      else
          NowAddress = _Sector0_Block0_ ;
      //------------------------------------------------------------------------
      CheckData = SPI_Flash_ReadWord(NowAddress) ;
      if( CheckData == (_BlockIndex_+i) )
          {
          CheckSum = SPI_Flash_ReadWord((NowAddress+EE_CheckSum)) ;
          if( EEROM_CheckEEMemoryDataStatus((NowAddress+EE_DataStart),(_Sector_BlockSize_-EE_DataStart),CheckSum) == 0 )
              {
              CheckLength = SPI_Flash_ReadWord(NowAddress+EE_ParameterNo) ;  
              if( CheckLength == _ParameterMax )
                  {
                  NowMemoryBlockNo = CheckData - _BlockIndex_ + 1 ;
                  MemoryStatus = _NoEmpty_ ;
                  }
              }
          }
      else
          {
          if(( NowMemoryBlockNo != 0 ) && (CheckData == 0xFFFF ) )
              break ;
          }
      }
  //
  if( MemoryStatus == _NoEmpty_ )
      {
      //  
      if( (NowMemoryBlockNo-1) != 0 )
          NowAddress = _Sector0_Block0_+(NowMemoryBlockNo-1) * _Sector_BlockSize_  ;
      else
          NowAddress = _Sector0_Block0_ ;
      //
      SPI_Flash_ReadBlock(NowAddress,_Sector_BlockSize_,&EEMemory.Block[0]) ;
      return C_PASS ;
      }
  else
      {
      // Set default
      NowMemoryBlockNo = 0 ;
      EEROM_ParameterDefault(0) ;
      //
      if( EEROM_CheckSectorErase(_Sector0_Block0_) == 1 )
          SPI_Flash_SectorErase(_Sector0_) ;
      
      if( EEROM_CheckSectorErase(_Sector1_Block0_) == 1 )
          SPI_Flash_SectorErase(_Sector1_) ;
      //
      MemoryErrorStatus = EEROM_WriteParameter() ;
      //
      }
  //
  return C_FAIL ;
}


void EEROM_ParameterDefault(unsigned char by_D)
{
  //
  unsigned short i ;
  
  if(by_D == 0)
  {
      for( i = 0 ; i < _Sector_BlockSize_ ; i++ )
      {
          EEMemory.Block[i] = 0 ;
      }

  }
  //
  if(by_D != 0)
  {
      EEROM_WriteParameter() ;
  }
  return ;
}

//------------------------------------------------------------------------------
unsigned char EEROM_WriteParameter(void)
{
 unsigned long WriteAddress ;
 unsigned char RetryCount ;
 RetryCount = 0 ;
   if( NowMemoryBlockNo >= 8 )
      NowMemoryBlockNo = 1 ;
  else
      NowMemoryBlockNo += 1 ;
  //
  if( NowMemoryBlockNo == 0 )
      NowMemoryBlockNo = 1 ;
  //
  EEMemory.Parameter.MLength = _ParameterMax ;
  EEMemory.Parameter.CheckNo = _BlockIndex_ + (NowMemoryBlockNo-1) ;   
  EEMemory.Parameter.CheckSum = EEROM_CalculatorChecksum( &EEMemory.Block[EE_DataStart],(_Sector_BlockSize_-EE_DataStart) ) ; 
 do
     {
      #ifdef WDT_ENABLE
      HAL_Clear_WatchDog();
#endif
     switch( NowMemoryBlockNo )
         {
         default  :
                  WriteAddress = _Sector0_Block0_ ;
                  NowMemoryBlockNo = 1 ;
                  //
                  if( EEROM_CheckSectorErase(WriteAddress) == 1 )
                    SPI_Flash_SectorErase(_Sector0_) ;
                  
                  //
                  break ;
         case 2   :
                  WriteAddress = _Sector0_Block1_ ;
                  break ;
         case 3   :
                  WriteAddress = _Sector0_Block2_ ;
                  break ;
         case 4   :
                  WriteAddress = _Sector0_Block3_ ;
                  break ;
         case 5   :
                  WriteAddress = _Sector1_Block0_ ;
                  //
                  if( EEROM_CheckSectorErase(WriteAddress) == 1 )
                      SPI_Flash_SectorErase(_Sector1_) ;
                  //
                  break ;
         case 6   :
                  WriteAddress = _Sector1_Block1_ ;
                  break ;
         case 7   :
                  WriteAddress = _Sector1_Block2_ ;
                  break ;
         case 8   :
                  WriteAddress = _Sector1_Block3_ ;
                  break ;
         }
     //
     SPI_Flash_WriteBlock((WriteAddress+EE_DataStart),(_Sector_BlockSize_-EE_DataStart),&EEMemory.Block[EE_DataStart]) ;
     SPI_Flash_WriteBlock(WriteAddress,EE_BlockIDSize,&EEMemory.Block[0]) ;
     
     if( EEROM_VerifyData(WriteAddress,&EEMemory.Block[0],_Sector_BlockSize_) == _WriteError_ )
          {
          RetryCount += 1 ;
          if( NowMemoryBlockNo >= 8 )
              NowMemoryBlockNo = 1 ;
          else
              NowMemoryBlockNo += 1 ;
          // for Check Memory
          EEMemory.Parameter.CheckNo = _BlockIndex_+ ( NowMemoryBlockNo-1 );   
          EEMemory.Parameter.CheckSum = EEROM_CalculatorChecksum( &EEMemory.Block[EE_DataStart],(_Sector_BlockSize_-EE_DataStart) ) ;    
          EEMemory.Parameter.MLength = _ParameterMax ;
          //
          if( NowMemoryBlockNo == _Sector1_StartNo_ )
              {
              if( EEROM_CheckSectorErase(_Sector1_Block0_) == 1 )
                  SPI_Flash_SectorErase(_Sector1_) ;
              }
          else
              {
              if( NowMemoryBlockNo == _Sector0_StartNo_ )
                  {
                  if( EEROM_CheckSectorErase(_Sector0_Block0_) == 1 )
                    SPI_Flash_SectorErase(_Sector0_) ;
                  }
              }
          //
          }
     else
          {
          if( NowMemoryBlockNo > _Sector1_StartNo_ )
              {
              if( EEROM_CheckSectorErase(_Sector0_Block0_) == 1 )
                SPI_Flash_SectorErase(_Sector0_) ;
              }
          else
              {
              if( NowMemoryBlockNo != _Sector1_StartNo_ )
                  {
                  if( NowMemoryBlockNo > _Sector0_StartNo_ )
                      {
                      if( EEROM_CheckSectorErase(_Sector1_Block0_) == 1 )
                          SPI_Flash_SectorErase(_Sector1_) ;
                      }
                  }
              }
          return _WriteOK_ ;
          }
     } while( RetryCount < 5 ) ;
 //
 return _WriteError_ ;
}


unsigned char EEROM_CheckSectorErase( unsigned long SetcorAddr )
{
    //
    unsigned short i ;
    for( i = 0 ; i < _SectorBase_ ; i++ )
    {
#ifdef WDT_ENABLE
        HAL_Clear_WatchDog();
#endif
        if( 0xFF != SPI_Flash_ReadByte((SetcorAddr+i)) )
            return 1 ;
    }
    //
    return 0 ;
}

//
unsigned char EEROM_VerifyData( unsigned long WriteAddress, unsigned char *Dptr, unsigned short size ) 
{
  unsigned short i ;
  for( i = 0 ; i < size ; i++ )
      {
      if( *(Dptr+i) != SPI_Flash_ReadByte((WriteAddress+i)) )
          return _WriteError_ ;
      }
  return _WriteOK_ ;  
}


//------------------------------------------------------------------------------------------------------
unsigned short EEROM_CalculatorChecksum( unsigned char *Dptr, unsigned short size )
{
  unsigned short CheckSum ;
  unsigned short i ;
  CheckSum = 0 ;
  for( i = 0 ; i < size ; i++ )
      {
      CheckSum += *(Dptr+i) ;
      }
  //
  CheckSum = 0xFFFF - CheckSum ;
  //
  return CheckSum ;
}

//------------------------------------------------------------------------------------------------------
unsigned char EEROM_CheckEEMemoryDataStatus( unsigned long Maddr, unsigned short size, unsigned short CheckData )
{
  unsigned short CheckSum  ;
  unsigned short i ;
  CheckSum = 0 ;
  for( i = 0; i < size ;i++ )
      {
      CheckSum += SPI_Flash_ReadByte((Maddr+i)) ;
      }
  //
  if( (CheckSum + CheckData) != 0xFFFF )
      return 1 ;
  //
  return 0 ;
}


//------------------------------------------------------------------------------
unsigned char EEROM_GetMemoryErrorStatus(void)
{
  if( MemoryErrorStatus == _WriteOK_ )
      {
      return  0 ;
      }
  return 1 ;
}

unsigned char EEROM_ExportEEMemoryData(unsigned short Addr ,unsigned char *ExportData)
{
  if( Addr >= _Sector_BlockSize_ )
      return 0 ;

  *ExportData = EEMemory.Block[Addr] ;
  return 1 ;
}


//-----------------------------------------------------------------------------------
unsigned char EEROM_InportEEMemoryData( unsigned short Addr, unsigned char InportData )
{
  if( Addr >= _Sector_BlockSize_ )
      return 0 ;

  EEMemory.Block[Addr] = InportData ;
  return 1 ;
}





