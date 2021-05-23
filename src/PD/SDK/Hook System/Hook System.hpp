#pragma once
#include <map>
#include <string>
class CHookSystem
{
	struct HookInfo_t
	{
		void* Target;
		void* Detour;
		void* Origin;
		bool  Unhooked;
	};

	std::map<std::string, HookInfo_t> prHookMap;
public:
	template <typename Fn>
	Fn GetOriginal(const char* Name)
	{
		//TODO: Check if it's valid memory
		return reinterpret_cast<Fn>(prHookMap.at(std::string(Name)).Origin);
	}
	
	void* Hook(const char* lpFuncName, void* lpTarget, void* lpDetour);
	void  Unhook(const char* lpFuncName);
	void  UnhookAll();
};