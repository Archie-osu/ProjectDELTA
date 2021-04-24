#pragma once
#include "../../API/PD.hpp"
#include "vadefs.h"  // for uintptr_t
#include "xstddef"   // for less
#include "xstring"   // for string
#include <map>

class CHookManager
{
private:
	friend class CProjectDelta;

	struct __CHookInfo
	{
		void* pTarget;
		void* pDetour;
		void* pOriginal;
	};

	void __UnhookAll();

	inline static std::map<std::string, __CHookInfo> __mHookMap;
public:
	unsigned long	FindPattern(const char* Pattern, const char* Mask, bool StringMode = false, bool Override = false, uintptr_t SearchBase = 0, uintptr_t SearchLength = 0);
	void*			Hook(const char* lpFunctionName, void* lpTargetFn, void* lpHookFn); //Returns the original function which you can call manually.
	void			Unhook(const char* lpFunctionName);

	template <typename T>
	T GetOriginal(const char* lpFunctionName)
	{
		std::string sFunction(lpFunctionName);
		if (this->__mHookMap.contains(sFunction))
			return reinterpret_cast<T>(__mHookMap.at(sFunction).pOriginal);

		gDelta.RaiseError("HookManager::GetOriginal('%s') failed!\nThe process will now exit.", lpFunctionName);

		return nullptr;
	}
};