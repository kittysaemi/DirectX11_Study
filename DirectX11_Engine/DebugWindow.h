//////////////////////////////////////////////////////////////////////////
// Tutorial 22
// CBitmapClass �� ������ ���, �ڽ��� �ؽ��Ĵ� ������ ���� �ʰ� RTT�� �׷��� �ؽ��� ���.
// �ϳ��� 2D ����׿� �������� �������� �Ϲ����� ��Ʈ�� �̹����� �ƴ� RTT �ؽ��ĸ� �׸��� ��.


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
