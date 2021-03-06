// WindowPixel.cpp : Defines the entry point for the application.
// The very old gl libs
//https://onedrive.live.com/?id=E38F9FC6490B29D9!1065&cid=E38F9FC6490B29D9&group=0&parId=E38F9FC6490B29D9!977&action=locate
////http://nehe.gamedev.net/tutorial/texture_filters,_lighting_&_keyboard_control/15002/
// http://www.rastertek.com/dx11tut13.html
// TEST WITH THE H KEY to fire off Direct Input Exit....
// Hope to get Joy up and going... Been A long time since don't have an audio card
// that support Force FeedBack  But usb should be ok. for my Radial Pro

           #define _USE_INPUT_CLASS_ONE true
// Using This controler to work with.

/// Interact Raider Pro Digital Flight Stick Joystick SV - 251 Black with USB Cable



#include "framework.h"

#include <thread>


#include <WinUser.h>

#include <system_error>
#include <memory>
#include <processthreadsapi.h>

#include "WindowPixel.h"
#include "CustomRunner.h"
#include "GraphicsDemonstration.h"
#include "QueryTimer.h"

#include <Stringapiset.h> // MultiByteToWideChar
#include "ErrorExit.h"


#include "Location.h"

/*
*  https ://github.com/milliet/DDReader/tree/master/c/sdk-directx9/Lib
Disabling option "Image has Safe Exception Handlers" 
in Project properties->Configuration Properties->Linker->Advanced tab helped me.
*/

//   #include "DirectSoundAudio.h"


#define IS_MIXED_AUDIO  true

//#define _YALDEX_AUDIO_
#ifdef _YALDEX_AUDIO_
#include "YaldexAudio.h"
#endif


#if(IS_MIXED_AUDIO)
#include "CDXAudio.h"
#include "C3DSound.h"
#endif

#if _USE_INPUT_CLASS_ONE
#include "InputClass.h"

#endif
//#include "DirectSoundAudio.h"


#include "Plasma.h"
#include "Scratch.h"


#ifdef _YALDEX_AUDIO_


#endif



 static olved__buffer plasmaBuffer;

LARGE_INTEGER StartingTime, EndingTime, ElapsedMicrososeconds;
LARGE_INTEGER Frequency;
WPARAM MessageAndGameLoop(PMSG msg, HWND hWnd);


bool keys[256]; // Array used for the keyboard routine

static bool isRunning;

bool btrue = true;

static CustomRunner* customRunner; 

#if _USE_INPUT_CLASS_ONE
static InputClass* input;
#endif


static bool Stop_create = false;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                               
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];    

// Forward declarations of functions included in this code module:
  // ATOM                MyRegisterClass(HINSTANCE hInstance);

  LRESULT  CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


//////////////////////////////
//CHECK CHECK ONE TWO THRE

#if(IS_MIXED_AUDIO)
 CDXAudio Audio;
static C3DSound SoundEffect[4];

void foo(C3DSound* c3d);
#endif

//





int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(nCmdShow);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    WNDCLASSEXW wc = {};
    ZeroMemory(&wc, sizeof(WNDCLASSEXW));

     wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName =  TEXT( "WindowPixel");

    
    wc.cbSize = sizeof(WNDCLASSEXW);


#ifndef _DEMO_OUT_

    wc.style = CS_HREDRAW | CS_VREDRAW;
  
   

 //   wc.lpszClassName = szWindowClass;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hIconSm = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    


//#ifdef _DEMO_OUT_
    #define    _DEMO_OUT_
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWPIXEL));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWPIXEL);
    wc.hIconSm = LoadIconW(wc.hInstance, MAKEINTRESOURCEW(IDI_SMALL));
