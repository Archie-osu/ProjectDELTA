#include "Invoker.hpp"
#include "../SDK/Memory/Memory.hpp"

struct CInstance;
using fnGML = void(__cdecl*)(RValue* Result, CInstance* pSelfInst, CInstance* pOtherInst, int argc, RValue* pArgs);

RValue __invoke(unsigned long Address, std::vector<RValue> pArguments, RValue& lpReturnVal)
{
	fnGML func = reinterpret_cast<fnGML>(Address);

	func(&lpReturnVal, 0, 0, pArguments.size(), pArguments.data());

	return lpReturnVal;
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

	if (!FuncMap.contains(szFuncName)) //First check the function map, O(1) is better than O(n^2)!
	{
		//If it's not there, get the function address and put it into the map, so we won't have to search for it again.
		dwAddress = getFnAddress(szFuncName);
		FuncMap.emplace(szFuncName, dwAddress);
	}
	else
	{
		//If it is in the function map already, just retrieve it from there, and call.
		dwAddress = FuncMap.at(szFuncName);
	}
	__invoke(dwAddress, pArguments, Result);

	return Result;  //Finish.
}

void Invoker::set_var(const char* szVarName, RValue Value)
{
	unsigned long dwAddress;
	RValue Result;

	if (!FuncMap.contains("variable_global_set"))
	{
		dwAddress = getFnAddress("variable_global_set");
		FuncMap.emplace("variable_global_set", dwAddress);
	}
	else
	{
		dwAddress = FuncMap.at("variable_global_set");
	}

	__invoke(dwAddress, { RValue(&szVarName), Value }, Result);
}

void Invoker::get_var(const char* szVarName, RValue& Value)
{
	unsigned long dwAddress;

	if (!FuncMap.contains("variable_global_get"))
	{
		dwAddress = getFnAddress("variable_global_get");
		FuncMap.emplace("variable_global_get", dwAddress);
	}
	else
	{
		dwAddress = FuncMap.at("variable_global_get");
	}

	__invoke(dwAddress, { RValue(&szVarName) }, Value);
}


