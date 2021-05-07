#include "Structs.hpp"

RValue& RValue::at(int index)
{
	if (this->Kind == RV_Array)
		if (this->ArrayValue)
			if (this->ArrayValue->pArray)
				return reinterpret_cast<RValue&>(this->ArrayValue->pArray->pArray[index]);

	return *this;
}

RValue::RValue(double Value)
{
	this->Kind = RV_Real;
	this->DoubleValue = Value;
}

RValue::RValue(std::string str)
{
	this->Kind = RV_String;
	this->pStringVal = new RStringRef;
	this->pStringVal->nRefCount = 1;
	this->HeapAllocString = new char[512];
	memcpy(HeapAllocString, str.c_str(), str.length() + 1);
	this->pStringVal->String = HeapAllocString;
}

RValue::RValue(const char** Value)
{
	this->Kind = RV_String;
	this->ppCharValue = Value;
	this->HeapAllocString = 0;
}

RValue::RValue(void* Value)
{
	this->Kind = RV_Pointer;
	this->PointerValue = Value;
	this->HeapAllocString = 0;
}

RValue::RValue(RStringRef* Value)
{
	this->Kind = RV_String;
	this->StringValue = Value;
	this->HeapAllocString = 0;
}

RValue* RValue::operator&()
{
	return this;
}

const char* GameForm_t::ReadString(unsigned long Offset)
{
	return reinterpret_cast<const char*>(reinterpret_cast<char*>(this) + Offset);
}
