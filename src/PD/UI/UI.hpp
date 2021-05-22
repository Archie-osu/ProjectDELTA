#pragma once
#include <vector>
#include <string>
namespace UI
{
	inline bool bOpen = true;
	inline bool bSetStyle = false;
	inline bool bNoMovementInMenu = true;
	inline bool bUseExperimentalSig = false;

	inline bool bDrawLuaConsole = false;
	inline bool bDrawDebugMenu = false;

	void Render(std::vector<void*>);
	void ApplyStyle();
	void ShowGameWarning();
	void DrawMainMenuBar();
	void DrawLuaConsole();

	void DrawDebug();

	void Deltarune();
	void Undertale();
	void Sus(); //haha i did a funny

	void Default();
}