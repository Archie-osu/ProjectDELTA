#include "../Core.hpp"

//https://youtu.be/kxKKHKSMGIg - this guy saved my ass :D
CPlayerData* CGameObject::GetPlayerData()
{
	switch (Core::CurrentGame)
	{
	case Core::GameType::Undertale:
		return (0); //TODO: Put the pointer there
	case Core::GameType::Deltarune:
		return (0); //TODO: Put the pointer there
	case Core::GameType::Underswap:
		return (0); //TODO: Put the pointer there
	default:
		return nullptr;
	}
}
