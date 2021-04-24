#include "hkExecuteIt.hpp"
#include "../../API/PD.hpp"
#include "../../SDK/Hooks/HookManager.hpp"

bool __cdecl Hooks::ExecuteIt::Hook(void* pSelf, void* pOther, void* pCode, RValue* Args)
{
	return gDelta.HookManager->GetOriginal<HookFn>("hkExecuteIt")(pSelf, pOther, pCode, Args);
}

void Hooks::ExecuteIt::Init()
{

}
