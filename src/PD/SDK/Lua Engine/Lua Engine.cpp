#include "Lua Engine.hpp"

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
