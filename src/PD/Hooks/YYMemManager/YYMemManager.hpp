#pragma once
namespace Hooks
{
	namespace YYMemManager
	{
		using FNAlloc = void* (__cdecl*)(unsigned int, const char*, int, bool);
		using FNFree = void (__cdecl*)(void*);

		void* AllocHook(unsigned int size, const char* szFile, int line, bool clear);
		void FreeHook(void* block);

		void Init();
	}
}