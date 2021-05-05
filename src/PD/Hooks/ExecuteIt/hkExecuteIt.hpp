#pragma once
#include "../../SDK/Structs/Structs.hpp"
namespace Hooks
{
	namespace ExecuteIt
	{
		using FN = bool(__cdecl*)(CInstance*, CInstance*, CCode*, RValue*);
		bool __cdecl Hook(CInstance* Self, CInstance* Other, CCode* pCode, RValue* pArgs);

		void* GetTargetAddress();
	}
}
