#ifndef CTYPES_H
#define CTYPES_H

#define uint8_t  unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#define int8_t  signed char
#define int16_t signed short
#define int32_t signed int
#undef NULL
#define NULL  ((void *)0)

typedef enum
{
    false = 0,
    true = 1,
} boolean_t;

#define bool boolean_t

#endif
