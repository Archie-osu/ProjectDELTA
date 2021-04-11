#pragma once
#include "RValue.hpp"
struct CBasePlayerData {}; //Superclass

struct CUndertaleData : public CBasePlayerData
{
	RValue Violence; //0x10
	RValue XP; //0x20
	RValue Gold; //0x30
	RValue Unknowns[0xB]; //0x90 (I think)
	RValue MaxHP; //0x100
	RValue CurHP; //0x110
	RValue DebugMode; //0x120
};

struct CDeltaruneData : public CBasePlayerData
{
	RValue Kris_MaxHP; //0x10
	RValue Susie_MaxHP; //0x20
	RValue Ralsei_MaxHP; //0x30
	RValue Unknown; //0x40
	RValue Kris_CurHP; //0x50
	RValue Susie_CurHP; //0x60
	RValue Ralsei_CurHP; //0x70
};

struct CUnderswapData : public CBasePlayerData
{
	//Underswap memory-layout stuff
};