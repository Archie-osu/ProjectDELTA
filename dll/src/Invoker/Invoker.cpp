#include "Invoker.hpp"
#include "../Core/Structures/CallData.hpp"
#include "../SDK/Memory/Memory.hpp"
#include "../Core/Core.hpp"

RValue Invoker::invoke(unsigned long Address, std::vector<RValue> pArguments)
{
	unsigned char* function = reinterpret_cast<unsigned char*>(Address);

	RValue ReturnVal = RValue(nullptr);

	fnGML func = reinterpret_cast<fnGML>(function);
	func(&ReturnVal, 0, 0, pArguments.size(), pArguments.data());

	return ReturnVal;
}

unsigned long Invoker::getFnAddress(const char* szFuncName)
{
	//This prevents stuff like draw_text() from being matched with action_draw_text
	std::string sFunc = szFuncName;
	std::string sMask;

	for (size_t i = 0; i <= sFunc.size(); i++)
		sMask.push_back('x');

	auto Pattern = Memory::FindPattern(sFunc.data(), sMask.c_str(), true);

	unsigned char mem[5];
	mem[0] = '\x68'; //Push instruction
	memcpy(mem + 1, &Pattern, 4); //Create new pattern - push <offset>

	unsigned char* dwReference = (unsigned char*)Memory::FindPattern(
		(const char*)mem,
		"xxxxx"
	) - 4; //Get the reference to the function

	return *(DWORD*)(dwReference); //Read address
}

RValue Invoker::invoke(const char* szFuncName, std::vector<RValue> pArguments)
{
	DWORD dwAddress;
	RValue Result;

	if (!Core::FuncMap.contains(szFuncName)) //First check the function map, O(1) is better than O(n^2)!
	{
		//If it's not there, get the function address and put it into the map, so we won't have to search for it again.
		dwAddress = getFnAddress(szFuncName);
		Core::FuncMap.emplace(szFuncName, dwAddress);
	}
	else
	{
		//If it is in the function map already, just retrieve it from there, and call.
		dwAddress = Core::FuncMap.at(szFuncName);
	}
	return invoke(dwAddress, pArguments); //Finish.
}
