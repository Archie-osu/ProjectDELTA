#pragma once
#define SOL_NO_EXCEPTIONS 1
#include <sol/sol.hpp>
#include <ImGui/TextEditor.hpp>

class CLuaEngine
{
private:
	sol::state prState;
public:
	std::string RunScript(std::string Script);
	sol::state& GetState();
	void Init();

	void SetupLanguage(TextEditor& texteditor);
};

//Handles Lua callbacks, see Callback Manager.hpp for the C++ version (which is more powerful).
class CLuaCallbackManager
{
public:
	CLuaCallbackManager();

	enum class Types
	{
		ON_FRAME,
		ON_VMEXEC,
		ON_DRAW
	};
	void RegisterCallback(Types type, std::string name);
	void UnregisterCallback(Types type, std::string name);

	void Call(Types type);
	void Purge(); //Reinitialize
private:
	std::map<Types, std::forward_list<std::string>> prCallbackMap;
};

//For C++, use the normal Callback Manager with VMEXEC_xxx (see LuaScriptCallback() for reference)
class CLuaScriptHookSystem
{
public:
	friend void LuaScriptCallback(std::vector<void*>);

	void Call(std::string ScriptName);
	void AddHook(std::string ScriptName, std::string LuaName);
	void RemoveHook(std::string ScriptName, std::string LuaName);
private:
	std::map<std::string, std::forward_list<std::string>> prScriptMap;
};

void LuaScriptCallback(std::vector<void*>);