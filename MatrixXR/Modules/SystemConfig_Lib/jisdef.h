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

#include "integer.h"
#include <stdbool.h>
#include "stdio.h"
#include "string.h"
#ifndef _INTEGER



typedef signed   int  INT;
typedef unsigned   int  UINT;

typedef unsigned char     UCHAR;
typedef unsigned char       BYTE;

typedef unsigned short      WORD;
typedef unsigned long       DWORD;
#endif

typedef char     SCHAR;
typedef void              VOID;
typedef void *            PVOID;

typedef signed char       INT8;
typedef signed char *     PINT8;
typedef unsigned char     UINT8;
typedef unsigned char *   PUINT8;


typedef signed   int*  PINT;

typedef unsigned   int*  PUINT;

typedef signed char *     PCHAR;

typedef unsigned char *   PUCHAR;
typedef signed char *            PSTR;
typedef const signed char *      PCSTR;

typedef short             INT16;
typedef short *           PINT16;
typedef unsigned short    UINT16;
typedef unsigned short *  PUINT16;

#if defined (__MSP430F1491__) || defined(__MSP430F149__) || defined (__MSP430F249__) || defined (__MSP430F2491__)
typedef signed long long    INT64;
typedef unsigned long long  UINT64;
typedef signed long       INT32;
typedef signed long *     PINT32;
typedef unsigned long     UINT32;
typedef unsigned long *   PUINT32;

#elif defined (__IOR8C2A_H__)

typedef signed long       INT32;
typedef signed long *     PINT32;
typedef unsigned long     UINT32;
typedef unsigned long *   PUINT32;

#else

typedef signed long        INT32;
typedef signed long *      PINT32;
typedef unsigned long      UINT32;
typedef unsigned long *    PUINT32;
#endif


typedef float             FLOAT;
typedef float *           PFLOAT;

typedef double            DOUBLE;
typedef double *          PDOUBLE;


typedef unsigned char *     PBYTE;

typedef unsigned short *    PWORD;

typedef unsigned long *     PDWORD;

typedef void (*PF)(void);


#ifndef NULL
#define NULL   ((void *) 0)
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

#define FUNCTION_AUTO_MODEL  


#define __WIFI_FUNC__

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
