#pragma once
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

	//Visual studio is fucking autistic thinking this fn doesnt exist...
	void UnregisterCallback(Types type, std::string name);

	void Call(Types type);

	void Purge(); //Reinitialize
private:
	std::map<Types, std::forward_list<std::string>> prCallbackMap;
};