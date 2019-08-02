#include "ModelClass.h"


CModelClass::CModelClass(void)
{
	m_pVertexBuffer = 0;
	m_pIndexBuffer = 0;

	// T5
	m_pTexture = 0;

	// T7
	m_pModelType = 0;

	// T17
	m_pTextureArray = 0;

	m_nTutorialType = 0;
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
bool CModelClass::Initialize(ID3D11Device* _device, char* _modelfilename, WCHAR* _texurefilename)
{
	if(!LoadModel(_modelfilename))
	{
		return false;
	}

	if(!InitializeBuffers(_device))
	{
		return false;
	}
	
	if(!LoadTexture(_device, _texurefilename))
	{
		return false;
	}

	return true;
}
bool CModelClass::Initialize(ID3D11Device* _device, char* _modelfilename, WCHAR* _fileLIst[], int nTutorialType)
{
	if(!LoadModel(_modelfilename))
		return false;

	m_nTutorialType = nTutorialType;
	if(m_nTutorialType == 20 || m_nTutorialType == 21)
	{
		// Calculate the normal, tangent, and binormal vectors for the model.
		CalculateModelVectors();
	}

	if(!InitializeBuffers(_device))
		return false;

	if(!LoadTexture(_device, _fileLIst))
		return false;

	return true;
}
void CModelClass::Shutdown()
{
	 // RELEASE the model texture.
	ReleaseTexture();

	// ���� ���ۿ� �ε��� ���� ����
	ShutdownBuffers();

	ReleaseModel();
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
ID3D11ShaderResourceView** CModelClass::GetTextureArray()
{
	return m_pTextureArray->GetTextureArray();
}

// ���� ���ۿ� �ε��� ���۸� �����ϴ� �۾��� �����Ѵ�. ������ ���Ϸκ��� ���� ������ �о� �ͼ� ���۵��� ����� ���� �Ѵ�. �ﰢ�� �ϳ����� �ٷ�� ������ ������ ���� ���ۿ� �ε��� ���ۿ� ���� �����ϴ� �ϸ��� �Ѵ�.
bool CModelClass::InitializeBuffers(ID3D11Device* _device)
{
	/*
		������ �ε��� �����͸� ��Ƶ� �ΰ��� �ӽ� �迭�� ����� ���߿� ���� ���۸� �����Ҷ� ����ϵ��� �Ѵ�.
	*/
	/* Tutorial 4 ~ 6
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
	*/
	
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
//  Tutorial 4 - 6
// 	D3DXVECTOR3 LeftBottom = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
// 	D3DXVECTOR3 LeftTop = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
// 	D3DXVECTOR3 TopCenter = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
// 	D3DXVECTOR3 RightTop = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
// 	D3DXVECTOR3 RightBottom = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
// 	D3DXVECTOR3 Center = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
// 
// 	if(m_nVertexCount == 3)
// 	{
// 		// �ﰢ��
// 		_vertices[0].position = LeftBottom;
// 		_vertices[1].position = TopCenter;
// 		_vertices[2].position =  RightBottom ;
// 	}
// 	else if(m_nVertexCount == 6)
// 	{
// 		// �簢��
// 		_vertices[0].position = LeftBottom;
// 		_vertices[1].position = LeftTop;
// 		_vertices[2].position = RightTop;
// 		_vertices[3].position = LeftBottom;
// 		_vertices[4].position = RightTop;
// 		_vertices[5].position = RightBottom;
// 	}
// 
// 
// 
// 	//T4
// // 	D3DXVECTOR4 greenColor = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
// // 	D3DXVECTOR4 RedColor = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
// // 
// // 	// ���� ���
// // 	float Bright = 0.5f;
// // 
// // 
// // 	for(int i=0; i<m_nIndexCount; i++)
// // 	{
// // 		_vertices[i].color = greenColor * Bright;
// // 	}
// 		
// 	// T5
// 	D3DXVECTOR2 leftTopUV = D3DXVECTOR2(0.0f, 0.0f);
// 	D3DXVECTOR2 TopCenterUV = D3DXVECTOR2(0.5f, 0.0f);
// 	D3DXVECTOR2 rightTopUV = D3DXVECTOR2(1.0f, 0.0f);
// 	D3DXVECTOR2 leftCenterUV = D3DXVECTOR2(0.0f, 0.5f);
// 	D3DXVECTOR2 CenterUV = D3DXVECTOR2(0.5f, 0.5f);
// 	D3DXVECTOR2 rightCenterUV = D3DXVECTOR2(1.0f, 0.5f);
// 	D3DXVECTOR2 leftBottomUV = D3DXVECTOR2(0.0f, 1.0f);
// 	D3DXVECTOR2 CenterBottomUV = D3DXVECTOR2(0.5f, 1.0f);
// 	D3DXVECTOR2 rightBottomUV = D3DXVECTOR2(1.0f, 1.0f);
// 
// 	if(m_nVertexCount == 3)
// 	{
// 		_vertices[0].texture =  D3DXVECTOR2(0.0f, 1.0f);
// 		_vertices[1].texture = D3DXVECTOR2(0.5f, 0.0f);
// 		_vertices[2].texture =D3DXVECTOR2(1.0f, 1.0f);
// 	}
// 	else
// 	{
// 		_vertices[0].texture = leftBottomUV;
// 		_vertices[1].texture = leftTopUV;
// 		_vertices[2].texture = rightTopUV;
// 		_vertices[3].texture = leftBottomUV;
// 		_vertices[4].texture = rightTopUV;
// 		_vertices[5].texture = rightBottomUV;
// 	}
// 
// 	//////////////////////////////////////////////////////////////////////////
// 
// 	for(int i=0; i<m_nIndexCount; i++)
// 	{
// 		// T6
// 		/*
// 			�� ������ ���� ����� ���Ͽ� ������ ������ ������ �ȴ�.
// 			������ ������ ǥ�鿡 �����ϴ� ���̱� ������ ǥ���� �ٶ󺸰� �ִ� ������ ��Ȯ�� ����� �� �ֵ�.
// 			�ڵ��� �������� �����ϱ� ���� ������ ��û�ڿ��Է� ���ϵ��� �� ������ ������ Z������ -1.0f�� �Ͽ���
// 		*/
// 		_vertices[i].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
// 		// T6 End
// 				
// 		_indices[i] = i;
// 	}


	/*
		�����迭�� �ε��� �迭 ä��, �̸� �̿��Ͽ� ���� ���ۿ� �ε��� ���۸� ����.
		���ۿ� ���� description �ۼ�. description���� ByteWidth(����ũ��)�� BindFlags(����Ÿ��)�� ��Ȯ�� �Է��ؾ� �Ѵ�.
		���� �迭�� �ε��� �迭�� subresource�����Ϳ� �����Ѵ�.
		descrition�� subresource ������, �׸��� d3d ����̽��� createbuffer �Լ��� ����Ͽ� �� ������ ������ �޾� �´�.
	*/


	// Load the vertex array and index array with data.


	if(m_nTutorialType < 20)
	{
		for(int i=0; i<m_nVertexCount; i++)
		{
			_vertices[i].position = D3DXVECTOR3(m_pModelType[i].location[0], m_pModelType[i].location[1], m_pModelType[i].location[2]);
			_vertices[i].texture = D3DXVECTOR2(m_pModelType[i].texture[0], m_pModelType[i].texture[1]);
		//	_vertices[i].normal = D3DXVECTOR3(m_pModelType[i].normalvetor[0], m_pModelType[i].normalvetor[1], m_pModelType[i].normalvetor[2]);

			_indices[i] = i;
		}
	}
	else if(m_nTutorialType == 20 || m_nTutorialType == 21)
	{
		for(int i=0; i<m_nVertexCount; i++)
		{
			_vertices[i].position = D3DXVECTOR3(m_pModelType[i].location[0], m_pModelType[i].location[1], m_pModelType[i].location[2]);
			_vertices[i].texture = D3DXVECTOR2(m_pModelType[i].texture[0], m_pModelType[i].texture[1]);
			_vertices[i].normal = D3DXVECTOR3(m_pModelType[i].normalvetor[0], m_pModelType[i].normalvetor[1], m_pModelType[i].normalvetor[2]);
			_vertices[i].tangent = D3DXVECTOR3(m_pModelType[i].tangentvetor[0], m_pModelType[i].tangentvetor[1], m_pModelType[i].tangentvetor[2]);
			_vertices[i].binormal = D3DXVECTOR3(m_pModelType[i].binormalvetor[0], m_pModelType[i].binormalvetor[1], m_pModelType[i].binormalvetor[2]);

			_indices[i] = i;
		}
	}

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
bool CModelClass::LoadTexture(ID3D11Device* _device, WCHAR* fileList[])
{
	m_pTextureArray = new CTexturearray;
	if(!m_pTextureArray)
		return false;

	if(!m_pTextureArray->Initialize(_device, fileList))
		return false;

	return true;
}

void CModelClass::ReleaseTexture()
{
	if(m_pTextureArray)
	{
		m_pTextureArray->Shutdown();
		delete m_pTextureArray;
		m_pTextureArray = 0;
	}

	if(m_pTexture)
	{
		m_pTexture->Shutdown();
		delete m_pTexture;
		m_pTexture = 0;
	}
	return;
}

bool CModelClass::LoadModel(char* _fileName)
{
	// open the model file
	ifstream fileIn;
	fileIn.open(_fileName);

	if(fileIn.fail())
	{
		return false;
	}

	char input;
	fileIn.get(input);
	while(input != ':')
	{
		fileIn.get(input);
	}

	fileIn >> m_nVertexCount;

	m_nIndexCount = m_nVertexCount;

	m_pModelType = new ModelType[m_nVertexCount];
	if(!m_pModelType)
	{
		return false;
	}

	fileIn.get(input);
	while(input != ':')
	{
		fileIn.get(input);
	}

	fileIn.get(input);
	fileIn.get(input);

	for(int i=0; i<m_nVertexCount; i++)
	{
		fileIn >> m_pModelType[i].location[0] >> m_pModelType[i].location[1] >> m_pModelType[i].location[2];
		fileIn >> m_pModelType[i].texture[0] >> m_pModelType[i].texture[1];
		fileIn >> m_pModelType[i].normalvetor[0] >> m_pModelType[i].normalvetor[1] >> m_pModelType[i].normalvetor[2];
	}

	fileIn.close();

	return true;
}

void CModelClass::ReleaseModel()
{
	if(m_pModelType)
	{
		delete [] m_pModelType;
		m_pModelType = 0;
	}

	return;
}

void CModelClass::CalculateModelVectors()
{
	// Calculate the number of faces in the model.
	int faceCount = m_nVertexCount / 3;

	// Initialize the index to the model data.
	int index = 0;

	// Go through all the faces and calculate the tangent, binormal, and normal vectors.
	for(int i=0; i<faceCount; i++)
	{
		TempVertexType v1, v2, v3;
		VectorType tangent, binormal, normal;

		// Get the three vetices for this face from the model.
		SetData(v1, index++);
		SetData(v2, index++);
		SetData(v3, index++);

		// Calculate the tangent and binormal of that face.
		CalculateTangentBinormal(v1, v2, v3, tangent, binormal);

		// Calculate the new normal using the tangent and binormal.
		CalculateNormal(tangent, binormal, normal);

		// Store the normal, tangent, and binormal for this face back in the model structure.
		SetData(index - 1, normal, tangent, binormal);
		SetData(index - 2, normal, tangent, binormal);
		SetData(index - 3, normal, tangent, binormal);

	}
}

void CModelClass::SetData(TempVertexType &vertex, int allidx)
{
	for(int loop = 0; loop <3; loop++)
	{
		vertex.location[loop] = m_pModelType[allidx].location[loop];
		vertex.normalvetor[loop] = m_pModelType[allidx].normalvetor[loop];
	}
	for(int loop2 = 0; loop2 < 2; loop2++)
	{
		vertex.texture[loop2] = m_pModelType[allidx].texture[loop2];
	}
}
void CModelClass::SetData(int allidx, VectorType normal, VectorType tangent, VectorType binormal)
{
	for(int i = 0; i< 3; i++)
	{
		m_pModelType[allidx].normalvetor[i] = normal.location[i];
		m_pModelType[allidx].tangentvetor[i] = tangent.location[i];
		m_pModelType[allidx].binormalvetor[i] = binormal.location[i];
	}
}

void CModelClass::CalculateTangentBinormal(TempVertexType _TempVT1, TempVertexType _TempVT2, TempVertexType _TempVT3, VectorType& tangent, VectorType& binormal)
{
	float vector1[3], vector2[3], tuVector[2], tvVector[2];

	for(int i=0; i<3; i++)
	{
		// Calculate the two vectors for this face.
		vector1[i] = _TempVT2.location[i] - _TempVT1.location[i];
		vector2[i] = _TempVT3.location[i] - _TempVT1.location[i];
	}

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = _TempVT2.texture[0] - _TempVT1.texture[0];
	tvVector[0] = _TempVT2.texture[1] - _TempVT1.texture[1];

	tuVector[1] = _TempVT3.texture[0] - _TempVT1.texture[0];	
	tvVector[1] = _TempVT3.texture[1] - _TempVT1.texture[1];

	// Calculate the denominator of the tangent / binormal equation.
	float den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.location[0] = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.location[1] = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.location[2] = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.location[0] = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.location[1] = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.location[2] = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	float len = sqrt((tangent.location[0] * tangent.location[0]) + (tangent.location[1] * tangent.location[1]) + (tangent.location[2] * tangent.location[2]));

	// Normalize the normal and then store it
	for(int i=0; i<3; i++)
	{
		tangent.location[i] = tangent.location[i] / len;
	}

	// Calculate the length of this normal.

	len = sqrt((binormal.location[0] * binormal.location[0]) + (binormal.location[1] * binormal.location[1]) + (binormal.location[2] * binormal.location[2]));

	// Normalize the normal and then store it.
	for(int i=0; i<3; i++)
		binormal.location[i] = binormal.location[i] / len;

}

void CModelClass::CalculateNormal(VectorType tangent, VectorType binormal, VectorType& normal)
{
	// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal.location[0] = (tangent.location[1] * binormal.location[2]) - (tangent.location[2] * binormal.location[1]);
	normal.location[1] = (tangent.location[2] * binormal.location[0]) - (tangent.location[0] * binormal.location[2]);
	normal.location[2] = (tangent.location[0] * binormal.location[1]) - (tangent.location[1] * binormal.location[0]);

	// Calculate the length of the normal.
	float len = sqrt((normal.location[0] * normal.location[0]) + (normal.location[1] * normal.location[1]) + (normal.location[2] * normal.location[2]));

	// Normalize the normal.
	normal.location[0] = normal.location[0] / len;
	normal.location[1] = normal.location[1] / len;
	normal.location[2] = normal.location[2] / len;
}