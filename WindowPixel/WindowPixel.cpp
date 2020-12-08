// WindowPixel.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WindowPixel.h"

#include "QueryTimer.h"

LARGE_INTEGER StartingTime, EndingTime, ElapsedMicrososeconds;
LARGE_INTEGER Frequency;



bool keys[256]; // Array used for the keyboard routine

static bool isRunning;
bool btrue = true;

using namespace std;





#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
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

    WNDCLASSEXW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = _T("WindowPixel");

    wc.cbSize = sizeof(WNDCLASSEXW);

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWPIXEL));
    wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = MAKEINTRESOURCE(IDC_WINDOWPIXEL);
    wc.lpszClassName = szWindowClass;
    wc.hIconSm = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
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

            while (isRunning) 
            {

                while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
                {
                    if (msg.message == WM_QUIT)
                    {
                        isRunning = false;
                        OutputDebugString(_T(" WM_QUIT \n"));
                    }
                    
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
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



                static bool bforce = true;

                if (double(0.010000) <= accum)
                {
                    switchCool++;
                    switchCool = switchCool % 3;
                    accum = fmod(accum, 0.01000);
                    btrue = !btrue;
                }

                if (btrue)
                {
#ifdef _USE_C_R_

#endif
                    HDC DC = GetDC(hWnd);
                    RECT CR;
                    GetClientRect(hWnd, &CR);
                    int RmL = CR.right - CR.left;
                    int Bmt = CR.bottom - CR.top;

                    ReleaseDC(hWnd, DC);
                    ++xOffset;
#ifdef _USE_C_R_

#endif
                }
                else {
                    switch (switchCool)
                    {
                    case 0:
                        OutputDebugString(_T(" case 0 \n"));
                        break;
                    case 1:
                        OutputDebugString(_T(" case 1 \n"));
                        break;
                    case 2:
                        OutputDebugString(_T(" case 2 \n"));
                        break;
                    case 3:
                        OutputDebugString(_T(" case 3 \n"));
                        break;
                    default:
                        OutputDebugString(_T(" case 4 \n"));
                        break;
                    }
                }




            }
        }
    }

    // FIN... NO LONGER RUNNING!!!
    // Clean House Before Windows gose out... completely 

#ifndef _DO_CLEAN_UP_EXT__
#define _DO_CLEAN_UP_EXT__


#endif

    /*
    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
    
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWPIXEL));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    */
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
    wcex.lpfnWndProc    = WndProc;
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;
    LRESULT Result = 0;

    DBG_UNREFERENCED_LOCAL_VARIABLE(ps);
    UNREFERENCED_PARAMETER(hdc);

    switch (message)
    {
    case WM_CREATE:
    {

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

    case WM_SIZE:
    {
        RECT CR;
        GetClientRect(hWnd, &CR);
        LONG W = CR.right - CR.left;
        LONG H = CR.bottom - CR.top;

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

    case WM_QUIT:
    {
        OutputDebugString(_T(" WM_QUIT \n"));
    } break;

    case WM_PAINT:
    {
        if (btrue)
        {

        }
        else
        {

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


/*
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
*/


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
