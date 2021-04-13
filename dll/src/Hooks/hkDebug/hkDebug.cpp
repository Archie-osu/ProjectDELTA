#include "hkDebug.hpp"
#include <MinHook.h>
#include "../../Core/Structures/RValue.hpp"

void __cdecl Hooks::DebugHook::hkDebug(RValue* Result, CInstance* pSelfInst, CInstance* pOtherInst, int argc, RValue* pArgs)
{
	return pOriginal(Result, pSelfInst, pOtherInst, argc, pArgs);
}

void Hooks::DebugHook::Init()
{
	MH_CreateHook((LPVOID)0x52B390, hkDebug, reinterpret_cast<LPVOID*>(&pOriginal));
	MH_EnableHook(MH_ALL_HOOKS);
}

