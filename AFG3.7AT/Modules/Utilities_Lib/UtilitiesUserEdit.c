#include "UtilitiesUserEdit.h"
#include <wifi_interface.h>
#include "SystemConfig.h"
#include "Strings.h"

struct Delete_User Delete_Item;// = {0};
USERS_INFO User_info ;
char sprintfBuf[16];
//struct str_Program_Menu_Item _menu_items[] = //const UCHAR* Strings[STRINGIDEND][LANGUAGEIDEND]
//      {
//        {(unsigned char *)Strings[STR_USER1_ID][0], 0},
//        {(unsigned char *)Strings[STR_USER2_ID][0], 1},
//        {(unsigned char *)Strings[STR_USER3_ID][0], 2},
//        {(unsigned char *)Strings[STR_USER4_ID][0], 3}
//      };
//void Utl_Edit_user_initial()
//{
//memset( Delete_Item, 0, sizeof(USERS_INFO) );
//}
char Wifi_Connect(void)
{
  
  if(SystemConfig_Get_Char( AP_MODE_FLAG_UCHAR )==0)
  {
  return 0;
  }
  if(WIFIIsOnline())
  {
  return 1;
  }
  
}
char Utl_isExisting_user(char userid)
{
  User_info=SystemConfig_Get_UserInfo(userid);
//  if(userid == PU_GUEST || userid == PU_EDIT_USER || userid == PU_DELETE_USER)
//    return 0;
  
  return ((User_info.name[0]!=0x00)||
          ( strncmp(User_info.xid, DEFAULTXID, XIDMAXLENGTH) != 0 ));
}
unsigned char* Utl_Get_User_Name(char userid)
{
  User_info=SystemConfig_Get_UserInfo(userid);
//  if(userid == PU_GUEST || userid == PU_EDIT_USER || userid == PU_DELETE_USER)
//    return 0;
  if(User_info.name[0]!=0)
  {
     return User_info.name;
  }
  else if( strncmp(User_info.xid, DEFAULTXID, XIDMAXLENGTH) != 0 )
  {
    memcpy(sprintfBuf, User_info.xid, XIDMAXLENGTH);
    sprintfBuf[XIDMAXLENGTH] = '\0';
    return sprintfBuf;
  }
}
//char Utl_Delete_User()
//{
//  unsigned char index = 0;
//  struct str_Program_Menu_Item _menu_items[] = //const UCHAR* Strings[STRINGIDEND][LANGUAGEIDEND]
//      {
//        {(unsigned char *)Strings[STR_USER1_ID][0], 0},
//        {(unsigned char *)Strings[STR_USER2_ID][0], 1},
//        {(unsigned char *)Strings[STR_USER3_ID][0], 2},
//        {(unsigned char *)Strings[STR_USER4_ID][0], 3}
//      };
//  Delete_Item.Menu_List =  &_menu_items;
//  for(char userid=0;userid<=3;userid++)
//  {
//    if(Utl_isExisting_user(userid))
//    {
//      _menu_items[index].Menu_Value = userid;
//      _menu_items[0].Display_String = Utl_Get_User_Name(userid);
////      Main_Get_UserName_For_User(_menu_items[0].Menu_Value);
////      Delete_Item->Menu_List[index].Display_String=Utl_Get_User_Name(userid);
//      Delete_Item.Menu_Length++;
//      index++;
//    }
//  }
////  if(Delete_Item.Menu_Length==0)
////  {
////  return 0;
////  }
//  return Delete_Item.Menu_Length;
//}
unsigned char* Utl_Get_Delete_User(char index)
{
//    return Delete_Item.Menu_List[index].Display_String;
    return Utl_Get_User_Name(index);
}
unsigned char Utl_Get_Delete_User_Length()
{
    return Delete_Item.Menu_Length;
}
//unsigned char Utl_Get_Delete_Userid(char index)
//{
//    return _menu_items[index].Menu_Value; 
//}
