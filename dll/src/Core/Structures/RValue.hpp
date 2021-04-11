#pragma once
#pragma pack(push, 4)

struct RValue
{
	//Confusing GML datatypes go!
	using Boolean = double; 
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
		const char* szValue;
		Boolean bValue; //Literally the same thing as the Real, but I choose to keep consistent with GML.
	}; //0x8

	__int32 nFlags; //0xC
	__int32 nKind; //0x10

	RValue();

	RValue(Real v); //Same as RValue(boolean v)

	RValue(Int32 v);

	RValue(Int64 v);

	RValue(Pointer pv);

	RValue(const char* szv);

	void SetValue(Real v);

	void operator=(Real v); //Same as void operator=(boolean v)

	void operator=(Int32 v);

	void operator=(Int64 v);

	void operator=(Pointer pv);

	void operator=(const char* szv);

	explicit operator int();

	explicit operator double();

	explicit operator double* ();
};

#pragma pack(pop)