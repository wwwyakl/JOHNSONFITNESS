//
#include  "SPI_Flash.h"

//==============================================================================

union {
  struct {
    unsigned char WIP:1 ;//0 = Not Busy, no embedded operation in progress;1 = Busy, embedded operation in progress
    unsigned char WEL:1 ;//0 = Not Write Enabled, no embedded operation can start;1 =Write Enabled, embedded operation can start
    unsigned char BP:3 ; //Block Protect Bits
    unsigned char TB:1 ; //0 = BP2-BP0 protect from the Top down;1 = BP2-BP0 protect from the Bottom up
    unsigned char SEC:1 ;//0 = BP2-BP0 protect 64KB blocks;1 = BP2-BP0 protect 4KB sectors
    unsigned char SRP0:1;//0 = /WP input has no effect or Power Supply Lock Down mode;1 = /WP input can protect the Status Register or OTP Lock Down
  } Bit ;
  unsigned char All ;
} MX_Status ;




__no_init unsigned char SPI_FlashError;

//------------------------------------------------------------------------------
  
void SPI_Flash_CommandOut( unsigned char Cmd, unsigned long Addr, unsigned char Data ) ;
void SPI_Flash_ByteOut(unsigned char Data) ;
unsigned char SPI_Flash_ByteInput(void) ;
unsigned char SPI_Flash_ReadStatus(void) ;
void SPI_Flash_BlockProtectionEnable( void ) ;
unsigned char SPI_Flash_BlockProtectionDisable( void );
void SPI_Flash_WriteEnable(void);
void SPI_Flash_WriteDisable(void);
unsigned char SPI_Flash_WaitBusyFree(void) ;
void SPI_Flash_InsertDummyCycle( unsigned char dummy_cycle ) ;
void SPI_Flash_PageProgram( unsigned long MAddr, unsigned short Size, unsigned char *Rptr ) ;


void  SPI_Flash_Initial(void)
{
  HAL_SPI_Flash_Initial();
}
void SPI_Flash_Initial_Data(void)
{
  unsigned char ErrorCount ;
  ErrorCount = 0 ;
  SPI_FlashError=0;
  SPI_Flash_ReadStatus() ;
  if( MX_Status.Bit.BP != 0 )
  {
    while( ErrorCount < 3 && SPI_Flash_BlockProtectionDisable() != 0 ) 
    {
      ErrorCount += 1 ;
    }
  }
  if(ErrorCount==3)
  {
    SPI_FlashError=FLASH_ERROR;
  }
}
unsigned char SPI_Flash_Error(void)
{
  return SPI_FlashError;
}

//---------------------------------------------------------------------------------
void  SPI_Flash_CommandOut( unsigned char Cmd, unsigned long Addr, unsigned char Data )
{
  //
  union {
    struct {
      unsigned char Byte0:8 ;
      unsigned char Byte1:8 ;
      unsigned char Byte2:8 ;
      unsigned char Byte3:8 ;
    } Byte ;
    unsigned long All ;
  } AddrData ;
  //
  AddrData.All = Addr ;
  //
  switch( Cmd ) 
      {
      case  _WRSR_  :// Write Status Register
                    SPI_Flash_ByteOut(Cmd) ;
                    SPI_Flash_ByteOut(Data) ;
                    break ;
      case  _CE_    :// Chip Erase
      case  _RDSR_  :// Read Status Register
      case  _WREN_  :// Write Enable
      case  _WRDI_  :// Write Disable
                    SPI_Flash_ByteOut(Cmd) ;
                    break ;
      case  _RD_    :// Read
      case  _SE_    :// Sector Erase 4Kbyte  x (A12~A16)
      case  _BE_    :// Block Erase 32Kbyte X (A15~A16)                    
      case  _RDID_  :// Read ID
      case  _PP_    :// Page Programmer        
                    SPI_Flash_ByteOut(Cmd) ;
                    SPI_Flash_ByteOut(AddrData.Byte.Byte2) ;
                    SPI_Flash_ByteOut(AddrData.Byte.Byte1) ;
                    SPI_Flash_ByteOut(AddrData.Byte.Byte0) ;
                    break ;
      case  _DREAD_ :
      case  _FRD_   :          
                    SPI_Flash_ByteOut(Cmd) ;
                    SPI_Flash_ByteOut(AddrData.Byte.Byte2) ;
                    SPI_Flash_ByteOut(AddrData.Byte.Byte1) ;
                    SPI_Flash_ByteOut(AddrData.Byte.Byte0) ;
                    SPI_Flash_InsertDummyCycle(8) ;
                    break ;
      default       :
                    break ;
      }   
  return ;
}


