#pragma once
#include <string>
class CLuaConsole
{
private:
	std::string Text; //The text inside the console
public:
	char Buffer[512] = { 0 }; //The text input
	void ExecuteCommand(std::string Command);
	void Echo(std::string text);

	void Render();
};

inline CLuaConsole gLuaConsole;