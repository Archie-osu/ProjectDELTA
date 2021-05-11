#include "hkExecuteIt.hpp"
#include "../../SDK/Void.hpp"
#include "../../SDK/Hook System/Hook System.hpp"
#include "../../SDK/Memory Manager/Memory Manager.hpp"
#include "../../SDK/Callback Manager/Callback Manager.hpp"
#include "../../SDK/Lua Engine/Lua Engine.hpp"

bool __cdecl Hooks::ExecuteIt::Hook(CInstance* Self, CInstance* Other, CCode* pCode, RValue* pArgs)
{
	bool bIsDrawEvent = false;

	if (strstr(pCode->i_pName, "Draw_0"))
		bIsDrawEvent = true;

	Void.CallbackManager->Call(CCallbackManager::Types::VMEXEC_BEGIN, { Self, Other, pCode, pArgs });

	if (bIsDrawEvent)
	{
		Void.CallbackManager->Call(CCallbackManager::Types::DRAW_BEGIN, { Self, Other, pCode, pArgs });
		Void.LuaCallbackManager->Call(CLuaCallbackManager::Types::ON_DRAW);
	}

	auto ret = Void.HookSystem->GetOriginal<FN>("ExecuteIt")(Self, Other, pCode, pArgs);

	if (bIsDrawEvent)
		Void.CallbackManager->Call(CCallbackManager::Types::DRAW_END, { &ret, Self, Other, pCode, pArgs });

	Void.LuaCallbackManager->Call(CLuaCallbackManager::Types::ON_VMEXEC);
	Void.CallbackManager->Call(CCallbackManager::Types::VMEXEC_END, { &ret, Self, Other, pCode, pArgs });

	return ret;
}

uint8_t NormalizeHook(uint32_t base)
{
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
		
	p -= NormalizeHook(p);

	return ReCa<void*>(p);
}
