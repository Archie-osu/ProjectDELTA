#include "Void.hpp"

#include "../Hooks/Present/hkPresent.hpp"
#include "../Hooks/EndScene/hkEndScene.hpp"
#include "../Hooks/Window Proc/hkWndProc.hpp"

#include <MinHook.h>
#include <stdio.h>
#include <Windows.h>
#include <fstream>
#include <mutex>

#include "Callback Manager/Callback Manager.hpp"
#include "Hook System/Hook System.hpp"
#include "Invoker/Invoker.hpp"
#include "Lua Engine/Lua Engine.hpp"
#include "Memory Manager/Memory Manager.hpp"

#include "../UI/UI.hpp"

void CVoid::Load()
{
	this->HookSystem = new CHookSystem;
	this->Invoker = new CInvoker;
	this->LuaEngine = new CLuaEngine;
	this->CallbackManager = new CCallbackManager;
	this->MemoryManager = new CMemoryManager;
	this->lpData = 0;

	{
		//Init internals
		this->lpData = FindGameData();

		if (!lpData)
			Void.Warning("Failed to find the data.win file\nInitializing in compatibility mode.");
	}

	MH_Initialize();
	{
		//Hooks
		if (GetModuleHandleA("d3d9.dll") || GetModuleHandleA("d3d10level9.dll"))
			Void.HookSystem->Hook("EndScene", Hooks::EndScene::GetTargetAddress(), Hooks::EndScene::Hook);

		if (GetModuleHandleA("d3d11.dll"))
			Void.HookSystem->Hook("Present", Hooks::Present::GetTargetAddress(), Hooks::Present::Hook);

		Hooks::WndProc::Init();
	}

	{
		//Callbacks
		Void.CallbackManager->RegisterCallback(CCallbackManager::Types::FRAME_RENDER, ReCa<CCallbackManager::PD_Routine>(UI::Render));
	}
}

void CVoid::Unload()
{
	SetWindowLongW(ReCa<HWND>(GetGameWindow()), GWLP_WNDPROC, ReCa<ULONG>(Hooks::WndProc::Original));
	MH_Uninitialize();

	delete this->HookSystem;
	delete this->Invoker;
	delete this->LuaEngine;
	delete this->CallbackManager;
	delete this->MemoryManager;
	Beep(500, 100);
}

bool CVoid::ShouldUnload()
{
	return GetAsyncKeyState(VK_F10);
}

void* CVoid::GetGameWindow()
{
	return Invoker->Call("window_handle", {}).PointerValue;
}

void* CVoid::GetGameDevice()
{
	return Invoker->Call("window_device", {}).PointerValue;
}

void* CVoid::GetGameContext()
{
	ID3D11Device* pDevice = ReCa<ID3D11Device*>(GetGameDevice());
	ID3D11DeviceContext* pContext;

	pDevice->GetImmediateContext(&pContext);

	return pContext;
}

void* CVoid::FindGameData()
{
	void* p = ReCa<void*>(MemoryManager->RegionScan(128, "\x46\x4F\x52\x4D\x00\x00\x00\x00\x47\x45\x4E\x38", "xxxx????xxxx"));

	if (!p)
	{
		//This should never happen.
		//cCA9IFJlQ2E8dm9pZCo+KE1lbW9yeU1hbmFnZXItPlJlZ2lvblNjYW4oNDA5NiwgIlx4MDBceDAwXHgwMFx4MDBceDAwXHgwMFx4MDBceDAwXHg0N1x4NDVceDRFXHgzOCIsICI/Pz8/Pz8/P3h4eHgiKSk7
	}

	return p;
}

void* CVoid::GetGameData()
{
	if (!lpData)
		lpData = FindGameData();

	return lpData;
}


void CVoid::DumpDataToFile()
{
	byte* MemoryRegion = ReCa<byte*>(GetGameData());
	Int32 Size = *(Int32*)(MemoryRegion + 0x4) + 8;
	char buffer[MAX_PATH];
	GetEnvironmentVariableA("USERPROFILE", buffer, MAX_PATH);

	std::ofstream file(std::string(buffer + std::string("\\dump.bin")).c_str(), std::ios::binary | std::ios::out);

	if (!file.is_open())
		Void.Error("Failed to open the dump file!");
	
	for (byte* addr = MemoryRegion; addr < MemoryRegion + Size; addr += 1)
		file.write(reinterpret_cast<const char*>(addr), sizeof(byte));

	Void.Warning("Dumped to file: %s\\dump.bin", buffer);
}

const char* CVoid::GetGameName()
{
	return ReCa<GameForm_t*>(GetGameData())->ReadString(ReCa<GameForm_t*>(GetGameData())->Gen8.DisplayNameOffset);
}

bool CVoid::IsGameFullscreen()
{
	return Invoker->Call("window_get_fullscreen", {}).DoubleValue;
}

void CVoid::Error(const char* Format, ...)
{
	va_list vaArgs;
	va_start(vaArgs, Format);

	char Buffer[512]; memset(Buffer, 0, 512);
	strcpy_s<512>(Buffer, Format);

	vsprintf_s<512>(Buffer, Format, vaArgs);
	va_end(vaArgs);

	MessageBoxA(0, Buffer, "Void Engine - Fatal Error", MB_TOPMOST | MB_ICONERROR | MB_OK);

	if (IsDebuggerPresent())
		throw "Void - Encountered an error!";

	exit(1);
}

void CVoid::Warning(const char* Format, ...)
{
	va_list vaArgs;
	va_start(vaArgs, Format);

	char Buffer[512]; memset(Buffer, 0, 512);
	strcpy_s<512>(Buffer, Format);

	vsprintf_s<512>(Buffer, Format, vaArgs);
	va_end(vaArgs);

	MessageBoxA(0, Buffer, "Void Engine - Warning", MB_TOPMOST | MB_ICONWARNING | MB_OK);
}