#endif

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    HACCEL hAccelTable;

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWPIXEL, szWindowClass, MAX_LOADSTRING);
 
    if (RegisterClassExW(&wc))
    {
        HWND hWnd = CreateWindowExW( 0,  wc.lpszClassName, L"Our Game ext",  WS_OVERLAPPEDWINDOW | WS_VISIBLE
            , CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
        
        hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCEW(IDC_WINDOWPIXEL));
 
    /////////////////////////////////////////////////////////////////////////////
      
#if _USE_INPUT_CLASS_ONE
        HRESULT dxResult = 0;
        if (input != NULL)
        {
            RECT Rect;
            if (GetClientRect(hWnd, &Rect))
            {
                dxResult = input->Initialize(hInstance, hWnd, Rect.right, Rect.bottom);
                if (!dxResult) {
                    MessageBoxW(hWnd, L"Could not initialize the input object.", L"Error", MB_OK);
                    SAFE_DELETE(input);
                    SAFE_DELETE(customRunner);
                    return false;
                }
            }
        }
#endif
        
        /////////////////////////////////////

     //   DirectSoundAudio dsa;
    //    dsa.Initialize(hInstance, hWnd, 48000, 48000 * sizeof(int16)* 2 );

#ifdef _YALDEX_AUDIO_     
   static  YaldexAudio yda;              
        if (yda.Game_Init(hWnd)) { }
#endif



        // CHECK CHECK ONE TWO THREE CHECK CHECK
         // setup DirectX Audio
        //https://documentation.help/DirectMusic/step2retrievethebuffer.htm
#if(IS_MIXED_AUDIO)
        if (FAILED(Audio.Setup(hWnd)))
            return false;  // FatalError("Couldn't setup DirectX Audio");

            if (FAILED(SoundEffect[0].Setup((WCHAR*)L"drumpad-speech.wav", Audio.getPerformance(), Audio.getLoader())))
                return false; // FatalError("Couldn't load 3d sound effect.");
            if (FAILED(SoundEffect[1].Setup((WCHAR*)L"drumpad-voc_female_ec.wav", Audio.getPerformance(), Audio.getLoader())))
                return false; // FatalError("Couldn't load 3d sound effect.");
            if (FAILED(SoundEffect[2].Setup((WCHAR*)L"sound1.wav", Audio.getPerformance(), Audio.getLoader())))
                return false; // FatalError("Couldn't load 3d sound effect.");
            if (FAILED(SoundEffect[3].Setup((WCHAR*)L"sound2.wav", Audio.getPerformance(), Audio.getLoader())))
                return false; // FatalError("Couldn't load 3d sound effect.");



        Sleep(200);
       // D3DVALUE

         
        std::thread thread_obj (foo, SoundEffect);
        
        thread_obj.joinable();
 Sleep(4000);
        thread_obj;
      //  thread_obj.join();

        thread_obj.joinable();
       

        thread_obj;

        thread_obj.joinable();


            SoundEffect[0].setPos(5.0f, 0.0f, 0.0f, DS3D_DEFERRED);
        SoundEffect[0].Play();
      
     //   MessageBoxA(NULL, "The sound effect just played to your right.", "3D Sound Tutorial", MB_OK);

        SoundEffect[1].setPos(0.0f, 5.0f, 0.0f, DS3D_DEFERRED);
        SoundEffect[1].Play();
    //    MessageBoxA(NULL, "The sound effect just played in front of you.", "3D Sound Tutorial", MB_OK);
  
        SoundEffect[2].setPos(-0.1f, 0.0f, 0.0f, DS3D_DEFERRED);
        SoundEffect[2].Play();
    //    MessageBoxA(NULL, "The sound effect just played to your left.", "3D Sound Tutorial", MB_OK);
      
        SoundEffect[3].setPos(0.0f, -5.0f, 0.0f, DS3D_DEFERRED);
        SoundEffect[3].Play();
   //     MessageBoxA(NULL, "The sound effect just played behind you.", "3D Sound Tutorial", MB_OK);

#endif
        //////////////////////////////////////




        if (hWnd) { if (MessageAndGameLoop(&msg, hWnd)) {}; } 

#if( IS_MIXED_AUDIO)
        thread_obj.join();
#endif



#if _USE_INPUT_CLASS_ONE
        input->Shutdown();
#endif

#ifdef _YALDEX_AUDIO_

#endif


#if(IS_MIXED_AUDIO)
        // kill the sound
        SoundEffect[3].Kill();
        SoundEffect[2].Kill();
        SoundEffect[1].Kill();
        SoundEffect[0].Kill();

        // kill directX Audio
        Audio.Kill();
#endif


        GetMessage(&msg, hWnd,0,0);

        // No longer running.....
        OutputDebugString(_T(" Class Pt Clean Up\n "));

#if _USE_INPUT_CLASS_ONE
        SAFE_DELETE(input);
#endif

        SAFE_DELETE(customRunner);
    
        OutputDebugString(_T("\n RegisterClassEx finnished \n"));

    }

    OutputDebugString(_T(" Class Pt Clean Up\n "));
