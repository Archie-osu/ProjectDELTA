#pragma once
#define SOL_NO_EXCEPTIONS 1
#include <sol/sol.hpp>
#include <ImGui/TextEditor.hpp>

class CLuaEngine
{
private:
	sol::state prState;
	std::map<std::string, std::forward_list<std::string>> prHookMap;
public:
	TextEditor puTextEditor;
	std::string RunScript(std::string Script);
	sol::state& GetState();

	void Init();
	void SetupLanguage(TextEditor& texteditor);

	void CreateCallback(std::string GameEvent, std::string LuaFunction);
	void RemoveCallback(std::string GameEvent, std::string LuaFunction);
	void PurgeCallbacks();
	void RunCallbacks(std::string GameEvent);
};

void LuaScriptCallback(std::vector<void*> vpArgs);

inline constexpr std::string_view LCT_ONDRAW = "\\DrawEvent";
inline constexpr std::string_view LCT_ONSCRIPT = "\\Script";
inline constexpr std::string_view LCT_ONFRAME = "\\Frame";