//------------------------------------------------------------------------------
void SPI_Flash_ByteOut(unsigned char Data)
{
  //
  union {
    struct {
      unsigned char B0:1 ;
      unsigned char B1:1 ;
      unsigned char B2:1 ;
      unsigned char B3:1 ;
      unsigned char B4:1 ;
      unsigned char B5:1 ;
      unsigned char B6:1 ;
      unsigned char B7:1 ;
    } Bit ;
    unsigned char All ;
  } OutData ;
  unsigned char i ;
  OutData.All = Data ;
  //
  for( i = 0 ; i < 8 ; i++ )  
    {
    if( OutData.Bit.B7 == 1 )
        {
            HAL_SPI_Flash_DI(Bit_SET);  
        }
    else
        {
       
        HAL_SPI_Flash_DI(Bit_RESET);  
        }
    //
    
    HAL_SPI_Flash_CLK(Bit_RESET);
    asm("nop") ;
    OutData.All = OutData.All << 1 ;
    
    HAL_SPI_Flash_CLK(Bit_SET);
    asm("nop") ;
    }
  //
  return ;
}

//------------------------------------------------------------------------------
unsigned char SPI_Flash_ByteInput(void)
{
  //
  union {
    struct {
      unsigned char B0:1 ;
      unsigned char B1:1 ;
      unsigned char B2:1 ;
      unsigned char B3:1 ;
      unsigned char B4:1 ;
      unsigned char B5:1 ;
      unsigned char B6:1 ;
      unsigned char B7:1 ;
    } Bit ;
    unsigned char All ;
  } RData ;
  //
  RData.All = 0 ;
  unsigned char i ;
  //
  for( i = 0 ; i < 8 ; i++ )
      {
      RData.All = RData.All << 1 ;
       
      HAL_SPI_Flash_CLK(Bit_RESET);
      asm("nop") ;
      RData.Bit.B0 = HAL_SPI_Flash_DO();      
      
      HAL_SPI_Flash_CLK(Bit_SET);
      asm("nop") ;
      }
  //
  return RData.All ;
}



//------------------------------------------------------------------------------
unsigned char SPI_Flash_ReadByte( unsigned long MAddr )
{
  unsigned char ByteData ;
  //
  
  HAL_SPI_Flash_CS(Bit_RESET);
  //
  SPI_Flash_CommandOut(_RD_,MAddr,0) ;
  ByteData = SPI_Flash_ByteInput()  ;
  //
  
  HAL_SPI_Flash_CS(Bit_SET);
  //
  return ByteData ;
}



//------------------------------------------------------------------------------
unsigned short SPI_Flash_ReadWord( unsigned long MAddr )
{
  //
  union {
    struct {
      unsigned char Low:8 ;
      unsigned char High:8 ;
    } Byte ;
    unsigned int All ;
  } RData ;
  //
  
  HAL_SPI_Flash_CS(Bit_RESET);
  //
  SPI_Flash_CommandOut(_RD_,MAddr,0) ;
  RData.Byte.Low = SPI_Flash_ByteInput()  ;
  RData.Byte.High = SPI_Flash_ByteInput()  ;
  //
  
  HAL_SPI_Flash_CS(Bit_SET);
  //
  return RData.All ;
}

