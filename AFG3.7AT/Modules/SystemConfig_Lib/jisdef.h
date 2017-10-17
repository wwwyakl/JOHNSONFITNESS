/****************************************************************************
 *
 * FILENAME
 *     JISdef.h
 *
 * VERSION
 *     2.0
 *
 * DESCRIPTION
 *     This file contains basic macros and inline functions.
 *
 * DATA STRUCTURES
 *     None
 *
 * FUNCTIONS
 *     None
 *
 * HISTORY
 *     08/10/29       Ver 1.0 Created by Liugt
 *     08/12/05       Ver 2.0 added macro BV BF
 *
 * REMARK
 *     None
 **************************************************************************/

#ifndef _JISDEF_H_
#define _JISDEF_H_

#ifndef _JISTYPES_H_
#include "JIStypes.h"
#endif


#ifndef CONST
  #define CONST             const
#endif

#ifndef FALSE
  #define FALSE             false
#endif

#ifndef TRUE
  #define TRUE              true
#endif

#ifndef FAIL
  #define FAIL      -1
#endif

#ifndef SUCCEFULL
  #define SUCCEFULL  1
#endif

//----------------------------------------------------------------------------------
//  Macros
//----------------------------------------------------------------------------------

//#define RF_MODULE_EXIST_

#define _4_16C24_Display_
//#define _6_16C24_Display_

//#define FUNCTION_AUTO_MODEL  

#define __BLUETOOTH_FUNC__
//#define __WIFI_FUNC__

#ifndef BV
#define BV(n)      (1 << (n))
#endif

#ifndef BF
#define BF(x,b,s)  (((x) & (b)) >> (s))
#endif

#define Maximum(a,b)          (a>b ? a : b)
#define Minimum(a,b)          (a<b ? a : b)
#define Middle(a,b)           ((a+b)/2)
#define Equal(a,b)            (a==b ? 1 : 0)
#define NotEqual(a,b)         (a==b ? 0 : 1)
#define GreaterEqual(a,b)     (a>=b ? 1 : 0)
#define LittleEqual(a,b)      (a<=b ? 1 : 0)

static inline UINT16 Swap16(UINT16 val)
{
    return (val<<8) | (val>>8);
}

static inline UINT32 Swap32(UINT32 val)
{
    return (val<<24) | ((val<<8)&0xff0000) | ((val>>8)&0xff00) | (val>>24);
}

static inline UINT16 Get16(PUINT8 addr)
{
    return ((addr[1]<<8) | addr[0]);
}
#ifdef ST_STM32
static inline UINT32 Get32(PUINT8 addr)
{
    return (addr[3]<<24) | (addr[2]<<16) | (addr[1]<<8) | addr[0];
}
#endif
static inline void Set16(UINT16 val, PUINT8 addr)
{
    addr[0] = (UINT8) val;
    addr[1] = (UINT8)(val >> 8);
}

static inline void Set32(UINT32 val, PUINT8 addr)
{
    addr[0] = (UINT8) val;
    addr[1] = (UINT8)(val >> 8);
    addr[2] = (UINT8)(val >> 16);
    addr[3] = (UINT8)(val >> 24);
}
#endif /*  _JISDEF_H_ */
