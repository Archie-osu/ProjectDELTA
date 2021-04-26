#include "Callback Manager.hpp"
#include "../Void.hpp"

CCallbackManager::CCallbackManager()
{
	std::forward_list<PD_Routine> BaseList; BaseList.clear(); //Unnecessary cleaning..

	this->Callbacks.clear();

	this->Callbacks.emplace(Types::FRAME_BEGIN, BaseList);
	this->Callbacks.emplace(Types::FRAME_RENDER, BaseList);
	this->Callbacks.emplace(Types::FRAME_END, BaseList);

	this->Callbacks.emplace(Types::DRAW_BEGIN, BaseList);
	this->Callbacks.emplace(Types::DRAW_END, BaseList);

	this->Callbacks.emplace(Types::VMEXEC_BEGIN, BaseList);
	this->Callbacks.emplace(Types::VMEXEC_END, BaseList);
}

void CCallbackManager::RegisterCallback(Types type, PD_Routine Routine)
{
	if (!Callbacks.contains(type))
		Void.Error("Type %d already registered!", ReCa<int>(Routine));

	this->Callbacks.at(type).push_front(Routine);
}

void CCallbackManager::UnregisterCallback(Types type, PD_Routine Routine)
{
	if (!Callbacks.contains(type))
		Void.Error("Type %d not registered!", StCa<int>(type));

	this->Callbacks.at(type).remove(Routine);
}

void CCallbackManager::UnregisterAllCallbacks()
{
	Callbacks.clear();
}

void CCallbackManager::Call(Types type, std::vector<RValue*> vpArgs)
{
	if (!Callbacks.contains(type))
		Void.Error("Attempted to call invalid type %d!", StCa<int>(type));

	for (auto ref : Callbacks.at(type))
	{
		if (!Void.ShouldUnload())
			ref(vpArgs);
	}
		
}