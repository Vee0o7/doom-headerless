#ifndef INCLUDE_H
#define INCLUDE_H

#define SILENT

#undef NULL
#define NULL 0

#define INT_MAX 2147483647
#define INT_MIN (-INT_MAX - 1)
#define SHRT_MAX 32767

#define halt __halt
extern void halt();

#define abs __abs
static inline int
abs(int num) {
    if (num  >= 0) return num;
    return -num;
}

#define toupper __toupper
static inline int
toupper(int c) {
    if ('a' <= c && c <= 'z')
        return c - 0x20;
    return c;
}

#include "../std_r/stdint.h"
#include "../std_r/string.h"

extern void* memcpy(void* dest, const void* src, size_t count);

static inline char*
sprint_str(char* into, const char* str) {
    while (*str) *into++ = *str++;
    *into = 0;
    return into;
}

int printf(const char* restrict format, ...);
static inline char*
sprint_int(char* into, int i, int l) {
    int d = 1;
    if (l == 0) {
        while (d <= i) d *= 10;
        if (d > 1) d /= 10;
    } else {
        l--;
        while (l--) d *= 10;
    }

    while (d) {
        *into++ = '0' + i / d;
        i %= d;
        d /= 10;
    }
    *into = 0;
    return into;
}

#ifndef DOOM64
int printf(const char* restrict format, ...);
#define I_Error(...) do {printf(__VA_ARGS__); halt();} while(0)
#define I_NonfatalError(...) do {printf(__VA_ARGS__);} while(0)
#define I_Info(...) do {printf(__VA_ARGS__);} while(0)
#define I_InfoBit(...)
#define I_InfoInt(...)
#else

#ifdef SILENT
#define I_Error(...)
#define I_NonfatalError(...)
#define I_Info(...)
#define I_InfoBit(i, v)
#define I_InfoInt(i, v)
#else
#define I_Error(...) do {halt();} while(0);
#define I_NonfatalError(...) do {*(uint8_t*) (32 * 1024*1024 + 1) = 0xf0;} while(0);
#define I_Info(...) do {*(uint8_t*) (32 * 1024*1024) = 0xff;} while(0);
#define I_InfoBit(i, v) do { asm volatile(""); *(uint8_t*) (32 * 1024*1024 + (i)) = v; asm volatile("");} while(0);
#define I_InfoInt(i, v) do { I_InfoBit(i, 0b00011100); I_InfoBit(i+8, 0b00011100); I_InfoBit(i+16, 0b00011100); I_InfoBit(i+24, 0b00011100); I_InfoBit(i+32, 0b00011100); for(int j = 0; j < 32; ++j) { I_InfoBit(i+160 + 32 - j, ((v >> j) & 1) ? 0b11100000 : (j & 1) ? 0b00100100 : 0);} } while(0);
#endif
#endif

extern uint8_t palette_map[256];
#endif
