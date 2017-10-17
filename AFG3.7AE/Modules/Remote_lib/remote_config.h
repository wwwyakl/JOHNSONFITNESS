#ifndef __REMOTE_CONFIG_H__
#define __REMOTE_CONFIG_H__
/****************************************************************************
*
*
*
*
*
*
****************************************************************************/


#ifndef __REMOTE_LIB__
#define __REMOTE_LIB__
#endif

#ifndef __REMOTE_USER_MANAGE__
#define __REMOTE_USER_MANAGE__
#endif

#ifndef __REMOTE_ASSET_MANAGE__
//#define __REMOTE_ASSET_MANAGE__
#endif

#ifndef __REMOTE_WORKOUT_TRACKING__
#define __REMOTE_WORKOUT_TRACKING__
#endif

#ifndef __REMOTE_CONTROL__
#define __REMOTE_CONTROL__
#endif


#ifndef __REMOTE_GENERAL__
#define __REMOTE_GENERAL__
#endif



#ifndef __WIFI_FUNC__
//#define __WIFI_FUNC__
#endif

#ifdef __WIFI_FUNC__
#define __HIFLY_A11__
#endif

#ifndef __BLUETOOTH_FUNC__
// #define __BLUETOOTH_FUNC__
#define __BLUETOOTH_WLT2654__
#endif

#ifndef __BLUETOOTH_WLT2654__
//#define __BLUETOOTH_WLT2654__
#endif

#ifndef MAX_DEVICES_COUNT
#define MAX_DEVICES_COUNT 2
#endif

#ifdef __REMOTE_USER_MANAGE__
#define XIDMAXLENGTH 14
#define DEFAULTXID  "00000000000000"
#endif

#ifndef WIFISSID
#define WIFISSID     "MATRIX XR"
#endif

#endif
