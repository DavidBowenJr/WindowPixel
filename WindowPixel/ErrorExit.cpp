//#include <windows.h>
#include "ErrorExit.h"
#include <system_error>
#include <memory>
#include <strsafe.h>


//  https:  //docs.microsoft.com/en-us/windows/win32/debug/retrieving-the-last-error-code

void ErrorExit(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dwErrorCode = GetLastError();


	// LOCALE_NAME_SYSTEM_DEFAULT _INVARIANT _USER_DEFAULT



	LPSTR psz{ nullptr };
const DWORD cchMsg =	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
		0, NULL);




	// Display the error message and exit the process
	// Maybe.
	lpDisplayBuf = (LPVOID)LocalAlloc(
		LMEM_ZEROINIT,
		(lstrlen( (LPCTSTR)lpMsgBuf) + lstrlen( (LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));

	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dwErrorCode, lpMsgBuf);

	MessageBox(NULL, (LPCTSTR)cchMsg, TEXT("Error"), MB_OK);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dwErrorCode);

}

/*
void main()
{
	// Generate an error
	if(!GetProcessID(NULL))
	ErrorExit(TEXT("GetProcessId"));
	}
*/