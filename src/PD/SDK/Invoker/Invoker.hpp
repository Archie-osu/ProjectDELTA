#pragma once
#include <map>
#include <string>
#include <vector>
#include "../Structs/Structs.hpp"

class CInvoker
{
	std::map<std::string, unsigned long> prFunctionMap;
	void Invoke(RValue& result, std::vector<RValue> vArgs, unsigned long Function);
	unsigned long FindFunction(const char* Name);
public:
	RValue Call(const char* Function, std::vector<RValue> vArgs);
	RValue GetGlobal(const char* Name);
	RValue SetGlobal(const char* Name, const RValue& Value);
};