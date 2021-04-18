#pragma once
#include "../Core/Structures/RValue.hpp"
#include <vector>
#include <map>
#include <string>

namespace Invoker
{
	RValue invoke(const char* szFuncName, std::vector<RValue> pArguments);
	unsigned long getFnAddress(const char* szFuncName);

	void set_var(const char* szVarName, RValue Value);
	void get_var(const char* szVarName, RValue& Value);

	inline std::map<std::string, unsigned long> FuncMap;
};