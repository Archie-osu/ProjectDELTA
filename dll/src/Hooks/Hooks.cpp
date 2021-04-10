#include "Hooks.hpp"
#include <MinHook.h>
#include <stdio.h>
#include "../Invoker/Invoker.hpp"

void Hooks::Initialize()
{	
	//Initialize MinHook
	if (MH_Initialize() != MH_OK)
		printf("ERROR - Could not initialize MinHook.\n");

	//Find DXGISwapChain
	//To hook D3D, I'm using the Dummy Device Method.
	//Let's just fill out the swapchain descriptor with arbitrary values to grab the VMT.

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
			printf("SUCCESS - SwapChain found: %p\n", pSwapchain);
			//Get the VMT
			void** pVMT = *(void***)pSwapchain;
			
			//Get the address of IDXGISwapChain::Present()
			uPresent = pVMT[8];

			printf("SUCCESS - Present found: %p\n", uPresent);

			//Throw these away, they're useless now.
			if (pSwapchain)
				pSwapchain->Release();

			if (pDevice)
				pDevice->Release();
		}
	}

	//Hook traditional game functions - I'd like a main loop hook also :)

	//Find the call to hkConvertRValue, extract the relative offset, recalculate, hook.
	ghl::ptr_t pattern = Invoker::FindPattern("\xE8\x00\x00\x00\x00\x01\x06", "x????xx");
	unsigned char* pRelativeOffset = pattern.add(1).get<unsigned char*>();
	LPVOID lpCallDest = (pRelativeOffset + (DWORD)pattern.add(5).getraw());

	MH_CreateHook(lpCallDest, hkConvertRValue, &oConvertRValue);
	MH_CreateHook(uPresent, hkPresent, &oPresent);
	//MH_CreateHook(Globals::pDebug, hkDebugHook, &oDebug);
	MH_EnableHook(MH_ALL_HOOKS);

	printf("IDXGISwapChain::Present hooked!\n");
	printf("ConvertRValue hooked!\n");
	//Hook WndProc
	oWndProc = (WNDPROC)(SetWindowLongW(Useful::GetProcessWindow(), GWLP_WNDPROC, (LONG_PTR)(&Hooks::hkWndProc)));

	printf("WndProc hooked!\n");

	//Find pointers
	Globals::pCrewData = ghl::ptr_t(GetModuleHandleA(NULL)).autofollow({ 0x6B9558, 0x104, 0x150, 0x38, 0x950 });
	Globals::nRoomNumber = ghl::ptr_t(GetModuleHandleA(NULL)).add(0x6AC9F0);
	Globals::pGlobalVars = ghl::ptr_t(GetModuleHandleA(NULL)).autofollow({ 0x49C3E0, 0x60, 0x10, 0x9E8, 0x150 });
	Globals::pRoomGoTo = Invoker::FindPattern("\xFF\x35\x00\x00\x00\x00\x6A\x00\xFF\x74\x24\x1C", "xx????xxxxxx");


	printf("Pointers found!\n");
}