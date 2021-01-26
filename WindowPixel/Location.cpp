#include "framework.h"
#include "Location.h"

void Location::m_GetCurrentDirectory()
{
	GetCurrentDirectoryA(256, this->searchPath);
}

void Location::m_MultiByteToWideChar()
{
	MultiByteToWideChar(CP_ACP, 0, this->searchPath, -1, wSearchPath, 256);
}


