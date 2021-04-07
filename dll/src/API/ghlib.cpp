#define WIN32_LEAN_AND_MEAN 1

#include "ghlib.hpp"
#include <stdio.h>
#include <Windows.h>

void ghl::AllocConsoleStream()
{
	FILE* pFile;
	AllocConsole();
	freopen_s(&pFile, "CONIN$", "r", stdin);
	freopen_s(&pFile, "CONOUT$", "w", stderr);
	freopen_s(&pFile, "CONOUT$", "w", stdout);
	SetConsoleTitleA("Project DELTA");
}