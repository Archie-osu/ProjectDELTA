#include "hkDoCallScript.hpp"
#include "../../SDK/Void.hpp"
#include "../../SDK/Hook System/Hook System.hpp"
#include "../../SDK/Memory Manager/Memory Manager.hpp"
#include "../../SDK/Callback Manager/Callback Manager.hpp"
#include "../../SDK/Structs/Structs.hpp"

uint8_t* __cdecl Hooks::DoCallScript::Hook(CScript* pScript, int argc, uint8_t* pStackPointer, VMExec* pVM, YYObjectBase* pLocals, YYObjectBase* pArguments)
{
	Void.CallbackManager->Call(CCallbackManager::Types::VMEXEC_BEGIN, { pScript, pVM, pScript->s_code, pLocals, pArguments });

	auto ret = Void.HookSystem->GetOriginal<FN>("DoCallScript")(pScript, argc, pStackPointer, pVM, pLocals, pArguments);

	Void.CallbackManager->Call(CCallbackManager::Types::VMEXEC_END, { &ret, pScript, pVM, pScript->s_code, pLocals, pArguments });
	return ret;
}

uint8_t Hooks::DoCallScript::NormalizeHook(uint32_t Base)
{
	constexpr char sub_esp_12[] = { "\x83\xEC\x0C" };

	if (memcmp(ReCa<void*>(Base - 4), sub_esp_12, 3) == 0)
		return 4;

	uint8_t Instruction;
	memcpy(&Instruction, ReCa<const void*>(Base - 1), 1);

	if (Instruction > 0x4F && Instruction < 0x58) /* push eax <-> push edi */
	{
		/* TS!US version? */
		uint8_t SecondInstruction;
		memcpy(&SecondInstruction, ReCa<const void*>(Base - 2), 1);

		if (SecondInstruction > 0x4F && SecondInstruction < 0x58) /* push eax <-> push edi */
			return 2;

		return 1;
	}

	return 0;
}

void* Hooks::DoCallScript::GetTargetAddress()
{
	auto p = Void.MemoryManager->PatternScan("\x8B\x00\x24\x00\x00\x8B\x00\x24\x00\x85\x00\x75\x0E\x68", "x?x??x?x?x?xxx", false);
	
	if (!p)
	{
		Void.Warning("Cannot find the DoCallScript function.");
		return nullptr;
	}

	p -= Hooks::DoCallScript::NormalizeHook(p);

	return ReCa<void*>(p);
}
