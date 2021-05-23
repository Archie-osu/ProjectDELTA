#pragma once
#include <stdint.h>
struct RValue;

struct RArray
{
	int32_t nArrayLength;
	RValue* pArray;
};

struct RArrayRef
{
	int32_t nRefCount;
	RArray* pArray;
	RValue* pOwner;
};
