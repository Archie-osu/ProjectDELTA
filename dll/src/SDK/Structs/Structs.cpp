#include "Structs.hpp"
#include "../../Core/Core.hpp"
#include "../Memory/Memory.hpp"

//Just a proof of concept of how you might get this without the Core.hpp include
//Returns a CBasePlayerData pointer.
void* proof_of_concept(void* pGameObject)
{
	return *reinterpret_cast<void**>(pGameObject);
}

void InitUndertale(CUndertaleData** pUndertaleData, CUndertaleGlobals** pUndertaleGlobals)
{
	*pUndertaleData = reinterpret_cast<CUndertaleData*>(
		ghl::ptr_t(GetModuleHandleA(NULL)).autofollow({ 0x40894C, 0x44, 0x10, 0x298, 0x0 }).getraw()
		);

	*pUndertaleGlobals = reinterpret_cast<CUndertaleGlobals*>
		(ghl::ptr_t(GetModuleHandleA(NULL)).autofollow({ 0x40894C, 0x44, 0x10, 0x214, -0x20 }).getraw()
	);
}

void InitUnderswap(CUnderswapData** pUndertaleData, CUnderswapGlobals** pUndertaleGlobals)
{

}

void InitDeltarune(CDeltaruneData** pDeltaruneData, CDeltaruneGlobals** pDeltaruneGlobals)
{
	*pDeltaruneData = reinterpret_cast<CDeltaruneData*>(
		ghl::ptr_t(GetModuleHandleA(NULL)).autofollow({ 0x6B9558, 0x104, 0x150, 0x38, 0x950 }).getraw()
	);

	*pDeltaruneGlobals = reinterpret_cast<CDeltaruneGlobals*>
		(ghl::ptr_t(GetModuleHandleA(NULL)).autofollow({ 0x49C3E0, 0x60, 0x10, 0x9E8, 0x150 }).getraw()
	);
}

void SDK::Structs::Init(void* pGame)
{
	CGameObject* pGameObject = reinterpret_cast<CGameObject*>(pGame);
	switch (Core::CurrentGame)
	{
	case Core::GameType::Deltarune:
		printf("[+] Initializing for Deltarune!\n");
		InitDeltarune(reinterpret_cast<CDeltaruneData**>(&pGameObject->pPlayerData), reinterpret_cast<CDeltaruneGlobals**>(&pGameObject->pGlobals));
		break;
	case Core::GameType::Undertale:
		printf("[+] Initializing for Undertale!\n");
		InitUndertale(reinterpret_cast<CUndertaleData**>(&pGameObject->pPlayerData), reinterpret_cast<CUndertaleGlobals**>(&pGameObject->pGlobals));
		break;
	}
}
