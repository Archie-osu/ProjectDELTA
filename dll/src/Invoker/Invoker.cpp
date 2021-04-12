#include "Invoker.hpp"
#include "../Core/Structures/CallData.hpp" //Includes RValue.hpp
#include "../SDK/Memory/Memory.hpp"
#include "../Core/Core.hpp"

RValue Invoker::invoke(unsigned long Address, std::vector<RValue> pArguments)
{
	unsigned char* function = reinterpret_cast<unsigned char*>(Address);

	using Fn = int(__cdecl*)(CCallData Data);
	RValue ReturnVal(nullptr);
	CCallData pData;

	pData.ArgumentCount = pArguments.size();
	pData.pArguments = pArguments.data();

	pData.pCallback = nullptr;
	pData.pCallback2 = nullptr;
	pData.pNull = nullptr;

	pData.pResult = &ReturnVal;

	Fn func = reinterpret_cast<Fn>(function);
	func(pData);

	return *pData.pResult;
}

unsigned long Invoker::getFnAddress(const char* szFuncName)
{
	std::string sFunc = szFuncName; sFunc.push_back('\x00');
	std::string sMask;

	for (size_t i = 0; i <= sFunc.length(); i++)
		sMask.push_back('x');

	auto Pattern = Memory::FindPattern(sFunc.data(), sMask.c_str());

	unsigned char mem[5];
	mem[0] = '\x68';
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

	if (!Core::szFuncMap.contains(szFuncName)) //First check the function map, O(1) is better than O(n^2)!
	{
		//If it's not there, get the function address and put it into the map, so we won't have to search for it again.
		dwAddress = getFnAddress(szFuncName);
		Core::szFuncMap.emplace(szFuncName, dwAddress);
	}
	else
	{
		//If it is in the function map already, just retrieve it from there, and call.
		dwAddress = Core::szFuncMap.at(szFuncName);
	}
	return invoke(dwAddress, pArguments); //Finish.
}
