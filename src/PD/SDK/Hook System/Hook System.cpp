#include "Hook System.hpp"
#include "../Void.hpp"
#include "../Memory Manager/Memory Manager.hpp"
#include <MinHook.h>

void* CHookSystem::Hook(const char* lpFuncName, void* lpTarget, void* lpDetour)
{
	void* lpOriginal = nullptr;

	if (!Void.PatternManager->IsValidMemory(lpTarget)) {
		Void.Error("Hook %s is in invalid memory %p!", lpFuncName, lpTarget);
	}

	auto Status = MH_CreateHook(lpTarget, lpDetour, &lpOriginal);

	//Prepare the Info structure
	HookInfo_t Info;
	Info.Target = lpTarget;
	Info.Detour = lpDetour;
	Info.Origin = lpOriginal;

	if (prHookMap.contains(std::string(lpFuncName)))
	{
		if (prHookMap.at(std::string(lpFuncName)).Unhooked)
		{
			MH_EnableHook(lpTarget);
			prHookMap.at(std::string(lpFuncName)).Unhooked = false;
			prHookMap.at(std::string(lpFuncName)).Origin = lpOriginal;
			return lpOriginal;
		}
		
		Void.Error("Hook %s already registered!", lpFuncName);
	}
	
	//Cache the hook
	prHookMap.try_emplace(std::string(lpFuncName), Info);

	if (Status == MH_OK)
	{
		MH_EnableHook(lpTarget);
		return lpOriginal;
	}

	Void.Error("Hook('%s', 0x%p, 0x%p) failed with error %d!", lpFuncName, lpTarget, lpDetour, Status);

	//Erase the hook, since it apparently wasn't successful.
	prHookMap.erase(std::string(lpFuncName));

	return nullptr;
}

void CHookSystem::Unhook(const char* lpFuncName)
{
	std::string sFunction(lpFuncName);
	MH_DisableHook(prHookMap.at(sFunction).Target);
	MH_RemoveHook(prHookMap.at(sFunction).Target);

	prHookMap.at(sFunction).Unhooked = true;
}

void CHookSystem::UnhookAll()
{
	for (auto& entry : prHookMap)
		this->Unhook(entry.first.c_str());
}
