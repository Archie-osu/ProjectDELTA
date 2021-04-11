#include "Memory.hpp"
#include <TlHelp32.h>

HWND g_GameWindow = NULL;

std::string Memory::GetCurrentProcessName()
{
	DWORD ProcID = GetCurrentProcessId();
	
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0xDE17A);
	PROCESSENTRY32 peEntry = { sizeof(PROCESSENTRY32) };

	Process32First(hSnapshot, &peEntry);
	do
	{
		if (peEntry.th32ProcessID == ProcID)
		{
			std::wstring wsProcessName(peEntry.szExeFile);
			std::string sProcessName(wsProcessName.begin(), wsProcessName.end());

			CloseHandle(hSnapshot);
			return sProcessName;
		}

	} while (Process32Next(hSnapshot, &peEntry));

	return std::string();
}

BOOL __stdcall EnumWndCallback(HWND hwnd, LPARAM)
{
	DWORD wndProcId;
	GetWindowThreadProcessId(hwnd, &wndProcId);

	if (GetCurrentProcessId() != wndProcId)
		return true; // skip to next window

	char wndText[24];
	//Exclude the console window, this causes bugs when it's enabled.
	if (GetWindowTextA(hwnd, wndText, 24))
	{
		if (_stricmp("Project DELTA v2", wndText) == 0)
			return true;
	}

	g_GameWindow = hwnd;
	return false;
}

HWND Memory::GetCurrentWindow()
{
	g_GameWindow = 0;
	EnumWindows(EnumWndCallback, NULL);
	return g_GameWindow;
}

void ghl::AllocConsoleStream()
{
	FILE* pFile;
	AllocConsole();
	freopen_s(&pFile, "CONIN$", "r", stdin);
	freopen_s(&pFile, "CONOUT$", "w", stderr);
	freopen_s(&pFile, "CONOUT$", "w", stdout);
	SetConsoleTitleA("Project DELTA v2");
}