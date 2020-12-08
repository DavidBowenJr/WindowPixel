#include "QueryTimer.h"
#include "framework.h"

// Note this was borrowed from some where else
// There are many many many ways to do time.
// KISS keep it simple stupid.
// Macro
/* Use to initialize the clock */
#define TIMER_INIT \
LARGE_INTEGER frequency; \
LARGE_INTEGER t1, t2; \
double elapsedTime; \
QueryPerformanceFrequency(&frequency);

/* Use to start the performance timer */
#define TIMER_START QueryPerformanceCounter(&t1);

#define TIMER_STOP \
QueryPerformanceCounter(&t2); \
elapsedTime = (double) (t2.QuadPart - t1.QuadPart) / frequency.QuadPart;

QueryTimer::QueryTimer()
{
	// double
	PCFreq = 0.0;
	// __int64
	CounterStart = 0;
}

QueryTimer::~QueryTimer()
{
	OutputDebugString(_T("Class QueryTimer ~Called.\n"));
}

void QueryTimer::StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
	{
		// Report Failure
	}
	PCFreq = double(li.QuadPart) / 1000.0;
	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}

double QueryTimer::GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}

/**
	This program should output a number close to 1000
(windows sleep isn't that accurate, but it should be like 999).
	The StartCounter() function records the number of ticks the performance counter has in the CounterStart variable.
	If you wan't to have the timer use seconds then change
	PCFreq = double(li.QuadPart)/1000.0
to
PCFreq = double(li.QuadPart);
or if you want microseconds the use
PCFreq = double(li.QuadPart) / 1000000.0;
// But really it's about convenience since it returns a double.
**/

/*
int  main()
{
StartCounter();
Sleep(1000);
cout << GetCounter() << "\n";
return 0;
*/