#pragma once
#include "RValue.hpp"

//The universal calling convention that GML uses
struct CCallData
{
	RValue* pResult;
	void** pCallback;
	void** pCallback2;
	int ArgumentCount;
	RValue* pArguments;
	void* pNull;
};