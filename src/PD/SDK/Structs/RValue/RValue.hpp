#pragma once
#include <stdint.h>
#include <string>
#include "../RefThing/RefThing.hpp"
#include "../RArray/RArray.hpp"

enum RVKinds : int32_t
{
	RV_Real = 0,
	RV_String = 1,
	RV_Array = 2,
	RV_Pointer = 3,
	RV_Vector3 = 4,
	RV_Undefined = 5,
	RV_Object = 6,
	RV_Int32 = 7,
	RV_Vector4 = 8,
	RV_Matrix4x4 = 9,
	RV_Int64 = 10,
	RV_Accessor = 11,
	RV_JSNull = 12,
	RV_Bool = 13,
	RV_Iterator = 14,
	RV_Reference = 15,
	RV_Fail = 16, //Used by PD, not actually a Kind in-game
	RV_Unset = 0xFFFFFF
};

#pragma pack(push, 4)
struct RValue
{
	union
	{
		double DoubleValue;
		int32_t Int32Value;
		int64_t Int64Value;
		void* PointerValue;
		const char** ppCharValue;
		RArrayRef* ArrayValue;
		RefString* StringValue;
	};

	int32_t Flags;
	int32_t Kind;

	RValue() : PointerValue(nullptr) {}

	RValue(const double& Value);

	RValue(const std::string& str);

	RValue(const RValue& Other);

	RValue(const char** Value);

	RValue(void* Value);

	RValue& operator[](int index);

	RValue* operator& ();

	RValue& at(const int& index);

	~RValue();
};
#pragma pack(pop)