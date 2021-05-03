#include "hkExecuteIt.hpp"
#include "../../SDK/Void.hpp"
#include "../../SDK/Hook System/Hook System.hpp"
#include "../../SDK/Memory Manager/Memory Manager.hpp"

void __cdecl Hooks::ExecuteIt::Hook(CInstance* Self, CInstance* Other, CCode* pCode, RValue* pArgs)
{
	return Void.HookSystem->GetOriginal<FN>("ExecuteIt")(Self, Other, pCode, pArgs);
}

uint8_t CalculateStackMovement(uint32_t base)
{
	//THAT'S A LOT OF CHAR ARRAYS
	constexpr char sub_esp_8[] = { "\x83\xEC\x08" };

	if (memcmp(ReCa<void*>(base - 3), sub_esp_8, 3) == 0)
		return 3;

	uint8_t Instruction;
	memcpy(&Instruction, ReCa<const void*>(base - 1), 1);

	if (Instruction > 0x4F && Instruction < 0x58) /* push eax <-> push edi */
			return 1;

	return 0;
}

void* Hooks::ExecuteIt::GetTargetAddress()
{
	auto p = Void.MemoryManager->PatternScan("\x80\x3D\x00\x00\x00\x00\x00\x00\x0F\x84", "xx????x?xx", false);

	if (!p)
	{
		Void.Warning("Cannot find the ExecuteIt function. This is usually because the game is made with YYC.");
		return nullptr;
	}
		
	p -= CalculateStackMovement(p);

	return ReCa<void*>(p);
}
