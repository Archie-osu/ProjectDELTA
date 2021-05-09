#include "Lua Engine.hpp"
#include "../Structs/Structs.hpp"
#include "../Void.hpp"
#include "../Invoker/Invoker.hpp"

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
		RValue(double),
		RValue(std::string)
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
	
}

void CLuaEngine::SetupLanguage(TextEditor& editor)
{
	auto Language = TextEditor::LanguageDefinition::Lua();
	editor.SetShowWhitespaces(false);

	std::vector<std::string> szAPINames = 
	{
		"yyvalue", 
		"create_obj", 
		"get_global", 
		"set_global", 
		"call_fn", 
		"get_obj_id", 
		"get_obj_instances", 
		"array_get_element", 
		"array_get_size", 
		"array_set_element"
	};
	std::vector<std::string> szAPIDecls =
	{
		/* YYValue */
		"A generic data value",

		/* create_obj */
		"Create an instance of an object\n"
		"create_obj(Name, PosX, PosY)\n"
		"Return value: Instance ID of the created object (YYValue)",

		/* get_global */
		"Get a game global\n"
		"get_global(Name)\n"
		"Return value: Value of the global (YYValue)",

		/* set_global */
		"Set a game global\n"
		"set_global(Name, Value)\n"
		"Return value: None",

		/* call_fn */
		"Call a GML Function\ncall_fn(Name, ...)\n"
		"Arguments variable, see GML reference.\n"
		"Return value: Whatever the GML function returns (YYValue).",

		/* get_obj_id */
		"Get the master ID of a given asset\n"
		"get_obj_id(Name)\n"
		"Return value: The master ID (native Lua number).",

		/* get_obj_instances */
		"Get all instances of a given object\n"
		"get_obj_instances(Master ID)\n"
		"Return value: All instances (Lua-style array of YYValues)\n"
		"Remarks: Using array_get_element on Lua-style arrays is unsupported. Use for loops.",

		/* array_get_element */
		"Get an element of a GML-style YYValue array\n"
		"array_get_element(Value, Index)\n"
		"Return value: Element at the given index (starts at 1).\n"
		"Remarks: Use array_get_size to get the size of the array.",

		/* array_get_size */
		"Get a size of a GML-style YYValue array\n"
		"array_get_size(YYValue)\n"
		"Return value: -1 if the array isn't valid, otherwise a non-zero native number.\n"
		"Remarks: Only use this function on GML-style arrays!",

		/* array_set_element */
		"Set an element of a GML-style YYValue array\n"
		"array_set_element(Array, Index, Value)\n"
		"Return value: None",
	};

	for (size_t n = 0; n < szAPINames.size(); n++)
	{
		TextEditor::Identifier id;
		id.mDeclaration = szAPIDecls[n];
		Language.mIdentifiers.insert(std::make_pair(szAPINames[n], id));
	}

	editor.SetLanguageDefinition(Language);
}