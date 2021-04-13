#pragma once
#include <map>
#include <string>
#include "Structures/RValue.hpp"

namespace Core
{
	//Enumerates different types of games supported
	enum class GameType
	{
		Undertale,
		Deltarune,
		Underswap
	};

	inline GameType CurrentGame;
	
	inline std::map<std::string, RValue> GlobalMap;
	inline std::map<std::string, unsigned long> FuncMap;

	void Init();
}