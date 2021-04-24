#pragma once
#include <Windows.h>

namespace Hooks
{
	namespace WndProc
	{
		LRESULT WINAPI hkWndProc(HWND hWnd, UINT Msg, WPARAM w, LPARAM l);
		void Init();

		inline WNDPROC Original;
	}
}