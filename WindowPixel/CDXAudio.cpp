/*============================================================================
  Class:         CDXAudio (body)

  Please see accompanying definition of class.
   
  Written by:    Toby Murray, April 2002
  Last Modified: Toby Murray, 2nd April 2002
============================================================================*/
#include "framework.h"
//#include "stdafx.h"

#define INITGUID
#include "CDXAudio.h"
#include <cguid.h>

// constructor
CDXAudio::CDXAudio(){
   m_pPerformance = NULL;
   m_pLoader = NULL;
}

// destructor
CDXAudio::~CDXAudio(){
   Kill();
}

HRESULT CDXAudio::Setup(HWND hWnd){
    this->hWnd = hWnd;
   HRESULT hr;
   // Initialize COM
   if (FAILED(hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED ))) // COINIT_MULTITHREADED)))
      return hr;
    
   // Create loader object
   if (FAILED(hr = CoCreateInstance( CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC, 
                                     IID_IDirectMusicLoader8, (void**)&m_pLoader )))
      return hr;


   // Create performance object
   if (FAILED(hr = CoCreateInstance( CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC, 
                                     IID_IDirectMusicPerformance8, (void**)&m_pPerformance )))
      return hr;



   // This initializes both DirectMusic and DirectSound and 
   // sets up the synthesizer. 
  
   hr = this->m_pPerformance->InitAudio((IDirectMusic**) m_pIDM8, NULL, this->hWnd, DMUS_APATH_DYNAMIC_3D ,64 , DMUS_AUDIOF_ALL, NULL);
   if (FAILED(hr)) { return hr; }

 


   return S_OK;
}

// release everything
void CDXAudio::Kill(){
   // Stop the music
   if (m_pPerformance)
      m_pPerformance->Stop( NULL, NULL, 0, 0 );
   

   if (m_pLoader)
      m_pLoader->Release(); 
   m_pLoader = NULL;   
   
   if (m_pPerformance)
      m_pPerformance->CloseDown();

   if (m_pPerformance)
      m_pPerformance->Release();
   m_pPerformance = NULL;

   // Close down COM
   CoUninitialize();

}

HRESULT CDXAudio::GetPerformance(IDirectMusicPerformance8** ppPerf)
{
    return CoCreateInstance(CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC, IID_IDirectMusicPerformance8, (void**)ppPerf);
}
