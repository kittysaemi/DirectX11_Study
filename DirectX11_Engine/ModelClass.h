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

	Tutorial 7
	3D ��
*/


#pragma once
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <D3DX10math.h>

// T7
#include <fstream>
using namespace std;

// T5
#include "TextureClass.h"

// T17
#include "Texturearray.h"

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

	// T5, T17
// 	struct VertexType
// 	{
// 		D3DXVECTOR3 position;
// 		D3DXVECTOR2 texture;
// 	};

	// T20
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binormal;
	};

	// T6
// 	struct VertexType
// 	{
// 		D3DXVECTOR3 position;
// 		D3DXVECTOR2 texture;
// 		D3DXVECTOR3 normal;
// 	};

	// T7
// 	struct ModelType
// 	{
// 		float location[3];		// ��ġ
// 		float texture[2];		// �ؽ���
// 		float normalvetor[3];	// ���� ����
// 	};

	// T20
	struct ModelType
	{
		float location[3];		// ��ġ
		float texture[2];		// �ؽ���
		float normalvetor[3];	// ���� ����
		float tangentvetor[3];
		float binormalvetor[3];
	};
	struct TempVertexType
	{
		float location[3];
		float texture[2];
		float normalvetor[3];
	};
	struct VectorType
	{
		float location[3];
	};

public:
	CModelClass(void);
	CModelClass(const CModelClass& other);
	~CModelClass(void);

	bool Initialize(ID3D11Device* _device);
	bool Initialize(ID3D11Device* _device, WCHAR* _filename);	// T5 ~
	bool Initialize(ID3D11Device* _device, char* _modelfilename, WCHAR* _texurefilename, int nTutorialType = 7);	// T7
	bool Initialize(ID3D11Device* _device, char* _modelfilename, WCHAR* _fileLIst[], int nTutorialType);	// T17 - T20
	void Shutdown();
	void Render(ID3D11DeviceContext* _devContext);

	int GetIndexCount();

	// T5
	ID3D11ShaderResourceView* GetTexture();

	// T11
	ID3D11ShaderResourceView** GetTextureArray();


private:
	bool InitializeBuffers(ID3D11Device* _device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* _devContext);

	// T5
	bool LoadTexture(ID3D11Device* _device, WCHAR* _filename);
	// T17
	bool LoadTexture(ID3D11Device* _device, WCHAR* fileList[]);
	void ReleaseTexture();

	// T7
	bool LoadModel(char* _fileName);
	void ReleaseModel();

	// T20
	void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertexType _TempVT1, TempVertexType _TempVT2, TempVertexType _TempVT3, VectorType& tangent, VectorType& binormal);
	void CalculateNormal(VectorType tangent, VectorType binormal, VectorType& normal);

	ID3D11Buffer *m_pVertexBuffer;
	ID3D11Buffer *m_pIndexBuffer;
	int	m_nVertexCount;
	int m_nIndexCount;

	// T5
	CTextureClass* m_pTexture;

	// T7
	ModelType * m_pModelType;

	// T17
	CTexturearray * m_pTextureArray;


	int m_nTutorialType;
	void SetData(TempVertexType &vertex, int allidx);
	void SetData(int allidx, VectorType normal, VectorType tangent, VectorType binormal);
};

#endif



