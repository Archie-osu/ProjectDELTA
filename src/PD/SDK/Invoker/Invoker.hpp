#pragma once
#include <map>
#include <string>
#include <vector>
#include "../Structs/Structs.hpp"

class CInvoker
{
	std::map<std::string, unsigned long> prFunctionMap;
	void Invoke(RValue& result, std::vector<RValue> vArgs, unsigned long Function);
public:
	unsigned long FindFunction(const char* Name);
	RValue Call(const char* Function, std::vector<RValue> vArgs);
	RValue GetGlobal(std::string Name);
	RValue SetGlobal(std::string Name, const RValue& Value);
	RValue CreateObject(const char* Name, double PosX, double PosY);
	RValue CreateObjectSpoofed(const char* Name, double PosX, double PosY, OSFlavors Flavor);
	RValue CallSpoofed(const char* Function, std::vector<RValue> vArgs, OSFlavors Flavor);
};