#pragma once

#define DIRECTSOUND_VERSION  0x0700
#include <dsound.h>
#include <dmusics.h>
#include "dmusici.h"
#include <cguid.h>


#ifndef DSBCAPS_CTRLDEFAULT
#define DSBCAPS_CTRLDEFAULT (DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME )
#endif


#define MAX_SOUNDS 256
//   256 // max number of sounds in system at once 
// digital sound object state defines
#define SOUND_NULL     0 // " "
#define SOUND_LOADED   1
#define SOUND_PLAYING  2
#define SOUND_STOPPED  3


// this holds a single sound
typedef struct pcm_sound_typ
{
	LPDIRECTSOUNDBUFFER dsbuffer;   // the ds buffer containing the sound
	int state;                      // state of the sound
	int rate;                       // playback rate
	int size;                       // size of sound
	int id;                         // id number of the sound
}  pcm_sound, * pcm_sound_ptr;


static pcm_sound sound_fx[MAX_SOUNDS];    // the array of secondary sound buffers



//#define MAX_SOUNDS 4
/// <summary>
/// http://en.verysource.com/item/windowstricksofgamecode_rar-155069.html
/// </summary>

class YaldexAudio
{
public:
//	pcm_sound sound_fx[MAX_SOUNDS];  // the array of secondary sound buffers

	int Game_Init( HWND hWnd);
	int DSound_Load_WAV(char* filename, int control_flags = DSBCAPS_CTRLDEFAULT);
	HWND hWnd;

	WAVEFORMATEX pcmwf;
	LPDIRECTSOUND lpds;           // directsound interface pointer
	DSBUFFERDESC dsbd;           // directsound description
	DSCAPS dscaps;         // directsound caps
	HRESULT dsresult;       // general directsound result
	DSBCAPS             dsbcaps;        // directsound buffer caps
//	pcm_sound sound_fx[MAX_SOUNDS];    // the array of secondary sound buffers
	HWND                freq_hwnd,      // window handles for controls
		volume_hwnd,
		pan_hwnd;
	int                 sound_id = -1;  // id of sound we load for demo



};

