#include "Hooks.hpp"
#include "../../Dependencies/ImGui/imgui.h"
#include "../../Dependencies/ImGui/imgui_impl_dx11.h"
#include "../../Dependencies/ImGui/imgui_impl_win32.h"
#include "../Menu/Menu.hpp"
#include "../API/API.hpp"

using fnPresent = HRESULT(__stdcall*)(
	IDXGISwapChain* pSwapchain,
	UINT SyncInterval,
	UINT Flags
);

bool bInit = false;
ID3D11RenderTargetView* pTargetView;

//GUI Drawing is here.
HRESULT __stdcall Hooks::hkPresent(IDXGISwapChain* pSwapchain, UINT Sync, UINT Flags)
{
	fnPresent ofnPresent = (fnPresent)oPresent;	

	if (!bInit)
	{
		pSwapchain->GetDevice(__uuidof(ID3D11Device), (LPVOID*)&pDevice);
		pDevice->GetImmediateContext(&pContext);

		ImGui::CreateContext();

		ImGuiIO& curIO = ImGui::GetIO();
		ImFont* font = curIO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Bahnschrift.ttf", 18.0f);

		Menu::SetStyle();

		HWND hWnd = Useful::GetProcessWindow();

		ImGui_ImplWin32_Init(hWnd);
		ImGui_ImplDX11_Init(pDevice, pContext);

		ID3D11Texture2D* pBackBuffer;

		if (SUCCEEDED(pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pTargetView);

		bInit = true;
	}

	if (GetAsyncKeyState(VK_INSERT) & 1)
		Menu::bOpen = !Menu::bOpen;

	if (Menu::bOpen)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		Menu::Render();

		ImGui::Render();

		pContext->OMSetRenderTargets(1, &pTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	return ofnPresent(pSwapchain, Sync, Flags);
}
