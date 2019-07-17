//////////////////////////////////////////////////////////////////////////
// Tutorial 5
// ColorShaderClass의 개정판
// 정점 셰이더와 픽셀 셰이더를 이용하여 3D모델을 그리는데 사용된다.
#pragma once
#ifndef _TEXTURESHADERCLASS_H_
#define _TEXTURESHADERCLASS_H_

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>

using namespace std;

class CTextureshaderclass
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

public:
	CTextureshaderclass(void);
	CTextureshaderclass(const CTextureshaderclass& other);
	~CTextureshaderclass(void);

	bool Initialize(ID3D11Device* _device, HWND _hWnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* _deviceContext, int idxCount, D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProjection, ID3D11ShaderResourceView* texture );

private:
	bool InitializeShader(ID3D11Device* _device, HWND hWnd, WCHAR* vsFileName, WCHAR* psFileName);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* _errMsg, HWND _hWnd, WCHAR* _fileName);

	bool SetShaderParameters(ID3D11DeviceContext* _deviceContext, D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProjection, ID3D11ShaderResourceView* textrue);
	void RenderShader(ID3D11DeviceContext* _deviceContext, int _idxCount);

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pMBuffer;

	ID3D11SamplerState* m_sampleState;

};



#endif

