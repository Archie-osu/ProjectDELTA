#include "hkWndProc.hpp"
#include "../../Dependencies/ImGui/imgui.h"
#include "../../SDK/Memory/Memory.hpp"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void Hooks::WndProc::Init()
{
	pOriginal = (WNDPROC)(SetWindowLongW(Memory::GetCurrentWindow(), GWLP_WNDPROC, (LONG_PTR)(&hkWndProc)));
}

LRESULT __stdcall Hooks::WndProc::hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(pOriginal, hWnd, uMsg, wParam, lParam);
}
