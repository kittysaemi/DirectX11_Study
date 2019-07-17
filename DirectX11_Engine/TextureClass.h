//////////////////////////////////////////////////////////////////////////
// Tutorial 5

#pragma once

#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

#include <d3d11.h>
#include <D3DX11tex.h>

class CTextureClass
{
public:
	CTextureClass(void);
	CTextureClass(const CTextureClass& other);
	~CTextureClass(void);

	bool Initialize(ID3D11Device* _device, WCHAR* _filename);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_pShaderResourceViewTexture;
};


#endif
