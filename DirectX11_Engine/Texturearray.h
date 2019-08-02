//////////////////////////////////////////////////////////////////////////
// Tutorial 17
// TextureClass ��ü
// �������� �ؽ��ĸ� ������ ���� �� �� �ֵ��� ���ش�.



#pragma once
#ifndef _TEXTUREARRAY_H_
#define _TEXTUREARRAY_H_

#include <d3d11.h>
#include <D3DX11tex.h>

class CTexturearray
{
public:
	CTexturearray(void);
	CTexturearray(const CTexturearray& otehr);
	~CTexturearray(void);

	bool Initialize(ID3D11Device * dev, WCHAR* fileList[]);
	void Shutdown();

	ID3D11ShaderResourceView ** GetTextureArray();

private:
	// This is the two element texture array private variable.
	// T19 , ���� �ø�
	ID3D11ShaderResourceView* m_TextureList[3];
};


#endif
