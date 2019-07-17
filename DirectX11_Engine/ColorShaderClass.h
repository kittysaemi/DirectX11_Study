//////////////////////////////////////////////////////////////////////////
// ColorShaderClass / tutorial 4
// GPU상에 존재하는 3D모델들을 그리는데 사용하는 HLSL셰이더를 호출하는 클래스

#pragma once
#ifndef _COLORSHADERCLASS_H_
#define _COLORSHADERCLASS_H_

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>

using namespace std;

class CColorShaderClass
{
private:
	// 정점 셰이더에 사용될 CBuffer 구조체 정의, 
	struct MatrixBufferType
	{
		D3DMATRIX world;
		D3DMATRIX view;
		D3DMATRIX projection;
	};

public:
	CColorShaderClass(void);
	CColorShaderClass(const CColorShaderClass& other);
	~CColorShaderClass(void);

	bool Initialize(ID3D11Device * _device, HWND _hWnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* _devContext, int _idxCount, D3DXMATRIX _world, D3DXMATRIX _view, D3DXMATRIX _projection);

private:
	bool InitializeShader(ID3D11Device* _device, HWND _hWnd, WCHAR* _vsFileName, WCHAR* _psFileName);
	void ShutdownShader();
	void OutputShaderErrorMesage(ID3D10Blob* _errMsg, HWND _hWnd, WCHAR* sdFileName);

	bool SetShaderParameters(ID3D11DeviceContext* _devContext, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection );
	void RenderShader(ID3D11DeviceContext* _devContext, int _idxCount);

	ID3D11VertexShader * m_pVetexShader;
	ID3D11PixelShader * m_pPixelShader;
	ID3D11InputLayout * m_pLayout;
	ID3D11Buffer * m_pBuffer;
};


#endif

