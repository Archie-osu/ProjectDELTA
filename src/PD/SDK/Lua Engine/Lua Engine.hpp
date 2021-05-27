#pragma once
#define SOL_NO_EXCEPTIONS 1
#include <sol/sol.hpp>
#include <ImGui/TextEditor.hpp>

class CLuaEngine
{
private:
	sol::state prState;
	std::map<std::string, std::forward_list<std::pair<std::string, bool>>> prHookMap;
	std::map<std::string, sol::function> prDirectHooks; //Only one function per hook allowed!
public:
	TextEditor puTextEditor;
	std::string RunScript(std::string Script);
	sol::state& GetState();

	void Init();
	void SetupLanguage(TextEditor& texteditor);

	void CreateDirectHook(std::string GameFn, sol::function LuaFn);
	void RemoveDirectHook(std::string GameFn);

	void CreateCallback(std::string GameEvent, std::string LuaFunction, bool bRunBeforeScript);
	void RemoveCallback(std::string GameEvent, std::string LuaFunction);
	void PurgeCallbacks();
	void RunCallbacks(bool bBeforeScript, std::string GameEvent, std::string TrueEventName = std::string());
};

void HookHandler(struct RValue* pResult, struct CInstance* pSelf, struct CInstance* pOther, int argc, struct RValue* pArgs);

void LuaScriptCallback(std::vector<void*> vpArgs);

inline constexpr std::string_view LCT_ONDRAW = "\\DrawEvent";
inline constexpr std::string_view LCT_ONSCRIPT = "\\Script";
inline constexpr std::string_view LCT_ONFRAME = "\\Frame";