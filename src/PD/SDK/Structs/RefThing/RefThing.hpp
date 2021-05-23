#pragma once
#include <vcruntime_string.h>
struct RValue;

void* YYAlloc(size_t size, bool zero);

void YYFree(void* block);

void __cdecl YYSetString(RValue* _pVal, const char* _pS);

void __cdecl YYCreateString(RValue* _pVal, const char* _pS);

const char* __cdecl YYStrDup(const char* _pS);

template <typename T>
struct _RefThing
{
	T m_Thing;
	int m_refCount;
	int m_Size;

	//This is entirely transcripted from IDA disassembly, as I'm too dumb to code this
	_RefThing(T thing, size_t size)
	{
		void* block = nullptr;

		if (thing)
		{
			this->m_Size = size;
			block = YYAlloc(size + 1, false);
			memcpy(block, thing, size + 1);
		}
		else
		{
			this->m_Size = 0;
		}

		this->m_Thing = (T)(block);
		this->m_refCount = 'PD3'; //Start with a large refcount, to trick the game into not freeing our memory
	}

	bool Dec()
	{
		auto ShouldFree = this->m_refCount-- == 'PD3'; //Free manually if the refCount is 'PD3'.
		//The game handles normal refcounts itself, so I don't need to worry about that.

		if (ShouldFree)
		{
			YYFree((void*)this->m_Thing);
			this->m_Size = 0;
			this->m_Thing = nullptr;
			return true;
		}
		return false;
	}

	void Inc()
	{
		this->m_refCount += 1;
	}

	~_RefThing()
	{
		this->Dec();
	}

	static _RefThing<T>* assign(_RefThing<T>* _other) { if (_other != nullptr) { _other->Inc(); } return _other; }
	static _RefThing<T>* remove(_RefThing<T>* _other) { if (_other != nullptr) { _other->Dec(); } return nullptr; }
};

using RefString = _RefThing<const char*>;