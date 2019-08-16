//////////////////////////////////////////////////////////////////////////
// Tutorial 22
// CBitmapClass 와 동일한 기능, 자신의 텍스쳐는 가지고 있지 않고 RTT로 그려진 텍스쳐 사용.
// 하나의 2D 디버그용 윈도우의 개념으로 일반적인 비트맵 이미지가 아닌 RTT 텍스쳐를 그리는 것.


#pragma once
#ifndef _DEBUGWINDOW_H_
#define _DEBUGWINDOW_H_

#include <d3d11.h>
#include <D3DX10math.h>

class CDebugWindow
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	CDebugWindow(void);
	~CDebugWindow(void);

	bool Initialize(ID3D11Device * device, int scW, int scH, int imgW, int imgH);
	void Shutdown();
	bool Render(ID3D11DeviceContext * devCon, int posX, int poxY);

	int GetIndexCount();

private:
	bool InitializeBuffer(ID3D11Device * device);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext * devCon, int posX, int posY);
	void RenderBuffers(ID3D11DeviceContext * devCon);

private:
	ID3D11Buffer * m_vBuffer;
	ID3D11Buffer * m_iBuffer;
	int m_vCount;
	int m_iCount;
	int m_scW, m_scH;
	int m_imgW, m_imgH;
	int m_prevPosX, m_prevPosY;
};


#endif
