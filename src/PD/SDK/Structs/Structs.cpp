#include "Structs.hpp"

RValue& RValue::at(int index)
{
	if (this->Kind == RV_Array)
		if (this->ArrayValue)
			if (this->ArrayValue->pArray)
				return reinterpret_cast<RValue&>(this->ArrayValue->pArray[index]);

	return *this;
}

RValue::RValue(Int32 Value)
{
	this->Kind = RV_Int32;
	this->Int32Value = Value;
}

RValue::RValue(Int64 Value)
{
	this->Kind = RV_Int64;
	this->Int64Value = Value;
}

RValue::RValue(double Value)
{
	this->Kind = RV_Real;
	this->DoubleValue = Value;
}

RValue::RValue(const char** Value)
{
	this->Kind = RV_String;
	this->ppCharValue = Value;
}

RValue::RValue(RStringRef* Value)
{
	this->Kind = RV_String;
	this->StringValue = Value;
}

RValue* RValue::operator&()
{
	return this;
}