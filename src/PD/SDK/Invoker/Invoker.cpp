#include "Invoker.hpp"
#include "../Void.hpp"
#include "../Memory Manager/Memory Manager.hpp"

void CInvoker::Invoke(RValue& result, std::vector<RValue> vArgs, unsigned long Function)
{
	struct CInstance;
	using fnGML = void(__cdecl*)(RValue*, CInstance*, CInstance*, int, RValue*);

	fnGML func = reinterpret_cast<fnGML>(Function);

	func(&result, 0, 0, vArgs.size(), vArgs.data());
}

unsigned long CInvoker::FindFunction(const char* Name)
{
	//This prevents stuff like draw_text() from being matched with action_draw_text
	std::string sFunc(Name);
	std::string sMask;

	for (size_t i = 0; i <= sFunc.size(); i++)
		sMask.push_back('x');

	auto Pattern = Void.PatternManager->PatternScan(sFunc.data(), sMask.c_str(), true);

	if (!Pattern)
		return 0;

	unsigned char mem[5];
	mem[0] = '\x68'; //Push instruction
	memcpy(mem + 1, &Pattern, 4); //Create new pattern - push <offset>

	unsigned char* dwReference = (unsigned char*)Void.PatternManager->PatternScan(
		(const char*)mem,
		"xxxxx",
		false
	) - 4; //Get the reference to the function

	return *(unsigned long*)(dwReference); //Read address
}

RValue CInvoker::Call(const char* Function, std::vector<RValue> vArgs)
{
	unsigned long dwAddress;
	RValue Result;

	if (!prFunctionMap.contains(Function)) //First check the function map, O(n) is better than O(n^2)!
	{
		//If it's not there, get the function address and put it into the map, so we won't have to search for it again.
		dwAddress = FindFunction(Function);

		if (dwAddress)
			prFunctionMap.emplace(Function, dwAddress);
	}
	else
	{
		//If it is in the function map already, just retrieve it from there, and call.
		dwAddress = prFunctionMap.at(Function);
	}
	if (!dwAddress) {
		Result.Int64Value = 0LL;
		Result.Kind = RV_Fail;
		return Result;
	}
		
	Invoke(Result, vArgs, dwAddress);

	return Result;  //Finish.
}

RValue CInvoker::GetGlobal(std::string Name)
{
	return Call("variable_global_get", { Name });
}

RValue CInvoker::SetGlobal(std::string Name, const RValue& Value)
{
	return Call("variable_global_set", { Name, Value });
}

RValue CInvoker::CreateObject(const char* Name, double PosX, double PosY)
{
	RValue rvAssetID = Call("asset_get_index", { std::string(Name) });
	return Call("instance_create", { PosX, PosY, rvAssetID });
}

RValue CInvoker::CallSpoofed(const char* Function, std::vector<RValue> vArgs, OSFlavors Flavor)
{
	RValue oFlavor = GetGlobal("osflavor");
	SetGlobal("osflavor", StCa<double>(Flavor));
	auto Return = Call(Function, vArgs);
	SetGlobal("osflavor", oFlavor);
	return Return;
}

RValue CInvoker::CreateObjectSpoofed(const char* Name, double PosX, double PosY, OSFlavors Flavor)
{
	RValue oFlavor = GetGlobal("osflavor");
	SetGlobal("osflavor", StCa<double>(Flavor));
	auto Return = CreateObject(Name, PosX, PosY);
	SetGlobal("osflavor", oFlavor);
	return Return;
}