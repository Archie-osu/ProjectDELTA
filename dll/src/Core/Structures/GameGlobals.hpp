#pragma once
#include "RValue.hpp"
struct CBaseGameGlobals {}; //Superclass

struct CDeltaruneGlobals : public CBaseGameGlobals
{
	RValue Interact; //ecx+10
	RValue Unknown; //ecx+20
	RValue Unknown2; //ecx+30
	RValue DebugMode; //ecx+40
};

struct CUndertaleGlobals : public CBaseGameGlobals
{

};

struct CUnderswapGlobals : public CBaseGameGlobals
{

};