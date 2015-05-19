#ifndef GLOW_INTEGERS_H_
#define GLOW_INTEGERS_H_

#include <limits.h>

#if UINT_MAX == 0xFFFFFFFF
typedef int glow_int32;
typedef unsigned int glow_uint32;
#elif ULONG_MAX == 0xFFFFFFFF
typedef long int glow_int32;
typedef unsigned long int glow_uint32;
#endif

#if UINT_MAX == 0xFFFF
typedef int glow_int16;
typedef unsigned int glow_uint16;
#elif USHRT_MAX == 0xFFFF
typedef short int glow_int16;
typedef unsigned short int glow_uint16;
#endif

#if UCHAR_MAX == 0xFF
typedef char glow_int8;
typedef unsigned char glow_uint8;
#elif USHRT_MAX == 0xFF
typedef short int glow_int8;
typedef unsigned short int glow_uint8;
#endif

#if ULONG_MAX == 0xFFFFFFFFFFFFFFFF
typedef long int glow_int64;
typedef unsigned long int glow_uint64;
#elif ULLONG_MAX == 0xFFFFFFFFFFFFFFFF
typedef long long int glow_int64;
typedef unsigned long long int glow_uint64;
#endif

/*!
 * machine word size
 */
typedef unsigned long glow_word;


typedef float glow_float32;
typedef double glow_float64;


#endif // GLOW_INTEGERS_H_
