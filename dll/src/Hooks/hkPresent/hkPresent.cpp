#include "hkPresent.hpp"
#include "../../API/PD.hpp"
#include "../../SDK/Hooks/HookManager.hpp"
#include "../../SDK/Invoker/Invoker.hpp"
#include <mutex>

#include "../../../Dependencies/ImGui/imgui.h"
#include "../../../Dependencies/ImGui/imgui_impl_dx11.h"
#include "../../../Dependencies/ImGui/imgui_impl_win32.h"

inline std::once_flag g_Init;

inline HRESULT __stdcall Hooks::Present::Hook(IDXGISwapChain* pSwapChain, UINT Sync, UINT Flags)
{
	std::call_once(g_Init, [&]() {
		RValue CurrentWindow = gDelta.Invoker->Call("window_handle", {});
		RValue CurrentDevice = gDelta.Invoker->Call("window_device", {});

		HWND Window = ReCa<HWND>(CurrentWindow.PointerValue);
		auto pDevice = ReCa<ID3D11Device*>(CurrentDevice.PointerValue);

		pDevice->GetImmediateContext(&pContext);

		ImGui::CreateContext();
		ImGui_ImplWin32_Init(Window);
		ImGui_ImplDX11_Init(pDevice, pContext);

		ID3D11Texture2D* pBackBuffer;

		HRESULT ret = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		if (FAILED(ret))
			gDelta.RaiseError("pSwapChain->GetBuffer failed!\nReturn: %X", ret);

		ret = pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderView);

		if (FAILED(ret))
			gDelta.RaiseError("pDevice->CreateRenderTargetView failed!\nReturn: %X", ret);
	});

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
	ImGui::Render();

	const static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	const static float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };

	pContext->OMSetRenderTargets(1, &pRenderView, NULL);
	//pContext->ClearRenderTargetView(pRenderView, clear_color_with_alpha);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return gDelta.HookManager->GetOriginal<HookFn>("hkPresent")(pSwapChain, Sync, Flags);
}

void Hooks::Present::Init()
{
	//Grab these dynamically, since you don't wanna load D3D11.dll when it's already loaded.
	//This could also be used as a bypass for anticheats scanning for LoadLibraryA calls, but shush.
	using Fn = PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN;

	Fn pD3D11CreateDeviceAndSwapChain = ReCa<Fn>(GetProcAddress(GetModuleHandleA("d3d11.dll"), "D3D11CreateDeviceAndSwapChain"));

	if (!pD3D11CreateDeviceAndSwapChain)
		gDelta.RaiseError("Failed to get D3D11CreateDeviceAndSwapChain!");

	DXGI_SWAP_CHAIN_DESC desc{ 0 };
	ID3D11Device* pDevice = nullptr; ID3D11DeviceContext* pContext = nullptr;

	desc.BufferCount = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferDesc = { 800, 600, {60, 1}, DXGI_FORMAT_R8G8B8A8_UNORM };

	bool bFullscreen = gDelta.Invoker->Call("window_get_fullscreen", {}).DoubleValue;
	HWND Window = (HWND)gDelta.Invoker->Call("window_handle", {}).PointerValue;

	desc.OutputWindow = Window;
	desc.Windowed = !bFullscreen;
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
		gDelta.RaiseError("D3D11CreateDeviceAndSwapChain failed!\nReturn: 0x%X", ret);

	//These two should never be hit, but as colinator27 said: Given gamemaker has like 1000 silent crashes/segfaults right now I'll take any nullptr checks
	if (!pSwapChain)
		gDelta.RaiseError("pSwapChain was nullptr!\nD3D11CreateDeviceAndSwapChain return: 0x%X", ret);

	if (!pDevice)
		gDelta.RaiseError("pDevice was nullptr!\nD3D11CreateDeviceAndSwapChain return: 0x%X", ret);

	void** ppVMT = *ReCa<void***>(pSwapChain);

	//Get the address of IDXGISwapChain::Present()
	void* pPresent = ppVMT[8];

	//Throw these away, they're useless now.
	pSwapChain->Release();
	pDevice->Release();

	gDelta.HookManager->Hook("hkPresent", pPresent, Hook);
}
