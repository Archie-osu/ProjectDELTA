#pragma once
#include <sol/sol.hpp>
class CLuaEngine
{
private:
	sol::state prState;
public:
	std::string RunScript(std::string Script);
	sol::state& GetState();
};