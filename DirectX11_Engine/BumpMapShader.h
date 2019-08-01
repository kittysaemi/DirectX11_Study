//////////////////////////////////////////////////////////////////////////
// Tutorial 20
// 범프 매핑

#pragma once
#ifndef _BUMPMAPSHADER_H_
#define _BUMPMAPSHADER_H_

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>
using namespace std;

class CBumpMapShader
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct LightBufferType
	{
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR3 lightDirection;
		float padding;
	};

public:
	CBumpMapShader(void);
	~CBumpMapShader(void);

	bool Initialize(ID3D11Device * dev, HWND hWnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext * devCon, int idxCnt, D3DXMATRIX wMatrix, D3DXMATRIX vMatrix, D3DXMATRIX pMatrix, ID3D11ShaderResourceView** txtureArray, D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColor);

private:
	bool InitializeShader(ID3D11Device * dev, HWND hWnd, WCHAR * vsFileName, WCHAR * psFileName);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errMsg, HWND hWnd, WCHAR * errFileName);
	
	bool SetShaderParameters(ID3D11DeviceContext * devCon, D3DXMATRIX wMatrix, D3DXMATRIX vMatrix, D3DXMATRIX pMatrix, ID3D11ShaderResourceView** txtureArray, D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColor);
	void RenderShader(ID3D11DeviceContext * devCon, int idxCnt);

	ID3D11VertexShader * m_vertexShader;
	ID3D11PixelShader * m_pixelShader;
	ID3D11InputLayout * m_inputLayout;
	ID3D11Buffer * m_mBuffer;
	ID3D11SamplerState * m_sampleState;
	ID3D11Buffer * m_lBuffer;
};

#endif
