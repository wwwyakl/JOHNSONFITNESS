//------------------------------------------------------------------------------
// Version : 2
// ���Դ���     : �� 1 ��ʼ�࣬ 0����Ӣ�ģ�������������е�Ĭ��
// һ���������� : 200
// �ַ�������     : �� 0 ��ʼ��
// 
// Language_LanguageChecking() : �趨����,
// Language_GetString() : �ַ�����ȡ���轫Lang_StringBuffer[]���룬�ַ������ݻ���ڴ˻�����
//                        ʹ��ʱ���趨�ַ�������(Name or Data)���ַ�������
// Lang_StringBuffer[] : �ַ���������
//------------------------------------------------------------------------------
// ͼ����������� : ���50��
// ͼ�ļ�����(�����������С) :��200�� ��� 400 Byte
// ͼ����� : �� 1 ��ʼ, 0����MATRIXͼ����������������е�Ĭ��
// 
// Pattern_WritePatternToEEPRom() : ��¼ͼ������ͼ���ļ���(·��)���뼴�ɿ�¼��
// Pattern_CheckingPattern() : ͼ���趨���Ὣָ���ĵ�ͼ�����ơ����ݶ��������������ͼ�������ڿ�����ѡͼ����¼ʱ����ִ�С�
// Pattern_PatternInfo[] : ͼ�ļ����ϣ�һ��16 dot��
// Pattern_PatternName[] : ͼ������
// Pattern_PatternColumnAmount : ͼ�ļ�����������
// Pattern_PatternAmount : ͼ������
// Pattern_PatternMotion : ͼ����ʾ���� ==> 0 ͼ��ֹͣ   1 ͼ������ת��   2 ͼ����ת����ֹͣ 
//           
//------------------------------------------------------------------------------
//#include  "stm32f10x_conf.h"
//#include  "hcc_types.h"
//#include  "usb_host.h"
//#include  "pmgr.h"
#include "em_usb.h"
#include "ff.h"
#include "msdh.h"
#include "usbconfig.h"
#include  "EEPRom_Language.h"
#include  "SPI_Flash.h"

//----- The words of one string ------------------------------------------------
//һ��������� 150��Ӣ����
unsigned char Lang_StringBuffer[_TheWordsOfOneString_];


//unsigned long Lang_CheckUSBTime = 0;
unsigned char Lang_LanguageAmount = 0;
unsigned char Lang_StringAmount = 0;
unsigned short Lang_Version = 0;
unsigned long by_LanguageIndexTableStartAddr = 0;
unsigned long by_LanguageDataStartAddr = 0;
unsigned long LanguageNameStartAddr = 0;

unsigned char Lang_EEPROM_State = 0;  // Fail
unsigned short Lang_NowLanguage = 0;

unsigned char Lang_ReloadString = 0;
unsigned char Lang_OldStringNumber = 0xFE;
unsigned char Lang_OldStringType = 0xFE;

unsigned char Lang_CheckState = _Lang_Fail_;  // 20140127
// 
//F_FIND usbfind;
//F_FILE *usbfile;
extern char firmwarePathFilename[50];
extern FIL Fileinfo;
UINT Readsize ;
//------ File ----------------------------------------------------------------//
unsigned char Open_UsbFile(char *by_UsbFileName)
{
    unsigned char ret = 0;
    ret = f_open(&Fileinfo, by_UsbFileName, FA_READ | FA_OPEN_EXISTING);
    return (ret == FR_OK)?_Lang_Success_:_Lang_Fail_;
}
void Close_UsbFile(void)
{
    f_close(&Fileinfo);
}
unsigned char Read_UsbFile(unsigned long by_FileAddress, unsigned char *by_DataBuff, UINT by_ReadSize)
{
    f_lseek(&Fileinfo,by_FileAddress); 
    f_read(&Fileinfo,by_DataBuff,by_ReadSize,&Readsize);
    return _Lang_Success_;
}


//------------------------------------------------------------------------------
// 1. ���EEPROM �ռ� 
// 2. �� Sector(4K) Ϊ��λ
//
//------------------------------------------------------------------------------
unsigned char Erase_EEPROM_Data(unsigned long by_Address)
{
    SPI_Flash_BlockErase(by_Address);//����LANGUAGE�Ĵ洢��
    return _Lang_Success_;
}


