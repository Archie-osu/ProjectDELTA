#pragma once
#include "../../SDK/Structs/Structs.hpp"

//uint8 *__fastcall DoCallScript(CScript *pScript, int argc, uint8 *pSP, VMExec *pVMExec, YYObjectBase *pLocals, YYObjectBase *pArguments)
namespace Hooks
{
	namespace DoCallScript
	{
		using FN = uint8_t*(__cdecl*)(CScript*, int, uint8_t*, VMExec*, void*, void*);

		uint8_t* __cdecl Hook(CScript* pScript, int argc, uint8_t* pStackPointer, VMExec* pVM, YYObjectBase* pLocals, YYObjectBase* pArguments);

		uint8_t NormalizeHook(uint32_t Base);

		void* GetTargetAddress();
	}
}