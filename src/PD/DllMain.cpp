#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "SDK/Void.hpp"
#include <stdio.h>

//This is the 69th commit, nice.

#define DLL_EXPORT extern "C" __declspec(dllexport)
HMODULE g_hDLL = nullptr;

DWORD WINAPI Main(LPVOID)
{
#if _DEBUG
	AllocConsole();
	FILE* dum;
	freopen_s(&dum, "CONOUT$", "w", stdout);
#endif

#ifdef _WIN64
	#error Compile in x86, dummy!
#endif
	Void.Load();

	while (!Void.ShouldUnload())
		Sleep(33); //we sleepin'	

	Void.bIsUnloading = true;
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