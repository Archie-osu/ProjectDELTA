#include "hkEndScene.hpp"
#include "../../API/PD.hpp"
#include "../../SDK/Hooks/HookManager.hpp"
#include "../../SDK/Invoker/Invoker.hpp"
#include <mutex>

#include "../../../Dependencies/ImGui/imgui.h"
#include "../../../Dependencies/ImGui/imgui_impl_dx9.h"
#include "../../../Dependencies/ImGui/imgui_impl_win32.h"

inline std::once_flag g_Init;

inline HRESULT __stdcall Hooks::EndScene::Hook(LPDIRECT3DDEVICE9 lpDevice)
{
	std::call_once(g_Init, [&]() {
		gDelta.Invoker->Call("window_set_size", { RValue(1280.0), RValue(720.0) });
		RValue CurrentWindow = gDelta.Invoker->Call("window_handle", {});

		HWND Window = ReCa<HWND>(CurrentWindow.PointerValue);

		ImGui::CreateContext();
		ImGui_ImplWin32_Init(Window);
		ImGui_ImplDX9_Init(lpDevice);
	});

	auto Return = gDelta.HookManager->GetOriginal<HookFn>("hkEndScene")(lpDevice);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
	ImGui::SetNextWindowPos(ImGui::GetCursorPos());
	ImGui::ShowDemoWindow();
	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return Return;
}

void Hooks::EndScene::Init()
{
	RValue rvD3D = gDelta.Invoker->Call("window_device", {});

	//Visual Studio is retarded and thinks this doesn't close the app, so enjoy warnings.
	if (!rvD3D.PointerValue) 
		gDelta.RaiseError("Failed hooking DX9 - Device was nullptr!");

	pDevice = ReCa<IDirect3DDevice9*>(rvD3D.PointerValue);

	memcpy(ppTable, *ReCa<void***>(pDevice), sizeof(ppTable));

	gDelta.HookManager->Hook("hkEndScene", ReCa<char*>(ppTable[42]), Hook);
}
