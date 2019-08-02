//////////////////////////////////////////////////////////////////////////
// tutorial 21
// Bump Map Shader ¼öÁ¤


#pragma once
#ifndef _SPECMAPSHADER_H_
#define _SPECMAPSHADER_H_

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>
using namespace std;

class CSpecMapShader
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
		D3DXVECTOR4 specularColor;
		float specularPower;
		D3DXVECTOR3 lightDirection;
	};

	struct CameraBufferType
	{
		D3DXVECTOR3 cameraPosition;
		float padding;
	};
public:
	CSpecMapShader(void);
	~CSpecMapShader(void);

	bool Initialize(ID3D11Device* device, HWND hWnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceCon, int idxCnt, D3DXMATRIX wMatrix, D3DXMATRIX vMatrix, D3DXMATRIX pMatrix, ID3D11ShaderResourceView ** texturArray, D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower);

private:
	bool InitializeShader(ID3D11Device* device, HWND hWnd, WCHAR * vsFIlename, WCHAR * psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errMsg, HWND hWnd, WCHAR* errFileName);
	bool SetShaderParameters(ID3D11DeviceContext * deviceCon, D3DXMATRIX wMatrix, D3DXMATRIX vMatrix, D3DXMATRIX pMatrix, ID3D11ShaderResourceView ** texturArray, D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 sepecularColor, float specularPower);
	void RenderShader(ID3D11DeviceContext * deviceCon, int idxCnt);

	ID3D11VertexShader * m_vertexShader;
	ID3D11PixelShader * m_pixelShader;
	ID3D11InputLayout * m_inputLayout;
	ID3D11Buffer * m_mBuffer;
	ID3D11SamplerState * m_sampleState;
	ID3D11Buffer * m_lBuffer;
	ID3D11Buffer * m_cBuffer;
};

#endif
