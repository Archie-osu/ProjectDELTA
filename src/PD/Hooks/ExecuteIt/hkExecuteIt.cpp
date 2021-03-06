#include "hkExecuteIt.hpp"
#include "../../SDK/Void.hpp"
#include "../../SDK/Hook System/Hook System.hpp"
#include "../../SDK/Memory Manager/Memory Manager.hpp"
#include "../../SDK/Callback Manager/Callback Manager.hpp"
#include "../../SDK/Lua Engine/Lua Engine.hpp"
#include <intrin.h>

bool __cdecl Hooks::ExecuteIt::Hook(CInstance* Self, CInstance* Other, CCode* pCode, RValue* pArgs)
{
	bool bIsDrawEvent = false;
	if (pCode)
		if (pCode->i_pName)
			bIsDrawEvent = strstr(pCode->i_pName, "Draw_0");

	Void.CallbackManager->Call(CCallbackManager::Types::VMEXEC_BEGIN, {
		ReCa<void*>(CCallbackManager::Types::VMEXEC_BEGIN),
		ReCa<void*>(Self),
		ReCa<void*>(Other),
		ReCa<void*>(pCode),
		ReCa<void*>(pArgs)
	});

	if (bIsDrawEvent)
	{
		Void.CallbackManager->Call(CCallbackManager::Types::DRAW_BEGIN, {
			ReCa<void*>(CCallbackManager::Types::DRAW_BEGIN),
			ReCa<void*>(Self),
			ReCa<void*>(Other),
			ReCa<void*>(pCode),
			ReCa<void*>(pArgs)
		});

	}

	auto ret = Void.HookSystem->GetOriginal<FN>("ExecuteIt")(Self, Other, pCode, pArgs);

	if (bIsDrawEvent)
	{
		Void.CallbackManager->Call(CCallbackManager::Types::DRAW_END, {
			ReCa<void*>(CCallbackManager::Types::DRAW_END),
			ReCa<void*>(&ret),
			ReCa<void*>(Self),
			ReCa<void*>(Other),
			ReCa<void*>(pCode),
			ReCa<void*>(pArgs)
		});
	}

	Void.CallbackManager->Call(CCallbackManager::Types::VMEXEC_END, {
		ReCa<void*>(CCallbackManager::Types::VMEXEC_END),
		ReCa<void*>(&ret),
		ReCa<void*>(Self),
		ReCa<void*>(Other),
		ReCa<void*>(pCode),
		ReCa<void*>(pArgs)
	});

	return ret;
}

uint8_t Hooks::ExecuteIt::NormalizeHook(uint32_t Base)
{
	constexpr char sub_esp_8[] = { "\x83\xEC\x08" };

	if (memcmp(ReCa<void*>(Base - 3), sub_esp_8, 3) == 0)
		return 3;

	uint8_t Instruction;
	memcpy(&Instruction, ReCa<const void*>(Base - 1), 1);

	if (Instruction > 0x4F && Instruction < 0x58) /* push eax <-> push edi */
		return 1;

	return 0;
}

void* Hooks::ExecuteIt::GetTargetAddress()
{
	auto p = Void.PatternManager->PatternScan("\x80\x3D\x00\x00\x00\x00\x00\x00\x0F\x84", "xx????x?xx", false);

	if (!p)
	{
		Void.Warning("Cannot find the ExecuteIt function. This is usually because the game is made with YYC.");
		return nullptr;
	}
	
	p -= Hooks::ExecuteIt::NormalizeHook(p);

	return ReCa<void*>(p);
}
