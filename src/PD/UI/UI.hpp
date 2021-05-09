#pragma once
#include <vector>
#include <string>
namespace UI
{
	struct prRValue;
	inline bool bOpen = true;
	inline bool bNoMovementInMenu = true;
	inline bool bUseExperimentalSig = false;

	inline bool bDrawLuaConsole = false;
	inline bool bDrawDebugMenu = false;

	void Render(std::vector<prRValue*>);
	void ApplyStyle();
	void ShowGameWarning();
	void DrawMainMenuBar(); //Only returns true if we're unloading

	void DrawDebug();

	void Deltarune();
	void Undertale();
	void Sus(); //haha i did a funny

	void Default();
}