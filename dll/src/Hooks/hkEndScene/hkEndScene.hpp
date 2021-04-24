#pragma once
#include <d3d9.h>
namespace Hooks
{
	namespace EndScene
	{
		using HookFn = HRESULT(__stdcall*)(LPDIRECT3DDEVICE9);

		inline LPDIRECT3DDEVICE9 pDevice;
		inline LPVOID ppTable[119];

		inline HRESULT __stdcall Hook(LPDIRECT3DDEVICE9);
		void Init();
	}
}