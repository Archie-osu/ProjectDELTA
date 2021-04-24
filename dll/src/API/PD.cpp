#include "PD.hpp"
#include "../SDK/Callbacks/CallbackManager.hpp"
#include "../SDK/Hooks/HookManager.hpp"
#include "../SDK/Invoker/Invoker.hpp"
#include "../SDK/Scripts/ScriptManager.hpp"

#include <MinHook.h>

void CProjectDelta::RaiseError(const char* Format, ...)
{
	va_list vaArgs;
	va_start(vaArgs, Format);

	char Buffer[512]; memset(Buffer, 0, 512);
	strcpy_s<512>(Buffer, Format);

	vsprintf_s<512>(Buffer, Format, vaArgs);
	va_end(vaArgs);

	MessageBoxA(0, Buffer, "Project DELTA v3 - Unrecoverable Error", MB_TOPMOST | MB_ICONERROR | MB_OK);

	exit(1);
}

void CProjectDelta::Begin()
{
	MH_Initialize();

	this->Invoker = new CInvoker();
	this->CallbackManager = new CCallbackManager();
	this->ScriptManager = new CScriptManager();
	this->HookManager = new CHookManager();
}

void CProjectDelta::Stop()
{
	CallbackManager->__UnregisterAllCallbacks();
	HookManager->__UnhookAll();

	delete this->CallbackManager;
	delete this->HookManager;
	delete this->Invoker;
	delete this->ScriptManager;

	MH_Uninitialize();
}
