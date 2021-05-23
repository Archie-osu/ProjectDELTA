#include "RefThing.hpp"
#include "../RValue/RValue.hpp"
#include "../../Void.hpp"
#include "../../Memory Manager/Memory Manager.hpp"
void* YYAlloc(size_t size, bool zero)
{
	return Void.MemoryManager->Alloc(size, zero);
}

void YYFree(void* block)
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
	//No need to decrement manually, the destructor gets called if the object's valid!
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
		auto Block = YYAlloc(StrLen, false);
		memcpy(Block, _pS, StrLen);
		return cast<const char*>(Block);
	}

	return nullptr;
}
