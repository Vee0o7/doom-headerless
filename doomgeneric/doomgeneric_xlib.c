#define _DEFAULT_SOURCE

#define ONLY_UNIX
#include <ctype.h>

#include "include.h"

#include "doomkeys.h"

#include "doomgeneric.h"
#include "i_video.h"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/time.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/XKBlib.h>

static Display *s_Display = NULL;
static Window s_Window = 0;
static int s_Screen = 0;
static GC s_Gc = 0;
static XImage *s_Image = NULL;

#define KEYQUEUE_SIZE 16

static unsigned short s_KeyQueue[KEYQUEUE_SIZE];
static unsigned int s_KeyQueueWriteIndex = 0;
static unsigned int s_KeyQueueReadIndex = 0;

static unsigned char convertToDoomKey(unsigned int key)
{
	switch (key)
	{
    case 'a':
        key = KEY_STRAFE_L;
        break;
    case 'd':
        key = KEY_STRAFE_R;
        break;
    case 'w':
        key = KEY_UPARROW;
        break;
    case 's':
        key = KEY_DOWNARROW;
        break;
    case XK_Return:
		key = KEY_ENTER;
		break;
    case XK_Escape:
		key = KEY_ESCAPE;
		break;
    case XK_Left:
		key = KEY_LEFTARROW;
		break;
    case XK_Right:
		key = KEY_RIGHTARROW;
		break;
    case XK_Up:
		key = KEY_UPARROW;
		break;
    case XK_Down:
		key = KEY_DOWNARROW;
		break;
    case XK_Control_L:
    case XK_Control_R:
		key = KEY_FIRE;
		break;
    case XK_space:
		key = KEY_USE;
		break;
    case XK_Shift_L:
    case XK_Shift_R:
		key = KEY_RSHIFT;
		break;
	default:
		key = tolower(key);
		break;
	}

	return key;
}

static void addKeyToQueue(int pressed, unsigned int keyCode)
{
	unsigned char key = convertToDoomKey(keyCode);

	unsigned short keyData = (pressed << 8) | key;

	s_KeyQueue[s_KeyQueueWriteIndex] = keyData;
	s_KeyQueueWriteIndex++;
	s_KeyQueueWriteIndex %= KEYQUEUE_SIZE;
}

void DG_Init()
{
	DG_ScreenBuffer = malloc(DOOMGENERIC_RESX * DOOMGENERIC_RESY * 4);

	memset(s_KeyQueue, 0, KEYQUEUE_SIZE * sizeof(unsigned short));

    // window creation

    s_Display = XOpenDisplay(NULL);

    s_Screen = DefaultScreen(s_Display);

    int blackColor = BlackPixel(s_Display, s_Screen);
    int whiteColor = WhitePixel(s_Display, s_Screen);

    XSetWindowAttributes attr;
    memset(&attr, 0, sizeof(XSetWindowAttributes));
    attr.event_mask = ExposureMask | KeyPressMask;
    attr.background_pixel = BlackPixel(s_Display, s_Screen);

    int depth = DefaultDepth(s_Display, s_Screen);

    s_Window = XCreateSimpleWindow(s_Display, DefaultRootWindow(s_Display), 0, 0, DOOMGENERIC_RESX, DOOMGENERIC_RESY, 0, blackColor, blackColor);

    XSelectInput(s_Display, s_Window, StructureNotifyMask | KeyPressMask | KeyReleaseMask);

    XMapWindow(s_Display, s_Window);

    s_Gc = XCreateGC(s_Display, s_Window, 0, NULL);

    XSetForeground(s_Display, s_Gc, whiteColor);

    XkbSetDetectableAutoRepeat(s_Display, 1, 0);

    // Wait for the MapNotify event

    while(1)
    {
        XEvent e;
        XNextEvent(s_Display, &e);
        if (e.type == MapNotify)
        {
            break;
        }
    }

    s_Image = XCreateImage(s_Display, DefaultVisual(s_Display, s_Screen), depth, ZPixmap, 0, (char *)DG_ScreenBuffer, DOOMGENERIC_RESX, DOOMGENERIC_RESX, 32, 0);
}


void DG_DrawFrame()
{
    if (s_Display)
    {
        while (XPending(s_Display) > 0)
        {
            XEvent e;
            XNextEvent(s_Display, &e);
            if (e.type == KeyPress)
            {
                KeySym sym = XkbKeycodeToKeysym(s_Display, e.xkey.keycode, 0, 0);

                addKeyToQueue(1, sym);
            }
            else if (e.type == KeyRelease)
            {
                KeySym sym = XkbKeycodeToKeysym(s_Display, e.xkey.keycode, 0, 0);
                addKeyToQueue(0, sym);
            }
        }

        XPutImage(s_Display, s_Window, s_Gc, s_Image, 0, 0, 0, 0, DOOMGENERIC_RESX, DOOMGENERIC_RESY);

        //XFlush(s_Display);
    }
}
void DH_remap_palette(uint8_t remap[256]) {}

void DH_setup_screen_info (struct FB_ScreenInfo* s_Fb) {
	s_Fb->bits_per_pixel = 32;

	s_Fb->blue.length = 8;
	s_Fb->green.length = 8;
	s_Fb->red.length = 8;
	s_Fb->transp.length = 8;

	s_Fb->blue.offset = 0;
	s_Fb->green.offset = 8;
	s_Fb->red.offset = 16;
	s_Fb->transp.offset = 24;
}

void DG_SleepMs(uint32_t ms)
{
    usleep (ms * 1000);
}

uint32_t DG_GetTicksMs()
{
    struct timeval  tp;
    struct timezone tzp;

    gettimeofday(&tp, &tzp);

    return (tp.tv_sec * 1000) + (tp.tv_usec / 1000); /* return milliseconds */
}

int DG_GetKey(int* pressed, unsigned char* doomKey)
{
	if (s_KeyQueueReadIndex == s_KeyQueueWriteIndex)
	{
		//key queue is empty

		return 0;
	}
	else
	{
		unsigned short keyData = s_KeyQueue[s_KeyQueueReadIndex];
		s_KeyQueueReadIndex++;
		s_KeyQueueReadIndex %= KEYQUEUE_SIZE;

		*pressed = keyData >> 8;
		*doomKey = keyData & 0xFF;

		return 1;
	}
}

FILE *file_wad;
int main(int argc, char **argv)
{
    file_wad = fopen("/usr/share/games/doom/doom1.wad", "rb");
    if (file_wad == NULL) {
			return 1;
		}

    char* args[] = {"./doomgeneric", "-iwad", "/usr/share/games/doom/doom1.wad"};
    doomgeneric_Create(3, args);

    while(1) {
      doomgeneric_Tick(); 
    }

    return 0;
}

// Read data from the specified position in WAD into the 
// provided buffer.

void DH_read_wad(unsigned int offset, void *buffer, size_t buffer_len) {
    // Jump to the specified position in the file.
    fseek(file_wad, offset, SEEK_SET);

    // Read into the buffer.
    fread(buffer, 1, buffer_len, file_wad);
}

uint8_t* I_ZoneBase (int *size) {
    *size = 8 * 1024 * 1024;
    return malloc (*size);
}

void* malloc_lump_info (size_t s) {
    return malloc (s);
}

void halt() {
    exit(1);
}
