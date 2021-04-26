#pragma once
#include <d3d9.h>
namespace Hooks
{
	namespace EndScene
	{
		using FN = HRESULT(WINAPI*)(LPDIRECT3DDEVICE9);

		HRESULT WINAPI Hook(LPDIRECT3DDEVICE9);

		void* GetTargetAddress();
	}
}