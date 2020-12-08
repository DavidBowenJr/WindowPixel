#pragma once
class QueryTimer
{
	// Very Cheasy chrono might be better
public:
	QueryTimer(void);
	~QueryTimer(void);

	double PCFreq; // = 0.0d;
	__int64 CounterStart; // = 0;

	void StartCounter();
	double GetCounter();

};

