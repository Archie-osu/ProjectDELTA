#pragma once
#include <vadefs.h>
class CMemoryManager
{
public:
	unsigned long GetFuncAddress(const char* ModuleName, const char* FunctionName);
	unsigned long PatternScan(const char* Pattern, const char* Mask, bool StringMode);

	//Limited pattern scan for custom base
	unsigned long OffsetScan(uintptr_t Base, uintptr_t MaxOffset, const char* Pattern, const char* Mask);

	//Limited pattern scanning for all modules
	unsigned long RegionScan(uintptr_t MaxOffset, const char* Pattern, const char* Mask);
};