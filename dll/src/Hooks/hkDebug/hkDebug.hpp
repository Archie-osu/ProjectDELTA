#pragma once
#include "../../Core/Structures/CallData.hpp"
namespace Hooks
{
	namespace DebugHook
	{
		using HookFn = fnGML;

		inline HookFn pOriginal;

		void __cdecl hkDebug(RValue* Result, CInstance* pSelfInst, CInstance* pOtherInst, int argc, RValue* pArgs);

		void Init();
	}
}