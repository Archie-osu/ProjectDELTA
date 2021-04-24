#pragma once
#include <d3d11.h>
namespace Hooks
{
	namespace Present
	{
		using HookFn = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT);

		inline HRESULT __stdcall Hook(IDXGISwapChain* pSwapChain, UINT Sync, UINT Flags);
		void Init();

		inline ID3D11RenderTargetView* pRenderView		= nullptr;
		inline ID3D11DeviceContext* pContext			= nullptr;
	}
}