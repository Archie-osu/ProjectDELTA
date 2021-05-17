#include "Structs.hpp"
#include "../Void.hpp"
#include "../Memory Manager/Memory Manager.hpp"
#include <memory>

void* WrapperYYAlloc(size_t size, bool zero)
{
	return Void.MemoryManager->Alloc(size, zero);
}

void WrapperYYFree(void* block)
{
	Void.MemoryManager->Free(block);
}

void __cdecl YYSetString(RValue* _pVal, const char* _pS)
{
	auto StringValue = new RefString(_pS, strlen(_pS));
	_pVal->StringValue = StringValue;
	_pVal->Kind = RV_String;
}

void __cdecl YYCreateString(RValue* _pVal, const char* _pS)
{
	switch (_pVal->Kind)
	{
	case RV_String:
		if (_pVal->StringValue)
			_pVal->StringValue->Dec();
		break;
	default:
		break;
	}

	_pVal->Flags = 0;
	_pVal->StringValue = nullptr;
	_pVal->StringValue = new RefString(_pS, strlen(_pS));
	_pVal->Kind = RV_String;
}

const char* __cdecl YYStrDup(const char* _pS)
{
	if (_pS)
	{
		auto StrLen = strlen(_pS) + 1;
		auto Block = WrapperYYAlloc(StrLen, false);
		memcpy(Block, _pS, StrLen);
		return cast<const char*>(Block);
	}

	return nullptr;
}

RValue& RValue::at(const int& index)
{
	if (this->Kind == RV_Array)
		if (this->ArrayValue)
			if (this->ArrayValue->pArray)
				return reinterpret_cast<RValue&>(this->ArrayValue->pArray->pArray[index]);

	return *this;
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
		if (this->pStringVal)
			this->pStringVal = RefString::assign(old.StringValue);
		else
			YYCreateString(this, "<Undeclared Project DELTA String>");
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

RValue* RValue::operator&()
{
	return this;
}

const char* GameForm_t::ReadString(unsigned long Offset)
{
	return reinterpret_cast<const char*>(reinterpret_cast<char*>(this) + Offset);
}