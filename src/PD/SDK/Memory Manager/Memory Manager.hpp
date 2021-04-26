#pragma once

class CMemoryManager
{
public:
	unsigned long GetFuncAddress(const char* ModuleName, const char* FunctionName);
	unsigned long PatternScan(const char* Pattern, const char* Mask, bool StringMode);
};