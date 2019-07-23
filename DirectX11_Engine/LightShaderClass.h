//////////////////////////////////////////////////////////////////////////
// Tutorial 6
// 모델에 빛이 비치는 것을 다루는 클래스
// TextureShaderClass 의 Upgrade Version

// Tutorial 9
// 주변광

// Tutorial 10
// 반사광


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

	struct CameraBufferType 
	{
		D3DXVECTOR3 cameraPosition;
		float padding; // 16배수
	};

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

	// T10,  각 변수들이 16byte로 정렬되도록 변수 배치
	struct LightBufferType_10
	{
		D3DXVECTOR4 ambientColor;
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR3 lightDirection;
		float specularPower;
		D3DXVECTOR4 specularColor;
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
	// T10
	bool Render(ID3D11DeviceContext* _devContext, int idxCount, D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProjection, ID3D11ShaderResourceView *_resourceView, D3DXVECTOR3 _lightDesc, D3DXVECTOR4 _DiffuseColor, D3DXVECTOR4 _ambientColor, D3DXVECTOR3 _camPosition, D3DXVECTOR4 _specColor, float _specPower);

private:
	bool InitializeShader(ID3D11Device* _device, HWND _hWnd, WCHAR* _vsFileName, WCHAR* _psFileName);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* _errMsg, HWND hWnd, WCHAR* _fileName);

	bool SetShaderParameters(ID3D11DeviceContext* _devContext, D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProjection, ID3D11ShaderResourceView *_resourceView, D3DXVECTOR3 _lightDesc, D3DXVECTOR4 _DiffuseColor );
	// T9
	bool SetShaderParameters(ID3D11DeviceContext* _devContext, D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProjection, ID3D11ShaderResourceView *_resourceView, D3DXVECTOR3 _lightDesc, D3DXVECTOR4 _DiffuseColor , D3DXVECTOR4 _ambientColor);
	// T10
	bool SetShaderParameters(ID3D11DeviceContext* _devContext, D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProjection, ID3D11ShaderResourceView *_resourceView, D3DXVECTOR3 _lightDesc, D3DXVECTOR4 _DiffuseColor , D3DXVECTOR4 _ambientColor, D3DXVECTOR3 _camPosition, D3DXVECTOR4 _specColor, float _specPower);
	void RenderShader(ID3D11DeviceContext* _devContext, int idxCount);

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_mBuffer;
	ID3D11Buffer* m_lBuffer;

	// 카메라 위치, 상수 버퍼
	ID3D11Buffer* m_cBuffer;

	// saemi
	int TutorialNum;
};

#endif