//------------------------------------------------------------------------------
void SPI_Flash_ReadBlock( unsigned long MAddr, unsigned long Size, unsigned char *Rptr )
{
  unsigned long ByteCount ;
  //
  
  HAL_SPI_Flash_CS(Bit_RESET);
  //
  SPI_Flash_CommandOut(_FRD_,MAddr,0) ;
  for( ByteCount = 0 ; ByteCount < Size ; ByteCount++ )
      {
      *(Rptr+ByteCount) = SPI_Flash_ByteInput()  ;
      }
  //
  
  HAL_SPI_Flash_CS(Bit_SET);
  //
  //
  return ;
}

//------------------------------------------------------------------------------
void SPI_Flash_WriteByte( unsigned long MAddr, unsigned char Data )
{
  unsigned char ErrorCount ;
  ErrorCount = 0 ;
  if( SPI_FlashError !=FLASH_ERROR)
  {
    //
    SPI_Flash_ReadStatus() ;
    if( MX_Status.Bit.BP != 0 )
    {
      while( ErrorCount < 3 && SPI_Flash_BlockProtectionDisable() != 0 ) 
      {
        ErrorCount += 1 ;
      }
    }
    //
    if( ErrorCount < 3 )
    {
      //
      SPI_Flash_WaitBusyFree() ;
      //
      SPI_Flash_WriteEnable() ;
      //
      
      HAL_SPI_Flash_CS(Bit_RESET);
      SPI_Flash_CommandOut(_PP_ ,MAddr,0) ;
      SPI_Flash_ByteOut(Data) ;
      
      HAL_SPI_Flash_CS(Bit_SET);
      asm("nop") ;
      //
      SPI_Flash_WaitBusyFree() ;
      //
      SPI_Flash_WriteDisable() ;
      //
      SPI_Flash_BlockProtectionEnable() ;
      //
    }
    else
    {
      SPI_FlashError =FLASH_ERROR;
    }
    //
  }
  return ;
}


//------------------------------------------------------------------------------
void SPI_Flash_WriteBlock( unsigned long MAddr, unsigned long Size, unsigned char *Rptr )
{
  unsigned char ErrorCount ;
  unsigned short DataIndex ;
  unsigned long DataSize ;
  unsigned long TotalSize ;
  unsigned long MemAddress ;
  
  ErrorCount = 0 ;
  TotalSize = Size ;
  DataIndex = 0 ;
  MemAddress = MAddr ;
  if( SPI_FlashError !=FLASH_ERROR)
  {
    SPI_Flash_ReadStatus() ;
    if( MX_Status.Bit.BP != 0 )
    {
      while( ErrorCount < 3 && SPI_Flash_BlockProtectionDisable() != 0 ) 
      {
        ErrorCount += 1 ;
      }
    }
    
    if( ErrorCount < 3 )
    {
      //
      do
      {
#ifdef WDT_ENABLE
        HAL_Clear_WatchDog();
#endif
        //  
        DataSize = _PP_PageSize_ - (MemAddress%_PP_PageSize_) ;
        if( TotalSize <= DataSize )
          DataSize = (unsigned short)TotalSize ;
        
        //
        SPI_Flash_PageProgram(MemAddress,DataSize,(Rptr+DataIndex)) ;
        //
        //
        MemAddress += DataSize ;
        DataIndex += DataSize ;
        TotalSize -=  DataSize ;
        //
      } while( TotalSize != 0 ) ;
      //
      SPI_Flash_BlockProtectionEnable() ;
      //
    }
    else
    {
      SPI_FlashError =FLASH_ERROR;
    }    
    //
  }
  return ;
}

void SPI_Flash_PageProgram( unsigned long MAddr, unsigned short Size, unsigned char *Rptr )
{
  unsigned long ByteCount ;
 
  //
  SPI_Flash_WaitBusyFree() ;
  //
  SPI_Flash_WriteEnable() ;
  //
  
  HAL_SPI_Flash_CS(Bit_RESET);
  SPI_Flash_CommandOut( _PP_ , MAddr , 0 ) ;
  //
  for( ByteCount = 0 ; ByteCount < Size ; ByteCount++ )
      {
      SPI_Flash_ByteOut(*(Rptr+ByteCount)) ;
      }
  
  HAL_SPI_Flash_CS(Bit_SET);
  asm("nop") ;
  //
  SPI_Flash_WaitBusyFree() ;
  //
  SPI_Flash_WriteDisable() ;
  //
  return ;
}



