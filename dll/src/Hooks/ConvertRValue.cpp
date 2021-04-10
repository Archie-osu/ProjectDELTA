#include "Hooks.hpp"
#include "../Invoker/Invoker.hpp"
#include <intrin.h>

//TODO: Figure out why it's putting null pointers there
using fnConvertRValue = int(__cdecl*)(
	Game::RValue* prValue,
	void* pNull,
	void* pZero
);

int __cdecl Hooks::hkConvertRValue(Game::RValue* prValue, void* pNull, void* pZero)
{
	fnConvertRValue ofnConvertRValue = (fnConvertRValue)Hooks::oConvertRValue;

	//Took me long enough to figure this out.
	Globals::szCurFunc = ghl::ptr_t((LPVOID)0xAB96AC);
	DWORD* CurrentOperand = *(DWORD**)((unsigned char*)_AddressOfReturnAddress() + 0x4);

	if (Globals::szCurFunc.follow().valid())
	{
		std::string sFunction(Globals::szCurFunc.get<const char*>());
		//Debug prints go here.
	}

	return ofnConvertRValue(prValue, pNull, pZero);
}