#if _USE_INPUT_CLASS_ONE
    SAFE_DELETE(input);
#endif

    SAFE_DELETE(customRunner);
   
  
    return (int) msg.wParam;
}



WPARAM MessageAndGameLoop(PMSG pMsg, HWND hWnd)
{
    isRunning = true;
    while (isRunning)
    {
        while (PeekMessage(pMsg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(pMsg);
            DispatchMessage(pMsg);
        }

        if (pMsg->message == WM_QUIT) {
            isRunning = false;
        } // else {}  // NEED TO CONSIDER.


#if _USE_INPUT_CLASS_ONE
        if (!input->Frame())
        {
            MessageBoxW(hWnd, L"Frame Processing Failed", L"Error", MB_OK);
        }

        if (input->IsEscapePressed())
        {
            SendNotifyMessage(hWnd, WM_CLOSE, pMsg->wParam, pMsg->lParam);
            return pMsg->wParam;
        }

        if (input->IsJoyEtc())
        {
            SendNotifyMessage(hWnd, WM_CLOSE, pMsg->wParam, pMsg->lParam);
            return pMsg->wParam;
        }
/*
        if (input->jlx != 0)
        {
            OutputDebugStringW(L"Ax: ");
            OutputDebugStringW(std::to_wstring(input->jlx).c_str());
        }

        if (input->jly != 0)
        {
            OutputDebugStringW(L"\n Ay: ");
            OutputDebugStringW(std::to_wstring(input->jly).c_str());
        }

        if (input->jlz != 0)
        {
            OutputDebugStringW(L"\n Az: ");
            OutputDebugStringW(std::to_wstring(input->jlz).c_str());
            OutputDebugStringW(L"\n");
        }


       const LONG POVNEUTRAL = -1;
       LONG hatresult = POVNEUTRAL;

        if (input->JHat != -1)
        {
            OutputDebugStringW(L"\n jPOV: ");
            OutputDebugStringW(std::to_wstring(input->JHat).c_str());
            OutputDebugStringW(L"\n");
            hatresult = input->JHat;
        }
        if (input->jSlider1 != -1)
        {
            OutputDebugStringW(L"\n jSlider1: ");
            OutputDebugStringW(std::to_wstring(input->jSlider1).c_str());
            OutputDebugStringW(L"\n");
        }

        
        if (input->jSlider2 != -1)
        {
            OutputDebugStringW(L"\n jSlider2: ");
            OutputDebugStringW(std::to_wstring(input->jSlider2).c_str());
            OutputDebugStringW(L"\n");
        }
        

        if (input->JButton != 0)
        {
            for (int i = 0; i < 32; i++)
            {
                if (input->JButton[i] & 0x80)
                {
                    OutputDebugStringW(L" Button :\t");
                    OutputDebugStringW(  std::to_wstring(i).c_str());
                    OutputDebugStringW(L" was Pressed \n");
                }
            }



        } 
        */


#endif

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            SendNotifyMessageW(hWnd, WM_CLOSE, pMsg->wParam, pMsg->lParam);
            return pMsg->wParam;
        }

        if (GetAsyncKeyState(VK_F1) & 0X8000) {
            DestroyWindow(hWnd);
        }

        if (GetAsyncKeyState(VK_SPACE) & 0X8000) {
            {
                std::string errorcheck = "An Error Check quit";
                ErrorExit((LPTSTR)errorcheck.c_str());
            }
        }

        if(customRunner)
        {         
            customRunner->Render();
            customRunner->Win32UpdateWindow(); //sealed function
        }
    }

    return pMsg->wParam;
}

