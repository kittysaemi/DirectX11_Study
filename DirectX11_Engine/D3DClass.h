#pragma once
//////////////////////////////////////////////////////////////////////////
// tutorial 3 | DirectX 11의 초기화
//////////////////////////////////////////////////////////////////////////

// Tutorial 11
// Z buffer On/Off Function Additon.

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <D3Dcommon.h>
#include <d3d11.h>
#include <D3DX10math.h>

#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_

class CD3DClass
{
public:
	CD3DClass(void);
	CD3DClass(const CD3DClass&);
	~CD3DClass(void);

	// scW, scH : 윈도우 너비, 높이 -> Direct3D에서 동일한 크기의 영역을 초기화하고 이용.
	// hWnd : 윈도우 핸들
	// vsync : Direct3D 렌더링을 모니터의 주사율에 맞출 것인지, 가능한 한 빠르게 그릴 것인지 지정 (일반적으로 게임의 수직 동기화 기능)
	bool Initialize(int scW, int scH, bool vsync, HWND hWnd, bool fullscreenMode, float scDepth, float scNear);
	void Shutdown();

	void BeginScene(float R, float G, float B, float A);
	void EndScene();

	ID3D11Device * GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(D3DXMATRIX& matrix);
	void GetWorldMatrix(D3DXMATRIX& matrix);
	void GetOtherMatrix(D3DXMATRIX& matrix);

	void GetVideoCardInfo(char* cardName, int& mem);

	// Z buffer On/Off
	void TurnZBufferOn();
	void TurnZBufferOff();

private:
	bool m_bVsyncEnable;
	int m_nVideoCardMemeory;
	char m_sVideoCardDescription[128];
	IDXGISwapChain* m_dpSwapChain;
	ID3D11Device* m_dpDevice;
	ID3D11DeviceContext* m_dpDeviceContext;
	ID3D11RenderTargetView* m_dpRenderTargetView;
	ID3D11Texture2D* m_dpTexture2DBuffer;
	ID3D11DepthStencilState* m_dpDepthStencilState;
	ID3D11DepthStencilState* m_dpDisableDepthStencilState;
	ID3D11DepthStencilView* m_dpDepthStencilView;
	ID3D11RasterizerState* m_dpRasterizerState;
	D3DXMATRIX m_matrixProjection;
	D3DXMATRIX m_matrixWorld;
	D3DXMATRIX m_matrixOther;
};

#endif 
