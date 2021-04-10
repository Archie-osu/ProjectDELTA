#include <string>
#include "../API/API.hpp"
#include <optional>

namespace Invoker
{
	template <typename _Ret>
	_Ret invoke(unsigned char* pFunction)
	{
		using _Fn = _Ret(__cdecl*)(Game::RValue, void*);

		Game::RValue fixArg1;

		_Fn function = (_Fn)(pFunction);
		_Ret result = (_Ret)function(fixArg1, nullptr);

		return result;
	}

	template <typename _Ret, typename _Arg1>
	_Ret invoke(unsigned char* pFunction, _Arg1 arg1)
	{
		using _Fn = _Ret(__cdecl*)(Game::RValue, _Arg1 properArg1, void*);

		Game::RValue fixArg1;

		_Fn function = (_Fn)(pFunction);
		_Ret result = (_Ret)function(fixArg1, arg1, nullptr);

		return result;
	}

	template <typename _Ret, typename _Arg1, typename _Arg2>
	_Ret invoke(unsigned char* pFunction, _Arg1 arg1, _Arg2 arg2)
	{
		using _Fn = _Ret(__cdecl*)(Game::RValue, _Arg1 properArg1, _Arg2 properArg2, void*);

		Game::RValue fixArg1;

		_Fn function = (_Fn)(pFunction);
		_Ret result = (_Ret)function(fixArg1, arg1, arg2, nullptr);

		return result;
	}

	template <typename _Fn, typename _Ret, typename _Arg1, typename _Arg2, typename _Arg3>
	_Ret invoke(unsigned char* pFunction, _Arg1 arg1, _Arg2 arg2, _Arg3 arg3)
	{
		using _Fn = _Ret(__cdecl*)(Game::RValue, _Arg1 properArg1, _Arg2 properArg2, _Arg3 properArg3, void*);

		Game::RValue fixArg1;

		_Fn function = (_Fn)(pFunction);
		_Ret result = (_Ret)function(fixArg1, arg1, arg2, arg3, nullptr);

		return result;
	}

	ghl::ptr_t FindPattern(const char* pattern, const char* mask);
}