//------------------------------------------------------------------------------
// 1. �������ļ�д��EEPROM��
//
// *by_UcbMachineType : �����ַ�����ͬ�������ļ���Ļ����ַ���
// *by_FileName : �ļ���(��·��)
//
// Return : Implemented state
//------------------------------------------------------------------------------
unsigned char Lang_WriteLanguageToEEPRom(unsigned char *by_UcbMachineType, char *by_FileName)
{
    unsigned char by_Return;
    
    by_Return = Open_UsbFile(by_FileName);
    
    if(by_Return == _Lang_Success_)
        by_Return = WriteLanguageToEEPRom(by_UcbMachineType);
    
    Close_UsbFile();
    return by_Return;
  //
}
unsigned char WriteLanguageToEEPRom(unsigned char *by_UcbMachineType)
{
  unsigned long i, j, k;
  unsigned long by_Lang_Addr = 0;
  unsigned char by_BinData;
  unsigned char by_LanguageNumer = 0, by_StringNumber = 0;
  unsigned char by_Lang_Checksum = 0;
//  unsigned short by_LanguageVersion = 0;
  unsigned char by_Data[1024];
  unsigned char *by_WriteOKString = _LanguageWriteOK_;

  
  if(Read_UsbFile(by_Lang_Addr, by_Data, _StringName_) != _Lang_Success_)
      return _Lang_OpenFile_Fail_;
    
  // Check Start Code (1 byte)
  if(by_Data[_StartCode_] != _Lang_Start_Code_)
  {
      return _Lang_FileData_Error_;
  }
  // ��ȡ������ (1 byte)
  by_LanguageNumer = by_Data[_LangAmount_];
  // ��ȡ�ַ����� (1 byte)
  by_StringNumber = by_Data[_StringAmount_];
  // ��ȡ�汾 (2 byte) 
  //by_LanguageVersion = (by_Data[_VersionHighByte_]<<8) + by_Data[_VersionLowByte_];
  //
  by_Lang_Addr += _StringName_;
  by_Lang_Checksum = 0;
  for(i=0;i<_StringName_;i++)
    by_Lang_Checksum += by_Data[i];

  // ������ (�ȶ��ַ���)
  while(1)
  {
      if(Read_UsbFile(by_Lang_Addr++, &by_BinData, 1) != _Lang_Success_)
          return _Lang_OpenFile_Fail_;
          
      by_Lang_Checksum += by_BinData;
      if(by_BinData == 0)
          break;
      else if(by_BinData != *(by_UcbMachineType++))
      {// ���ִ���
          return _Lang_Machine_Error_;
      }    
  }
  // ��ȡ��������(N byte)��������ʼ��ַ(4 bytes)������Checksum
  for(i=0; i<by_LanguageNumer; i++)
  { 
      by_BinData = 1; // �����0���֣��ſ�ִ��while()
      while(by_BinData != 0)
      {
          if(Read_UsbFile(by_Lang_Addr++, &by_BinData, 1) != _Lang_Success_)
              return _Lang_OpenFile_Fail_;
          
          by_Lang_Checksum += by_BinData;
          if(by_BinData == 0)
          {
              for(j=0; j<4; j++)
              {
                  if(Read_UsbFile(by_Lang_Addr++, &by_BinData, 1) != _Lang_Success_)
                      return _Lang_OpenFile_Fail_;     
                  by_Lang_Checksum += by_BinData;
              }
              by_BinData = 0;
          }
      } 
  }
  // �ȶ�Checksum
  if(Read_UsbFile(by_Lang_Addr++, &by_BinData, 1) != _Lang_Success_)
      return _Lang_OpenFile_Fail_;
  
  if(by_Lang_Checksum != by_BinData)
  {// ��������
      return _Lang_FileData_Error_;
  }
  //----- ��һ����������� ---------------------------------------------------
  //--- ���ÿһ�����Ե������� -------------------------------------------------
  for(i=0; i<by_LanguageNumer; i++)
  {
      by_Lang_Checksum = 0;
      for(j=0; j<(by_StringNumber*4); j++)
      {
          if(Read_UsbFile(by_Lang_Addr++, &by_BinData, 1) != _Lang_Success_)
              return _Lang_OpenFile_Fail_;
          
          by_Lang_Checksum += by_BinData;
      }
      if(Read_UsbFile(by_Lang_Addr++, &by_BinData, 1) != _Lang_Success_)
          return _Lang_OpenFile_Fail_;
      
      if(by_Lang_Checksum != by_BinData)
      {// ��������
          return _Lang_FileData_Error_;
      }    
  }
  //--- ����������� -------------------------------------------------------
  //--- ����������� ---------------------------------------------------------
  for(i=0; i<by_LanguageNumer; i++)
  {
      by_Lang_Checksum = 0;
      for(j=0; j<by_StringNumber; j++)
      {
          by_BinData = 1; // �����0���֣��ſ�ִ��while()
          while(by_BinData != 0)
          {
              if(Read_UsbFile(by_Lang_Addr++, &by_BinData, 1) != _Lang_Success_)
                  return _Lang_OpenFile_Fail_;
              by_Lang_Checksum += by_BinData;
          }
      }
      //
      if(Read_UsbFile(by_Lang_Addr++, &by_BinData, 1) != _Lang_Success_)
          return _Lang_OpenFile_Fail_;
      if(by_Lang_Checksum != by_BinData)
      {// ��������
          return _Lang_FileData_Error_;
      } 
  }
  //--- �ȶԽ����� -------------------------------------------------------------
  if(Read_UsbFile(by_Lang_Addr++, &by_BinData, 1) != _Lang_Success_)
      return _Lang_OpenFile_Fail_;
  
  if(by_BinData != _Lang_End_Code_)
  {// ��������
      return _Lang_FileData_Error_;
  }
  if(by_Lang_Addr > _Language_Size_)
  {// ��������
      return _Lang_FileOverSize_;
  }
  //---- Bin��д��EEPROM ------------------------------------------------------------
  if(Erase_EEPROM_Data(_LanguageBin_StartBlock) != _Lang_Success_)
  {
      return _Lang_Erase_Fail_;
  }
  
  k=0;
  while(k<by_Lang_Addr)
  {
      if((by_Lang_Addr - k) <= 1024)
      {
        
          if(Read_UsbFile(k, by_Data, (by_Lang_Addr - k)) != _Lang_Success_)
              return _Lang_WriteRRPRom_Error_;
          
//          Mx25L1606E_WriteBlock((_Language_Data_Address_+k),(by_Lang_Addr - k) ,by_Data) ;
          SPI_Flash_WriteBlock((_Language_Data_Address_ + k),(by_Lang_Addr - k) ,by_Data);
          break;
      }
      else
      {
          if(Read_UsbFile(k, by_Data, 1024) != _Lang_Success_)
            return _Lang_WriteRRPRom_Error_;
  
//          Mx25L1606E_WriteBlock((_Language_Data_Address_+k),256 ,by_Data) ;
          SPI_Flash_WriteBlock((_Language_Data_Address_ + k),1024 ,by_Data);
          k += 1024;
      }
  }
//  //---- д��ȷ�� --------------------------------------------------------------
//  for(k=0; k<by_Lang_Addr; k++)
//  {
//      HAL_Clear_WatchDog();
//      if(Read_UsbFile(k, &by_BinData, 1) != _Lang_Success_)
//          return _Lang_WriteRRPRom_Error_;
//
////      if(by_BinData != Mx25L1606E_ReadByte((_Language_Data_Address_+k)))
//      if(by_BinData != SPI_Flash_ReadByte((_Language_Data_Address_+k)))
//      {
//          return _Lang_WriteRRPRom_Error_;
//      }
//  }
  
  //---- ��EEPROM��д��ȷ���ַ������Ա㿪��ʱ����Ƿ���Language ------------------
//  Mx25L1606E_WriteBlock(_Language_Block_, (_CheckStringSize_-1), by_WriteOKString) ;

//  Mx25L1606E_WriteByte((_Language_Block_+_CheckStringSize_-1), 0);
  SPI_Flash_WriteBlock(_Language_Block_, (_CheckStringSize_-1), by_WriteOKString) ;
  SPI_Flash_WriteByte((_Language_Block_+_CheckStringSize_-1), 0);
  
//  for(k=0;k<(_CheckStringSize_-1);k++)
//  {
////      if(*(by_WriteOKString+k) != Mx25L1606E_ReadByte((_Language_Block_+k)))
//      if(*(by_WriteOKString+k) != SPI_Flash_ReadByte((_Language_Block_+k)))
//        return _Lang_WriteRRPRom_Error_;
//  }
  return _Lang_Success_;
}

