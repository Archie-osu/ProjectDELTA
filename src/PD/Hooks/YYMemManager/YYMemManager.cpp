#include "YYMemManager.hpp"
#include "../../SDK/Void.hpp"
#include "../../SDK/Invoker/Invoker.hpp"
#include "../../SDK/Hook System/Hook System.hpp"
#include "../../SDK/Memory Manager/Memory Manager.hpp"

void* Hooks::YYMemManager::AllocHook(unsigned int size, const char* szFile, int line, bool clear)
{
	return Void.HookSystem->GetOriginal<FNAlloc>("YYAlloc")(size, szFile, line, clear);
}

void Hooks::YYMemManager::FreeHook(void* block)
{
	printf("Freeing %p\n", block);
	Void.HookSystem->GetOriginal<FNFree>("YYFree")(block);
}

void Hooks::YYMemManager::Init()
{
	uintptr_t lpCall = 0;
	uintptr_t lpDest = 0;

	//YYMemoryManager::Alloc
	{
		/*
			call YYMemoryManager::Alloc (first call in the function)
		*/

		lpCall = Void.PatternManager->OffsetScan(Void.Invoker->FindFunction("show_message"), 0x100, "\xE8\x00\x00\x00\x00", "x????");
		lpDest = *cast<uintptr_t*>(lpCall + 1); //Offset for E8 call
		lpDest += lpCall + 5; //Account for EIP change.

		Void.HookSystem->Hook("YYAlloc", cast<void*>(lpDest), AllocHook);
	}

	//YYMemoryManager::Free
	{
		/*
			call YYMemoryManager::Free
			add esp, 10
			ret
		*/
		
		lpCall = Void.PatternManager->OffsetScan(Void.Invoker->FindFunction("show_message"), 0x100, "\xE8\x00\x00\x00\x00\x83\xC4\x10\xC3", "x????xxxx");
		lpDest = *cast<uintptr_t*>(lpCall + 1); //Offset for E8 call
		lpDest += lpCall + 5; //Account for EIP change.
		Void.HookSystem->Hook("YYFree", cast<void*>(lpDest), FreeHook);
	}
}
