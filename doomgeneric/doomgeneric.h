#ifndef DOOM_GENERIC
#define DOOM_GENERIC

#include "include.h"
#include "doomgeneric_defines.h"

//#define RANGECHECK

#ifdef CMAP256

typedef uint8_t pixel_t;

#else  // CMAP256

typedef uint32_t pixel_t;

#endif  // CMAP256


extern pixel_t* DG_ScreenBuffer;

void doomgeneric_Create(int argc, char **argv);
void doomgeneric_Tick();


//Implement below functions for your platform
void DG_Init();
void DG_DrawFrame();
void DG_SleepMs(uint32_t ms);
uint32_t DG_GetTicksMs();
int DG_GetKey(int* pressed, unsigned char* key);

void DH_remap_palette(uint8_t remap[256]);
void DH_setup_screen_info();
void DH_read_wad(unsigned int offset, void *buffer, size_t buffer_len);
uint8_t *I_ZoneBase (int *size);
void* malloc_lump_info (size_t s);
void* memcpy(void* dest, const void* src, size_t count);

#endif //DOOM_GENERIC
