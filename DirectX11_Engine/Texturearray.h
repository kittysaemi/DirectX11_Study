//////////////////////////////////////////////////////////////////////////
// Tutorial 17
// TextureClass 대체
// 여러개의 텍스쳐를 가지고 접근 할 수 있도록 해준다.



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
	// T19 , 갯수 늘림
	ID3D11ShaderResourceView* m_TextureList[3];
};


#endif
