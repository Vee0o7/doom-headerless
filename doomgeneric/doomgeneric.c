#include "m_argv.h"

#include "doomgeneric.h"

#ifdef DOOM32IM
#define SCREENBUFFER

pixel_t* DG_ScreenBuffer __attribute((__section__(".screen"))) = NULL;

#endif /* ifdef DOOM32IM */

#ifndef SCREENBUFFER
#define SCREENBUFFER

pixel_t* DG_ScreenBuffer = NULL;

#endif /* ifndef SCREENBUFFER */

void D_DoomMain (void);


void doomgeneric_Create(int argc, char **argv)
{
	// save arguments
  myargc = argc;
  myargv = argv;

	DG_Init();

	D_DoomMain ();
}

