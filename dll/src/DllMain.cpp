#define WIN32_LEAN_AND_MEAN
#include "Core/Core.hpp"
#include <Windows.h>

#define DLL_EXPORT extern "C" __declspec(dllexport)
HMODULE g_hDLL = nullptr;

DWORD WINAPI Main(LPVOID)
{
	Core::Start();

	while (!GetAsyncKeyState(VK_F12))
		Sleep(50);

	Core::Kill();
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