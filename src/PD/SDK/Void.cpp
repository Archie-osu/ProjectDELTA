#include "Void.hpp"

#include "../Hooks/Present/hkPresent.hpp"
#include "../Hooks/EndScene/hkEndScene.hpp"
#include "../Hooks/Window Proc/hkWndProc.hpp"
#include "../Hooks/ExecuteIt/hkExecuteIt.hpp"

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
	
	
	{
		//Expose Lua Functions
		auto& state = this->LuaEngine->GetState();
		state.open_libraries();

		sol::constructors<
			RValue(),
			RValue(double),
			RValue(Int32),
			RValue(Int64)>
		ConstrList;

		state.new_enum("yykind",
			"real",
			RVKinds::RV_Real,
			"int32",
			RVKinds::RV_Int32,
			"array",
			RVKinds::RV_Array,
			"int64",
			RVKinds::RV_Int64,
			"string",
			RVKinds::RV_String,
			"undefined",
			RVKinds::RV_Undefined,
			"unset",
			RVKinds::RV_Unset
		);

		sol::usertype<RValue> yyValue = state.new_usertype<RValue>("yyvalue", ConstrList, "kind", &RValue::Kind, "realval", &RValue::DoubleValue, "i32val", &RValue::Int32Value, "i64val", &RValue::Int64Value);

		state.set_function("call_fn", [](std::string String, sol::variadic_args va) 
		{
			std::vector<RValue> vecRv;

			for (auto arg : va)
			{
				RValue rv = arg.as<RValue>();
				vecRv.push_back(rv);
			}

			return Void.Invoker->Call(String.c_str(), vecRv);
		});

		state.set_function("create_obj", [](std::string ObjName, double X, double Y) 
		{
			return Void.Invoker->CreateObject(ObjName.c_str(), X, Y);
		});

		state.set_function("set_global", [](std::string Name, RValue Val)
		{
			Void.Invoker->SetGlobal(Name.c_str(), Val);
		});

		state.set_function("get_global", [](std::string Name)
		{
			return Void.Invoker->GetGlobal(Name.c_str());
		});

		state.set_function("get_obj_id", [](std::string Name)
		{
			const char* szString = Name.c_str();
			RValue rv(&szString);
			return Void.Invoker->Call("asset_get_index", { rv }).DoubleValue;
		});

		state.set_function("get_obj_instances", [](double Object)
		{
			std::vector<RValue> ret;
			RValue rvTotalInstances = Void.Invoker->Call("instance_number", { Object });

			int TotalInstances = rvTotalInstances.DoubleValue;

			for (int i = 0; i < TotalInstances; i++)
				ret.push_back(Void.Invoker->Call("instance_find", { Object, RValue((double)(i)) }));

			return ret;
		});

		state.set_function("array_get_element", [](RValue Value, int index)
		{
			return Value.at(index);
		});

		state.set_function("array_set_element", [](RValue Array, int index, RValue Value)
		{
			Array.at(index) = Value;
		});

		state.set_function("array_get_size", [](RValue Value)
		{
			if (Value.ArrayValue && Value.Kind == RVKinds::RV_Array)
				if (Value.ArrayValue->pArray)
					return Value.ArrayValue->pArray->nArrayLength;
			return -1;
		});
	}
}

void CVoid::Unload()
{
	SetWindowLongW(ReCa<HWND>(GetGameWindow()), GWLP_WNDPROC, ReCa<ULONG>(Hooks::WndProc::Original));
	
	Void.HookSystem->UnhookAll();
	Sleep(100); //Wait for stuff to unhook

	MH_Uninitialize();
	FreeConsole();

	delete this->HookSystem;
	delete this->Invoker;
	delete this->LuaEngine;
	delete this->CallbackManager;
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