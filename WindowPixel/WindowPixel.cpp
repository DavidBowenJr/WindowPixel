// WindowPixel.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WindowPixel.h"
#include "CustomRunner.h"
#include "GraphicsDemonstration.h"
#include "QueryTimer.h"

LARGE_INTEGER StartingTime, EndingTime, ElapsedMicrososeconds;
LARGE_INTEGER Frequency;



bool keys[256]; // Array used for the keyboard routine

static bool isRunning;
bool btrue = true;

using namespace std;

CustomRunner* customRunner = new CustomRunner();

#ifdef __USE_OLD_GRAPHICS_
GraphicsDemonstration* graphicsDemonstration = new GraphicsDemonstration();
#endif

static bool Stop_create = false;



#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
//BOOL                InitInstance(HINSTANCE, int);
  LRESULT  CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(nCmdShow);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    WNDCLASSEX wc = {};
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.lpfnWndProc =WndProc;
    wc.hInstance = hInstance;
  //  wc.lpszClassName = _T("WindowPixel");

    wc.cbSize = sizeof(WNDCLASSEXW);

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = szWindowClass;


#ifndef _DEMO_OUT_
    #define    _DEMO_OUT_
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWPIXEL));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = MAKEINTRESOURCE(IDC_WINDOWPIXEL);
    wc.hIconSm = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_SMALL));
#endif


    //Ready for our Message Pump....
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    HACCEL hAccelTable;

  
    // Initialize global strings
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_WINDOWPIXEL, szWindowClass, MAX_LOADSTRING);
   // MyRegisterClass(hInstance);

    if (RegisterClassEx(&wc))
    {
        HWND hWnd = CreateWindowEx(
            0,
            wc.lpszClassName,
            _T("Our Game ext"),
         //   WS_EX_OVERLAPPEDWINDOW | WS_VISIBLE,
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
           // WS_OVERLAPPED | WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0, 0, hInstance, 0);

        if (hWnd)
        {
            OutputDebugString(_T("Start up msg Pump\n"));


            isRunning = true;

            int xOffset = 0;
            int yOffset = 0;

            hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWPIXEL));


            //

           
            while (isRunning) 
            {


              
                while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))

                {
                   
                    /*
                    if (msg.message == WM_QUIT)
                    {
                        isRunning = false;
                        OutputDebugString(_T("xxx WM_QUIT \n"));
                    }
                    */
                   

                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }

                if (msg.message == WM_QUIT)
                {
                      OutputDebugString(_T(" WM_QUIT AND BREAK"));   Stop_create = true;  break; 
                }
             
                                      

              




                // lock button lb lock B key
                // Dirty.
                static bool lb = false;
                if (keys['B'] && !lb) // B key being pressed and not held toggle
                {
                    lb = true;
                    btrue = !btrue; // Toggle
                }
                if (!keys['B']) // has B key been released?
                {
                    lb = false;  // if so, lock button becomes false
                }

                static bool lc = false; // a lock for c key
                static int switchCool = 1;
                if (keys['C'] && !lc) // C key being pressed and not held? toggle
                {
                    switchCool++;
                    lc = true;
                    switchCool = switchCool % 3;
                }
                if (!keys['C']) // has C key been released?
                {
                    lc = false; // if so , the lock becomes false
                }

                if (keys[VK_ESCAPE]) { DestroyWindow(hWnd); }
               
              
                
                QueryTimer qt = QueryTimer();
                qt.StartCounter();
                double em = qt.GetCounter();
                static double accum = 0;
                accum += em;



               // static bool bforce = true;

                if (double(0.010000) <= accum)
                {
                    switchCool++;
                    switchCool = switchCool % 3;
                    accum = fmod(accum, 0.01000);
                    btrue = !btrue;
                }
                

                if (btrue)
                {
                   
                    customRunner->Render();

                     //   customRunner->RenderWeirdGradient((uint8)xOffset, (uint8)yOffset);
                        customRunner->DrawRect();

                        HDC DC = GetDC(hWnd);
                        RECT CR;
                        GetClientRect(hWnd, &CR);
                        int RmL = CR.right - CR.left;
                        int BmT = CR.bottom - CR.top;
                        customRunner->Win32UpdateWindow(DC, &CR, 0, 0, RmL, BmT);
                        ReleaseDC(hWnd, DC);
                        ++xOffset;
                    
                }
                else {

#ifdef __USE_OLD_GRAPHICS_
                    switch (switchCool)
                    {
                    case 0:
                     //  OutputDebugString(_T(" case 0 \n"));
                        graphicsDemonstration->ClearBackGround(hWnd);
                        graphicsDemonstration->CoolStuff3(hWnd);
                        break;
                    case 1:
                       // OutputDebugString(_T(" case 1 \n"));
                        graphicsDemonstration->ClearBackGround(hWnd);
                        graphicsDemonstration->CoolStuff(hWnd);
                        break;
                    case 2:
                       // OutputDebugString(_T(" case 2 \n"));
                        graphicsDemonstration->ClearBackGround(hWnd);
                        graphicsDemonstration->CoolStuff2(hWnd);
                        break;
                    case 3:
                       // OutputDebugString(_T(" case 3 \n"));
                        break;
                    default:
                      //  OutputDebugString(_T(" default \n"));
                        graphicsDemonstration->ClearBackGround(hWnd);
                        graphicsDemonstration->CoolStuff3(hWnd);
                        break;
                    }
#endif
                }




            }
        }