LRESULT 
 CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId , wmEvent;
    PAINTSTRUCT ps;
    
      HDC hdc;
    LRESULT  Result = 0;

    DBG_UNREFERENCED_LOCAL_VARIABLE(ps);
    UNREFERENCED_PARAMETER(hdc);

    switch (message)
    {

    case WM_PAINT:
    {
        hWnd = customRunner->myPaint();
    } break;


    case WM_INPUT:
        
        break;

        
    case WM_KEYDOWN:
    {
  
       // keys[wParam] = TRUE;
      //  OutputDebugString(_T(" KEYDOWN \n"));
    }break;

    case WM_KEYUP:
    {
      //  keys[wParam] = FALSE;
     //   OutputDebugString(_T(" KEYUP \n"));
    } break;
    

    case WM_ACTIVATE:
    {
        wmEvent = HIWORD(wParam);
        if (!wmEvent) // Check Minimization State
        {

        }
        else {
            // Program is InActive
        }
    }
    break;

    case WM_TIMER:
        break;

    case WM_SIZE:
    {
    
    } break;

    case WM_ERASEBKGND:
        break;

    case WM_CREATE:
    {
#if(0)
        {
            Location loc;
            loc.m_GetCurrentDirectory();
            OutputDebugStringA(loc.searchPath);
            loc.m_MultiByteToWideChar();
            OutputDebugStringW(loc.wSearchPath);
        }
#endif
        // Time 2:37 PM
        // 12/12/2020 Now it will scale 
        
        RECT CR;
        GetClientRect(hWnd, &CR);
        LONG W = CR.right - CR.left;
        LONG H = CR.bottom - CR.top;

        // COULD PASS IN GLOBAL
        customRunner = new CustomRunner();
        customRunner->Win32ResizeDibSection((uint32)W, (uint32)H);
        customRunner->hWnd = hWnd;


#if _USE_INPUT_CLASS_ONE
        input = new InputClass;
        if (!input)
        {
        }
#endif

        // We will allow the outer here ... to destroy input we just wan't  customRunner to host it somewhat
        customRunner->inputclass = input;

        // Inject into class
        customRunner->pplasma = new Plasma(plasmaBuffer);
      //  plasmaBuffer = customRunner->pplasma->olvedBuffer; // ?
        customRunner->scratch = new Scratch();


       
    } break;


    case WM_DESTROY:
    {
        isRunning = false;
        OutputDebugString(_T(" WM_DESTROY\n"));
        PostQuitMessage(0);

    } break;

    case WM_CLOSE:
    {
        isRunning = false;
        OutputDebugString(_T(" WM_CLOSE\n"));
        DestroyWindow(hWnd);
        PostQuitMessage(0);
    } break;

    //.......
    case WM_COMMAND:
        wmId = LOWORD(wParam);
        wmEvent = HIWORD(wParam);

        switch (wmId)
        {
    

        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;

        case IDM_EXIT:
        {
            OutputDebugString(_T(" DestroyWindow(hWnd) at IDM_EXIT\n"));
            DestroyWindow(hWnd);
        }
        break;

        default:
            Result = DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;

    case SC_SCREENSAVE: // Screensave trying to start?
    case SC_MONITORPOWER: // Monitor trying to enter power save
        break;
  
    default:
    {
        Result = DefWindowProc(hWnd, message, wParam, lParam);
    } break;
    }
    return (Result);
}


// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


#if(IS_MIXED_AUDIO)
void foo(C3DSound* c3d)
{
    c3d[2].setPos(5.0f, 0.0f, 0.0f, DS3D_DEFERRED);
    c3d[2].Play();

    c3d[2].setPos(5.0f, 0.0f, 0.0f, DS3D_DEFERRED);
    c3d[2].Play();

    Sleep(40);
    c3d[2].setPos(5.0f, 0.0f, 0.0f, DS3D_DEFERRED);
    c3d[2].Play();
    Sleep(8000);

    c3d[2].setPos(5.0f, 0.0f, 0.0f, DS3D_DEFERRED);
    c3d[2].Play();
  
}
#endif

