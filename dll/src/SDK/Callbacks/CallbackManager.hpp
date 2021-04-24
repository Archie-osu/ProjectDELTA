#pragma once
#include <map>
#include <string>
#include <forward_list>

struct RValue;
using PD_Callback_Routine = void(__cdecl*)(RValue* pResult, int nArgCount, RValue* pArgs);

class CCallbackManager
{
public:
	enum class ECallbackType
	{
		FRAME_BEGIN,
		FRAME_END,
		SCRIPT_EXEC_BEGIN,
		SCRIPT_EXEC_END,
		DRAW_BEGIN,
		DRAW_END
	};

	bool RegisterCallback(PD_Callback_Routine pCallback, ECallbackType Type);
	bool UnregisterCallback(PD_Callback_Routine pCallback, ECallbackType Type);

	bool RegisterScript(const char* ScriptName, ECallbackType Type);
	bool UnregisterScript(const char* ScriptName, ECallbackType Type);
private:
	std::map<ECallbackType, std::forward_list<PD_Callback_Routine>> __mCallbacks;
	std::map<ECallbackType, std::forward_list<std::string>> __mScripts;

	void __UnregisterAllCallbacks();

	friend class CProjectDelta;
};