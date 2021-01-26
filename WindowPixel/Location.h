#pragma once
class Location
{
public:
	char searchPath[256];
	wchar_t wSearchPath[256];

	void m_GetCurrentDirectory();
	void m_MultiByteToWideChar();

};

