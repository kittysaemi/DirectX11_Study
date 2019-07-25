//////////////////////////////////////////////////////////////////////////
// Tutorial 12
// �۲� �����͸� ������ ���ڿ��� �׸��� ���� ���� ���۵��� �����

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
		float nLeft;		// �ؽ����� U��ǥ
		float nRight;
		int nSize;			// ������ �ȼ� �ʺ�
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

