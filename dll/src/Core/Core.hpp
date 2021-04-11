#pragma once
#include "Structures/GameObject.hpp"

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

	void Init();
}