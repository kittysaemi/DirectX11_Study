#include "BitmapClass.h"


CBitmapClass::CBitmapClass(void)
{
	m_vBuffer = 0;
	m_iBuffer = 0;
	m_pTexture = 0;
}
CBitmapClass::CBitmapClass(const CBitmapClass& other)
{

}
CBitmapClass::~CBitmapClass(void)
{
}

bool CBitmapClass::Initialize(ID3D11Device * _device, int _ScW, int _ScH, WCHAR* _texFileName, int _imgW, int _imgH)
{
	m_nScreen.nWidth = _ScW;
	m_nScreen.nHeight = _ScH;

	m_nImage.nWidth = _imgW;
	m_nImage.nHeight = _imgH;

	m_nPrevPos.nX = -1;
	m_nPrevPos.nY = -1;

	if(!InitializeBuffers(_device))
		return false;

	if(!LoadTexture(_device, _texFileName))
		return false;

	return true;
}

void CBitmapClass::Shutdown()
{
	ReleaseTexture();

	ShutdownBuffers();
}

bool CBitmapClass::Render(ID3D11DeviceContext * _devContext, int _posX, int _poxY)
{
	if(!UpdateBuffers(_devContext, _posX, _poxY))
		return false;

	RenderBuffers(_devContext);
	return true;
}

int CBitmapClass::GetIndexCount(){return m_nICount;}
ID3D11ShaderResourceView* CBitmapClass::GetTexture(){return m_pTexture->GetTexture();}

bool CBitmapClass::InitializeBuffers(ID3D11Device* _device)
{
	m_nVCount = m_nICount = 6;

	VertexType* _vertics = new VertexType[m_nVCount];
	if(!_vertics)
		return false;
	
	unsigned long* _indices = new unsigned long[m_nICount];
	if(!_indices)
		return false;

	memset(_vertics, 0, (sizeof(VertexType) * m_nVCount));

	for(int i=0; i<m_nICount; i++)
		_indices[i] = i;

	D3D11_BUFFER_DESC _vertexBufDesc;
	_vertexBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	_vertexBufDesc.ByteWidth = sizeof(VertexType) * m_nVCount;
	_vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	_vertexBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	_vertexBufDesc.MiscFlags = 0;
	_vertexBufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA _vertexData;
	_vertexData.pSysMem = _vertics;
	_vertexData.SysMemPitch = 0;
	_vertexData.SysMemSlicePitch = 0;

	HRESULT hResult = _device->CreateBuffer(&_vertexBufDesc, &_vertexData, &m_vBuffer);
	if(FAILED(hResult))
		return false;

	D3D11_BUFFER_DESC _idxBufDesc;
	_idxBufDesc.Usage = D3D11_USAGE_DEFAULT;
	_idxBufDesc.ByteWidth = sizeof(unsigned long) * m_nICount;
	_idxBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	_idxBufDesc.CPUAccessFlags = 0;
	_idxBufDesc.MiscFlags = 0;
	_idxBufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA _idxData;
	_idxData.pSysMem = _indices;
	_idxData.SysMemPitch = 0;
	_idxData.SysMemSlicePitch = 0;

	hResult = _device->CreateBuffer(&_idxBufDesc, &_idxData, &m_iBuffer);
	if(FAILED(hResult))
		return false;

	delete[] _vertics;
	_vertics = 0;

	delete[] _indices;
	_indices;

	return true;

}

void CBitmapClass::ShutdownBuffers()
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

bool CBitmapClass::UpdateBuffers(ID3D11DeviceContext * _devContext, int _posX, int _posY)
{
	if((_posX == m_nPrevPos.nX && _posY == m_nPrevPos.nY))
		return true;

	m_nPrevPos.nX = _posX;
	m_nPrevPos.nY = _posY;

	// 이미지의 네변의 위치 계산
	float pLeft = 0.0f, pRight = 0.0f, pTop = 0.0f, pBottom = 0.0f;

	pLeft = (float)((m_nScreen.nWidth / 2) * -1) + (float)_posX;
	pRight = pLeft + (float)m_nImage.nWidth;
	pTop = (float)(m_nScreen.nHeight / 2) - (float)_posY;
	pBottom = pTop - (float)m_nImage.nHeight;

	// 새로운 정점 위치
	VertexType* _vertices = new VertexType[m_nVCount];
	if(!_vertices)
		return false;

	D3DXVECTOR3 TopLeft = D3DXVECTOR3(pLeft, pTop, 0.0f);
	D3DXVECTOR3 BottomRight = D3DXVECTOR3(pRight, pBottom, 0.0f);
	D3DXVECTOR3 BottomLeft = D3DXVECTOR3(pLeft, pBottom, 0.0f);
	D3DXVECTOR3 TopRight = D3DXVECTOR3(pRight, pTop, 0.0f);

	// First Triangle.
	_vertices[0].position = TopLeft;
	_vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

	_vertices[1].position = BottomRight;
	_vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

	_vertices[2].position = BottomLeft;
	_vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);


	// Second Triangle
	_vertices[3].position = TopLeft;
	_vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	_vertices[4].position = TopRight;
	_vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);
	
	_vertices[5].position = BottomRight;
	_vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);


	// Map, memcpy 정점 배열 내용을 정점 버퍼로 복사
	D3D11_MAPPED_SUBRESOURCE _mapResource;
	HRESULT hResult = _devContext->Map(m_vBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &_mapResource);
	if(FAILED(hResult))
		return false;

	VertexType* ptrVertex = (VertexType*)_mapResource.pData;

	memcpy(ptrVertex, (void*)_vertices, (sizeof(VertexType) * m_nVCount));

	_devContext->Unmap(m_vBuffer, 0);

	delete[] _vertices;
	_vertices = 0;

	return true;
	
}

void CBitmapClass::RenderBuffers(ID3D11DeviceContext * _devContext)
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	_devContext->IASetVertexBuffers(0, 1, &m_vBuffer, &stride, &offset);

	_devContext->IASetIndexBuffer(m_iBuffer, DXGI_FORMAT_R32_UINT, 0);

	_devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool CBitmapClass::LoadTexture(ID3D11Device * _device, WCHAR* _texFileName)
{
	m_pTexture = new CTextureClass;
	if(!m_pTexture)
		return false;

	if(!m_pTexture->Initialize(_device, _texFileName))
		return false;

	return true;
}

void CBitmapClass::ReleaseTexture()
{
	if(m_pTexture)
	{
		m_pTexture->Shutdown();
		delete m_pTexture;
		m_pTexture = 0;
	}
}