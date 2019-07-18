//////////////////////////////////////////////////////////////////////////
// ModelClass : 3D ��ü���� ���������� �κ��� �ٷ�� Ŭ���� ( 3D�𵨵��� ������ �����е��� ĸ��ȭ�ϴ� Ŭ���� )

/* 
	Tutorial 4
	. ��� �ﰢ�� �ϳ��� ����� ���� �����͸� �����.
	  �� �ﰢ���� ȭ�鿡 �׷����� ���� �ʿ��� ���� ���ۿ� �ε��� ���۵� �����.

	Tutorial 5
	�ؽ���

	Tutorial 6
	����
*/


#pragma once
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <D3DX10math.h>

// T5
#include "TextureClass.h"

class CModelClass
{
private:

	// �������ۿ� ����� ������ ����ü ���� ( ColorShaderClass���� ����� �Ͱ� ������ ���ƾ� �Ѵ�.)

	// T4
// 	struct VertexType 
// 	{
// 		D3DXVECTOR3 position;
// 		D3DXVECTOR4 color;
// 	};

	// T5
// 	struct VertexType
// 	{
// 		D3DXVECTOR3 position;
// 		D3DXVECTOR2 texture;
// 	};

	// T6
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};


public:
	CModelClass(void);
	CModelClass(const CModelClass& other);
	~CModelClass(void);

	bool Initialize(ID3D11Device* _device);
	bool Initialize(ID3D11Device* _device, WCHAR* _filename);	// T5 �߰�
	void Shutdown();
	void Render(ID3D11DeviceContext* _devContext);

	int GetIndexCount();

	// T5
	ID3D11ShaderResourceView* GetTexture();


private:
	bool InitializeBuffers(ID3D11Device* _device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* _devContext);

	// T5
	bool LoadTexture(ID3D11Device* _device, WCHAR* _filename);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_pVertexBuffer;
	ID3D11Buffer *m_pIndexBuffer;
	int	m_nVertexCount;
	int m_nIndexCount;

	// T5
	CTextureClass* m_pTexture;
};

#endif



