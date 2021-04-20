#include "../../Hooks/hkEndScene/hkEndScene.hpp"
#include "../../Hooks/hkPresent/hkPresent.hpp"
#include "DirectX.hpp"
#include "../../Core/Core.hpp"
#include <stdio.h>

void SDK::DX::Init()
{
	switch (Core::CurrentGame)
	{
	case Core::GameType::Undertale:
	case Core::GameType::Unknown_DX9:
		printf("[+] Initializing in DirectX9 mode\n");
		Hooks::EndScene::Init();
		break;
	default: //I assume more games use DX11 than DX9, so it's the default condition
		printf("[+] Initializing in DirectX11 mode\n");
		Hooks::Present::Init();
		break;
	}
}