void Language_ClearBasicInfo(void)
{
    Lang_LanguageAmount = 0;
    Lang_StringAmount = 0;
    Lang_Version = 0;
}

//------------------------------------------------------------------------------
// 1. ȷ���Ƿ���Language��EEPROM
// 2. �ȶ�����Ļ����ַ�����EEPROM�Ļ����ַ����Ƿ�һ�� 
//
// *by_UCBTypeString : �����ַ�����ͬ�������ļ���Ļ����ַ���
// Return : Implemented state
//------------------------------------------------------------------------------
unsigned char Language_EEPROMCheck(unsigned char *by_UCBTypeString)
{// 
    unsigned char by_CheckData[_CheckStringSize_];
    unsigned char i = 0;
    unsigned char *by_WriteOKString = _LanguageWriteOK_;
    
    Lang_CheckState = _Lang_Success_;  // 20140127
    //
//    Mx25L1606E_ReadBlock( _Language_Block_,_CheckStringSize_, by_CheckData );
    SPI_Flash_ReadBlock( _Language_Block_,_CheckStringSize_, by_CheckData );
    // 
    for(i=0; i<(_CheckStringSize_-1); i++)
    {
        if(by_CheckData[i] != *(by_WriteOKString+i))
        {
            Lang_CheckState = _Lang_EEPROMData_Error_; // 20140127
            break;
            //return _Lang_EEPROMData_Error_;
        }
    }    
    //
    if(Lang_CheckState == _Lang_Success_)
    {// �ȶԻ����ַ���
        i = 0;
        while(1)
        {
            by_CheckData[0] = SPI_Flash_ReadByte((_Language_Data_Address_ + _StringName_ + i));
            if(*(by_UCBTypeString + i) != by_CheckData[0])
            {
                Lang_CheckState = _Lang_Machine_Error_; // 20140127
                break;
                //return _Lang_Machine_Error_; 
            }
            else if(by_CheckData[0] == 0)
                break;
            i++;
        }
    }
    
    return Lang_CheckState; // 20140127
} 

