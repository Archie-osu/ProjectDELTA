#pragma once
#include <d3d11.h>
namespace Hooks
{
	namespace Present
	{
		using FN = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT);
		HRESULT WINAPI Hook(IDXGISwapChain* pThis, UINT, UINT);

		void* GetTargetAddress();

		inline ID3D11RenderTargetView* pView;
	}
}