//////////////////////////////////////////////////////////////////////////
// Tutorial 22
// DirectX 11의 RTT기능을 캡슐화.
// 백버퍼 대신 텍스쳐를 렌더 타켓으로 설정할 수 있게 해준다.

#pragma once
#ifndef _RENDERTEXTURE_H_
#define _RENDERTEXTURE_H_

#include <d3d11.h>

class CRenderTexture
{
public:
	CRenderTexture(void);
	~CRenderTexture(void);

	bool Initialize(ID3D11Device * device, int texWidth, int texHeight);
	void Shutdown();

	void SetRenderTarget(ID3D11DeviceContext * devCon, ID3D11DepthStencilView * depthStencilView);
	void ClearRenderTarget(ID3D11DeviceContext * devCont, ID3D11DepthStencilView * depthStencilVIew, float rgba[4]);
	ID3D11ShaderResourceView* GetShaderResourceVIew();

private:
	ID3D11Texture2D * m_renderTargetTexture;
	ID3D11RenderTargetView * m_renderTargetView;
	ID3D11ShaderResourceView * m_ShaderResourceView;
};

#endif
