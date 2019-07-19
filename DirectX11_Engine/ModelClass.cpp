#include "ModelClass.h"


CModelClass::CModelClass(void)
{
	m_pVertexBuffer = 0;
	m_pIndexBuffer = 0;

	// T5
	m_pTexture = 0;
}
CModelClass::CModelClass(const CModelClass& other)
{

}


CModelClass::~CModelClass(void)
{
}

bool CModelClass::Initialize(ID3D11Device* _device)
{
	// ���� ���ۿ� �ε��� ���� �ʱ�ȭ
	if(!InitializeBuffers(_device))
	{
		OutputDebugStringA("CModelClass InitializeBuffers faild");
		return false;
	}

	return true;
}
bool CModelClass::Initialize(ID3D11Device* _device, WCHAR* _filename)
{
	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	if(!InitializeBuffers(_device))
	{
		OutputDebugStringA("ModelClass InitializeBuffers faild");
		return false;
	}
	// Load the texture for this model.
	 if(!LoadTexture(_device, _filename))
	 {
		 OutputDebugStringA("LoadTExture Faild");
		 return false;
	 }

	 return true;
}

void CModelClass::Shutdown()
{
	 // RELEASE the model texture.
	ReleaseTexture();

	// ���� ���ۿ� �ε��� ���� ����
	ShutdownBuffers();
	return;
}

void CModelClass::Render(ID3D11DeviceContext* _devContext)
{
	// �������ۿ� �ε��� ���۸� �׷��Ƚ� ���������ο� �־� ȭ�鿡 �׸� �غ� �Ѵ�.
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(_devContext);

	return;
}

// �÷� ���̴����� ���� �׸��� ���ؼ� �ʿ��� ����
int CModelClass::GetIndexCount()
{
	return m_nIndexCount;
}

ID3D11ShaderResourceView* CModelClass::GetTexture()
{
	return m_pTexture->GetTexture();
}

// ���� ���ۿ� �ε��� ���۸� �����ϴ� �۾��� �����Ѵ�. ������ ���Ϸκ��� ���� ������ �о� �ͼ� ���۵��� ����� ���� �Ѵ�. �ﰢ�� �ϳ����� �ٷ�� ������ ������ ���� ���ۿ� �ε��� ���ۿ� ���� �����ϴ� �ϸ��� �Ѵ�.
bool CModelClass::InitializeBuffers(ID3D11Device* _device)
{
	/*
		������ �ε��� �����͸� ��Ƶ� �ΰ��� �ӽ� �迭�� ����� ���߿� ���� ���۸� �����Ҷ� ����ϵ��� �Ѵ�.
	*/

	bool Triangle = true;

	if(Triangle)
	{
		// ���� �迭�� ���� ����
		m_nVertexCount = 3;
		// �ε��� �迭�� ���� ����
		m_nIndexCount = 3;
	}
	else
	{
		m_nVertexCount = 6;
		m_nIndexCount = 6;
	}

	
	// ���� �迭 ����
	VertexType* _vertices = new VertexType[m_nVertexCount];
	if(!_vertices)
	{
		OutputDebugStringA("vertex array create faild");
		return false;
	}

	// �ε��� �迭 ����
	unsigned long* _indices = new unsigned long[m_nIndexCount];
	if(!_indices)
	{
		OutputDebugStringA("Index array create faild");
		return false;
	}

	/*
		����/�ε��� �迭�� �ﰢ���� ������ �� ������ ä�� �ִ´�.
		�̰��� �׸��� ���ؼ��� ������ �ð� �������� ������ �Ѵ�.
		�� �ð� �������� ����� �Ǹ� DirectX���� �� �ﰢ���� �ݴ����� �ٶ󺻴ٰ� �Ǵ��ϸ� backface culling�� ���� �׷����� �ʰ� �ȴ�.
		GPU���� ������ �׸����� �� ���� �� ������ ����ϴ� ���� �߿��ϴ�. 
		������ ������� ����.
	*/
	
	// vertex array insert value
	D3DXVECTOR3 LeftBottom = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	D3DXVECTOR3 LeftTop = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	D3DXVECTOR3 TopCenter = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 RightTop = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	D3DXVECTOR3 RightBottom = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	D3DXVECTOR3 Center = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if(m_nVertexCount == 3)
	{
		// �ﰢ��
		_vertices[0].position = LeftBottom;
		_vertices[1].position = TopCenter;
		_vertices[2].position =  RightBottom ;
	}
	else if(m_nVertexCount == 6)
	{
		// �簢��
		_vertices[0].position = LeftBottom;
		_vertices[1].position = LeftTop;
		_vertices[2].position = RightTop;
		_vertices[3].position = LeftBottom;
		_vertices[4].position = RightTop;
		_vertices[5].position = RightBottom;
	}



	//T4
// 	D3DXVECTOR4 greenColor = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
// 	D3DXVECTOR4 RedColor = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
// 
// 	// ���� ���
// 	float Bright = 0.5f;
// 
// 
// 	for(int i=0; i<m_nIndexCount; i++)
// 	{
// 		_vertices[i].color = greenColor * Bright;
// 	}
		
	// T5
	D3DXVECTOR2 leftTopUV = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 TopCenterUV = D3DXVECTOR2(0.5f, 0.0f);
	D3DXVECTOR2 rightTopUV = D3DXVECTOR2(1.0f, 0.0f);
	D3DXVECTOR2 leftCenterUV = D3DXVECTOR2(0.0f, 0.5f);
	D3DXVECTOR2 CenterUV = D3DXVECTOR2(0.5f, 0.5f);
	D3DXVECTOR2 rightCenterUV = D3DXVECTOR2(1.0f, 0.5f);
	D3DXVECTOR2 leftBottomUV = D3DXVECTOR2(0.0f, 1.0f);
	D3DXVECTOR2 CenterBottomUV = D3DXVECTOR2(0.5f, 1.0f);
	D3DXVECTOR2 rightBottomUV = D3DXVECTOR2(1.0f, 1.0f);

	if(m_nVertexCount == 3)
	{
		_vertices[0].texture =  D3DXVECTOR2(0.0f, 1.0f);
		_vertices[1].texture = D3DXVECTOR2(0.5f, 0.0f);
		_vertices[2].texture =D3DXVECTOR2(1.0f, 1.0f);
	}
	else
	{
		_vertices[0].texture = leftBottomUV;
		_vertices[1].texture = leftTopUV;
		_vertices[2].texture = rightTopUV;
		_vertices[3].texture = leftBottomUV;
		_vertices[4].texture = rightTopUV;
		_vertices[5].texture = rightBottomUV;
	}

	//////////////////////////////////////////////////////////////////////////

	for(int i=0; i<m_nIndexCount; i++)
	{
		// T6
		/*
			�� ������ ���� ����� ���Ͽ� ������ ������ ������ �ȴ�.
			������ ������ ǥ�鿡 �����ϴ� ���̱� ������ ǥ���� �ٶ󺸰� �ִ� ������ ��Ȯ�� ����� �� �ֵ�.
			�ڵ��� �������� �����ϱ� ���� ������ ��û�ڿ��Է� ���ϵ��� �� ������ ������ Z������ -1.0f�� �Ͽ���
		*/
		_vertices[i].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		// T6 End
				
		_indices[i] = i;
	}



	/*
		�����迭�� �ε��� �迭 ä��, �̸� �̿��Ͽ� ���� ���ۿ� �ε��� ���۸� ����.
		���ۿ� ���� description �ۼ�. description���� ByteWidth(����ũ��)�� BindFlags(����Ÿ��)�� ��Ȯ�� �Է��ؾ� �Ѵ�.
		���� �迭�� �ε��� �迭�� subresource�����Ϳ� �����Ѵ�.
		descrition�� subresource ������, �׸��� d3d ����̽��� createbuffer �Լ��� ����Ͽ� �� ������ ������ �޾� �´�.
	*/

	// Set up the description of the vertex buffer.
	D3D11_BUFFER_DESC _vertexBufferDesc;
	_vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	_vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_nVertexCount;
	_vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	_vertexBufferDesc.CPUAccessFlags = 0;
	_vertexBufferDesc.MiscFlags = 0;
	_vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA _vertexData;
	_vertexData.pSysMem = _vertices;
	_vertexData.SysMemPitch = 0;
	_vertexData.SysMemSlicePitch = 0;

	// now create the vertex buffer.
	HRESULT hResult = _device->CreateBuffer(&_vertexBufferDesc, &_vertexData, &m_pVertexBuffer);
	if(FAILED(hResult))
	{
		OutputDebugStringA("Faild, Createbuffer vertex array");
		return false;
	}

	// Set up the description of the static index buffer.
	D3D11_BUFFER_DESC _indexBufferDesc;
	_indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	_indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_nIndexCount;
	_indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	_indexBufferDesc.CPUAccessFlags = 0;
	_indexBufferDesc.MiscFlags = 0;
	_indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA _indexData;
	_indexData.pSysMem = _indices;
	_indexData.SysMemPitch = 0;			
	_indexData.SysMemSlicePitch = 0;	

	// Create the index buffer.
	hResult = _device->CreateBuffer(&_indexBufferDesc, &_indexData, &m_pIndexBuffer);
	if(FAILED(hResult))
	{
		OutputDebugStringA("Faild, Createbuffer Index array");
		return false;
	}

	// Release the arrats now that the vertex and index buffers have been created and loaded.
	delete [] _vertices;
	_vertices = 0;
	delete [] _indices;
	_indices = 0;

	return true;
}

