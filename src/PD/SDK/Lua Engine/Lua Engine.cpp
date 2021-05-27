#include "Lua Engine.hpp"
#include "../Structs/Structs.hpp"
#include "../Void.hpp"
#include "../Invoker/Invoker.hpp"
#include "../Memory Manager/Memory Manager.hpp"
#include <intrin.h>

std::string CLuaEngine::RunScript(std::string Script)
{
	auto result = prState.safe_script(Script);
	if (!result.valid())
	{
		sol::error e = result;
		return std::string(e.what());
	}
	return std::string();
}

sol::state& CLuaEngine::GetState()
{
	return prState;
}

void CLuaEngine::Init()
{
	sol::state& State = GetState();
	State.open_libraries(sol::lib::base);

	SetupLanguage(this->puTextEditor);

	this->PurgeCallbacks();

	sol::constructors<
		RValue(),
		RValue(const double&),
		RValue(const std::string&)
	>
		ConstrList;

	State.new_enum("RVKind",
		"Real",
		RVKinds::RV_Real,
		"Int32",
		RVKinds::RV_Int32,
		"Array",
		RVKinds::RV_Array,
		"Int64",
		RVKinds::RV_Int64,
		"String",
		RVKinds::RV_String,
		"Undefined",
		RVKinds::RV_Undefined,
		"Unset",
		RVKinds::RV_Unset
	);

	State.new_enum("LCT",
		"Frame",
		LCT_ONFRAME,
		"Script",
		LCT_ONSCRIPT,
		"DrawEvent",
		LCT_ONDRAW
	);

	sol::usertype<RValue> RValueUserType = State.new_usertype<RValue>("RValue", ConstrList,
		"kind", &RValue::Kind,
		"realval", &RValue::DoubleValue,
		"i32val", &RValue::Int32Value,
		"i64val", &RValue::Int64Value,

		sol::meta_function::index,
		[](RValue& rv, int index) { 
			printf("[get] index: %i\n", index);
			return rv.at(index); 
		},
		sol::meta_function::new_index,
		[](RValue& rv, int index, RValue value) {
			printf("[set] index: %i\n", index);
			rv.at(index) = value; 
		},

		sol::call_constructor,
		sol::factories(
			[](const double& d) {
				return RValue(d);
			},
			[](const std::string& s) {
				return RValue(s);
			}
		)
	);

	//Test code: local str = RValue.new("gold"); local gold = Call("variable_global_get", str); print(gold.realval);
	State.set_function("Call", [](std::string String, sol::variadic_args va)
	{
		std::vector<RValue> vecRv;

		for (auto arg : va)
		{
			RValue rv(arg);
			vecRv.push_back(rv);
		}

		auto result = Void.Invoker->Call(String.c_str(), vecRv);

		if (result.Kind == RV_Fail)
			Void.Warning("Attempted to call invalid function \"%s\"!", String.c_str());

		return result;
	});
	
	//Test code: local rv = CreateObject("obj_savepoint", 340, 230); print(rv.realval);
	State.set_function("CreateObject", [](std::string ObjName, double X, double Y)
	{
		return Void.Invoker->CreateObject(ObjName.c_str(), X, Y);
	});

	//Test code: local number = RValue.new(5000); SetGlobal("gold", number);
	State.set_function("SetGlobal", [](std::string Name, RValue Val)
	{
		Void.Invoker->SetGlobal(Name.c_str(), Val);
	});

	//Test code: local gold = GetGlobal("gold"); print(gold.realval);
	State.set_function("GetGlobal", [](std::string Name)
	{
		return Void.Invoker->GetGlobal(Name.c_str());
	});

	//Test code: local id = GetAssetID("obj_savepoint"); print(id);
	State.set_function("GetAssetID", [](std::string Name)
	{
		RValue rv(Name);
		return Void.Invoker->Call("asset_get_index", { rv });
	});

	/*Test code:
		local id = GetAssetID("obj_savepoint");
		local instances = GetObjectInstances(id);

		for index=1, #instances do
			local v = instances[index];
			call_fn("instance_destroy", v);
		end
	*/
	State.set_function("GetObjectInstances", [](double Object)
	{
		std::vector<RValue> ret;
		RValue rvTotalInstances = Void.Invoker->Call("instance_number", { Object });

		int TotalInstances = StCa<int>(rvTotalInstances.DoubleValue);

		for (int i = 0; i < TotalInstances; i++)
			ret.push_back(Void.Invoker->Call("instance_find", { Object, RValue((double)(i)) }));

		return ret;
	});

	//Test code: local maxhps = get_global("maxhp"); print(ArrayGetSize(maxhps));
	State.set_function("ArrayGetSize", [](RValue Value)
	{
		if (Value.ArrayValue && Value.Kind == RVKinds::RV_Array)
			if (Value.ArrayValue->pArray)
				return Value.ArrayValue->pArray->nArrayLength;
		return -1;
	});

	State.set_function("HookFunction", [](std::string Script, std::string LuaFunction, bool RunBeforeScript)
	{
		Void.LuaEngine->CreateCallback(Script, LuaFunction, RunBeforeScript);
	});

	State.set_function("UnhookFunction", [](std::string Script, std::string LuaFunction)
	{
		Void.LuaEngine->RemoveCallback(Script, LuaFunction);
	});

	State.set_function("ToString", [](RValue rv)
	{
		if (rv.Kind == RVKinds::RV_String)
			if (Void.PatternManager->IsValidMemory(rv.StringValue))
				if (rv.StringValue->m_Thing)
					return std::string(rv.StringValue->m_Thing);

		return std::string();
	});

	/* 
		Memory Leak Test Code
	
		local index = 0;
		while (index < 1000) do
			local string = "gold";
			local variable = get_global(string);
			set_global(string, variable);
			index = index + 1;
		end
	*/

}

