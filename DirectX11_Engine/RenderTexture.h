//////////////////////////////////////////////////////////////////////////
// Tutorial 22
// DirectX 11�� RTT����� ĸ��ȭ.
// ����� ��� �ؽ��ĸ� ���� Ÿ������ ������ �� �ְ� ���ش�.

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
