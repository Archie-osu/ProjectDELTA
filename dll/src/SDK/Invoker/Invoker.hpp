#pragma once
#include "../../Structs/GameMaker/RValue.hpp"
#include <map>
#include <string>
#include <vector>

class CInvoker
{
private:
	std::map<std::string, unsigned long> __funcMap;
	unsigned long __getFnAddr(const char* szFunc);

	RValue __invokeInternal(unsigned long Addr, std::vector<RValue> Args, RValue& ReturnValue);

public:
	RValue Call(const char* szFunc, std::vector<RValue> vArgs);
	
	void SetGlobal(const char* Name, const RValue& Value);
	void GetGlobal(const char* Name, RValue& outResult);

	friend class CProjectDelta;
};