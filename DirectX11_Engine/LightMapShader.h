//////////////////////////////////////////////////////////////////////////
// Tutorial 18

#pragma once
#ifndef _LIGHTMAPSHADER_H_
#define _LIGHTMAPSHADER_H_

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>
using namespace std;

class CLightMapShader
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};
public:
	CLightMapShader(void);
	CLightMapShader(const CLightMapShader& other);
	~CLightMapShader(void);

	bool Initialize(ID3D11Device * _dev, HWND _hWnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext * _devCon, int idxCnt, D3DXMATRIX wMatrix, D3DXMATRIX vMatrix, D3DXMATRIX pMatrix, ID3D11ShaderResourceView ** textureArray);

private:
	bool InitializeShader(ID3D11Device * _dev, HWND _hWnd, WCHAR* vsFileName, WCHAR* psFIleName);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errMsg, HWND hWnd, WCHAR* errFileName);

	bool SetShaderParameters(ID3D11DeviceContext * devCon, D3DXMATRIX wMatrix, D3DXMATRIX vMatrix, D3DXMATRIX pMatrix, ID3D11ShaderResourceView ** textureArray);
	void RenderShader(ID3D11DeviceContext * devCon, int idxCnt);

	ID3D11VertexShader * m_vertexShader;
	ID3D11PixelShader * m_pixelShader;
	ID3D11InputLayout * m_inputLayout;
	ID3D11Buffer * m_mBuffer;
	ID3D11SamplerState * m_sampleState;
};

#endif

