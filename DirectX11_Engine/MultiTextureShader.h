//////////////////////////////////////////////////////////////////////////
// Tutorial 17


#pragma once
#ifndef _MULTITEXTURESHADER_H_
#define _MULTITEXTURESHADER_H_

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>
using namespace std;

class CMultiTextureShader
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};
public:
	CMultiTextureShader(void);
	CMultiTextureShader(const CMultiTextureShader& other);
	~CMultiTextureShader(void);

	bool Initialize(ID3D11Device * _dev, HWND _hWnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext * _devContext, int idxCnt, D3DXMATRIX wMatrix, D3DXMATRIX vMatrix, D3DXMATRIX pMatrix, ID3D11ShaderResourceView ** textureArray);

private:
	bool InitializeShader(ID3D11Device* _dev, HWND _hWnd, WCHAR* vsFileName, WCHAR * psFileName);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errMsg, HWND hWnd, WCHAR* shaderFileName);

	bool SetShaderParameters(ID3D11DeviceContext* devContext, D3DXMATRIX wMatrix, D3DXMATRIX vMatrix, D3DXMATRIX pMatrix, ID3D11ShaderResourceView ** tetureArray);
	void RenderShader(ID3D11DeviceContext* devContext, int idxCnt);

	ID3D11VertexShader * m_pVertextShader;
	ID3D11PixelShader * m_pPixelShader;
	ID3D11InputLayout * m_pInputLayout;
	ID3D11Buffer * m_pBuffer;
	ID3D11SamplerState * m_pSamplerState;
};

#endif
