#pragma once
#include "../Core/Structures/RValue.hpp"
#include <vector>

namespace Invoker
{
	RValue invoke(unsigned long Address, std::vector<RValue> pArguments);

	unsigned long getFnAddress(const char* szFuncName);

	//Warning: The first call may be slow, but subsequent calls are as fast as invoke(addr)
	RValue invoke(const char* szFuncName, std::vector<RValue> pArguments);
};