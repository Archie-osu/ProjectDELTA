#pragma once
#include <Windows.h>
namespace Hooks
{
	namespace WndProc
	{
		void Init();

		LRESULT __stdcall hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		inline WNDPROC pOriginal;
	}
}