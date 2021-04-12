#pragma once
#include "Structures/GameObject.hpp"
#include <map>

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
	inline CGameObject pGame;

	inline std::map<const char*, unsigned long> szFuncMap;

	void Init();
}