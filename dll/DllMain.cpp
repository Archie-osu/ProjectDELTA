#include <Windows.h>
#include "src/Core/Core.hpp"
#define DLL_EXPORT extern "C" __declspec(dllexport)

DWORD WINAPI Main(LPVOID lpDLL)
{
	

	return 0;
}

DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0, Main, hinstDLL, 0, 0);
	}

	return true;
}