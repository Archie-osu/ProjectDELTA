#pragma once
#include <d3d9.h>
#include <d3d11.h>

namespace UI
{
	constexpr float flWindowX = 480.0f, flWindowY = 360.0f;

	inline bool bInit = false;
	inline bool bOpen = false;
	inline bool bDraw = false;

	inline int nTab = 0;

	//Maybe put these in separate files?

	void Render(); //Automatically determines which functions to call
	void SetStyle();
	
	void RenderInvoker(); //Shared, so I just have a function to avoid duplicate code.
	void RenderUndertale();
	void RenderDeltarune();
	void RenderUnderswap();

	void Init(PDIRECT3DDEVICE9 pDX9Device);
	void Init(ID3D11Device* pDX11Device, ID3D11DeviceContext* pContext);
}