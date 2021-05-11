#include "Void.hpp"

#include "../Hooks/Present/hkPresent.hpp"
#include "../Hooks/EndScene/hkEndScene.hpp"
#include "../Hooks/Window Proc/hkWndProc.hpp"
#include "../Hooks/ExecuteIt/hkExecuteIt.hpp"

#include <MinHook.h>
#include <fstream>

#include "Callback Manager/Callback Manager.hpp"
#include "Hook System/Hook System.hpp"
#include "Invoker/Invoker.hpp"
#include "Lua Engine/Lua Engine.hpp"
#include "Memory Manager/Memory Manager.hpp"

#include "../UI/UI.hpp"
#include <Dbghelp.h>

void DumpProcess(struct _EXCEPTION_POINTERS* pException)
{
	//POV: Microsoft naming convention
	using fnWriteDump = BOOL(WINAPI*)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, CONST PMINIDUMP_EXCEPTION_INFORMATION, CONST PMINIDUMP_USER_STREAM_INFORMATION, CONST PMINIDUMP_CALLBACK_INFORMATION);
	using fnGetEnvA = DWORD(WINAPI*)(LPCSTR, LPSTR, DWORD);

	MessageBoxA(0, "The Void Engine encountered a critical error.\n"
		"The game has been stopped to prevent damage to your save files.\n"
		"A process dump will be written to your user directory (VoidDump.dmp).\n"
		"Please report this to the Project DELTA GitHub or reach out to Archie#7097 on Discord.", "[Void Security Manager]", MB_OK | MB_TOPMOST | MB_ICONERROR);

	HMODULE dbgLib = LoadLibraryA("dbghelp.dll");
	fnWriteDump pMinidump = reinterpret_cast<fnWriteDump>(GetProcAddress(dbgLib, "MiniDumpWriteDump"));

	char UserDirectory[MAX_PATH];
	HMODULE krLib = LoadLibraryA("kernel32.dll");
	reinterpret_cast<fnGetEnvA>(GetProcAddress(krLib, "GetEnvironmentVariableA"))("USERPROFILE", UserDirectory, MAX_PATH);

	std::string Path = UserDirectory; Path.append("\\VoidDump.dmp");

	HANDLE hFile = CreateFileA(Path.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	_MINIDUMP_EXCEPTION_INFORMATION ExInfo;
	ExInfo.ThreadId = GetCurrentThreadId();
	ExInfo.ExceptionPointers = pException;
	ExInfo.ClientPointers = FALSE;

	pMinidump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL);
	::CloseHandle(hFile);
}

LONG WINAPI CrashHandler(struct _EXCEPTION_POINTERS* apExceptionInfo)
{
	DumpProcess(apExceptionInfo);
	return EXCEPTION_CONTINUE_SEARCH;
}

void CVoid::Load()
{
	SetUnhandledExceptionFilter(CrashHandler); //Set this first, so even errors in constructors get caught

	this->HookSystem = new CHookSystem;
	this->Invoker = new CInvoker;
	this->LuaEngine = new CLuaEngine;
	this->CallbackManager = new CCallbackManager;
	this->LuaCallbackManager = new CLuaCallbackManager;
	this->MemoryManager = new CMemoryManager;
	this->lpData = 0;

	{
		//Init internals
		this->lpData = FindGameData();

		if (!lpData)
			Void.Warning("Failed to find the data.win file\nInitializing in compatibility mode.");

		LuaEngine->Init();
	}

	MH_Initialize();
	{
		//Hooks
		if (GetModuleHandleA("d3d9.dll"))
			Void.HookSystem->Hook("EndScene", Hooks::EndScene::GetTargetAddress(), Hooks::EndScene::Hook);

		if (GetModuleHandleA("d3d11.dll"))
			Void.HookSystem->Hook("Present", Hooks::Present::GetTargetAddress(), Hooks::Present::Hook);

		void* lpExecuteIt = Hooks::ExecuteIt::GetTargetAddress();

		if (lpExecuteIt)
			Void.HookSystem->Hook("ExecuteIt", lpExecuteIt, Hooks::ExecuteIt::Hook);

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
	
	Void.HookSystem->UnhookAll();
	Void.LuaCallbackManager->Purge();
	Sleep(100); //Wait for stuff to unhook

	MH_Uninitialize();
	FreeConsole();

	delete this->HookSystem;
	delete this->Invoker;
	delete this->LuaEngine;
	delete this->CallbackManager;
	delete this->LuaCallbackManager;
	delete this->MemoryManager;
	Beep(500, 100);
}

bool CVoid::ShouldUnload()
{
	return GetAsyncKeyState(VK_END);
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

	if (!p && UI::bUseExperimentalSig)
	{
		//This should never happen.
		p = ReCa<void*>(MemoryManager->RegionScan(4096, "\x00\x00\x00\x00\x00\x00\x00\x00\x47\x45\x4E\x38", "????????xxxx"));
	}

	return p;
}

void* CVoid::GetGameData()
{
	if (!lpData)
		lpData = FindGameData();

	return lpData;
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