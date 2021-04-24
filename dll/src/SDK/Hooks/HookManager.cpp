#include "HookManager.hpp"
#include <MinHook.h>
#include <Psapi.h>

MODULEINFO GetModuleInfo()
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandleA(NULL);
	if (hModule == 0)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

void CHookManager::__UnhookAll()
{
	for (auto& entry : __mHookMap)
	{
		Unhook(entry.first.c_str());
	}
}

unsigned long CHookManager::FindPattern(const char* Pattern, const char* Mask, bool StringMode, bool Override, uintptr_t SearchBase, uintptr_t SearchLength)
{
	//Get all module related information
	MODULEINFO mInfo = GetModuleInfo();

	DWORD base, size;

	if (!Override)
	{
		//Assign our base and module size
		base = (DWORD)mInfo.lpBaseOfDll;
		size = (DWORD)mInfo.SizeOfImage;
	}
	else
	{
		//Force a base and a size
		base = SearchBase;
		size = SearchLength;
	}

	//Get length for our mask, this will allow us to loop through our array
	DWORD patternLength = strlen(Mask);

	for (unsigned i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (DWORD j = 0; j < patternLength; j++)
		{
			//if we have a ? in our mask then we have true by default,
			//or if the bytes match then we keep searching until finding it or not
			found &= Mask[j] == '?' || Pattern[j] == *(char*)(base + i + j);
		}

		//found = true, our entire pattern was found
		if (found)
		{
			if (StringMode && *(char*)(base + i - 1) != '\x00')
				continue;

			return (base + i);
		}
	}
	return 0;
}

void* CHookManager::Hook(const char* lpFunctionName, void* lpTarget, void* lpDetour)
{
	LPVOID lpOriginal;

	auto Status = MH_CreateHook(lpTarget, lpDetour, &lpOriginal);

	//Prepare the Info structure
	__CHookInfo Info;
	Info.pTarget = lpTarget;
	Info.pDetour = lpDetour;
	Info.pOriginal = lpOriginal;

	if (__mHookMap.contains(std::string(lpFunctionName)))
		gDelta.RaiseError("Hook %s already registered!", lpFunctionName);

	//Cache our hook
	__mHookMap.emplace(std::string(lpFunctionName), Info);

	if (Status == MH_OK)
	{
		MH_EnableHook(lpTarget);
		return lpOriginal;
	}

	gDelta.RaiseError("HookManager::Hook('%s', 0x%p, 0x%p) failed with error %d!\nThe process will now exit.", lpFunctionName, lpTarget, lpDetour, Status);

	//Erase the hook, since it apparently wasn't successful.
	__mHookMap.erase(std::string(lpFunctionName));

	return nullptr;
}

void CHookManager::Unhook(const char* lpFunctionName)
{
	std::string sFunction(lpFunctionName);
	MH_DisableHook(__mHookMap.at(sFunction).pTarget);
	MH_RemoveHook(__mHookMap.at(sFunction).pTarget);
}
