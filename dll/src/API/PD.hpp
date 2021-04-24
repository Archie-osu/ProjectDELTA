#pragma once
#define ReCa reinterpret_cast
//Include what you use - if you need access to the ScriptManager,
//include the script manager's header. No way around it.

class CProjectDelta
{
public:
	class CHookManager* HookManager;
	class CScriptManager* ScriptManager;
	class CCallbackManager* CallbackManager;
	class CInvoker* Invoker;

	void RaiseError(const char* Format, ...);

	void Begin();
	void Stop();
};

inline CProjectDelta gDelta;