//------------------------------------------------------------------------------
void SPI_Flash_BlockErase(unsigned short Block) 
{
  unsigned long BlockAddr = 0 ;
  unsigned char ErrorCount = 0 ;
  if( SPI_FlashError !=FLASH_ERROR)
  {
    //
    SPI_Flash_ReadStatus() ;
    if( MX_Status.Bit.BP != 0 )
    {
      while( ErrorCount < 3 && SPI_Flash_BlockProtectionDisable() != 0 )
      {
        ErrorCount += 1 ;
      }
    }
    //
    if( ErrorCount < 3 )
    {
      //----------------------------------------------------------------------------
      if( Block < _Block_Number_ )
      {
        if( Block == 0 )
          BlockAddr = 0 ;
        else
          BlockAddr = _Block_Size_ * Block ;
        //
        SPI_Flash_WriteEnable() ;
        //
        
        HAL_SPI_Flash_CS(Bit_RESET);
        asm("nop") ;
        SPI_Flash_CommandOut(_BE_,BlockAddr,0) ;
        //
        
        HAL_SPI_Flash_CS(Bit_SET);
        //----------------------------------------------------------------------------
        SPI_Flash_WriteDisable() ;
        //
        SPI_Flash_BlockProtectionEnable() ;
        //
      }
    }
    else
    {
      SPI_FlashError =FLASH_ERROR;
    }
  }
  //----------------------------------------------------------------------------
  return ;
}

//------------------------------------------------------------------------------
void SPI_Flash_SectorErase(unsigned short Sector) 
{
  unsigned long SectorAddr = 0 ;
  unsigned char ErrorCount = 0 ;
  if( SPI_FlashError !=FLASH_ERROR)
  {
    //
    SPI_Flash_ReadStatus() ;
    if( MX_Status.Bit.BP != 0 )
    {
      while( ErrorCount < 3 && SPI_Flash_BlockProtectionDisable() != 0 )
      {
        ErrorCount += 1 ;
      }
    }
    //
    if( ErrorCount < 3 )
    {
      //----------------------------------------------------------------------------
      if( Sector < _SectorNumber_ )
      {
        if( Sector == 0 )
          SectorAddr = 0 ;
        else
          SectorAddr = _SectorBase_ * Sector ;
        //
        SPI_Flash_WriteEnable() ;
        //
        
        HAL_SPI_Flash_CS(Bit_RESET);
        asm("nop") ;
        SPI_Flash_CommandOut(_SE_,SectorAddr,0) ;
        //
        
        HAL_SPI_Flash_CS(Bit_SET);
        //----------------------------------------------------------------------------
        SPI_Flash_WriteDisable() ;
        //
        SPI_Flash_BlockProtectionEnable() ;
        //
      }
    }
    else
    {
      SPI_FlashError =FLASH_ERROR;
    }
  }
  //----------------------------------------------------------------------------
  return ;
}

//------------------------------------------------------------------------------
unsigned int SPI_Flash_ReadID(void)
{
  //
  union {
    struct {
      unsigned char Low:8 ;
      unsigned char High:8 ;
    } Byte ;
    unsigned int All ;
  } RData ;
  //----------------------------------------------------------------------------
  
  HAL_SPI_Flash_CS(Bit_RESET);
  //
  SPI_Flash_CommandOut(_RDID_,0,0) ;
  RData.Byte.High = SPI_Flash_ByteInput()  ;
  RData.Byte.Low = SPI_Flash_ByteInput()  ;
  //
  
  HAL_SPI_Flash_CS(Bit_SET);
  //
  //----------------------------------------------------------------------------
  return RData.All ;
}



//------------------------------------------------------------------------------
unsigned char SPI_Flash_ReadStatus(void)
{
  //----------------------------------------------------------------------------
  
  HAL_SPI_Flash_CS(Bit_RESET);
  //
  SPI_Flash_CommandOut(_RDSR_,0,0) ;
  MX_Status.All = SPI_Flash_ByteInput()  ;
  //
  
  HAL_SPI_Flash_CS(Bit_SET);
  //
  //----------------------------------------------------------------------------
  return MX_Status.All;
}


