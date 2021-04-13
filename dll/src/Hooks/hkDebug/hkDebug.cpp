#include "hkDebug.hpp"
#include <MinHook.h>

int __cdecl Hooks::DebugHook::hkDebug(CCallData Data)
{
	return pOriginal(Data);
}

void Hooks::DebugHook::Init()
{
	MH_CreateHook((LPVOID)0x52B390, hkDebug, reinterpret_cast<LPVOID*>(&pOriginal));
	MH_EnableHook(MH_ALL_HOOKS);
}

