#include "hkWndProc.hpp"
#include "../../SDK/Void.hpp"
#include <ImGui/imgui.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall Hooks::WndProc::Hook(HWND hWnd, UINT Msg, WPARAM w, LPARAM l)
{
	LRESULT Result;
	if (Result = ImGui_ImplWin32_WndProcHandler(hWnd, Msg, w, l))
		return Result;

	return CallWindowProc(Original, hWnd, Msg, w, l);
}

void Hooks::WndProc::Init()
{
	Original = ReCa<WNDPROC>(SetWindowLongW(ReCa<HWND>(Void.GetGameWindow()), GWLP_WNDPROC, ReCa<ULONG>(&Hook)));
}
