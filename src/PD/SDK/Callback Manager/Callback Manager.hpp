#pragma once
#include <map>
#include <forward_list>
#include <vector>

//Handles C++ callbacks, see Lua Engine.hpp for Lua callbacks

class CCallbackManager
{
public:
	struct RValue;

	using PD_Routine = void(__cdecl*)(std::vector<void*>);

	enum class Types : int
	{
		FRAME_BEGIN, //Top of the D3D hook
		FRAME_RENDER, //ImGui rendering phase
		FRAME_END, //After calling original
		
		DRAW_BEGIN, //Top of the Draw Event hook
		DRAW_END, //After calling original

		VMEXEC_BEGIN, //Before the VM executes any code (script or object code)
		VMEXEC_END, //After the VM executes the code

		VMEXEC_SCRIPT_BEGIN,
		VMEXEC_SCRIPT_END
	};

	CCallbackManager();

	void RegisterCallback(Types type, PD_Routine Routine);
	void UnregisterCallback(Types type, PD_Routine Routine);
	void UnregisterAllCallbacks();
	void Call(Types type, std::vector<void*> vpArgs);
private:
	std::map<Types, std::forward_list<PD_Routine>> prCallbackMap;
};