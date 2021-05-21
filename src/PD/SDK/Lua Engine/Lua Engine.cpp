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

	sol::constructors<
		RValue(),
		RValue(const double&),
		RValue(const std::string&)
	>
		ConstrList;

	State.new_enum("yykind",
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

	State.new_enum("callbacktype",
		"on_frame",
		CLuaCallbackManager::Types::ON_FRAME,
		"on_script",
		CLuaCallbackManager::Types::ON_VMEXEC,
		"on_draw",
		CLuaCallbackManager::Types::ON_DRAW
	);

	sol::usertype<RValue> yyValue = State.new_usertype<RValue>("yyvalue", ConstrList, "kind", &RValue::Kind, "realval", &RValue::DoubleValue, "i32val", &RValue::Int32Value, "i64val", &RValue::Int64Value);

	//Test code: local str = yyvalue.new("gold"); local gold = call_fn("variable_global_get", str); print(gold.realval);
	State.set_function("call_fn", [](std::string String, sol::variadic_args va)
	{
		std::vector<RValue> vecRv;

		for (auto arg : va)
		{
			RValue rv = arg.as<RValue>();
			vecRv.push_back(rv);
		}

		return Void.Invoker->Call(String.c_str(), vecRv);
	});
	
	//Test code: local rv = create_obj("obj_savepoint", 340, 230); print(rv.realval);
	State.set_function("create_obj", [](std::string ObjName, double X, double Y)
	{
		return Void.Invoker->CreateObject(ObjName.c_str(), X, Y);
	});

	//Test code: local number = yyvalue.new(5000); set_global("gold", number);
	State.set_function("set_global", [](std::string Name, RValue Val)
	{
		Void.Invoker->SetGlobal(Name.c_str(), Val);
	});

	//Test code: local gold = get_global("gold"); print(gold.realval);
	State.set_function("get_global", [](std::string Name)
	{
		return Void.Invoker->GetGlobal(Name.c_str());
	});

	//Test code: local id = get_obj_id("obj_savepoint"); print(id);
	State.set_function("get_obj_id", [](std::string Name)
	{
		RValue rv(Name);
		return Void.Invoker->Call("asset_get_index", { rv }).DoubleValue;
	});

	/*Test code:
		local id = get_obj_id("obj_savepoint");
		local instances = get_obj_instances(id);

		for index=1, #instances do
			local v = instances[index];
			call_fn("instance_destroy", v);
		end
	*/
	State.set_function("get_obj_instances", [](double Object)
	{
		std::vector<RValue> ret;
		RValue rvTotalInstances = Void.Invoker->Call("instance_number", { Object });

		int TotalInstances = StCa<int>(rvTotalInstances.DoubleValue);

		for (int i = 0; i < TotalInstances; i++)
			ret.push_back(Void.Invoker->Call("instance_find", { Object, RValue((double)(i)) }));

		return ret;
	});

	State.set_function("array_get_element", [](RValue Value, int index)
	{
		return Value.at(index);
	});

	State.set_function("array_set_element", [](RValue Array, int index, RValue Value)
	{
		Array.at(index) = Value;
	});

	//Test code: local maxhps = get_global("maxhp"); print(array_get_size(maxhps));
	State.set_function("array_get_size", [](RValue Value)
	{
		if (Value.ArrayValue && Value.Kind == RVKinds::RV_Array)
			if (Value.ArrayValue->pArray)
				return Value.ArrayValue->pArray->nArrayLength;
		return -1;
	});

	State.set_function("add_callback", [](CLuaCallbackManager::Types type, std::string name)
	{
		Void.LuaCallbackManager->RegisterCallback(type, name);
	});

	State.set_function("remove_callback", [](CLuaCallbackManager::Types type, std::string name)
	{
		Void.LuaCallbackManager->UnregisterCallback(type, name);
	});

	State.set_function("rv_tostring", [](RValue rv)
	{
		if (rv.Kind == RVKinds::RV_String)
			if (Void.PatternManager->IsValidMemory(rv.StringValue))
				if (rv.StringValue->m_Thing)
					return std::string(rv.StringValue->m_Thing);

		return std::string();
	});

	State.set_function("add_scripthook", [](std::string ScriptName, std::string LuaFunctionName)
	{
		Void.LuaScriptHookManager->AddHook(ScriptName, LuaFunctionName);
	});

	State.set_function("remove_scripthook", [](std::string ScriptName, std::string LuaFunctionName)
	{
		Void.LuaScriptHookManager->RemoveHook(ScriptName, LuaFunctionName);
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
		"yyvalue", "callbacktype"
	};

	for (auto& k : keywords)
		Language.mKeywords.insert(k);

	std::vector<std::string> szAPINames =
	{
		"create_obj",
		"get_global",
		"set_global",
		"call_fn",
		"get_obj_id",
		"get_obj_instances",
		"array_get_element",
		"array_get_size",
		"array_set_element",
		"add_callback",
		"remove_callback",
		"rv_tostring",
		"add_scripthook",
		"remove_scripthook"
	};
	std::vector<std::string> szAPIDecls =
	{
		/* create_obj */
		"Create an instance of an object\n"
		"<yyvalue> create_obj(<string> Name, <real> PosX, <real> PosY)\n"
		"Return value: Instance ID of the created object",

		/* get_global */
		"Get a game global\n"
		"<yyvalue> get_global(<string> Name)\n"
		"Return value: Value of the global",

		/* set_global */
		"Set a game global\n"
		"<void> set_global(<string> Name, <yyvalue> Value)\n"
		"Return value: None",

		/* call_fn */
		"Call a GML Function\n"
		"<yyvalue> call_fn(<string> Name, <yyvalue?> ...)\n"
		"Arguments variable, see GML reference.\n"
		"Return value: Whatever the GML function returns",

		/* get_obj_id */
		"Get the master ID of a given asset\n"
		"<real> get_obj_id(<string> Name)\n"
		"Return value: The master ID",

		/* get_obj_instances */
		"Get all instances of a given object\n"
		"<yyvalue[]> get_obj_instances(<real> MasterID)\n"
		"Return value: All instances (Lua-Style Array)\n"
		"Remarks: Using array_get_element on Lua-Style arrays is unsupported. Use for loops.",

		/* array_get_element */
		"Get an element of a GML-style YYValue array\n"
		"<yyvalue> array_get_element(<yyvalue[]> Array, <real> Index)\n"
		"Return value: Element at the given index (starts at 1).\n"
		"Remarks: Use array_get_size to get the size of the array.",

		/* array_get_size */
		"Get a size of a GML-style YYValue array\n"
		"<real> array_get_size(<yyvalue[]> Array)\n"
		"Return value: -1 if the array isn't valid, otherwise a non-zero number.\n"
		"Remarks: Only use this function on GML-style arrays!",

		/* array_set_element */
		"Set an element of a GML-style YYValue array\n"
		"<void> array_set_element(<yyvalue[]> Array, <real> Index, <yyvalue> Value)\n"
		"Return value: None",

		/* add_callback */
		"Register a function to be called upon a game event\n"
		"<void> add_callback(<callbacktype> Type, <string> LuaFunctionName)\n"
		"Return value: None",

		/* remove_callback */
		"Unregister a function registered with add_callback\n"
		"<void> remove_callback(<callbacktype> Type, <string> LuaFunctionName)\n"
		"Return value: None",

		/* rv_tostring */
		"Converts a YYValue to a string\n"
		"<string> rv_tostring(<yyvalue> Value)\n"
		"Return value: The string, empty if invalid",

		/* add_scripthook */
		"Register a function to be called upon after a script\n"
		"<void> add_scripthook(<string> ScriptName, <string> LuaFunctionName)\n"
		"Return value: None\n",
		"Remarks: Use the full script name (usually begins with gml_Script)"
	};

	for (size_t n = 0; n < szAPINames.size(); n++)
	{
		TextEditor::Identifier id;
		id.mDeclaration = szAPIDecls[n];
		Language.mIdentifiers.insert(std::make_pair(szAPINames[n], id));
	}

	editor.SetLanguageDefinition(Language);
}

CLuaCallbackManager::CLuaCallbackManager()
{
	std::forward_list<std::string> BaseList; BaseList.clear(); //Unnecessary cleaning..

	this->prCallbackMap.clear();

	this->prCallbackMap.emplace(Types::ON_FRAME, BaseList);
	this->prCallbackMap.emplace(Types::ON_VMEXEC, BaseList);
	this->prCallbackMap.emplace(Types::ON_DRAW, BaseList);
}

void CLuaCallbackManager::RegisterCallback(Types type, std::string name)
{
	prCallbackMap.at(type).remove(name); //Fix for multiple functions with the same name
	prCallbackMap.at(type).push_front(name);
}

void CLuaCallbackManager::Call(Types type)
{
	if (!prCallbackMap.contains(type))
		Void.Error("[Lua] Attempted to call invalid type %i", StCa<int>(type));

	for (const auto& strName : prCallbackMap.at(type))
	{
		if (!Void.ShouldUnload())
		{
			sol::state& state = Void.LuaEngine->GetState();
			auto fn = state[strName];
			fn.call();
		}
	}
}

void CLuaCallbackManager::Purge()
{
	std::forward_list<std::string> BaseList; BaseList.clear(); //Unnecessary cleaning..
	this->prCallbackMap.clear();

	this->prCallbackMap.emplace(Types::ON_FRAME, BaseList);
	this->prCallbackMap.emplace(Types::ON_VMEXEC, BaseList);
	this->prCallbackMap.emplace(Types::ON_DRAW, BaseList);
}


void CLuaCallbackManager::UnregisterCallback(Types type, std::string name)
{
	if (!prCallbackMap.contains(type))
		Void.Error("[Lua] Attempted to unregister invalid type %i", StCa<int>(type));

	prCallbackMap.at(type).remove(name);
}

void LuaScriptCallback(std::vector<void*> vpArgs)
{
	constexpr int VMEXEC_SCRIPT_END = 8;

	//CallbackType, ReturnVal, CScript* pScript, int argc, uint8_t* pStackPointer, VMExec* pVM, YYObjectBase* pLocals, YYObjectBase* pArguments

	if (ReCa<int>(vpArgs.at(0)) != VMEXEC_SCRIPT_END)
		return;

	CScript* pScript = ReCa<CScript*>(vpArgs.at(2));

	char szScriptName[256] = { 0 };

	if (Void.PatternManager->IsValidMemory(cast<void*>(pScript->s_code)))
		if (Void.PatternManager->IsValidMemory(cast<void*>(pScript->s_code->i_pName)))
			strcpy_s<256>(szScriptName, pScript->s_code->i_pName);

	if (Void.LuaScriptHookManager->prScriptMap.contains(szScriptName))
		Void.LuaScriptHookManager->Call(szScriptName);
}

void CLuaScriptHookSystem::Call(std::string ScriptName)
{
	if (prScriptMap.contains(ScriptName))
	{
		sol::state& state = Void.LuaEngine->GetState();

		for (const auto& strName : prScriptMap.at(ScriptName))
		{
			if (!Void.ShouldUnload())
			{
				auto fn = state[strName];
				fn.call();
			}
		}
	}
}

void CLuaScriptHookSystem::AddHook(std::string ScriptName, std::string LuaName)
{
	this->prScriptMap[ScriptName].push_front(LuaName); //If it doesn't exist, create a new one.
}

void CLuaScriptHookSystem::RemoveHook(std::string ScriptName, std::string LuaName)
{
	if (prScriptMap.contains(ScriptName))
		this->prScriptMap.at(ScriptName).remove(LuaName);
}
