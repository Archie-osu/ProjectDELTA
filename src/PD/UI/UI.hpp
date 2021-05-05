#pragma once
#include <vector>
#include <string>
namespace UI
{
	struct prRValue;
	inline bool bOpen = true;
	void Render(std::vector<prRValue*>);
	void ApplyStyle();
	void ShowGameWarning();

	void Deltarune();
	void Undertale();
	void Sus(); //haha i did a funny

	void Default();
}

class CLuaConsole
{
private:
	std::string Text; //The text inside the console
public:
	char Buffer[512]; //The text input
	void ExecuteCommand(std::string Command);
	void Echo(std::string text);

	void Render();
};

inline CLuaConsole LuaConsole;