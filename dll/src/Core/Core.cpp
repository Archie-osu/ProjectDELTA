#include "Core.hpp"
#include "../SDK/SDK.hpp"
void Core::Init()
{
	//TODO: Figure out a way to check if DX11 or DX9 is present (loaded DLLs?!)
	//First let's check which game we're injecting into.
	ghl::AllocConsoleStream();

	std::string CurrentName = Memory::GetCurrentProcessName();

	if (CurrentName.find("DELTARUNE") != std::string::npos)
		CurrentGame = GameType::Deltarune;
	else if (CurrentName.find("UNDERTALE") != std::string::npos)
		CurrentGame = GameType::Undertale;
	else
		CurrentGame = GameType::Underswap;
		
	SDK::Structs::Init(&pGame);
	SDK::DX::Init();
	SDK::WndProc::Init();
}