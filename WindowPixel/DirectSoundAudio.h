#pragma once
//#define INITGUID
// https://github.com/milliet/DDReader/tree/master/c/sdk-directx9/Lib


#define DIRECTSOUND_VERSION  0x0800

//                           #pragma comment(lib, "dsound.lib")
//                           #pragma comment(lib, "dxguid.lib")
//#include <dsound.h>
#include <dmusics.h>
#include "dmusici.h"
#include <cguid.h>



class DirectSoundAudio
{
public:
	LPDIRECTSOUND8 lpDirectSound;

	//IDirectSound** lpDirectSound;
	//LPDIRECTSOUND lpDirectSound;
	DSBUFFERDESC bd;
	LPDIRECTSOUNDBUFFER SecondaryBuffer;
	LPDIRECTSOUNDBUFFER PrimaryBuffer;


//	WAVEFORMATEX wf;

/////////////////////
	




	bool Initialize(HINSTANCE hinstance, HWND hwnd, int32 , DWORD bufferSize );

	void FirstStep();
	
};

