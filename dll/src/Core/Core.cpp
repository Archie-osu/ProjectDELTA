#include "Core.hpp"
#include "../SDK/SDK.hpp"
#include "../Hooks/Hooks.hpp"
void Core::Init()
{
	//FILE* pFile;
	//AllocConsole();
	//freopen_s(&pFile, "CONOUT$", "w", stdout);

	//First let's check which game we're injecting into.
	std::string CurrentName = Memory::GetCurrentProcessName();

	if (CurrentName.find("DELTARUNE") != std::string::npos)
		CurrentGame = GameType::Deltarune;
	else if (CurrentName.find("UNDERTALE") != std::string::npos)
		CurrentGame = GameType::Undertale;
	else
		CurrentGame = GameType::Underswap;

	if (CurrentGame == GameType::Undertale)
		SDK::DX9::Init();
	else
		SDK::DX11::Init();
	SDK::WndProc::Init();
}