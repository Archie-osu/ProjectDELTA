#pragma once
#include <d3d9.h>
#include <d3d11.h>

namespace UI
{
	inline bool bInit = false;
	inline bool bOpen = false;

	void Render();

	void Init(PDIRECT3DDEVICE9 pDX9Device);
	void Init(ID3D11Device* pDX11Device, ID3D11DeviceContext* pContext);
}