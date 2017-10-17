#ifndef _UtilitiesUserEdit_H_
#define _UtilitiesUserEdit_H_


#endif
struct str_Program_Menu_Item
{
  unsigned char* Display_String;
  char Menu_Value;
};
struct Delete_User
{
  char Menu_Length;
  struct str_Program_Menu_Item (*Menu_List)[];
};
char Wifi_Connect(void);
char Utl_isExisting_user(char userid);
unsigned char* Utl_Get_User_Name(char userid);
//char Utl_Delete_User();
unsigned char* Utl_Get_Delete_User(char index);
unsigned char Utl_Get_Delete_User_Length();

