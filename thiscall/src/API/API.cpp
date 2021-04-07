#include "API.hpp"

BOOL __stdcall Useful::EnumWndCallback(HWND hwnd, LPARAM)
{
	DWORD wndProcId;
	GetWindowThreadProcessId(hwnd, &wndProcId);

	if (GetCurrentProcessId() != wndProcId)
		return true; // skip to next window

	char wndText[24];
	//Exclude the console window, this causes bugs when it's enabled.
	if (GetWindowTextA(hwnd, wndText, 24))
	{
		if (_stricmp("Project DELTA", wndText) == 0)
			return true;
	}

	g_GameWindow = hwnd;
	return false;
}

HWND Useful::GetProcessWindow()
{
	g_GameWindow = NULL;
	EnumWindows(EnumWndCallback, NULL);
	return g_GameWindow;
}