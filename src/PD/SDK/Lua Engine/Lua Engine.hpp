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