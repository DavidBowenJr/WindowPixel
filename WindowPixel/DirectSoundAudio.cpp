#include "framework.h"
#include "DirectSoundAudio.h"
//https://edn.embarcadero.com/article/20941#SettingThePrimaryFormat
//https://archive.gamedev.net/archive/reference/articles/article710.html


bool DirectSoundAudio::Initialize(HINSTANCE hinstance, HWND hwnd, int32 SamplesPerSecond, DWORD BufferSize )
{


static	WAVEFORMATEX wf;
	 wf = {};
	//memset(&wf, 0, sizeof(WAVEFORMATEX));
	wf.wFormatTag = WAVE_FORMAT_PCM;
	wf.nChannels = 2;
	wf.nSamplesPerSec = SamplesPerSecond;
	wf.wBitsPerSample = 16; // 8 bit mono 16 bit stero  half sample
	wf.nBlockAlign = (wf.nChannels * wf.wBitsPerSample) / 8;
	wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
	wf.cbSize = 0;

	//xaudio

	HRESULT hres = 0;
	//	if (FAILED(hres = DirectSoundCreate8() )) { return false; }
	hres = DirectSoundCreate8((LPCGUID)&DSDEVID_DefaultPlayback,   &lpDirectSound, NULL);
	if (FAILED(hres)) { return false; }

	OutputDebugStringA("\n direct sound create 8 \n");







	hres = lpDirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY); // DSSCL_NORMAL);
	if (FAILED(hres)) { return false; }

	//memset(&bd, 0, sizeof(DSBUFFERDESC));
	bd = {};
	bd.dwSize = sizeof(DSBUFFERDESC);
	bd.dwFlags = DSBCAPS_PRIMARYBUFFER;
	bd.dwBufferBytes = 0; // must be 0 for primary buffer
	bd.lpwfxFormat = NULL; // must be null for primary buffer

	OutputDebugStringA("\n set coop level \n");






	// int16 L R L R



	//LPDIRECTSOUND3DBUFFER PrimaryBuffer;
	hres = lpDirectSound->CreateSoundBuffer(&bd, &PrimaryBuffer, NULL);
	if (FAILED(hres)) {
		
		return false; 
	}

	OutputDebugStringA("\n create sound buffer \n");

	// Secondary Buffer
	bd.dwBufferBytes = BufferSize; // 2/


	PrimaryBuffer->SetFormat(&wf);

	/////////////////////////////////////////////////////////////////////

	

		DSBUFFERDESC BufferDescription = {};
		BufferDescription.dwSize = sizeof(BufferDescription);
		BufferDescription.dwFlags = 0;// DSBCAPS_PRIMARYBUFFER;
		BufferDescription.dwBufferBytes = BufferSize;
		BufferDescription.lpwfxFormat = &wf; // BufferDescription;

		OutputDebugStringA("\n trying \n");

	hres = lpDirectSound->CreateSoundBuffer(&BufferDescription, &SecondaryBuffer, 0); 
	if (FAILED(hres)) {
		return false;
	}
	
	OutputDebugStringA("\n create sound buffer secondary buffer \n");

//	SecondaryBuffer->SetFormat(&wf);
	
	

/////////////////////////////////////////////////////////////////////


return true; }




void DirectSoundAudio::FirstStep()
{
	HRESULT hres = 0;




	
}
