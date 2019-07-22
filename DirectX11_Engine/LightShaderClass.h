//////////////////////////////////////////////////////////////////////////
// Tutorial 6
// �𵨿� ���� ��ġ�� ���� �ٷ�� Ŭ����
// TextureShaderClass �� Upgrade Version

// Tutorial 9
// �ֺ��� �߰�


#pragma once
#ifndef _LIGHTSHADERCLASS_H_
#define _LIGHTSHADERCLASS_H_

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>
using namespace std;


class CLightshaderClass
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	// ����ü�� ũ�Ⱑ 16�� ����� �ǰ� �ϱ� ���ؼ� �������� ������� �ʴ� float�� ������.
	// sizeof(LightBufferType)�� 28Byte�� ũ��� CreateBuffer�� �ϸ� �Լ������� ������ 16��� ũ�⸦ �䱸�ϱ� ������ ������ �� �ִ�.
	struct LightBufferType
	{
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR3 lightDirection;
		float _padding;	// Added extra padding so structure is a multiple of 16 for createBuffer function requirements.
	};

	// T9
	struct LightBufferType_9
	{
		D3DXVECTOR4 ambientColor;
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR3 lightDirection;
		float _padding;
	};

public:
	CLightshaderClass(void);
	CLightshaderClass(const CLightshaderClass& other);
	~CLightshaderClass(void);

	bool Initialize(ID3D11Device* _device, HWND hWnd, int _TutorialNum);
	void Shutdown();
	bool Render(ID3D11DeviceContext* _devContext, int idxCount, D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProjection, ID3D11ShaderResourceView *_resourceView, D3DXVECTOR3 _lightDesc, D3DXVECTOR4 _DiffuseColor);
	// T 9
	bool Render(ID3D11DeviceContext* _devContext, int idxCount, D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProjection, ID3D11ShaderResourceView *_resourceView, D3DXVECTOR3 _lightDesc, D3DXVECTOR4 _DiffuseColor, D3DXVECTOR4 _ambientColor);

private:
	bool InitializeShader(ID3D11Device* _device, HWND _hWnd, WCHAR* _vsFileName, WCHAR* _psFileName);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* _errMsg, HWND hWnd, WCHAR* _fileName);

	bool SetShaderParameters(ID3D11DeviceContext* _devContext, D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProjection, ID3D11ShaderResourceView *_resourceView, D3DXVECTOR3 _lightDesc, D3DXVECTOR4 _DiffuseColor );
	// T9
		bool SetShaderParameters(ID3D11DeviceContext* _devContext, D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProjection, ID3D11ShaderResourceView *_resourceView, D3DXVECTOR3 _lightDesc, D3DXVECTOR4 _DiffuseColor , D3DXVECTOR4 _ambientColor);
	void RenderShader(ID3D11DeviceContext* _devContext, int idxCount);

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_mBuffer;
	ID3D11Buffer* m_lBuffer;

	// saemi
	int TutorialNum;
};

#endif
