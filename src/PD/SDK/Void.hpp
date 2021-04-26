#pragma once
#define ReCa reinterpret_cast
#define StCa static_cast

struct CVoid
{
private:
	bool bAllowUnsafeFunctions = false;
public:
	class CInvoker* Invoker;
	class CHookSystem* HookSystem;
	class CLuaEngine* LuaEngine;
	class CCallbackManager* CallbackManager;
	class CMemoryManager* MemoryManager;

	void* GetGameWindow(); //Cast it to a HWND
	void* GetGameDevice(); //Cast it to a D3DDevice*
	bool  IsGameFullscreen();
	void  Error(const char* Format, ...);
	void  Warning(const char* Format, ...);

	void Load();
	void Unload();
	bool ShouldUnload();
};

inline CVoid Void;