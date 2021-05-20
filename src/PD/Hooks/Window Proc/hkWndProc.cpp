#include "hkWndProc.hpp"
#include "../Present/hkPresent.hpp"
#include "../../SDK/Void.hpp"
#include "../../UI/UI.hpp"
#include <ImGui/imgui.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall Hooks::WndProc::Hook(HWND hWnd, UINT Msg, WPARAM w, LPARAM l)
{
	LRESULT Result;
	if (Result = ImGui_ImplWin32_WndProcHandler(hWnd, Msg, w, l))
		return Result;

	if (Msg == WM_CLOSE)
		exit(0);

	if (UI::bOpen && UI::bNoMovementInMenu)
		return DefWindowProc(hWnd, Msg, w, l); //Disable input into the game while in the menu

	return CallWindowProc(Original, hWnd, Msg, w, l);
}

void Hooks::WndProc::Init()
{
	Original = ReCa<WNDPROC>(SetWindowLong(ReCa<HWND>(Void.GetGameWindow()), GWL_WNDPROC, ReCa<ULONG>(&Hook)));
}
