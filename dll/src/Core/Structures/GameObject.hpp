#pragma once
#include "PlayerData.hpp"
/* Base class, ensures compatibility with games */
struct CGameObject
{
	CPlayerData* GetPlayerData();
};