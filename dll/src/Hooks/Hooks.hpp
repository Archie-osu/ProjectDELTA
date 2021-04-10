#pragma once
#include <d3d11.h>
#include "../API/API.hpp"
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

	int __cdecl hkConvertRValue(
		Game::RValue* prValue,
		void* pNull,
		void* pZero
	);



	inline ID3D11Device* pDevice = nullptr;
	inline ID3D11DeviceContext* pContext = nullptr;

	inline void* oDebug = nullptr; //Used whenever I feel like debugging hooks
	inline void* uPresent = nullptr; //Actual original, used when unloading. Can't call this one because of hook recursion.
	inline void* oPresent = nullptr;
	inline void* oConvertRValue = nullptr;
	inline WNDPROC oWndProc = nullptr;
}