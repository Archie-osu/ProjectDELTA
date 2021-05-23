#include "Callback Manager.hpp"
#include "../Void.hpp"

CCallbackManager::CCallbackManager()
{
	std::forward_list<PD_Routine> BaseList; BaseList.clear(); //Unnecessary cleaning..

	this->prCallbackMap.clear();

	this->prCallbackMap.emplace(Types::FRAME_BEGIN, BaseList);
	this->prCallbackMap.emplace(Types::FRAME_RENDER, BaseList);
	this->prCallbackMap.emplace(Types::FRAME_END, BaseList);

	this->prCallbackMap.emplace(Types::DRAW_BEGIN, BaseList);
	this->prCallbackMap.emplace(Types::DRAW_END, BaseList);

	this->prCallbackMap.emplace(Types::VMEXEC_BEGIN, BaseList);
	this->prCallbackMap.emplace(Types::VMEXEC_END, BaseList);
}

void CCallbackManager::RegisterCallback(Types type, PD_Routine Routine)
{
	if (type == CCallbackManager::Types::VMEXEC_SCRIPT_BEGIN)
		type = CCallbackManager::Types::VMEXEC_BEGIN;

	if (type == CCallbackManager::Types::VMEXEC_SCRIPT_END)
		type = CCallbackManager::Types::VMEXEC_END;

	this->prCallbackMap.at(type).push_front(Routine);
}

void CCallbackManager::UnregisterCallback(Types type, PD_Routine Routine)
{
	if (!prCallbackMap.contains(type))
		Void.Error("Type %d not registered!", StCa<int>(type));

	this->prCallbackMap.at(type).remove(Routine);
}

void CCallbackManager::UnregisterAllCallbacks()
{
	prCallbackMap.clear();
}

void CCallbackManager::Call(Types type, std::vector<void*> vpArgs)
{
	if (!prCallbackMap.contains(type))
		Void.Error("Attempted to call invalid type %d!", StCa<int>(type));

	for (auto ref : prCallbackMap.at(type))
	{
		if (!Void.bIsUnloading)
			ref(vpArgs);
	}
}