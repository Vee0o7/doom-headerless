/* Use 256 bit color palette in output */
#define CMAP256

/* Divide simulated resolution by 2, this is mainly useful with 160x100 screen, or if you prefer fullscreen with smaller resolution
   You can reduce simulated resolution in Doom's settings, but it will still use 320x200 HUD, only the smaller 3D scene will be in top left corner
   The HUD is harder to read */
#define DIV2

#define DOOMGENERIC_RESX 320
#define DOOMGENERIC_RESY 200
#define FB_SCALING 1

// Increasing resolution may be good for testing on normal PC, but may be incompatible with CMAP256 and DIV2
//#define DOOMGENERIC_RESX 1280
//#define DOOMGENERIC_RESY 800
//#define FB_SCALING 4
