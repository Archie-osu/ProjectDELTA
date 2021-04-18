#include <Windows.h>
#include "src/Core/Core.hpp"
#include <MinHook.h>
#define DLL_EXPORT extern "C" __declspec(dllexport)

DWORD WINAPI Main(LPVOID lpDLL)
{
	MH_Initialize();
	Core::Init();

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