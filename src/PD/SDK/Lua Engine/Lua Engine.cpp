#include "Lua Engine.hpp"
#include "../Structs/Structs.hpp"
#include "../Void.hpp"
#include "../Invoker/Invoker.hpp"
#include "../Memory Manager/Memory Manager.hpp"

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
	State.open_libraries();

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

	//TODO: Either fix this or add an .at function
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

		return Void.Invoker->Call(String.c_str(), vecRv);
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

	State.set_function("HookFunction", [](std::string Script, std::string LuaFunction)
	{
		Void.LuaEngine->CreateCallback(Script, LuaFunction);
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
		"LCT"
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
		"<RValue> GetAssetID(<string> GlobalName)",
		//GetObjectInstances
		"Get all instances of a given asset\n"
		"<RValue> GetObjectInstances(<Real> MasterID)",
		//ArrayGetSize
		"Get the size of a given RValue array (legacy function)\n"
		"<RValue> ArrayGetSize(<RValue> Array)",
		//HookFunction
		"Run your function whenever a game calls a script\n"
		"<void> HookFunction(<string / LCT> GameScript, <string> LuaFunctionName)",
		//UnhookFunction
		"Stop a hooked function from running\n"
		"<void> UnhookFunction(<string / LCT> GameScript, <string> LuaFunctionName)",
		//ToString
		"Get a string stored in an RValue\n"
		"<RValue> ToString(<RValue> Value)",
		//LCT
		"Lua Callback Type\n"
		"Valid values: LCT.Frame, LCT.DrawEvent, LCT.Script",
	};

	for (size_t n = 0; n < szAPINames.size(); n++)
	{
		TextEditor::Identifier id;
		id.mDeclaration = szAPIDecls[n];
		Language.mIdentifiers.insert(std::make_pair(szAPINames[n], id));
	}

	editor.SetLanguageDefinition(Language);
}

void CLuaEngine::CreateCallback(std::string GameEvent, std::string LuaFunction)
{
	prHookMap[GameEvent].push_front(LuaFunction);
	prHookMap[GameEvent].unique();
}

void CLuaEngine::RemoveCallback(std::string GameEvent, std::string LuaFunction)
{
	prHookMap[GameEvent].remove(LuaFunction);
}

void CLuaEngine::PurgeCallbacks()
{
	prHookMap.clear();
}

void CLuaEngine::RunCallbacks(std::string GameEvent)
{
	auto& state = this->GetState();
	for (const auto& string : prHookMap[GameEvent])
	{
		if (!Void.ShouldUnload())
		{
			auto fn = state[string];
			fn.call();
		}
	}
}

void LuaScriptCallback(std::vector<void*> vpArgs)
{
	constexpr int VMEXEC_SCRIPT_END = 8;
	char szScriptName[256] = { 0 };

	if (cast<int>(vpArgs.at(0)) == VMEXEC_SCRIPT_END)
	{
		//CallbackType, ReturnVal, CScript* pScript, int argc, uint8_t* pStackPointer, VMExec* pVM, YYObjectBase* pLocals, YYObjectBase* pArguments
		CScript* pScript = ReCa<CScript*>(vpArgs.at(2));		
		if (Void.PatternManager->IsValidMemory(cast<void*>(pScript->s_code)))
			if (Void.PatternManager->IsValidMemory(cast<void*>(pScript->s_code->i_pName)))
				strcpy_s<256>(szScriptName, pScript->s_code->i_pName);
	}

	else
	{
		//CallbackType, ReturnVal, CInstance* Self, CInstance* Other, CCode* pCode, RValue* Args
		CCode* pCode = ReCa<CCode*>(vpArgs.at(4));

		if (Void.PatternManager->IsValidMemory(cast<void*>(pCode)))
			if (Void.PatternManager->IsValidMemory(cast<void*>(pCode->i_pName)))
				strcpy_s<256>(szScriptName, pCode->i_pName);
	}

	if (strstr(szScriptName, "Draw_0"))
		Void.LuaEngine->RunCallbacks(std::string(LCT_ONDRAW));

	Void.LuaEngine->RunCallbacks(std::string(LCT_ONSCRIPT));
	Void.LuaEngine->RunCallbacks(std::string(szScriptName));
}