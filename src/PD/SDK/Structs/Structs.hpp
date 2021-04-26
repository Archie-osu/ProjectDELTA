#pragma once

enum RVKinds
{
	RV_Real,
	RV_String,
	RV_Array,
	RV_Pointer,
	RV_Vector3,
	RV_Undefined,
	RV_Object,
	RV_Int32,
	RV_Vector4,
	RV_Matrix4x4,
	RV_Int64,
	RV_Accessor,
	RV_JSNull,
	RV_Bool,
	RV_Iterator,
	RV_Reference,
	RV_Unset = 0xFFFFFF
};

using Int32 = __int32;
using Int64 = __int64;
struct RValue;

struct RArray
{
	Int32 nArrayLength;
	RValue* pArray;
};

struct RArrayRef
{
	Int32 nRefCount;
	RArray* pArray;
	RValue* pOwner;
};

struct RStringRef
{
	const char* String;
	Int32 nRefCount;
	Int32 nSize;
};

struct RValue
{
	union
	{
		double DoubleValue;
		Int32 Int32Value;
		Int64 Int64Value;
		void* PointerValue;
		const char** ppCharValue;
		RArrayRef* ArrayValue;
		RStringRef* StringValue;
	};

	Int32 Flags;
	Int32 Kind;

	RValue() : PointerValue(nullptr) {}

	RValue(Int32 Value);

	RValue(Int64 Value);

	RValue(double Value);

	RValue(RStringRef* Value);

	RValue(const char** Value);

	RValue* operator& ();

	RValue& at(int index);
};