#include "Void.hpp"

#include "../Hooks/Present/hkPresent.hpp"
#include "../Hooks/EndScene/hkEndScene.hpp"

#include <MinHook.h>
#include <stdio.h>
#include <Windows.h>

#include "Callback Manager/Callback Manager.hpp"
#include "Hook System/Hook System.hpp"
#include "Invoker/Invoker.hpp"
#include "Lua Engine/Lua Engine.hpp"
#include "Memory Manager/Memory Manager.hpp"

void CVoid::Load()
{
	this->HookSystem = new CHookSystem;
	this->Invoker = new CInvoker;
	this->LuaEngine = new CLuaEngine;
	this->CallbackManager = new CCallbackManager;
	this->MemoryManager = new CMemoryManager;

	MH_Initialize();
	{
		//Hooks
		if (GetModuleHandleA("d3d9.dll"));
			Void.HookSystem->Hook("EndScene", Hooks::EndScene::GetTargetAddress(), Hooks::EndScene::Hook);

		if (GetModuleHandleA("d3d11.dll"))
			Void.HookSystem->Hook("Present", Hooks::Present::GetTargetAddress(), Hooks::Present::Hook);
	}
}

void CVoid::Unload()
{
	MessageBoxA(0, "Unloading", "Void Engine", MB_TOPMOST | MB_ICONINFORMATION | MB_OK);
	MH_Uninitialize();

	delete this->HookSystem;
	delete this->Invoker;
	delete this->LuaEngine;
	delete this->CallbackManager;
	delete this->MemoryManager;
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
