#pragma once
#define ReCa reinterpret_cast
#define StCa static_cast

template <typename T, typename N>

T cast(N Stuff)
{
	return (T)(Stuff);
}

struct CVoid
{
private:
	void* lpData;
public:
	void* pMainFont = nullptr; 
	void* pCodeFont = nullptr;
	bool bIsUnloading = false;

	class CInvoker* Invoker;
	class CHookSystem* HookSystem;
	class CLuaEngine* LuaEngine;
	class CCallbackManager* CallbackManager;
	class CPatternManager* PatternManager;
	class CMemoryManager* MemoryManager;

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