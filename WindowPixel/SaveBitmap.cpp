#pragma once
#include "SaveBitmap.h"


// Modify some. Now it can size the region lWidth and lHeight  to the right and  bottom with the 
// Use of  GetClientRect
// biBitCount = 32
// NOTE IT IS OUR JOB TO RELEASE THE PASSED IN HDC ELSE WHERE.

//https://docs.microsoft.com/en-us/windows/win32/gdi/capturing-an-image?redirectedfrom=MSDN
//https://stackoverflow.com/questions/3291167/how-can-i-take-a-screenshot-in-a-windows-application

BOOL WINAPI SaveBitmap(WCHAR* wPath,HWND hWndParam, HDC hDC)
{
    BITMAPFILEHEADER bfHeader;
    BITMAPINFOHEADER biHeader;
    BITMAPINFO bInfo;
    HGDIOBJ hTempBitmap;
    HBITMAP hBitmap;
    BITMAP bAllDesktops;
   // HDC hDC, hMemDC;
    HDC hMemDC;
    LONG lWidth, lHeight;
    BYTE* bBits = NULL;
  //  HANDLE hHeap = GetProcessHeap();
    DWORD cbBits, dwWritten = 0;
    HANDLE hFile;
     INT x = GetSystemMetrics(SM_XVIRTUALSCREEN);
     INT y = GetSystemMetrics(SM_YVIRTUALSCREEN);



    ZeroMemory(&bfHeader, sizeof(BITMAPFILEHEADER));
    ZeroMemory(&biHeader, sizeof(BITMAPINFOHEADER));
    ZeroMemory(&bInfo, sizeof(BITMAPINFO));
    ZeroMemory(&bAllDesktops, sizeof(BITMAP));

   // hDC = GetDC(NULL);
  //  hDC = GetDC(hWnd);

    hTempBitmap = GetCurrentObject(hDC, OBJ_BITMAP);
    GetObjectW(hTempBitmap, sizeof(BITMAP), &bAllDesktops);
    
    lWidth = bAllDesktops.bmWidth;
    lHeight = bAllDesktops.bmHeight;

    // Made so it only get our Application
     RECT Rect;
   if (GetClientRect(hWndParam, &Rect)) {
       lWidth = Rect.right;
       lHeight = Rect.bottom;
   }





    DeleteObject(hTempBitmap);

    bfHeader.bfType = (WORD)('B' | ('M' << 8));
    bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    biHeader.biSize = sizeof(BITMAPINFOHEADER);
    biHeader.biBitCount = 32; // 24;
    biHeader.biCompression = BI_RGB;
    biHeader.biPlanes = 1;
    biHeader.biWidth = lWidth;
    biHeader.biHeight = lHeight;

    bInfo.bmiHeader = biHeader;

    cbBits = (((biHeader.biBitCount * lWidth + 31) & ~31) / 8) * lHeight;
   // cbBits = (((24 * lWidth + 31) & ~31) / 8) * lHeight;

    hMemDC = CreateCompatibleDC(hDC);
    hBitmap = CreateDIBSection(hDC, &bInfo, DIB_RGB_COLORS, (VOID**)&bBits, NULL, 0);
   
    
    SelectObject(hMemDC, hBitmap);
    BitBlt(hMemDC, 0, 0, lWidth, lHeight, hDC, x, y, SRCCOPY);


    hFile = CreateFileW(wPath, GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    WriteFile(hFile, &bfHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
    WriteFile(hFile, &biHeader, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
    WriteFile(hFile, bBits, cbBits, &dwWritten, NULL);

    CloseHandle(hFile);

    DeleteDC(hMemDC);
  //    ReleaseDC(NULL, hDC);
    DeleteObject(hBitmap);

    return TRUE;
}


