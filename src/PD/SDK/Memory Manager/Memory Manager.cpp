#include "Memory Manager.hpp"
#include "../Hook System/Hook System.hpp"
#include "../Void.hpp"
#include <Windows.h>
#include <Psapi.h>

MODULEINFO GetCurrentModuleInfo()
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandleA(NULL);
	if (hModule == 0)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

unsigned long CPatternManager::GetFuncAddress(const char* ModuleName, const char* FunctionName)
{
	HMODULE Module = GetModuleHandleA(ModuleName);

	if (!Module)
		return 0;

	return ReCa<unsigned long>(GetProcAddress(Module, FunctionName));
}

unsigned long CPatternManager::PatternScan(const char* Pattern, const char* Mask, bool StringMode)
{
	//Get all module related information
	MODULEINFO mInfo = GetCurrentModuleInfo();

	DWORD base, size;

	//Assign our base and module size
	base = (DWORD)mInfo.lpBaseOfDll;
	size = (DWORD)mInfo.SizeOfImage;

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

unsigned long CPatternManager::OffsetScan(uintptr_t Base, uintptr_t MaxOffset, const char* Pattern, const char* Mask)
{
	//Get length for our mask, this will allow us to loop through our array
	DWORD patternLength = strlen(Mask);

	for (unsigned i = 0; i < MaxOffset - patternLength; i++)
	{
		bool found = true;
		for (DWORD j = 0; j < patternLength; j++)
		{
			//if we have a ? in our mask then we have true by default,
			//or if the bytes match then we keep searching until finding it or not
			found &= Mask[j] == '?' || Pattern[j] == *(char*)(Base + i + j);
		}

		//found = true, our entire pattern was found
		if (found)
		{
			return (Base + i);
		}
	}
	return 0;
}

//Thanks to GuidedHacking, adapted the source code slightly.
unsigned long CPatternManager::RegionScan(uintptr_t MaxOffset, const char* Pattern, const char* Mask)
{
	MEMORY_BASIC_INFORMATION memInformation;
	unsigned long Address = ReCa<unsigned long>(GetModuleHandleA(NULL));
	
	while (VirtualQuery(ReCa<LPCVOID>(Address), &memInformation, sizeof(memInformation)))
	{
		Address += memInformation.RegionSize;

		//No exceptions, please..
		if (memInformation.Protect & PAGE_GUARD || memInformation.Protect & PAGE_NOACCESS)
			continue;

		if (!(memInformation.State & MEM_COMMIT))
			continue;

		for (unsigned i = 0; i < MaxOffset - strlen(Mask) && i < memInformation.RegionSize - strlen(Mask); i++)
		{
			bool found = true;
			for (unsigned j = 0; j < strlen(Mask); j++)
			{
				//if we have a ? in our mask then we have true by default,
				//or if the bytes match then we keep searching until finding it or not
				found &= Mask[j] == '?' || Pattern[j] == *((char*)memInformation.BaseAddress + i + j);
			}

			if (found)
				return ReCa<unsigned long>(ReCa<char*>(memInformation.BaseAddress) + i);
		}
	}

	return 0;
}

bool CPatternManager::IsValidMemory(void* Pointer)
{
	MEMORY_BASIC_INFORMATION info;
	if (VirtualQuery(Pointer, &info, sizeof(info)))
	{
		if (info.Protect & PAGE_GUARD || info.Protect & PAGE_NOACCESS)
			return false;

		return true;
	}

	return false;
}

void* CMemoryManager::Alloc(size_t nSize, bool bZeroMemory)
{
	void* Data = Void.HookSystem->GetOriginal<void* (__cdecl*)(size_t, const char*, int, bool)>
		("YYAlloc")(nSize, "PD\\SDK\\Memory Manager\\MemoryManager.cpp", __LINE__, bZeroMemory);

	return Data;
}

void CMemoryManager::Free(void* block)
{
	Void.HookSystem->GetOriginal<void (__cdecl*)(void*)>
		("YYFree")(block);
}