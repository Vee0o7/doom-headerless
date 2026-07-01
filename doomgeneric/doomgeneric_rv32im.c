#include "doomgeneric_defines.h"

#include "include.h"
#include "doomkeys.h"
#include "doomgeneric.h"
#include "i_video.h"
#include "rv32Funcs.h"
#include "wad.h"


void setupScreen(int op, int val) {
  asm("li a7, 0b1100");
  asm("ecall");
}

static unsigned char convertToDoomKey(unsigned int key) {
	switch (key)
	{
    case 'a':
        key = KEY_STRAFE_L;
        break;
    case 'd':
        key = KEY_STRAFE_R;
        break;
    case 16:
    case 'w':
        key = KEY_UPARROW;
        break;
    case 18:
    case 's':
        key = KEY_DOWNARROW;
        break;
    case 'h':
        key = KEY_LEFTARROW;
        break;
    case 'l':
        key = KEY_RIGHTARROW;
        break;
    case ' ':
        key = KEY_FIRE;
        break;
    case 59: // key below ESC
        key = KEY_ESCAPE;
        break;
    case 'j':
        key = KEY_USE;
        break;
    case '\n':
        key = KEY_ENTER;
        break;
	default:
		break;
	}

	return key;
}


void DG_Init() {
	DG_ScreenBuffer = (uint8_t*) (32 * 1024 * 1024);
}

/* For CMAP256.  */
void DH_remap_palette(uint8_t remap[256]) {
	for (size_t i = 0; i < 256; ++i) {
    struct color c = colors[i];
    uint8_t r, g, b;

		r = ((uint8_t)(c.r >> 5)) << 5;
		g = ((uint8_t)(c.g >> 5)) << 2;
		b = ((uint8_t)(c.b >> 6)) << 0;
		remap[i] = (r | g | b);
	}
}

void DG_DrawFrame() { }

void DG_SleepMs(uint32_t ms) { }

uint32_t DG_GetTicksMs() {
	int ticks;
  asm("li a7, 0b1000");
  asm("ecall");

	return ticks * 11;
}

int DG_GetKey(int* pressed, unsigned char* doomKey)
{
	int ret;
  ret = readKey();
	*pressed = ret >> 8;
	*doomKey = convertToDoomKey(ret & 0xff);
	return *doomKey;
}

__attribute__((used))
int start()
{
	/* Dummy arguments that are expected to detect WAD type.  */
  setupScreen(0, 2);
  setupScreen(1, (int) DG_ScreenBuffer);

	char* args[3] = {"./doomgeneric", "-iwad", "/usr/share/games/doom/doom1.wad"};
	doomgeneric_Create(3, args);

	while(1) {
		doomgeneric_Tick();
	}

	return 0;
}


void DH_read_wad(unsigned int offset, void *buffer, size_t buffer_len) {
    memcpy(buffer, wad_file + offset, buffer_len);
}

/* One time allocation for Doom allocator.  */
uint8_t* zone = (uint8_t*) (64 * 1024 * 1024);
__attribute__((noipa))
uint8_t *I_ZoneBase (int *size) {
    *size = 16 * 1024 * 1024;
    return zone;
}

/* One time allocation.  */
__attribute__((noipa))
void* malloc_lump_info (size_t s) {
    return (void*) (80 * 1024 * 1024);
}

void halt() {
    asm ("ebreak");
}

/* Should be fast.  */
void* memcpy(void* dest, const void* src, size_t count) {
    uint8_t* d = (uint8_t*) dest;
    uint8_t* s = (uint8_t*) src;

    int i = 0;

    for (; i < (int)count - 3; i += sizeof(uint32_t)) {
        *(uint32_t*)(d + i) = *(uint32_t*)(s + i);
    }

    for (; i < count; ++i) {
        d[i] = s[i];
    }

    return dest;
}


/* Reimplement missing instructions.  */

int32_t
__divsi3 (int32_t a, int32_t b) {
	uint32_t ua = abs(a);
	uint32_t ub = abs(b);
	int s = (a < 0) != (b < 0);

	uint32_t ur = ua/ub;

  if (s) return -ur;
	return ur;
}

int32_t
__modsi3 (int32_t a, int32_t b) {
  uint32_t ub = abs(b);
  if (a < 0) {
    uint32_t ua = -a;
    return -ua%ub;
  } else {
    return (uint32_t)a%ub;
  }
}
