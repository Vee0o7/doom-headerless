#ifndef STDINT_H
#define STDINT_H

#ifdef __unix__
#include <stdint.h>
typedef uint64_t size_t;
#else
typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;

typedef uint32_t size_t;
typedef size_t uintptr_t;
#endif

#endif
