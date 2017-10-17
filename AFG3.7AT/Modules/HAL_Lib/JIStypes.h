/****************************************************************************
*
* FILENAME
*     JIStypes.h
*
* VERSION
*     2.0
*
* DESCRIPTION
*     This PreDefined data types for JIS RD
*
* DATA STRUCTURES
*     None
*
* FUNCTIONS
*     None
*
* HISTORY
*     08/10/29       Ver 1.0 Created by Liugt
*			08/12/02       modified by Liugt
* REMARK
*     None
**************************************************************************/

#ifndef _JISTYPES_H_
#define _JISTYPES_H_

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
typedef unsigned long long  UINT64;
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

#endif /* _JISTYPES_H */
