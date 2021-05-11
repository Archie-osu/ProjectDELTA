#pragma once
#define ReCa reinterpret_cast
#define StCa static_cast

struct CVoid
{
private:
	void* lpData;
public:
	class CInvoker* Invoker;
	class CHookSystem* HookSystem;
	class CLuaEngine* LuaEngine;
	class CCallbackManager* CallbackManager;
	class CMemoryManager* MemoryManager;
	class CLuaCallbackManager* LuaCallbackManager; //Cut down version of the C++ callback manager.

	void* GetGameWindow(); //HWND
	void* GetGameDevice(); //D3DDevice*
	void* GetGameContext(); //D3DDeviceContext* (DX11 only)
	void* FindGameData(); //GameForm_t*
	void* GetGameData(); //GameForm_t*, utilizes the cached variable, USE THIS INSTEAD OF FindGameData()!
	bool  IsGameFullscreen();
	void  Error(const char* Format, ...);
	void  Warning(const char* Format, ...);

	void Load();
	void Unload();
	bool ShouldUnload();
};

inline CVoid Void;