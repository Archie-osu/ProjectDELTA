#pragma once
struct RValue;

struct RArray
{
	int nArrayLength; //0x04
	RValue* pArray; //0x08
};

struct RArrayReference
{
	int nReferenceCount; //edx+04
	RArray* pArray; //edx+08
	RValue* pOwner; //edx+0C
};