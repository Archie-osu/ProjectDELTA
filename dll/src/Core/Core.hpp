#pragma once
#include <map>
#include <vector>
#include <string>
#include "Structures/RValue.hpp"

namespace Core
{
	//Enumerates different types of games supported
	enum class GameType
	{
		Undertale,
		Deltarune,
		Underswap,
		Unknown_DX9,
		Unknown_DX11,
	};

	inline GameType CurrentGame;
	inline std::map<std::string, RValue> GlobalMap;
	inline std::vector<std::string> RoomList;

	void Init();
}