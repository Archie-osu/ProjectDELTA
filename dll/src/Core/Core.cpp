#include "Core.hpp"
#include "../SDK/SDK.hpp"
void Core::Init()
{
	//TODO: Figure out a way to check if DX11 or DX9 is present (loaded DLLs?!)
	//First let's check which game we're injecting into.
	//ghl::AllocConsoleStream();

	std::string CurrentName = Memory::GetCurrentProcessName();

	if (CurrentName.find("DELTARUNE") != std::string::npos)
		CurrentGame = GameType::Deltarune;
	else if (CurrentName.find("UNDERTALE") != std::string::npos)
		CurrentGame = GameType::Undertale;
	else if (CurrentName.find("Underswap") != std::string::npos)
		CurrentGame = GameType::Underswap;
	else
	{
		if (MessageBoxA(0,
			"Automatic game detection failed.\nPlease specify DirectX Runtime version.\nDoes this game use DirectX 11?",
			"DirectX Runtime",
			MB_TOPMOST | MB_ICONWARNING | MB_YESNO
		) == IDYES)
		{
			CurrentGame = GameType::Unknown_DX11;
		}
		else
		{
			CurrentGame = GameType::Unknown_DX9;
		}
	}
		

	SDK::Structs::CreateCache();
	SDK::DX::Init();
	SDK::WndProc::Init();
}