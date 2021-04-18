#pragma once
#include <d3d9.h>
namespace Hooks
{
	namespace EndScene
	{
		using HookFn = HRESULT(__stdcall*)(PDIRECT3DDEVICE9 pDevice);
		inline HookFn pOriginal(nullptr);
		inline PDIRECT3DDEVICE9 pDevice(nullptr);

		inline LPVOID pDummyTable[119];

		void Init();

		HRESULT __stdcall hkEndScene(PDIRECT3DDEVICE9 pDevice);
	}
}