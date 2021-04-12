#pragma once
#include "../../Core/Structures/CallData.hpp"
namespace Hooks
{
	namespace DebugHook
	{
		using HookFn = int(__cdecl*)(
			CCallData Data
		);

		inline HookFn pOriginal;

		int __cdecl hkDebug(CCallData Data);

		void Init();
	}
}