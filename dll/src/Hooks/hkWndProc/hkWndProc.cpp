#include "hkWndProc.hpp"
#include "../../API/PD.hpp"
#include "../../Dependencies/ImGui/imgui.h"
#include "../../SDK/Invoker/Invoker.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI Hooks::WndProc::hkWndProc(HWND hWnd, UINT Msg, WPARAM w, LPARAM l)
{
	LRESULT Result;
	if (Result = ImGui_ImplWin32_WndProcHandler(hWnd, Msg, w, l))
		return Result;

	return CallWindowProc(Original, hWnd, Msg, w, l);
}


void Hooks::WndProc::Init()
{
	RValue CurrentWindow = gDelta.Invoker->Call("window_handle", {});
	Original = (WNDPROC)(SetWindowLongW(ReCa<HWND>(CurrentWindow.PointerValue), GWLP_WNDPROC, (LONG_PTR)(&hkWndProc)));
}