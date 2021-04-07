#include "src/API/ghlib.hpp"
#include "src/Hooks/Hooks.hpp"

#define DLL_EXPORT extern "C" __declspec(dllexport)

DWORD WINAPI Main(LPVOID lpDLL)
{
	//Enable this to have a console open up at inject
	//Will have debug commands soon!
	
	//ghl::AllocConsoleStream();
	
	Hooks::Initialize();

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