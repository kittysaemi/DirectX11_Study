//////////////////////////////////////////////////////////////////////////
// Tutorial 12
// 글꼴 데이터를 가지고 문자열을 그리기 위한 정점 버퍼들을 만든다

#pragma once
#ifndef _FONTCLASS_H_
#define _FONTCLASS_H_

#include <d3d11.h>
#include <D3DX10math.h>
#include <fstream>
using namespace std;

#include "TextureClass.h"

class CFontClass
{
private:
	struct FontType
	{
		float nLeft;		// 텍스쳐의 U좌표
		float nRight;
		int nSize;			// 문자의 픽셀 너비
	};
	struct VertexType 
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	CFontClass(void);
	CFontClass(const CFontClass& other);
	~CFontClass(void);

	bool Initialize(ID3D11Device * _device, char * _idxFIleName, WCHAR * _texFIleName);
	void Shutdown();

	ID3D11ShaderResourceView * GetTexture();
	void BuildVertexArray(void* buffer, char * strData, float drawX, float drawY);

private:
	bool LoadFontData(char * _idxFileName);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device * _device, WCHAR * _texFileName);
	void ReleaseTexture();

	FontType * m_structFont;
	CTextureClass * m_pTexture;
};

#endif

