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
	// 정점 버퍼와 인덱스 버퍼 초기화
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

	// 정점 버퍼와 인덱스 버퍼 해제
	ShutdownBuffers();
	return;
}

void CModelClass::Render(ID3D11DeviceContext* _devContext)
{
	// 정점버퍼와 인덱스 버퍼를 그래픽스 파이프라인에 넣어 화면에 그릴 준비를 한다.
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(_devContext);

	return;
}

// 컬러 셰이더에서 모델을 그리기 위해서 필요한 정보
int CModelClass::GetIndexCount()
{
	return m_nIndexCount;
}

ID3D11ShaderResourceView* CModelClass::GetTexture()
{
	return m_pTexture->GetTexture();
}

// 정점 버퍼와 인덱스 버퍼를 생성하는 작업을 제어한다. 데이터 파일로부터 모델의 정보를 읽어 와서 버퍼들을 만드는 일을 한다. 삼각형 하나만을 다루기 떄문에 간단히 정점 버퍼와 인덱스 버퍼에 점을 세팅하는 일만을 한다.
bool CModelClass::InitializeBuffers(ID3D11Device* _device)
{
	/*
		정점과 인덱스 데이터를 담아둘 두개의 임시 배열을 만들고 나중에 최종 버퍼를 생설할때 사용하도록 한다.
	*/

	bool Triangle = true;

	if(Triangle)
	{
		// 정점 배열의 길이 설정
		m_nVertexCount = 3;
		// 인덱스 배열의 길이 설정
		m_nIndexCount = 3;
	}
	else
	{
		m_nVertexCount = 6;
		m_nIndexCount = 6;
	}

	
	// 정점 배열 생성
	VertexType* _vertices = new VertexType[m_nVertexCount];
	if(!_vertices)
	{
		OutputDebugStringA("vertex array create faild");
		return false;
	}

	// 인덱스 배열 생성
	unsigned long* _indices = new unsigned long[m_nIndexCount];
	if(!_indices)
	{
		OutputDebugStringA("Index array create faild");
		return false;
	}

	/*
		정점/인덱스 배열에 삼각형의 각점과 그 순서를 채워 넣는다.
		이것을 그리기 위해서는 점들을 시계 방향으로 만들어야 한다.
		반 시계 방향으로 만들게 되면 DirectX에서 이 삼각형은 반대편을 바라본다고 판단하며 backface culling에 의해 그려지지 않게 된다.
		GPU에게 도형을 그리도록 할 때는 이 순서를 기억하는 것이 중요하다. 
		색상은 녹색으로 정함.
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
		// 삼각형
		_vertices[0].position = LeftBottom;
		_vertices[1].position = TopCenter;
		_vertices[2].position =  RightBottom ;
	}
	else if(m_nVertexCount == 6)
	{
		// 사각형
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
// 	// 색상 밝기
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
			각 정점은 빛의 계산을 위하여 연관된 법선을 가지게 된다.
			법선은 도형의 표면에 수직하는 선이기 떄문에 표면이 바라보고 있는 방향을 정확히 계산할 수 있따.
			코드의 간단함을 유지하기 위해 법선이 시청자에게로 향하도록 각 정점의 법선의 Z성분을 -1.0f로 하였다
		*/
		_vertices[i].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		// T6 End
				
		_indices[i] = i;
	}



	/*
		정점배열과 인덱스 배열 채움, 이를 이용하여 정점 버퍼와 인덱스 버퍼를 생성.
		버퍼에 대한 description 작성. description에는 ByteWidth(버퍼크기)와 BindFlags(버퍼타입)을 정확히 입력해야 한다.
		정점 배열과 인덱스 배열을 subresource포인터에 연결한다.
		descrition과 subresource 포인터, 그리고 d3d 디바이스의 createbuffer 함수를 사용하여 새 버퍼의 포인터 받아 온다.
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
	Render 함수에서 호출 됨.
	목적 : Vertex buffer, Index buffer 를 GPU의 어셈블러의 버퍼로서 활성화시키는 것.
	GPU가 활성화된 정점 버퍼를 가지게 되면 셰이더를 이용하여 버퍼의 내용을 그릴 수 있게 된다.
	이 정점을 삼각형이나 선분, 부채꼴 등 어떤 모양으로 그리게 될지 정의한다.
	어셈블러의 입력에 정점 버퍼와 인덱스 버퍼를 넣고 DirectX의 IASetPrimitiveTopology 함수를 사용하여 GPU에게 정점들을 삼각형으로 그리도록 주문한다.
*/
void CModelClass::RenderBuffers(ID3D11DeviceContext* _devContext)
{
	// Vertex buffer, 단위와 오프셋 설정
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// input assembler에 vertex buffer 활성화
	_devContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// input assembler에 index buffer 활성화
	_devContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// vertex buffer로 그릴 기본형 설정. (삼각형)
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