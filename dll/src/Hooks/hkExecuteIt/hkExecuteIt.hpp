#pragma once
struct RValue;
namespace Hooks
{
	namespace ExecuteIt
	{
		using HookFn = bool(__cdecl*)(void*, void*, void*, RValue*);

		inline bool __cdecl Hook(void* pSelf, void* pOther, void* pCode, RValue* Args);
		void Init();
	}
}