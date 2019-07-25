//////////////////////////////////////////////////////////////////////////
// Tutorial 12
// ±Û¾¾¸¦ ±×¸®´Â ¼ÎÀÌ´õ 
#pragma once
#ifndef _FONTSHADER_H_
#define _FONTSHADER_H_

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>

using namespace std;

class CFontShader
{
private:	
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct PixelBufferType
	{
		D3DXVECTOR4 pixelColor;
	};

public:
	CFontShader(void);
	CFontShader(const CFontShader& other);
	~CFontShader(void);


	bool Initialize(ID3D11Device* _device, HWND _hWnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* _deviceContext, int idxCount, D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProjection, ID3D11ShaderResourceView* texture, D3DXVECTOR4 fcolor );

private:
	bool InitializeShader(ID3D11Device* _device, HWND hWnd, WCHAR* vsFileName, WCHAR* psFileName);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* _errMsg, HWND _hWnd, WCHAR* _fileName);

	bool SetShaderParameters(ID3D11DeviceContext* _deviceContext, D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProjection, ID3D11ShaderResourceView* textrue, D3DXVECTOR4 fcolor);
	void RenderShader(ID3D11DeviceContext* _deviceContext, int _idxCount);

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pMBuffer;

	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_pixelBuffer;
};

#endif


