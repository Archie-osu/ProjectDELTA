#pragma once
struct CPlayerData
{
	//This is a superclass, only used for polymorphism
};

struct CUndertaleData : public CPlayerData
{
	//Undertale memory-layout stuff
};

struct CDeltaruneData : public CPlayerData
{
	//Deltarune memory-layout stuff
};

struct CUnderswapData : public CPlayerData
{
	//Underswap memory-layout stuff
};