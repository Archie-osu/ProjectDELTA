#pragma once
#include "Structures/GameObject.hpp"

namespace Core
{
	//Enumerates different types of games supported
	enum class GameType
	{
		Undertale,
		Deltarune,
		Underswap,
		ComingSoon //Maybe? Eh, don't count on it.
	};

	inline GameType CurrentGame;
	inline CGameObject* pGame;

	void Init();
}