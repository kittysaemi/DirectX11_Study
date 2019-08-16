#include "DebugWindow.h"


CDebugWindow::CDebugWindow(void)
{
	m_vBuffer = 0;
	m_iBuffer = 0;
}


CDebugWindow::~CDebugWindow(void)
{
}

bool CDebugWindow::Initialize(ID3D11Device * device, int scW, int scH, int imgW, int imgH)
{
	m_scW = scW;
	m_scH = scH;

	m_imgW = imgW;
	m_imgH = imgH;

	m_prevPosX = -1;
	m_prevPosY = -1;

	if(!InitializeBuffer(device))
		return false;

	return true;
}

void CDebugWindow::Shutdown()
{
	ShutdownBuffers();
}

bool CDebugWindow::Render(ID3D11DeviceContext * devCon, int posX, int poxY)
{
	if(!UpdateBuffers(devCon, posX, poxY))
		return false;

	RenderBuffers(devCon);

	return true;
}

int CDebugWindow::GetIndexCount()
{
	return m_iCount;
}

bool CDebugWindow::InitializeBuffer(ID3D11Device * device)
{
	m_vCount = 6;
	m_iCount = m_vCount;


	VertexType * vertices = new VertexType[m_vCount];
	if(!vertices)
		return false;

	unsigned long * indices = new unsigned long[m_iCount];
	if(!indices)
		return false;


	memset(vertices, 0, (sizeof(VertexType) * m_vCount));

	for(int i=0; i<m_iCount; i++)
		indices[i] = i;


	D3D11_BUFFER_DESC vbufDesc;
	vbufDesc.Usage = D3D11_USAGE_DYNAMIC;
	vbufDesc.ByteWidth = sizeof(VertexType) * m_vCount;
	vbufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbufDesc.MiscFlags = 0;
	vbufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vData;
	vData.pSysMem = vertices;
	vData.SysMemPitch = 0;
	vData.SysMemSlicePitch = 0;

	HRESULT hResult = device->CreateBuffer(&vbufDesc, &vData, &m_vBuffer);
	if(FAILED(hResult))
		return false;

	D3D11_BUFFER_DESC ibufDesc;
	ibufDesc.Usage = D3D11_USAGE_DEFAULT;
	ibufDesc.ByteWidth = sizeof(unsigned long) * m_iCount;
	ibufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibufDesc.CPUAccessFlags = 0;
	ibufDesc.MiscFlags = 0;
	ibufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iData;
	iData.pSysMem = indices;
	iData.SysMemPitch = 0;
	iData.SysMemSlicePitch = 0;

	hResult = device->CreateBuffer(&ibufDesc, &iData, &m_iBuffer);
	if(FAILED(hResult))
		return false;

	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

void CDebugWindow::ShutdownBuffers()
{
	if(m_iBuffer)
	{
		m_iBuffer->Release();
		m_iBuffer = 0;
	}

	if(m_vBuffer)
	{
		m_vBuffer->Release();
		m_vBuffer = 0;
	}
}

bool CDebugWindow::UpdateBuffers(ID3D11DeviceContext * devCon, int posX, int posY)
{
	// if the position we are rendring this bitmap to has not changed then don't update the vertex buffer since it
	// currently has the correct parameters.
	if((posX == m_prevPosX) && (posY == m_prevPosY))
		return true;

	// if it has changed then update the position it is being rendered to.
	m_prevPosX = posX;
	m_prevPosY = posY;

	// Calculate the screen coordinates of the left side of the bitmap.
	float pLeft = (float)((m_scW / 2) * -1) + (float)posX;

	// Calculate the screen coordinates of the right side of the bitmap.
	float pRight = pLeft + (float)m_imgW;

	// Calculate the screen coordinates of the top of the bitmap.
	float pTop = (float)(m_scH / 2) - (float)posY;

	// Calculate the screen coordinates of the bottom of the bitmap.
	float pBottom = pTop - (float)m_imgH;

	// Create the vertex array.
	VertexType * vertices = new VertexType[m_vCount];
	if(!vertices)
		return false;

	// Load the vertex array with data.
	vertices[0].position = D3DXVECTOR3(pLeft, pTop, 0.0f);
	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(pRight, pBottom, 0.0f);
	vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(pLeft, pBottom, 0.0f);
	vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[3].position = D3DXVECTOR3(pLeft, pTop, 0.0f);
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3(pRight, pTop, 0.0f);
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(pRight, pBottom, 0.0f);
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

	D3D11_MAPPED_SUBRESOURCE mapResource;
	HRESULT hResult = devCon->Map(m_vBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
	if(FAILED(hResult))
		return false;

	VertexType * ptrVertices = (VertexType*)mapResource.pData;

	memcpy(ptrVertices, (void*)vertices, (sizeof(VertexType) * m_vCount));

	devCon->Unmap(m_vBuffer, 0);

	delete [] vertices;
	vertices = 0;

	return true;


}

void CDebugWindow::RenderBuffers(ID3D11DeviceContext * devCon)
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	devCon->IASetVertexBuffers(0, 1, &m_vBuffer, &stride, &offset);

	devCon->IASetIndexBuffer(m_iBuffer, DXGI_FORMAT_R32_UINT, 0);

	devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}