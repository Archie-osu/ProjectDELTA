#pragma once
#include "PlayerData.hpp"
#include "GameGlobals.hpp"
/* Base class, ensures compatibility with games */
struct CGameObject
{
	CBasePlayerData* pPlayerData;
	CBaseGameGlobals* pGlobals;
};