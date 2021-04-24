#include "CallbackManager.hpp"

bool CCallbackManager::RegisterCallback(PD_Callback_Routine pCallback, ECallbackType Type)
{
	if (__mCallbacks.contains(Type))
	{
		//Add elements to the underlying forward_list
		__mCallbacks.at(Type).push_front(pCallback);
		return true;
	}
	std::forward_list<PD_Callback_Routine> DummyList;

	__mCallbacks.emplace(Type, DummyList);
	__mCallbacks.at(Type).push_front(pCallback);

	return true;
}

bool CCallbackManager::UnregisterCallback(PD_Callback_Routine pCallback, ECallbackType Type)
{
	if (__mCallbacks.contains(Type))
	{
		__mCallbacks.at(Type).remove(pCallback);
		return true;
	}
	return false;
}

bool CCallbackManager::RegisterScript(const char* ScriptName, ECallbackType Type)
{
	if (__mScripts.contains(Type))
	{
		//Add elements to the underlying forward_list
		__mScripts.at(Type).push_front(ScriptName);
		return true;
	}
	std::forward_list<std::string> DummyList;

	__mScripts.emplace(Type, DummyList);
	__mScripts.at(Type).push_front(ScriptName);

	return true;
}

bool CCallbackManager::UnregisterScript(const char* ScriptName, ECallbackType Type)
{
	if (__mScripts.contains(Type))
	{
		__mScripts.at(Type).remove(ScriptName);
		return true;
	}
	return false;
}

void CCallbackManager::__UnregisterAllCallbacks()
{
	__mScripts.clear();
	__mCallbacks.clear();
}
