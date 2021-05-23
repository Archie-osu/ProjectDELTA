#include "Void.hpp"

#include "../Hooks/DoCallScript/hkDoCallScript.hpp"
#include "../Hooks/EndScene/hkEndScene.hpp"
#include "../Hooks/ExecuteIt/hkExecuteIt.hpp"
#include "../Hooks/Present/hkPresent.hpp"
#include "../Hooks/Window Proc/hkWndProc.hpp"
#include "../Hooks/YYMemManager/YYMemManager.hpp"

#include <MinHook.h>
#include <fstream>

#include "Callback Manager/Callback Manager.hpp"
#include "Hook System/Hook System.hpp"
#include "Invoker/Invoker.hpp"
#include "Lua Engine/Lua Engine.hpp"
#include "Memory Manager/Memory Manager.hpp"

#include "../UI/UI.hpp"
#include <Dbghelp.h>

bool DumpProcess(struct _EXCEPTION_POINTERS* pException)
{
	bool ret = false;

	//POV: Microsoft naming convention
	using fnWriteDump = BOOL(WINAPI*)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, CONST PMINIDUMP_EXCEPTION_INFORMATION, CONST PMINIDUMP_USER_STREAM_INFORMATION, CONST PMINIDUMP_CALLBACK_INFORMATION);
	using fnGetEnvA = DWORD(WINAPI*)(LPCSTR, LPSTR, DWORD);

	ret = (MessageBoxA(0, "The Void Engine encountered a critical error.\n"
		"The game has been paused to prevent potential damage to your save files.\n"
		"Process dumps will be written to your user directory.\n"
		"Please report this to the Project DELTA GitHub or to Archie#7097 on Discord.\n"
		"Continue anyway?", "Void Exception Handler", MB_TOPMOST | MB_ICONERROR | MB_YESNO) == IDYES);

	HMODULE dbgLib = LoadLibraryA("dbghelp.dll");
	fnWriteDump pMinidump = reinterpret_cast<fnWriteDump>(GetProcAddress(dbgLib, "MiniDumpWriteDump"));

	char UserDirectory[MAX_PATH];
	HMODULE krLib = LoadLibraryA("kernel32.dll");
	reinterpret_cast<fnGetEnvA>(GetProcAddress(krLib, "GetEnvironmentVariableA"))("USERPROFILE", UserDirectory, MAX_PATH);

	std::string Path = UserDirectory; Path.append("\\VoidEngine_FullDump.dmp");

	HANDLE hFullDumpFile = CreateFileA(Path.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	Path = UserDirectory; Path.append("\\VoidEngine_MiniDump.dmp");

	HANDLE hMiniDumpFile = CreateFileA(Path.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);


	_MINIDUMP_EXCEPTION_INFORMATION ExInfo;
	ExInfo.ThreadId = GetCurrentThreadId();
	ExInfo.ExceptionPointers = pException;
	ExInfo.ClientPointers = FALSE;

	pMinidump(GetCurrentProcess(), GetCurrentProcessId(), hFullDumpFile, MiniDumpWithIndirectlyReferencedMemory, &ExInfo, NULL, NULL);
	pMinidump(GetCurrentProcess(), GetCurrentProcessId(), hMiniDumpFile, MiniDumpNormal, &ExInfo, NULL, NULL);

	::CloseHandle(hFullDumpFile);
	::CloseHandle(hMiniDumpFile);

	return ret;
}

LONG WINAPI CrashHandler(struct _EXCEPTION_POINTERS* apExceptionInfo)
{
	if (DumpProcess(apExceptionInfo))
		return EXCEPTION_CONTINUE_EXECUTION;

	return EXCEPTION_CONTINUE_SEARCH;
}
void CVoid::Load()
{
	SetUnhandledExceptionFilter(CrashHandler); //Set this first, so even errors in constructors get caught

	this->HookSystem = new CHookSystem;
	this->Invoker = new CInvoker;
	this->LuaEngine = new CLuaEngine;
	this->CallbackManager = new CCallbackManager;
	this->PatternManager = new CPatternManager;
	this->MemoryManager = new CMemoryManager;
	this->lpData = 0;

	{
		//Pre-Hook Calls
		this->Invoker->Call("window_set_size", { 1280, 720 });
	}
	
	Sleep(66); //Sleep for two frames, just give it time to resize.

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

		void* lpDoCallScript = Hooks::DoCallScript::GetTargetAddress();

		if (lpDoCallScript)
			Void.HookSystem->Hook("DoCallScript", lpDoCallScript, Hooks::DoCallScript::Hook);

		Hooks::YYMemManager::Init();

		Hooks::WndProc::Init();
	}

	{
		//Callbacks
		Void.CallbackManager->RegisterCallback(CCallbackManager::Types::FRAME_RENDER, ReCa<CCallbackManager::PD_Routine>(UI::Render));
		Void.CallbackManager->RegisterCallback(CCallbackManager::Types::VMEXEC_BEGIN, ReCa<CCallbackManager::PD_Routine>(LuaScriptCallback));
		Void.CallbackManager->RegisterCallback(CCallbackManager::Types::VMEXEC_END, ReCa<CCallbackManager::PD_Routine>(LuaScriptCallback));
	}
}

void CVoid::Unload()
{
	SetWindowLongW(ReCa<HWND>(GetGameWindow()), GWLP_WNDPROC, ReCa<ULONG>(Hooks::WndProc::Original));
	
	Void.LuaEngine->PurgeCallbacks();
	Void.HookSystem->UnhookAll();
	Sleep(100); //Wait for stuff to unhook

	MH_Uninitialize();
	FreeConsole();


	delete this->Invoker;
	delete this->CallbackManager;
	delete this->LuaEngine;
	delete this->MemoryManager;
	delete this->PatternManager;
	delete this->HookSystem;
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
	void* p = ReCa<void*>(PatternManager->RegionScan(128, "\x46\x4F\x52\x4D\x00\x00\x00\x00\x47\x45\x4E\x38", "xxxx????xxxx"));

	if (!p && UI::bUseExperimentalSig)
	{
		//Works for YYC - bigger range, check for FORM header - wasteful to check for though, just use the more leniant one
		p = ReCa<void*>(PatternManager->RegionScan(0x7FFFF, "\x46\x4F\x52\x4D\x00\x00\x00\x00\x47\x45\x4E\x38", "xxxx????xxxx"));

		if (!p)
		{
			//YYC-level range, no FORM header, just GEN8
			p = ReCa<void*>(PatternManager->RegionScan(0x7FFFF, "\x00\x00\x00\x00\x00\x00\x00\x00\x47\x45\x4E\x38", "????????xxxx"));
		}
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