void CLuaEngine::SetupLanguage(TextEditor& editor)
{
	auto Language = TextEditor::LanguageDefinition::Lua();

	static const char* const keywords[] = {
		"RValue",
	};

	for (auto& k : keywords)
		Language.mKeywords.insert(k);

	std::vector<std::string> szAPINames =
	{
		"Call",
		"CreateObject",
		"SetGlobal",
		"GetGlobal",
		"GetAssetID",
		"GetObjectInstances",
		"ArrayGetSize",
		"HookFunction",
		"UnhookFunction",
		"ToString",
		"LCT",
		//"HookFunction",
		//"UnhookFunction"
	};
	std::vector<std::string> szAPIDecls =
	{
		//Call
		"Call a GameMaker function with your desired parameters\n"
		"<RValue> Call(<string> FunctionName, <RValue?> OptionalArgs)",
		//CreateObject
		"Instantiate an asset on your desired coordinates\n"
		"<RValue> CreateObject(<string> ObjectName, <Real> XCoord, <Real> YCoord)",
		//SetGlobal
		"Set a game global\n"
		"<void> SetGlobal(<string> GlobalName, <RValue> Value)",
		//GetGlobal
		"Get a game global\n"
		"<RValue> GetGlobal(<string> GlobalName)",
		//GetAssetID
		"Get the Master ID of a given asset (objects, scripts, audio files)\n"
		"<RValue> GetAssetID(<string> AssetName)",
		//GetObjectInstances
		"Get all instances of a given asset\n"
		"<RValue> GetObjectInstances(<Real> MasterID)",
		//ArrayGetSize
		"Get the size of a given RValue array (legacy function)\n"
		"<RValue> ArrayGetSize(<RValue> Array)",
		//HookScript
		"Run your code whenever a game calls a function\n"
		"<void> HookFunction(<string / LCT> GameScript, <string> LuaFunctionName, <boolean> RunBeforeScript)\n"
		"Remarks: If RunBeforeScript is false, hooks run after the game script has finished.",
		//UnhookScript
		"Stop a hooked script from running\n"
		"<void> UnhookFunction(<string / LCT> GameScript, <string> LuaFunctionName)",
		//ToString
		"Get a string stored in an RValue\n"
		"<RValue> ToString(<RValue> Value)",
		//LCT
		"Lua Callback Type\n"
		"Valid values: LCT.Frame, LCT.DrawEvent, LCT.Script",
		//HookFunction
		/*"Hooks function \"OldFunction\", replacing it with the function \"NewFunction\".\n"
		"The old function is returned, and you MUST use this function in order to call the original.\n"
		"<function> HookFunction(<string> OldFunction>, <function> NewFunction)",
		//UnhookFunction
		"Removes all hooks from a GML function\n"
		"<void> UnhookFunction(<string> FunctionName>"*/
	};

	for (size_t n = 0; n < szAPINames.size(); n++)
	{
		TextEditor::Identifier id;
		id.mDeclaration = szAPIDecls[n];
		Language.mIdentifiers.insert(std::make_pair(szAPINames[n], id));
	}

	editor.SetLanguageDefinition(Language);
}

