#pragma once
#include <d3d11.h>
namespace Hooks
{
	void Initialize();

	HRESULT __stdcall hkPresent(
		IDXGISwapChain* pSwapchain,
		UINT Sync,
		UINT Flags
	);

	LRESULT __stdcall hkWndProc(
		HWND hWnd, 
		UINT uMsg, 
		WPARAM wParam, 
		LPARAM lParam
	);

	inline ID3D11Device* pDevice = nullptr;
	inline ID3D11DeviceContext* pContext = nullptr;

	inline void* uPresent = nullptr; //Actual original, used when unloading. Can't call this one because of hook recursion.
	inline void* oPresent = nullptr;
	inline WNDPROC oWndProc = nullptr;
}