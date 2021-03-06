#include "hkEndScene.hpp"
#include "../../SDK/Callback Manager/Callback Manager.hpp"
#include "../../SDK/Hook System/Hook System.hpp"
#include "../../SDK/Lua Engine/Lua Engine.hpp"
#include "../../SDK/Void.hpp"
#include "../../UI/UI.hpp"
#include <ImGui/imgui_impl_dx9.h>
#include <ImGui/imgui_impl_win32.h>
#include <mutex>

inline std::once_flag Init;
HRESULT WINAPI Hooks::EndScene::Hook(LPDIRECT3DDEVICE9 lpDevice)
{
	Void.CallbackManager->Call(CCallbackManager::Types::FRAME_BEGIN, {});
	Void.LuaEngine->RunCallbacks(true, std::string(LCT_ONFRAME));

	std::call_once(Init, [&]() {
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(Void.GetGameWindow());
		ImGui_ImplDX9_Init(lpDevice);

		char Systemroot[MAX_PATH] = { 0 };
		GetEnvironmentVariableA("SystemRoot", Systemroot, MAX_PATH);

		std::string Path = Systemroot; Path.append("\\Fonts\\verdana.ttf");

		ImGuiIO& Io = ImGui::GetIO();

		Void.pMainFont = Io.Fonts->AddFontFromFileTTF(Path.c_str(), 16.0f);

		if (!Void.pMainFont)
			Void.Error("[D3D9 Init] pMainFont was nullptr!");

		Path = Systemroot; Path.append("\\Fonts\\consola.ttf");

		Void.pCodeFont = Io.Fonts->AddFontFromFileTTF(Path.c_str(), 14.0f);

		if (!Void.pCodeFont)
			Void.Error("[D3D9 Init] pCodeFont was nullptr!");
	});

	auto Return = Void.HookSystem->GetOriginal<FN>("EndScene")(lpDevice);

	if (GetAsyncKeyState(VK_INSERT) & 1)
		UI::bOpen = !UI::bOpen;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
	
	Void.CallbackManager->Call(CCallbackManager::Types::FRAME_RENDER, {});


	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	Void.CallbackManager->Call(CCallbackManager::Types::FRAME_END, { &Return });
	Void.LuaEngine->RunCallbacks(false, std::string(LCT_ONFRAME));
	return Return;
}

void* Hooks::EndScene::GetTargetAddress()
{
	void* ppTable[119]; //Array of void pointers (void**)

	auto Device = ReCa<LPDIRECT3DDEVICE9>(Void.GetGameDevice());

	if (!Device)
	{
		Void.Warning("D3DDevice was nullptr, will wait until it isn't.\nIf PD gets stuck, try restarting the game and injecting after loading in.");

		while (!Device)
		{
			Sleep(100);
			Device = ReCa<LPDIRECT3DDEVICE9>(Void.GetGameDevice());
		}
	}	

	memcpy(ppTable, *ReCa<void***>(Device), sizeof(ppTable));

	return ppTable[42];
}