void CModelClass::ShutdownBuffers()
{
	// Index buffer release
	if(m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = 0;
	}

	// Vertex Buffer release
	if(m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = 0;
	}

	return;
}

/*
	Render �Լ����� ȣ�� ��.
	���� : Vertex buffer, Index buffer �� GPU�� ������� ���۷μ� Ȱ��ȭ��Ű�� ��.
	GPU�� Ȱ��ȭ�� ���� ���۸� ������ �Ǹ� ���̴��� �̿��Ͽ� ������ ������ �׸� �� �ְ� �ȴ�.
	�� ������ �ﰢ���̳� ����, ��ä�� �� � ������� �׸��� ���� �����Ѵ�.
	������� �Է¿� ���� ���ۿ� �ε��� ���۸� �ְ� DirectX�� IASetPrimitiveTopology �Լ��� ����Ͽ� GPU���� �������� �ﰢ������ �׸����� �ֹ��Ѵ�.
*/
void CModelClass::RenderBuffers(ID3D11DeviceContext* _devContext)
{
	// Vertex buffer, ������ ������ ����
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// input assembler�� vertex buffer Ȱ��ȭ
	_devContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// input assembler�� index buffer Ȱ��ȭ
	_devContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// vertex buffer�� �׸� �⺻�� ����. (�ﰢ��)
	_devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool CModelClass::LoadTexture(ID3D11Device* _device, WCHAR* _filename)
{
	m_pTexture = new CTextureClass;
	if(!m_pTexture)
	{
		OutputDebugStringA("CmodelClass Faild");
		return false;
	}

	if(!m_pTexture->Initialize(_device, _filename))
	{
		OutputDebugStringA("Textrue Initalize Faild");
		return false;
	}

	return true;
}

void CModelClass::ReleaseTexture()
{
	if(m_pTexture)
	{
		m_pTexture->Shutdown();
		delete m_pTexture;
		m_pTexture = 0;
	}
	return;
}