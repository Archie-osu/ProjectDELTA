#pragma once
#include <Windows.h>
namespace Hooks
{
	namespace WndProc
	{
		LRESULT WINAPI Hook(HWND hWnd, UINT Msg, WPARAM w, LPARAM l);
		void Init();

		inline WNDPROC Original;
	}
}