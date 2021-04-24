#include "Invoker.hpp"
#include "../../API/PD.hpp"
#include "../Hooks/HookManager.hpp"

unsigned long CInvoker::__getFnAddr(const char* szFunc)
{
	//This prevents stuff like draw_text() from being matched with action_draw_text
	std::string sFunc = szFunc;
	std::string sMask;

	for (size_t i = 0; i <= sFunc.size(); i++)
		sMask.push_back('x');

	auto Pattern = gDelta.HookManager->FindPattern(sFunc.data(), sMask.c_str(), true);

	unsigned char mem[5];
	mem[0] = '\x68'; //Push instruction
	memcpy(mem + 1, &Pattern, 4); //Create new pattern - push <offset>

	unsigned char* dwReference = (unsigned char*)gDelta.HookManager->FindPattern(
		(const char*)mem,
		"xxxxx"
	) - 4; //Get the reference to the function

	return *(unsigned long*)(dwReference); //Read address
}

RValue CInvoker::__invokeInternal(unsigned long Addr, std::vector<RValue> Args, RValue& ReturnValue)
{
	struct CInstance;
	using fnGML = void(__cdecl*)(RValue*, CInstance*, CInstance*, int, RValue*);

	fnGML func = reinterpret_cast<fnGML>(Addr);

	func(&ReturnValue, 0, 0, Args.size(), Args.data());

	return ReturnValue;
}

RValue CInvoker::Call(const char* szFunc, std::vector<RValue> vArgs)
{
	unsigned long dwAddress;
	RValue Result;

	if (!__funcMap.contains(szFunc)) //First check the function map, O(n) is better than O(n^2)!
	{
		//If it's not there, get the function address and put it into the map, so we won't have to search for it again.
		dwAddress = __getFnAddr(szFunc);
		__funcMap.emplace(szFunc, dwAddress);
	}
	else
	{
		//If it is in the function map already, just retrieve it from there, and call.
		dwAddress = __funcMap.at(szFunc);
	}
	__invokeInternal(dwAddress, vArgs, Result);

	return Result;  //Finish.
}

void CInvoker::SetGlobal(const char* Name, const RValue& Value)
{
	Call("global_variable_set", { &Name, Value });
}

void CInvoker::GetGlobal(const char* Name, RValue& outResult)
{
	outResult = Call("global_variable_get", { &Name });
}
