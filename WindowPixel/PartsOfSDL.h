#pragma once
#include "framework.h"
#define SDL_ALPHA_OPAQUE 255
#define SDL_ALPHA_TRANSPARENT 0


typedef struct SDL_Rect {
	int16_t x;
	int16_t y;
	uint16_t w;
	uint16_t h;
}SDL_RECT;

typedef struct SDL_Color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t unused;
}SDL_Color;

#define SDL_Colour SDL_Color

typedef struct SDL_Palette {
	int ncolors;
	SDL_Color* colors;
}SDL_Palette;

typedef struct SDL_PixelFormat {
	SDL_Palette* palette;
	uint8 BitsPerPixel;
	uint8 BytesPerPixel;
	uint8 Rloss;
	uint8 Gloss;
	uint8 Bloss;
	uint8 Aloss;
	uint8 Rshift;
	uint8 Gshift;
	uint8 Bshift;
	uint8 Ashift;
	uint32 Rmask;
	uint32 Gmask;
	uint32 Bmask;
	uint32 Amask;
	/* RGB color key information */
	uint32 colorkey;
	/* Alpha value information (per-surface alpha) */
	uint8 alpha;
}SDL_PixelFormat;



/** This structure should be treated as read-only, 
except for 'pixels', which, if not NULL, contains the raw pixel data for the surface.**/


typedef struct SDL_Surface {
	uint32 flags;
	SDL_PixelFormat* format;
	int w, h;
	uint16 pitch;
	void* pixels;
	int offset;

	/*Hardware- specific surface info */
	struct private_hwdata* hwdata; // no

	/* clipping information */
	SDL_Rect clip_rect;
	uint32 unused1; // for binary compatibility

	/* Allow recursive locks */
	uint32 locked;

	/* info for fast blit mapping to other surface */
	struct SDL_BlitMap* map;

	unsigned int format_version;

	int refcount;
} SDL_Surface;


//SDL_Surface* scr;