//------------------------------------------------------------------------------
// 1. �������������ȷ��
// 2. �趨�������Ե�����������ַ
// 
// by_NowLanguage : ���Դ���( 1 ~ ), 0����Ӣ��
// Return : Implemented state
//------------------------------------------------------------------------------
unsigned char Language_LanguageChecking(unsigned char by_NowLanguage)
{
    unsigned long by_Count = _Language_Data_Address_;
    unsigned long i = 0, j = 0;
    unsigned char by_Dat, by_Lang_Checksum = 0;
    
    if(Lang_CheckState != _Lang_Success_) 
    {
        Language_ClearBasicInfo();
        Lang_EEPROM_State = Lang_CheckState;
        return Lang_EEPROM_State;
    }

    Lang_NowLanguage = by_NowLanguage;
    Lang_ReloadString = 1;    
    Lang_EEPROM_State = _Lang_Success_;
       
    if(SPI_Flash_ReadByte(by_Count) != _Lang_Start_Code_)
    {// Check StartCode // EEPROM ���ݴ���
        Language_ClearBasicInfo();
        
        Lang_EEPROM_State = _Lang_EEPROMData_Error_;
        return Lang_EEPROM_State;
    }
    else
    {// Check language name and String index start address
          by_Lang_Checksum = _Lang_Start_Code_; // 0
          by_Count = _Language_Data_Address_ + 1;
          Lang_LanguageAmount = SPI_Flash_ReadByte(by_Count++); // 1
          Lang_StringAmount = SPI_Flash_ReadByte(by_Count++); // 2
          by_Lang_Checksum = by_Lang_Checksum + Lang_LanguageAmount + Lang_StringAmount;
          //
          Lang_Version = 0;
          by_Dat = SPI_Flash_ReadByte(by_Count++);
          by_Lang_Checksum += by_Dat;
          Lang_Version = (Lang_Version<<8) +by_Dat; 
          by_Dat = SPI_Flash_ReadByte(by_Count++);
          by_Lang_Checksum += by_Dat;
          Lang_Version = (Lang_Version<<8) +by_Dat; 
          
          //
          while(1)
          {// �����ַ���
              by_Dat = SPI_Flash_ReadByte(by_Count++);
              by_Lang_Checksum += by_Dat;
              if(by_Dat == 0)
              {
                  break;
              }
          }
          
          // ���������ַ���
          LanguageNameStartAddr = by_Count;
          by_LanguageIndexTableStartAddr = 0;
          for(i=0; i<Lang_LanguageAmount; i++)
          {
              by_Dat = 1;
              while(by_Dat != 0)
              {
                  by_Dat = SPI_Flash_ReadByte(by_Count++);
                  by_Lang_Checksum += by_Dat;
              }
              for(j=0; j<4; j++)
              {// Language Index Table Start Addr
                  by_Dat = SPI_Flash_ReadByte(by_Count++);
                  by_Lang_Checksum += by_Dat;
                  
                  if(Lang_NowLanguage != 0 && i == (Lang_NowLanguage - 1))
                  {
                      by_LanguageIndexTableStartAddr = (by_LanguageIndexTableStartAddr<<8) + by_Dat;
                  }
              }             
          }
          by_Dat = SPI_Flash_ReadByte(by_Count++);
          if(by_Lang_Checksum != by_Dat)
          {//EEPROM ���ݴ���
              Language_ClearBasicInfo();
              
              Lang_EEPROM_State = _Lang_EEPRomData_Error_;
              return Lang_EEPROM_State;
          }
    }
    //--------------------------------------------------------------------------
    if(Lang_NowLanguage == 0)
    {// ���Դ���Ϊ0(English), �����index table and Data
        return _Lang_Success_;
    }
    // Check the language index table 
    by_LanguageIndexTableStartAddr += _Language_Data_Address_;
    by_Lang_Checksum = 0;
    by_Count = by_LanguageIndexTableStartAddr;
    by_LanguageDataStartAddr = 0;
    for(i=0; i<(Lang_StringAmount*4) ; i++)
    {
        by_Dat = SPI_Flash_ReadByte(by_Count++);
        by_Lang_Checksum += by_Dat;
        
        if(i<4)
        {
            if(i == 0 && by_Dat != Lang_NowLanguage)
            {// ���Դ���
                Lang_EEPROM_State = _Lang_Language_Error_;
                return Lang_EEPROM_State;
            }
            else
                by_LanguageDataStartAddr = (by_LanguageDataStartAddr<<8) + by_Dat;
        }
    }
    by_Dat = SPI_Flash_ReadByte(by_Count++);
    if(by_Lang_Checksum != by_Dat)
    {//EEPROM ���ݴ���
        Lang_EEPROM_State = _Lang_Language_Error_;
        return Lang_EEPROM_State;
    }
    by_LanguageDataStartAddr = (by_LanguageDataStartAddr&0x00FFFFFF) + _Language_Data_Address_;
    //--------------------------------------------------------------------------
    // Check the language data 
    by_Lang_Checksum = 0;
    by_Count = by_LanguageDataStartAddr;
    for(i=0; i<Lang_StringAmount; i++)
    {
        by_Dat = 1;
        while(by_Dat != 0x00)
        {
            by_Dat = SPI_Flash_ReadByte(by_Count++);
            by_Lang_Checksum += by_Dat;
        }
    }
    by_Dat = SPI_Flash_ReadByte(by_Count++);
    if(by_Lang_Checksum != by_Dat)
    {//EEPROM ���ݴ���
        Lang_EEPROM_State = _Lang_Language_Error_;
        return Lang_EEPROM_State;
    }  
    
    Lang_EEPROM_State = Language_GetString(_Lang_Language_Name_,Lang_NowLanguage);
    return Lang_EEPROM_State;
}
//------------------------------------------------------------------------------
// 1. ��EEPROM���ȡ�ַ������ַ������� Lang_StringBuffer[] �˾�����
//
// Lang_StringType     : �ַ�����̬ ==> �������� (_Lang_Language_Name_)
//                                ==> ���������ַ������� (_Lang_Language_Data_)
// Lang_StringBuffer[]  : �ַ���buffer
// Lang_StringNumber   : �ַ������(����)
//                       ���� Lang_StringType ����������(1)����˱����������Դ��� ==> �� 1 ��ʼ
//                       ���� Lang_StringType �Ǹ��������ַ�������(2)����˱��������ַ������ ==> �� 0 ��ʼ
// Return : Implemented state
//------------------------------------------------------------------------------
unsigned char Language_GetString(unsigned char Lang_StringType, unsigned char Lang_StringNumber)
{
    unsigned long by_StringDataStartAddr = 0;
    unsigned long by_StringIndexTableAddr = 0;
    unsigned char by_Dat;
    unsigned char i;
    unsigned short by_WordCount = 0;
    unsigned char *Lang_String;
    static unsigned char StringState = _Lang_Success_;
    
    if(Lang_CheckState != _Lang_Success_)// 20140127
    {
        return Lang_CheckState;
    }
    if(Lang_EEPROM_State != _Lang_Success_) 
    {
        return Lang_EEPROM_State;
        //return _Lang_EEPRomData_Error_;
    }
    
    if(Lang_NowLanguage < 1 || Lang_NowLanguage > Lang_LanguageAmount)
    {// ��EEPROM�ﴢ�������  20130128 
        Lang_ReloadString = 0;
        
        Lang_EEPROM_State = _Lang_Fail_;
        return Lang_EEPROM_State;
    }
    
    if(Lang_ReloadString == 1)
    {
        Lang_ReloadString = 0;
        Lang_OldStringNumber = Lang_StringNumber;
        Lang_OldStringType = Lang_StringType;
    }
    else if( Lang_StringNumber != Lang_OldStringNumber || Lang_StringType != Lang_OldStringType)
    {
        Lang_OldStringNumber = Lang_StringNumber;
        Lang_OldStringType = Lang_StringType; 
    }
    else
    {
        return StringState;
        //return _Lang_Success_;
    }
    
    Lang_String = Lang_StringBuffer;
    
    if(Lang_StringType == _Lang_Language_Name_)
    {
        if(Lang_StringNumber == 0 || Lang_StringNumber > Lang_LanguageAmount)
        {// Data Address Error // ��Ŵ���
            StringState = _Lang_StringNumber_Error_;
            return StringState;
            //return _Lang_StringNumber_Error_;
        }
        
        by_StringDataStartAddr = LanguageNameStartAddr;  
        for(i=0; i<Lang_StringNumber; i++)
        {    
            by_Dat = 1;
            by_WordCount = 0;
            while(by_Dat != 0)
            {
                by_Dat = SPI_Flash_ReadByte(by_StringDataStartAddr++);
                if(i == (Lang_StringNumber - 1))
                    *(Lang_String++) = by_Dat;
                
                by_WordCount += 1;
                if(by_WordCount >= _TheWordsOfOneString_)
                {// �ַ�������
                    StringState = _Lang_OverString_;
                    return StringState; 
                    //return _Lang_OverString_;
                }  
            }
            by_StringDataStartAddr += 4; 
                   
            if(i == (Lang_StringNumber - 1))       
                *Lang_String = 0;   
        }
    }
    else
    {
        by_StringIndexTableAddr = by_LanguageIndexTableStartAddr + Lang_StringNumber*4;
        if(Lang_StringNumber > (Lang_StringAmount - 1))
        {// Data Address Error // ��Ŵ���
            StringState = _Lang_StringNumber_Error_;
            return StringState;
            //return _Lang_StringNumber_Error_;
        }
        //Index Table�������ÿ4 bytesΪһ�飬��1��byte��Ϊ���Ա��,�������������ַ������ݵ���ʼ��ַ
        by_Dat = SPI_Flash_ReadByte(by_StringIndexTableAddr++);
        if(by_Dat != Lang_NowLanguage) 
        {// Data Address Error // ���Դ���
            StringState = _Lang_EEPRomData_Error_;
            return StringState; 
            //return _Lang_EEPRomData_Error_;
        }
        for(i=0; i< 3; i++)
        {
            by_Dat = SPI_Flash_ReadByte(by_StringIndexTableAddr++);
            by_StringDataStartAddr = (by_StringDataStartAddr<<8) + by_Dat;
        }
        by_StringDataStartAddr += _Language_Data_Address_;   
        //
        by_Dat = 1;
        by_WordCount = 0;
        while(by_Dat != 0)
        {
            by_Dat = SPI_Flash_ReadByte(by_StringDataStartAddr++);
            *(Lang_String++) = by_Dat;
            by_WordCount += 1;
            if(by_WordCount >= _TheWordsOfOneString_)
            {// �ַ�������
                StringState = _Lang_OverString_;
                return StringState; 
                //return _Lang_OverString_;
            }
        }
        *Lang_String = 0;
    }
    
    StringState = _Lang_Success_;
    return StringState; 
    //return _Lang_Success_;
}

//------------------------------------------------------------------------------
// ��ȡ��������
//------------------------------------------------------------------------------
unsigned char Language_GetLanguageAmount(void)
{
    return Lang_LanguageAmount;
}


//------------------------------------------------------------------------------
// ��ȡ�����ļ��汾
//------------------------------------------------------------------------------
unsigned short Language_Version(void)
{
    return Lang_Version;
}














