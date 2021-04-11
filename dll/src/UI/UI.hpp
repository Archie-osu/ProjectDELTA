#pragma once
#include <d3d9.h>
#include <d3d11.h>

namespace UI
{
	inline bool bInit = false;
	inline bool bOpen = false;
	inline int nTab = 0;

	//Maybe put these in separate files?

	void Render(); //Automatically determines which functions to call
	void SetStyle();
	
	void RenderUndertale();
	void RenderDeltarune();
	void RenderUnderswap();

	void Init(PDIRECT3DDEVICE9 pDX9Device);
	void Init(ID3D11Device* pDX11Device, ID3D11DeviceContext* pContext);
}