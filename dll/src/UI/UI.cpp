#include "UI.hpp"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_win32.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_dx9.h"
#include "../SDK/Memory/Memory.hpp"

void UI::Render()
{
	if (ImGui::BeginMainMenuBar())
	{
#ifdef _DEBUG
		ImGui::Text("Project DELTA v2 | Debug Version");
#else
		ImGui::Text("Project DELTA v2 by Archie");
#endif
		ImGui::EndMainMenuBar();
	}

	ImGui::SetNextWindowSize(ImVec2(480, 360));

	if (ImGui::Begin("Project DELTA v2", NULL,
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse
	))
	{
		ImGui::Text("It is working!\n");
		ImGui::End();
	}

}

void UI::Init(PDIRECT3DDEVICE9 pDX9Device)
{
	if (!bInit)
	{
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(Memory::GetCurrentWindow());
		ImGui_ImplDX9_Init(pDX9Device);

		ImGuiIO& curIO = ImGui::GetIO();
		ImFont* font = curIO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Bahnschrift.ttf", 18.0f);

		bInit = true;
	}
}

void UI::Init(ID3D11Device* pDX11Device, ID3D11DeviceContext* pContext)
{
	if (!bInit)
	{
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(Memory::GetCurrentWindow());
		ImGui_ImplDX11_Init(pDX11Device, pContext);

		ImGuiIO& curIO = ImGui::GetIO();
		ImFont* font = curIO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Bahnschrift.ttf", 18.0f);

		bInit = true;
	}
}
