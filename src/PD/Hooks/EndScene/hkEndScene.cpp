#include "hkEndScene.hpp"
#include "../../SDK/Void.hpp"
#include "../../SDK/Hook System/Hook System.hpp"
HRESULT WINAPI Hooks::EndScene::Hook(LPDIRECT3DDEVICE9 lpDevice)
{
	return Void.HookSystem->GetOriginal<FN>("EndScene")(lpDevice);
}

void* Hooks::EndScene::GetTargetAddress()
{
	void* ppTable[119]; //Array of void pointers (void**)

	auto Device = ReCa<LPDIRECT3DDEVICE9>(Void.GetGameDevice());

	if (!Device)
		Void.Error("D3DDevice was nullptr!");

	memcpy(ppTable, *ReCa<void***>(Device), sizeof(ppTable));

	return ppTable[42];
}
