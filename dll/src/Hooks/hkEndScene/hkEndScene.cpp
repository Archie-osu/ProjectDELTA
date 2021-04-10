#include <MinHook.h>
#include "hkEndScene.hpp"
#include "../../UI/UI.hpp"
#include "../ImGui/imgui_impl_dx9.h"
#include "../ImGui/imgui_impl_win32.h"
#include "../../SDK/Memory/Memory.hpp"

//Shamelessly copypasted (albeit a little changed) from Underhacks
void Hooks::EndScene::Init()
{
	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!pD3D)
		return;

	PDIRECT3DDEVICE9 pDummyDevice = nullptr;

	// options to create dummy device
	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = Memory::GetCurrentWindow();

	HRESULT dummyDeviceCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

	if (FAILED(dummyDeviceCreated))
	{
		// may fail in windowed fullscreen mode, trying again with windowed mode
		d3dpp.Windowed = !d3dpp.Windowed;

		dummyDeviceCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

		if (FAILED(dummyDeviceCreated))
		{
			pD3D->Release();
			return;
		}
	}

	memcpy(pDummyTable, *reinterpret_cast<void***>(pDummyDevice), sizeof(pDummyTable));

	pDummyDevice->Release();
	pD3D->Release();

	MH_CreateHook(reinterpret_cast<char*>(pDummyTable[42]), Hooks::EndScene::hkEndScene, reinterpret_cast<LPVOID*>(&Hooks::EndScene::pOriginal));
	MH_EnableHook(MH_ALL_HOOKS);
}

HRESULT __stdcall Hooks::EndScene::hkEndScene(PDIRECT3DDEVICE9 lpDevice)
{
	if (!pDevice)
	{
		pDevice = lpDevice;
		UI::Init(lpDevice);
	}

	lpDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);

	if (GetAsyncKeyState(VK_INSERT) & 1)
		UI::bOpen = !UI::bOpen;

	long oReturn = pOriginal(lpDevice);

	if (UI::bOpen)
	{
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		UI::Render();

		ImGui::EndFrame();
		ImGui::Render();

		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	return oReturn;
}
