#include "hkPresent.hpp"
#include "../../SDK/Callback Manager/Callback Manager.hpp"
#include "../../SDK/Hook System/Hook System.hpp"
#include "../../SDK/Void.hpp"

#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_dx11.h>

#include <mutex>
inline std::once_flag Init;

HRESULT __stdcall Hooks::Present::Hook(IDXGISwapChain* pThis, UINT Sync, UINT Flags)
{
	Void.CallbackManager->Call(CCallbackManager::Types::FRAME_BEGIN, {});

	std::call_once(Init, [&]() {

		ImGui::CreateContext();
		ImGui_ImplWin32_Init(Void.GetGameWindow());

		ID3D11Device* GameDevice = ReCa<ID3D11Device*>(Void.GetGameDevice());
		ImGui_ImplDX11_Init(GameDevice, ReCa<ID3D11DeviceContext*>(Void.GetGameContext()));

		ID3D11Texture2D* pBackBuffer;

		HRESULT ret = pThis->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		if (FAILED(ret))
			Void.Error("pSwapChain->GetBuffer failed!\nReturn: %X", ret);

		ret = GameDevice->CreateRenderTargetView(pBackBuffer, NULL, &pView);

		if (FAILED(ret))
			Void.Error("pDevice->CreateRenderTargetView failed!\nReturn: %X", ret);
	});

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Void.CallbackManager->Call(CCallbackManager::Types::FRAME_RENDER, {});

	ImGui::Render();

	ReCa<ID3D11DeviceContext*>(Void.GetGameContext())->OMSetRenderTargets(1, &pView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGui::EndFrame();

	//Run original
	auto ret = Void.HookSystem->GetOriginal<FN>("Present")(pThis, Sync, Flags);

	Void.CallbackManager->Call(CCallbackManager::Types::FRAME_END, {});

	return ret;
}

void* Hooks::Present::GetTargetAddress()
{
	//If you're looking over the code and found this comment, write "Found it @Archie#7097" in #undermodding without any context.

	using Fn = PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN;

	Fn pD3D11CreateDeviceAndSwapChain = ReCa<Fn>(GetProcAddress(GetModuleHandleA("d3d11.dll"), "D3D11CreateDeviceAndSwapChain"));

	if (!pD3D11CreateDeviceAndSwapChain)
		Void.Error("Failed to get D3D11CreateDeviceAndSwapChain!");

	DXGI_SWAP_CHAIN_DESC desc{ 0 };
	ID3D11Device* pDevice = nullptr; ID3D11DeviceContext* pContext = nullptr;

	desc.BufferCount = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferDesc = { 800, 600, {60, 1}, DXGI_FORMAT_R8G8B8A8_UNORM };

	HWND Window = ReCa<HWND>(Void.GetGameWindow());

	desc.OutputWindow = Window;
	desc.Windowed = !Void.IsGameFullscreen();
	desc.SampleDesc = { 1, 0 };
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	IDXGISwapChain* pSwapChain = nullptr;

	auto ret = pD3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 0, nullptr, 0,
		D3D11_SDK_VERSION,
		&desc,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pContext
	);

	if (FAILED(ret))
		Void.Error("D3D11CreateDeviceAndSwapChain failed!\nReturn: 0x%X", ret);

	//These two should never be hit, but as colinator27 said: Given gamemaker has like 1000 silent crashes/segfaults right now I'll take any nullptr checks
	if (!pSwapChain)
		Void.Error("pSwapChain was nullptr!\nD3D11CreateDeviceAndSwapChain return: 0x%X", ret);

	if (!pDevice)
		Void.Error("pDevice was nullptr!\nD3D11CreateDeviceAndSwapChain return: 0x%X", ret);

	void** ppVMT = *ReCa<void***>(pSwapChain);

	//Get the address of IDXGISwapChain::Present()
	void* pPresent = ppVMT[8];

	//Throw these away, they're useless now.
	pSwapChain->Release();
	pDevice->Release();

	return pPresent;
}