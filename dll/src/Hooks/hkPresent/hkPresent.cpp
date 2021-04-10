#include <MinHook.h>
#include "hkPresent.hpp"
#include "../../UI/UI.hpp"
#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_win32.h"

void Hooks::Present::Init()
{
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwapchain = nullptr;
	D3D_FEATURE_LEVEL FeatureLevel;
	DXGI_SWAP_CHAIN_DESC swapChainDescriptor{ 0 };
	swapChainDescriptor.BufferCount = 1;
	swapChainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDescriptor.BufferDesc.Height = 800;
	swapChainDescriptor.BufferDesc.Width = 600;
	swapChainDescriptor.BufferDesc.RefreshRate = { 60, 1 };
	swapChainDescriptor.OutputWindow = GetForegroundWindow();
	swapChainDescriptor.Windowed = TRUE;
	swapChainDescriptor.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDescriptor.SampleDesc.Count = 1;
	swapChainDescriptor.SampleDesc.Quality = 0;

	HRESULT ret = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_REFERENCE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDescriptor,
		&pSwapchain,
		&pDevice,
		&FeatureLevel,
		nullptr
	);

	if (!(FAILED(ret)))
	{
		//Get the VMT
		void** pVMT = *(void***)pSwapchain;

		//Get the address of IDXGISwapChain::Present()
		void* pPresent = pVMT[8];

		//Throw these away, they're useless now.
		if (pSwapchain)
			pSwapchain->Release();

		if (pDevice)
			pDevice->Release();

		MH_CreateHook(pPresent, hkPresent, reinterpret_cast<LPVOID*>(&pOriginal));
		MH_EnableHook(MH_ALL_HOOKS);
	}
}

HRESULT __stdcall Hooks::Present::hkPresent(IDXGISwapChain* pSwapchain, UINT Sync, UINT Flags)
{
	if (!pDevice || !pContext)
	{
		pSwapchain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<LPVOID*>(&pDevice));
		pDevice->GetImmediateContext(&pContext);

		UI::Init(pDevice, pContext);

		ID3D11Texture2D* pBackBuffer;

		if (SUCCEEDED(pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pTargetView);
	}

	if (GetAsyncKeyState(VK_INSERT) & 1)
		UI::bOpen = !UI::bOpen;

	if (UI::bOpen)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		UI::Render();

		ImGui::Render();

		pContext->OMSetRenderTargets(1, &pTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	return pOriginal(pSwapchain, Sync, Flags);
}
