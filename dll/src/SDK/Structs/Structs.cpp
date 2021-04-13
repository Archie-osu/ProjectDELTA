#include "Structs.hpp"
#include "../../Core/Core.hpp"
#include "../../Invoker/Invoker.hpp"
#include <vector>
#include <string>

void ReadValues(std::vector<std::string> VarNames)
{
	for (auto Name : VarNames)
	{
		const char* szGlobalName = Name.c_str();
		Core::GlobalMap[Name] = Invoker::invoke("variable_global_get", { RValue(&szGlobalName) });
	}
}

void WriteValues(std::vector<std::string> VarNames)
{
	for (auto Name : VarNames)
	{
		const char* szGlobalName = Name.c_str();
		Invoker::invoke("variable_global_set", { RValue(&szGlobalName), Core::GlobalMap.at(Name) });
	}
}

void SDK::Structs::Read()
{
	switch (Core::CurrentGame)
	{
	case Core::GameType::Deltarune:
		ReadValues({ "hp", "maxhp", "xp", "lv", "gold", "plot", "interact", "debug" });
		break;
	case Core::GameType::Undertale:
		ReadValues({ "hp", "maxhp", "xp", "lv", "gold", "plot", "interact", "debug" });
		break;
	case Core::GameType::Underswap:
		Read();
		break;
	}
}

void SDK::Structs::Write()
{
	switch (Core::CurrentGame)
	{
	case Core::GameType::Deltarune:
		ReadValues({ "hp", "maxhp", "xp", "lv", "gold", "plot", "interact", "debug" });
		break;
	case Core::GameType::Undertale:
		ReadValues({ "hp", "maxhp", "xp", "lv", "gold", "plot", "interact", "debug" });
		break;
	case Core::GameType::Underswap:
		Read();
		break;
	}
}
