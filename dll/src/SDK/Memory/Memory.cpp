#include "Memory.hpp"
#include <Psapi.h>
#include <TlHelp32.h>

HWND g_GameWindow = NULL;

MODULEINFO GetModuleInfo()
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandleA(NULL);
	if (hModule == 0)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

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

DWORD Memory::FindPattern(const char* Pattern, const char* Mask)
{
	//Get all module related information
	MODULEINFO mInfo = GetModuleInfo();

	//Assign our base and module size
	DWORD base = (DWORD)mInfo.lpBaseOfDll;
	DWORD size = (DWORD)mInfo.SizeOfImage;

	//Get length for our mask, this will allow us to loop through our array
	DWORD patternLength = (DWORD)strlen(Mask);

	for (unsigned i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (DWORD j = 0; j < patternLength; j++)
		{
			//if we have a ? in our mask then we have true by default,
			//or if the bytes match then we keep searching until finding it or not
			found &= Mask[j] == '?' || Pattern[j] == *(char*)(base + i + j);
		}

		//found = true, our entire pattern was found
		if (found)
		{
			return (base + i);
		}
	}
	return 0;
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