void CLuaEngine::CreateDirectHook(std::string GameFn, sol::function LuaFn)
{
	this->prDirectHooks[GameFn] = LuaFn;
}

void CLuaEngine::RemoveDirectHook(std::string GameFn)
{
	this->prDirectHooks.erase(GameFn);
}

void CLuaEngine::CreateCallback(std::string GameEvent, std::string LuaFunction, bool RunBeforeScript)
{
	prHookMap[GameEvent].push_front(std::make_pair(LuaFunction, RunBeforeScript));
	prHookMap[GameEvent].unique();
}

void CLuaEngine::RemoveCallback(std::string GameEvent, std::string LuaFunction)
{
	prHookMap[GameEvent].remove(std::make_pair(LuaFunction, false));
	prHookMap[GameEvent].remove(std::make_pair(LuaFunction, true));
}

void CLuaEngine::PurgeCallbacks()
{
	prHookMap.clear();
}

void CLuaEngine::RunCallbacks(bool BeginScript, std::string GameEvent, std::string TrueEventName)
{
	if (TrueEventName.empty())
		TrueEventName = GameEvent;

	auto& state = this->GetState();
	for (const auto& pair : prHookMap[GameEvent])
	{
		if (!Void.ShouldUnload())
		{
			if (pair.second == BeginScript) //Run begin callbacks only on begin, not on end.
			{
				auto fn = state[pair.first];
				fn.call(TrueEventName);
			}
		}
	}
}

void HookHandler(RValue* pResult, CInstance* pSelf, CInstance* pOther, int argc, RValue* pArgs)
{
	//First resolve the return address to check which function is calling this (we can have multiple hooks, remember?)
	unsigned char* pRetAddr = cast<unsigned char*>(_ReturnAddress());
	/*
		Layout of return address:
			0x00 | E8 ???????? | call <ThisFunction>
			0x05 | 0xFF		   | some other instruction | <--- return address
	*/

	auto pFuncOffset = pRetAddr - 0x4; //We don't want to read the E8, right...
	long FuncOffset = *cast<long*>(pFuncOffset); //Get the offset from the E8 call instruction (marked with ??'s in the layout)

	//http://jbremer.org/x86-api-hooking-demystified/#ah-basic
	//Essentially E8 is a relative jump, so the call destination is EIP + 5 + FuncOffset

	unsigned char* pFunc = pRetAddr + 0x5 + FuncOffset;

}

void LuaScriptCallback(std::vector<void*> vpArgs)
{
	constexpr int VMEXEC_BEGIN = 5;
	constexpr int VMEXEC_END = 6;
	constexpr int VMEXEC_SCRIPT_BEGIN = 7;
	constexpr int VMEXEC_SCRIPT_END = 8;

	bool IsBeforeScript = (cast<int>(vpArgs.at(0)) == VMEXEC_BEGIN || cast<int>(vpArgs.at(0)) == VMEXEC_SCRIPT_BEGIN);

	char szScriptName[256] = { 0 };

	if (cast<int>(vpArgs.at(0)) == VMEXEC_SCRIPT_BEGIN || cast<int>(vpArgs.at(0)) == VMEXEC_SCRIPT_END)
	{
		//CallbackType, (END) ReturnVal, CScript* pScript, int argc, uint8_t* pStackPointer, VMExec* pVM, YYObjectBase* pLocals, YYObjectBase* pArguments
		CScript* pScript = ReCa<CScript*>(vpArgs.at((IsBeforeScript ? 1 : 2))); //No access violations on BeforeScript, please.
		if (pScript->s_code)
			if (pScript->s_code->i_pName)
				strcpy_s<256>(szScriptName, pScript->s_code->i_pName);
	}

	else
	{
		//CallbackType, ReturnVal, CInstance* Self, CInstance* Other, CCode* pCode, RValue* Args
		CCode* pCode = ReCa<CCode*>(vpArgs.at(IsBeforeScript ? 3 : 4));

		if (pCode) //Having VirtualQuery calls here slowed down the callbacks by a ton, now we only have nullptr checks.
			if (pCode->i_pName)
				strcpy_s<256>(szScriptName, pCode->i_pName);
	}

	if (strstr(szScriptName, "Draw_0"))
		Void.LuaEngine->RunCallbacks(IsBeforeScript, std::string(LCT_ONDRAW), szScriptName);

	Void.LuaEngine->RunCallbacks(IsBeforeScript, std::string(LCT_ONSCRIPT), szScriptName);
	Void.LuaEngine->RunCallbacks(IsBeforeScript, std::string(szScriptName));
}