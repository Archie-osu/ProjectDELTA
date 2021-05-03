#pragma once
#include "../../SDK/Structs/Structs.hpp"
namespace Hooks
{
	namespace ExecuteIt
	{
		using FN = void(__cdecl*)(CInstance*, CInstance*, CCode*, RValue*);
		void __cdecl Hook(CInstance* Self, CInstance* Other, CCode* pCode, RValue* pArgs);

		void* GetTargetAddress();
	}
}
