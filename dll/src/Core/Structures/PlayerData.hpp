#pragma once
#include "RValue.hpp"
struct CBasePlayerData {}; //Superclass

struct CUndertaleData : public CBasePlayerData
{
	//Undertale memory-layout stuff
};

struct CDeltaruneData : public CBasePlayerData
{
	RValue Kris_MaxHP;
	RValue Susie_MaxHP;
	RValue Ralsei_MaxHP;
	RValue Unknown;
	RValue Kris_CurHP;
	RValue Susie_CurHP;
	RValue Ralsei_CurHP;
};

struct CUnderswapData : public CBasePlayerData
{
	//Underswap memory-layout stuff
};