//------------------------------------------------------------------------------
void SPI_Flash_BlockProtectionEnable( void )
{
  //----------------------------------------------------------------------------
  //
  SPI_Flash_WaitBusyFree() ;
  //
  //----------------------------------------------------------------------------
  //
  
  HAL_SPI_Flash_CS(Bit_RESET);
  //
  SPI_Flash_CommandOut(_WREN_,0,0) ;
  //
  
  HAL_SPI_Flash_CS(Bit_SET);
  //
  asm("nop") ;
  
  HAL_SPI_Flash_CS(Bit_RESET);
  //
  SPI_Flash_CommandOut(_WRSR_,0,0x3c) ;
  //
  
  HAL_SPI_Flash_CS(Bit_SET);
  //
  //----------------------------------------------------------------------------
  //
  SPI_Flash_WaitBusyFree() ;
  //
  //----------------------------------------------------------------------------
  //
  return ;
}


//------------------------------------------------------------------------------
unsigned char SPI_Flash_BlockProtectionDisable( void )
{
  //----------------------------------------------------------------------------
  //
  SPI_Flash_WaitBusyFree() ;
  //
  //----------------------------------------------------------------------------
  //
  
  HAL_SPI_Flash_CS(Bit_RESET);
  //
  SPI_Flash_CommandOut(_WREN_,0,0) ;
  //
  
  HAL_SPI_Flash_CS(Bit_SET);
  //
  asm("nop") ;
  
  HAL_SPI_Flash_CS(Bit_RESET);
  //
  SPI_Flash_CommandOut(_WRSR_,0,0) ;
  
  HAL_SPI_Flash_CS(Bit_SET);
  //
  //----------------------------------------------------------------------------
  //
  SPI_Flash_WaitBusyFree() ;
  //
  //----------------------------------------------------------------------------
  //
  if( MX_Status.Bit.BP != 0 )
      return 1 ;
  //
  return 0 ;
}


void SPI_Flash_WriteEnable(void)
{
  //
  SPI_Flash_WaitBusyFree() ;
  //
  
  HAL_SPI_Flash_CS(Bit_RESET);
  SPI_Flash_CommandOut(_WREN_,0,0) ;
  //
  
  HAL_SPI_Flash_CS(Bit_SET);
  asm("nop") ;
  //
  return ;
}


void SPI_Flash_WriteDisable(void)
{
  //
  SPI_Flash_WaitBusyFree() ;
  //
  
  HAL_SPI_Flash_CS(Bit_RESET);
  SPI_Flash_CommandOut(_WRDI_,0,0) ;
  //
  
  HAL_SPI_Flash_CS(Bit_SET);
  asm("nop") ;
  //
  return ;
}


//------------------------------------------------------------------------------
unsigned char SPI_Flash_WaitBusyFree(void)
{
  unsigned long DelayCount ;
  unsigned char DelayCheck ;
  DelayCount = 0 ;
  //
  do
  {
      #ifdef WDT_ENABLE
      HAL_Clear_WatchDog();
#endif
      for( DelayCheck = 0 ; DelayCheck < 0xFF ; DelayCheck++ );
      DelayCount += 1 ;
#ifdef WDT_ENABLE
      HAL_Clear_WatchDog();
#endif
      
      SPI_Flash_ReadStatus() ;  
  } while(MX_Status.Bit.WIP == 1 && DelayCount < 10000 ) ;
  //
  if( DelayCount > 10000 )
      return 0xFF ;
  //
  return 0 ;  
}



void SPI_Flash_InsertDummyCycle( unsigned char dummy_cycle )
{
  unsigned char i;
  for( i=0; i < dummy_cycle; i=i+1 )
      {
      
      HAL_SPI_Flash_CLK(Bit_RESET);  
      
      HAL_SPI_Flash_CLK(Bit_SET);
      }
  return ;
}






