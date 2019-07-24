//////////////////////////////////////////////////////////////////////////
// Tutorial 11
// 2D ������ (3D �������� ModelClass ��� ���Ǵ� Ŭ����)
// �� ��Ʈ�� �̹����� 3D��äó�� �׷����� ����, 2D �̹������� ���� ��ġ���Ϳ� �ؽ��� ��ǥ�� �ʿ��ϴ�.

#ifndef _BITMAPCLASS_H_
#define _BITMAPCLASS_H_

#pragma once
#include <d3d11.h>
#include <D3DX10math.h>

#include "TextureClass.h"

class CBitmapClass
{
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	CBitmapClass(void);
	CBitmapClass(const CBitmapClass& other);
	~CBitmapClass(void);

	bool Initialize(ID3D11Device * _device, int _ScW, int _ScH, WCHAR* _texFileName, int _imgW, int _imgH);
	void Shutdown();
	bool Render(ID3D11DeviceContext * _devContext, int _posX, int _poxY);

	int GetIndexCount();
	ID3D11ShaderResourceView * GetTexture();

private:
	bool InitializeBuffers(ID3D11Device* _device);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext * _devContext, int _posX, int _posY);
	void RenderBuffers(ID3D11DeviceContext * _devContext);

	bool LoadTexture(ID3D11Device * _device, WCHAR* _texFileName);
	void ReleaseTexture();

	ID3D11Buffer * m_vBuffer;
	ID3D11Buffer * m_iBuffer;
	int m_nVCount;
	int m_nICount;
	CTextureClass * m_pTexture;



	struct ObjectSize
	{
		int nWidth;
		int nHeight;
	};
	struct ObjectPos
	{
		int nX;
		int nY;
	};

	ObjectSize m_nScreen;
	ObjectSize m_nImage;
	ObjectPos m_nPrevPos;

};

#endif

