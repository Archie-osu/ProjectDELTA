#pragma once
#include <string>

//Refactor this
class CLuaConsole
{
public:
	void* pMainFont = nullptr;  void* pCodeFont = nullptr;

	void ExecuteCommand(std::string Command);
	void Render();
};

inline CLuaConsole gLuaConsole;