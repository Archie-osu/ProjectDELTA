#include "Structs.hpp"
#include "../../Core/Core.hpp"
#include "../../Invoker/Invoker.hpp"

void ReadValues(std::vector<std::string> VarNames)
{
	for (auto Name : VarNames)
	{
		const char* szGlobalName = Name.c_str();
		RValue Variable;
		Invoker::get_var(szGlobalName, Core::GlobalMap[Name]);
	}
}

void WriteValues(std::vector<std::string> VarNames)
{
	for (auto Name : VarNames)
	{
		const char* szGlobalName = Name.c_str();

		if (Core::GlobalMap.at(Name).nKind != 0xFFFFFF) //kind_unset
			Invoker::set_var(szGlobalName, Core::GlobalMap.at(Name));
	}
}

void SDK::Structs::Read()
{
	switch (Core::CurrentGame)
	{
	case Core::GameType::Deltarune:
		ReadValues({ "hp", "maxhp", "interact", "debug", "gold" });
		break;
	case Core::GameType::Undertale:
		ReadValues({ "hp", "maxhp", "gold", "lv", "xp", "plot", "debug", "interact" });
		break;
	case Core::GameType::Underswap:
		ReadValues({ "playerhp", "playermaxhp", "playergold", "playerlv", "playerxp", "canmove" });
		break;
	}
}

void SDK::Structs::Write()
{
	switch (Core::CurrentGame)
	{
	case Core::GameType::Deltarune:
		WriteValues({ "hp", "maxhp", "interact", "debug", "gold" });
		break;
	case Core::GameType::Undertale:
		WriteValues({ "hp", "maxhp", "gold", "lv", "xp", "plot", "debug", "interact" });
		break;
	case Core::GameType::Underswap:
		WriteValues({ "playerhp", "playermaxhp", "playergold", "playerlv", "playerxp", "canmove" });
		break;
	}
}

void SDK::Structs::CreateCache()
{
	int index = 0;
	while (1)
	{
		auto ret = Invoker::invoke("room_get_name", { RValue(index) });
		std::string roomName = ret.ToString();

		if (roomName == "<undefined>")
			break;

		Core::RoomList.push_back(roomName);
		index++;
	}
}