#ifndef _DO_CLEAN_UP_EXT__
#define _DO_CLEAN_UP_EXT__
        // No longer running.....
        OutputDebugString(_T(" Class Pt Clean Up\n "));

#ifdef __USE_OLD_GRAPHICS_
        if (graphicsDemonstration != NULL)
            delete graphicsDemonstration;
        graphicsDemonstration = NULL;
#endif

        if (customRunner != NULL)
            delete customRunner;
        customRunner = NULL;

#endif
      
      

  
       
       std::wstring ws; 
       ws = std::to_wstring(msg.message);
        OutputDebugString( ws.c_str());

#ifdef __DEBUG_EXTRA_INSPECTION_
//#define __DEBUG_EXTRA_INSPECTION_

        int messagenumber = (int)msg.message; // 275;
        const TCHAR* translatedMessage = wmTranslation[messagenumber];
        if (translatedMessage == NULL)
        {
            translatedMessage = _T("UnknownMessage");
        }
        OutputDebugString(translatedMessage);
#endif





        GetMessage(&msg, hWnd, 0, 0);

     //   PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE);
     //  PeekMessage(&msg, hWnd, 0, 0, PM_NOREMOVE);
        OutputDebugString(_T(" : "));
        ws = std::to_wstring(msg.message);
        OutputDebugString(ws.c_str());

        OutputDebugString(_T("\n RegisterClassEx finnished \n"));
    }



  


    // No longer running.....
    OutputDebugString(_T(" Class Pt Clean Up\n "));

#ifdef __USE_OLD_GRAPHICS_
    if (graphicsDemonstration != NULL)
        delete graphicsDemonstration;
    graphicsDemonstration = NULL;
#endif

    if (customRunner != NULL)
        delete customRunner;
    customRunner = NULL;

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    =  WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWPIXEL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWPIXEL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}




//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

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
        if (btrue)
        {
            hWnd = customRunner->myPaint(hWnd);
        }
        else
        {
            {
#ifdef __USE_OLD_GRAPHICS_
                graphicsDemonstration->Paint(hWnd);
#endif

            }
        }
    } break;



    case WM_KEYDOWN:
    {
        keys[wParam] = TRUE;
        OutputDebugString(_T(" KEYDOWN \n"));
    }break;

    case WM_KEYUP:
    {
        keys[wParam] = FALSE;
        OutputDebugString(_T(" KEYUP \n"));
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
        RECT CR;
        GetClientRect(hWnd, &CR);
        LONG W = CR.right - CR.left;
        LONG H = CR.bottom - CR.top;

       
        customRunner->Win32ResizeDibSection((uint32_t)W, (uint32_t)H);
        customRunner->hWnd = hWnd;
    } break;


    case WM_CREATE:
    {
#ifdef __USE_OLD_GRAPHICS_
        graphicsDemonstration->WindowMessageCreate(hWnd);
#endif

    } break;

   

    // Priority very low but needed.
    case WM_DESTROY:
    {
#ifdef __USE_OLD_GRAPHICS_
        if (graphicsDemonstration != NULL)
        {
            graphicsDemonstration->destroy(hWnd);
        }
#endif

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
        break; // case WM_COMMAND:
  //.....

    /// <summary>
    /// NEED TO TURN ON SCREEN SAVER AND TEST THIS
    /// to see if it is reached.
    /// </summary>
    /// <param name="hWnd"></param>
    /// <param name="message"></param>
    /// <param name="wParam"></param>
    /// <param name="lParam"></param>
    /// <returns></returns>
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
