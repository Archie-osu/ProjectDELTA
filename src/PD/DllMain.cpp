#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "SDK/Void.hpp"

#define DLL_EXPORT extern "C" __declspec(dllexport)
HMODULE g_hDLL = nullptr;

DWORD WINAPI Main(LPVOID)
{
	Void.Load();

	while (!Void.ShouldUnload())
	{
		if (GetAsyncKeyState(VK_F12))
			Void.DumpDataToFile();

		Sleep(33); //we sleepin'
	}
		

	Void.Unload();
	
	FreeLibraryAndExitThread(g_hDLL, 0);
	return 0x0;
}

BOOL APIENTRY DllMain(HMODULE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		g_hDLL = hinstDLL;
		CreateThread(0, 0, Main, 0, 0, 0);
	}
	return true;
}