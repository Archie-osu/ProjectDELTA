#include "../RValue/RValue.hpp"
#include "../RefThing/RefThing.hpp"

RValue& RValue::at(const int& index)
{
	if (this->Kind == RV_Array)
		if (this->ArrayValue)
			if (this->ArrayValue->pArray)
				return reinterpret_cast<RValue&>(this->ArrayValue->pArray->pArray[index]);

	return *this;
}

RValue::~RValue()
{
	if (Kind == RV_String)
	{
		RefString::remove(this->StringValue);
		this->StringValue = nullptr;
	}
	//Arrays too, sometime.
}

RValue::RValue(const RValue& old)
{
	this->Kind = old.Kind;
	this->Flags = 0;
	switch (old.Kind)
	{
	case RV_Array:
		this->ArrayValue = old.ArrayValue;
		if (this->ArrayValue)
			this->ArrayValue->nRefCount++;

		break;
	case RV_String:
		this->StringValue = RefString::assign(old.StringValue);
		if (!this->StringValue)
			YYCreateString(this, "<Undeclared PD String>");
		break;
	default:
		this->Int64Value = old.Int64Value; //Like memcpy, but without the call.
		break;
	}
}

RValue::RValue(const double& Value)
{
	this->Kind = RV_Real;
	this->DoubleValue = Value;
}

RValue::RValue(const std::string& str)
{
	this->Kind = RV_String;
	YYCreateString(this, str.c_str());
}

RValue::RValue(const char** Value)
{
	this->Kind = RV_String;
	this->ppCharValue = Value;
}

RValue::RValue(void* Value)
{
	this->Kind = RV_Pointer;
	this->PointerValue = Value;
}

RValue& RValue::operator[](int index)
{
	return this->at(index);
}

RValue* RValue::operator&()
{
	return this;
}
