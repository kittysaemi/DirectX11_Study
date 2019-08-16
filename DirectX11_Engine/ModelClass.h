//////////////////////////////////////////////////////////////////////////
// ModelClass : 3D 객체들의 기하학적인 부분을 다루는 클래스 ( 3D모델들의 복잡한 기하학들을 캡슐화하는 클래스 )

/* 
	Tutorial 4
	. 녹색 삼각형 하나를 만들기 위한 데이터를 만든다.
	  이 삼각형이 화면에 그려지기 위해 필요한 정점 버퍼와 인덱스 버퍼도 만든다.

	Tutorial 5
	텍스쳐

	Tutorial 6
	조명

	Tutorial 7
	3D 모델
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

	// 정점버퍼에 사용할 정점의 구조체 선언 ( ColorShaderClass에서 사용할 것과 구조가 같아야 한다.)

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
// 		float location[3];		// 위치
// 		float texture[2];		// 텍스쳐
// 		float normalvetor[3];	// 법선 벡터
// 	};

	// T20
	struct ModelType
	{
		float location[3];		// 위치
		float texture[2];		// 텍스쳐
		float normalvetor[3];	// 법선 벡터
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



