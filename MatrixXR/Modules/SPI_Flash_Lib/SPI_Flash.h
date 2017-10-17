#ifndef __SPI_Flash_H__
#define __SPI_Flash_H__

#include "HAL_Lib.H"

#define FLASH_ERROR 0xAA
typedef enum
{ Bit_RESET = 0,
  Bit_SET
}BitAction;
    
void  SPI_Flash_Initial(void);
void SPI_Flash_Initial_Data(void);
unsigned char SPI_Flash_Error(void);
unsigned char SPI_Flash_ReadByte( unsigned long MAddr ) ;
unsigned short SPI_Flash_ReadWord( unsigned long MAddr ) ;
void  SPI_Flash_ReadBlock( unsigned long MAddr, unsigned long Size, unsigned char *Rptr ) ;
unsigned int SPI_Flash_ReadID(void) ;
void SPI_Flash_WriteByte( unsigned long MAddr, unsigned char Data ) ;
void SPI_Flash_WriteBlock( unsigned long MAddr, unsigned long Size, unsigned char *Rptr ) ;
void SPI_Flash_BlockErase(unsigned short Block);
void SPI_Flash_SectorErase(unsigned short Sector) ;


#define   _Block_Size_        65536
#define   _Block_Number_      16

#define   _Block0_    (0*_Block_Size_)
#define   _Block1_    (1*_Block_Size_)
#define   _Block2_    (2*_Block_Size_)
#define   _Block3_    (3*_Block_Size_)
#define   _Block4_    (4*_Block_Size_)
#define   _Block5_    (5*_Block_Size_)
#define   _Block6_    (6*_Block_Size_)
#define   _Block7_    (7*_Block_Size_)
#define   _Block8_    (8*_Block_Size_)
#define   _Block9_    (9*_Block_Size_)
#define   _Block10_   (10*_Block_Size_)
#define   _Block11_   (11*_Block_Size_)
#define   _Block12_   (12*_Block_Size_)
#define   _Block13_   (13*_Block_Size_)
#define   _Block14_   (14*_Block_Size_)
#define   _Block15_   (15*_Block_Size_)

#define   _NewBin_StartBlock_  _Block0_/_Block_Size_
#define   _NewBin_EndBlock_    _Block4_/_Block_Size_
#define   _NewBin_BaseAddress_  _Block0_
#define   _OldBin_BaseAddress_  _Block4_
#define   _LCBABin_BaseAddress_  _Block8_
#define   _LCBBBin_BaseAddress_  _Block9_
#define   _EEROM_BaseAddress_   _Block10_
#define   _LanguageBin_StartBlock  _Block12_/_Block_Size_
#define   _LanguageBin_BaseAddress  _Block12_

#define   _SectorBase_        4096
#define   _SectorNumber_      512
#define   _PP_PageSize_       256

// SPI_Flash
#define   _Block0_Sector0_    0
#define   _Block0_Sector1_    (1*_SectorBase_)
#define   _Block0_Sector2_    (2*_SectorBase_)
#define   _Block0_Sector3_    (3*_SectorBase_)
#define   _Block0_Sector4_    (4*_SectorBase_)
#define   _Block0_Sector5_    (5*_SectorBase_)
#define   _Block0_Sector6_    (6*_SectorBase_)
#define   _Block0_Sector7_    (7*_SectorBase_)
#define   _Block0_Sector8_    (8*_SectorBase_)
#define   _Block0_Sector9_    (9*_SectorBase_)
#define   _Block0_SectorA_    (10*_SectorBase_)
#define   _Block0_SectorB_    (11*_SectorBase_)
#define   _Block0_SectorC_    (12*_SectorBase_)
#define   _Block0_SectorD_    (13*_SectorBase_)
#define   _Block0_SectorE_    (14*_SectorBase_)
#define   _Block0_SectorF_    (15*_SectorBase_)

//------------------------------------------------------------------------------
#define     _RD_                0x03    // Read
#define     _SE_                0x20    // Sector Erase 4Kbyte  x (A12~A16)
#define     _BE_                0xD8    // Block Erase 64Kbyte  X (A15~A16)
#define     _CE_                0x60    // Chip Erase
#define     _PP_                0x02    // Page Programmer
#define     _FRD_               0x0B    // Fast Read Data
#define     _RDSR_              0x05    // Read Status Register
#define     _EWSR_              0x50    // Enable Write Status Register
#define     _WRSR_              0x01    // Write Status Register
#define     _WREN_              0x06    // Write Enable
#define     _WRDI_              0x04    // Write Disable
#define     _RDID_              0x9F    // Read ID
#define     _RDSR_              0x05    // Read Status Register
#define     _DREAD_             0x3B    // Double Output Mode Command
#define     _REMS_              0x90    // Read Electronic manufacturer & device ID
#define     _RES_               0xAB    // Read Electronic ID
#define     _RJID_              0x9F    // Read JEDEC ID,The JEDEC ID instruction allows the 8-bit manufacturer identification to be read, followed by two bytes of device identification. The device identification indicates the memory type in the first byte, and the memory capacity of the device in the second byte.


#endif /* __SPI_Flash_H__ */




