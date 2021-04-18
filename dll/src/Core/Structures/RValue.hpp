#pragma once
#pragma pack(push, 4)
#include "Array.hpp"

struct RValue
{
	//Confusing GML datatypes go!
	using Real = double;
	using Int32 = __int32;
	using Int64 = __int64;
	using Pointer = void*;

	union
	{
		Real dValue;
		Int32 iValue;
		Int64 i64Value;
		Pointer pValue;
		const char** szValue;
		RArrayReference* pArrayReference;
	}; //0x8

	__int32 nFlags; //0xC
	__int32 nKind; //0x10

	RValue() { i64Value = 0; nFlags = 0; nKind = 0xFFFFFF; }

	RValue(std::nullptr_t);

	RValue(Real v); //Same as RValue(boolean v)

	RValue(Int32 v);

	RValue(Int64 v);

	RValue(Pointer pv);

	RValue(const char* szv);

	RValue(const char** szv);

	RValue& at(int Index);

	const char* ToString();

	void operator=(Real v); //Same as void operator=(boolean v)

	void operator=(Int32 v);

	void operator=(Int64 v);

	void operator=(Pointer pv);

	void operator=(const char* szv);

	RValue& operator[](int Index);

	explicit operator int();

	explicit operator double();

	explicit operator double* ();
};

#pragma pack(pop)