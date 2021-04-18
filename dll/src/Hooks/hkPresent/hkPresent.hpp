#pragma once
#include <d3d11.h>
namespace Hooks
{
	namespace Present
	{
		using HookFn = HRESULT(__stdcall*)(IDXGISwapChain* pSwapchain, UINT Sync, UINT Flags);
		inline HookFn pOriginal(nullptr);

		inline ID3D11Device* pDevice(nullptr);
		inline ID3D11DeviceContext* pContext(nullptr);
		inline ID3D11RenderTargetView* pTargetView(nullptr);

		void Init();

		HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapchain, UINT Sync, UINT Flags);
	}
}