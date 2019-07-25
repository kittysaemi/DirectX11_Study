#include "ColorShaderClass.h"


CColorShaderClass::CColorShaderClass(void)
{
	m_pVetexShader = 0;
	m_pPixelShader = 0;
	m_pLayout = 0;
	m_pBuffer = 0;
}

CColorShaderClass::CColorShaderClass(const CColorShaderClass& other)
{

}

CColorShaderClass::~CColorShaderClass(void)
{
}

bool CColorShaderClass::Initialize(ID3D11Device * _device, HWND _hWnd)
{
	if(!InitializeShader(_device, _hWnd, L"../DirectX11_Engine/HLSL/Color.vs", L"../DirectX11_Engine/HLSL/Color.ps"))
	{
		OutputDebugStringA("Faild, ColorShaderClass InitializeShader");
		return false;
	}

	return true;
}

void CColorShaderClass::Shutdown()
{
	// ���� ���̴��� �ȼ� ���̴��� �׿� ���õ� �͵��� ��ȯ.
	ShutdownShader();

	return;
}

bool CColorShaderClass::Render(ID3D11DeviceContext* _devContext, int _idxCount, D3DXMATRIX _world, D3DXMATRIX _view, D3DXMATRIX _projection)
{
	// �������� ��� �� ���̴��� ���ڸ� �Է��Ͽ� ����
	if(!SetShaderParameters(_devContext, _world, _view, _projection))
	{
		OutputDebugStringA("Faild, SetShaderParameters fun call");
		return false;
	}

	// ���̴��� �̿��Ͽ� �غ�� ���۸� �׸���.
	// HLSL ���̴��� �̿��� ��� �ﰢ�� �׸��� ����
	RenderShader(_devContext, _idxCount);

	return true;


}

bool CColorShaderClass::InitializeShader(ID3D11Device* _device, HWND _hWnd, WCHAR* _vsFileName, WCHAR* _psFileName)
{
	// ���� ������ ������
	ID3D10Blob* _vertexSBuffer = 0; 
	ID3D10Blob* _errMessage = 0; 
	HRESULT hResult = D3DX11CompileFromFile(_vsFileName, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &_vertexSBuffer, &_errMessage, NULL);

	if(FAILED(hResult))
	{
		// ���̴��� �����Ͽ� �����ϸ�, ���� �޼��� ���
		if(_errMessage)
			OutputShaderErrorMesage(_errMessage, _hWnd, _vsFileName);
		else
			OutputDebugStringA("Missing Vertex Shader File");

		return false;
	}

	// �ȼ� ���̴� ������
	ID3D10Blob* _pixelSBuffer = 0; 
	hResult = D3DX11CompileFromFile(_psFileName, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &_pixelSBuffer, &_errMessage, NULL);

	if(FAILED(hResult))
	{
		if(_errMessage)
			OutputShaderErrorMesage(_errMessage, _hWnd, _psFileName);
		else
			OutputDebugStringA("Missing Pixel Shader File");

		return false;
	}

	// ���۷κ��� ���� ���̴��� ����
	hResult = _device->CreateVertexShader(_vertexSBuffer->GetBufferPointer(), _vertexSBuffer->GetBufferSize(), NULL, &m_pVetexShader);
	if(FAILED(hResult))
	{
		OutputDebugStringA("Faild, CreateVertexShader");
		return false;
	}

	// ���۷κ��� �ȼ� ���̴��� ����
	hResult = _device->CreatePixelShader(_pixelSBuffer->GetBufferPointer(), _pixelSBuffer->GetBufferSize(), NULL, &m_pPixelShader);
	if(FAILED(hResult))
	{
		OutputDebugStringA("Faild, CreatePixelShader");
		return false;
	}

	/*
		���̴����� ����� ���� �������� ���̾ƿ� ����
		�� ���̴������� ��ġ ���Ϳ� ���� ���͸� ����ϹǷ� ���̾ƿ��� ������ ������ ũ�⸦ �����ϴ� �� ���̾ƿ��� �����.
		SemanticName�� �� ��Ұ� ���̾ƿ����� ��� ���Ǵ��� �˷��ֹǷ� ���̾ƿ����� ���� ���� ä������ �� �׸��̴�.
		POSITION�� ����, COLOR�� �ι�°�� ó���Ѵ�.
		���̾ƿ����� �߿��� �κ��� Format�̴�.
		��ġ ���Ϳ� ���ؼ��� DXGI_FORMAT_R32G32B32_FLOAT�� ����ϰ� ���� ���Ϳ� ���ؼ��� DXGI_FORMAT_R32G32B32A32_FLOAT�� ����Ѵ�.
		���ۿ� �����Ͱ� ��� �迭�Ǵ��� �˷��ִ� AlignedByteOffset �̴�.
		�� ���̾ƿ������� ó�� 12byte�� ��ġ ���Ϳ� ����ϰ� ���� 16byte�� �������� ����� ������ �˷���� �ϴµ�, AlignedByteOffset�� �� ��Ұ� ��� �����ϴ��� �����ش�. ���⼭ ���� ���� �Է��ϱ⺸�� D3D11_APPEND_ALIGNED_ELEMENT�� �����Ͽ� �ڵ����� �˾Ƴ����� �Ѵ�
	*/

	 // ���� �Է� ���̾ƿ� description�� �ۼ��Ѵ�.
	// �� ������ ModelClass�� ���̴��� �ִ� VertexType�� ��ġ�ؾ� �Ѵ�.
	D3D11_INPUT_ELEMENT_DESC _polygonLayout[2];
	_polygonLayout[0].SemanticName = "POSITION";
	_polygonLayout[0].SemanticIndex = 0;
	_polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	_polygonLayout[0].InputSlot = 0;
	_polygonLayout[0].AlignedByteOffset = 0;
	_polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	_polygonLayout[0].InstanceDataStepRate = 0;

	_polygonLayout[1].SemanticName = "COLOR";
	_polygonLayout[1].SemanticIndex  = 0;
	_polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	_polygonLayout[1].InputSlot = 0;
	_polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	_polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	_polygonLayout[1].InstanceDataStepRate = 0;

	/*
		D3D ����̽��� ����Ͽ� �Է� ���̾ƿ� ���� / �������� �Ҵ� ����
	*/

	// ���̾ƿ��� ��� ����
	unsigned int _nElements = sizeof(_polygonLayout) / sizeof(_polygonLayout[0]);

	// ���� �Է� ���̾ƿ� ����
	hResult = _device->CreateInputLayout(_polygonLayout, _nElements, _vertexSBuffer->GetBufferPointer(), _vertexSBuffer->GetBufferSize(), &m_pLayout);
	if(FAILED(hResult))
	{
		OutputDebugStringA("Faild, CreateInputLayout");
		return false;
	}

	_vertexSBuffer->Release();
	_vertexSBuffer = 0;

	_pixelSBuffer->Release();
	_pixelSBuffer = 0;

	/*
		��� ����(constant buffer)
		�� �����Ӹ��� ��� ���۸� ������Ʈ�ϱ� ������ ������ ����� ������ �� �ʿ䰡 �ִ�.
		BindFlags�� ��� ���۸� �� ���۷� �Ѵٴ� ���� �����Ѵ�.
		CPUAccessFlags�� �뵵�� ���߾�� ������, D3D11_CPU_ACCESS_WRITE�� �����Ѵ�.
	*/

	// ���� ���̴��� �ִ� ��� ��� ������ description�� �ۼ�
	D3D11_BUFFER_DESC _matrixBufferDesc; 
	_matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	_matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	_matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	_matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	_matrixBufferDesc.MiscFlags = 0;
	_matrixBufferDesc.StructureByteStride = 0;

	// ��� ���� �����͸� ����� �� Ŭ�������� ���� ���̴� ��� ���ۿ� �����Ҽ� �ְ� �Ѵ�.
	hResult = _device->CreateBuffer(&_matrixBufferDesc, NULL, &m_pBuffer);
	if(FAILED(hResult))
	{
		OutputDebugStringA("Faild, ColorShaderClass Create MtrixBuffer");
		return false;
	}

	return true;
}

void CColorShaderClass::ShutdownShader()
{
	if(m_pBuffer)
	{
		m_pBuffer->Release();
		m_pBuffer = 0;
	}

	if(m_pLayout)
	{
		m_pLayout->Release();
		m_pLayout = 0;
	}

	if(m_pPixelShader)
	{
		m_pPixelShader->Release();
		m_pPixelShader = 0;
	}

	if(m_pVetexShader)
	{
		m_pVetexShader->Release();
		m_pVetexShader = 0;
	}

	return ;
}

void CColorShaderClass::OutputShaderErrorMesage(ID3D10Blob* _errMsg, HWND _hWnd, WCHAR* sdFileName)
{
	// ErrMessage�� ��� �ִ� ���ڿ� ������ �����͸� �����´�.
	char * _ComplieErr = (char*)(_errMsg->GetBufferPointer());

	// ErrMessage Len
	unsigned long buffersize = _errMsg->GetBufferSize();

	// file open / Msg Writer
	ofstream fileOut;
	fileOut.open("shader-error.txt");
	for(unsigned long i =0; i<buffersize; i++)
	{
		fileOut << _ComplieErr[i];
	}

	fileOut.close();

	_errMsg->Release();
	_errMsg = 0;

	OutputDebugStringA("Error compiling shader. Check Shader-error.txt for message");

	return;
}

bool CColorShaderClass::SetShaderParameters(ID3D11DeviceContext* _devContext, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection )
{
	// ����� transpose�Ͽ� ���̴����� ����� �� �ְ� �Ѵ�.
	D3DXMatrixTranspose(&world, &world);
	D3DXMatrixTranspose(&view, &view);
	D3DXMatrixTranspose(&projection, &projection);

	// ��� ���� Lock
	 D3D11_MAPPED_SUBRESOURCE _mapResource; 
	HRESULT hResult = _devContext->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &_mapResource);
	if(FAILED(hResult))
	{
		OutputDebugStringA("Faild, SetShaderParameters fun, MAP()");
		return false;
	}

	// ��� ������ �����Ϳ� ���� ������ �����´�.
	 MatrixBufferType* _pMBtypePointor = (MatrixBufferType*)_mapResource.pData;

	 // ��� ���ۿ� ����� �����Ѵ�.
	 _pMBtypePointor->world = world;
	 _pMBtypePointor->view = view;
	 _pMBtypePointor->projection = projection;

	 // ��� ���� Lock ����
	 _devContext->Unmap(m_pBuffer, 0);

	 // ���� ���̴������� ��� ������ ��ġ�� �����Ѵ�.
	 unsigned int _nBufferNumber = 0;
	 _devContext->VSSetConstantBuffers(_nBufferNumber, 1, &m_pBuffer);

	 return true;
}

/*
	SetShaderParameters�Լ����� ���� ȣ��Ǿ� ���̴��� ���ڵ��� �ùٸ��� �����Ѵ�.
	�Է� ������� �Է� ���̾ƿ� ����.
	GPU ���� ������ �ڷᱸ���� �˰� �ȴ�.
	���� ���۸� �׸��� ���� ���� ���̴��� �ȼ� ���̴� ����.
	���̴��� �����Ǹ� D3D ����̽� ���ؽ�Ʈ���� DirectX 11�� DrawIndexed�Լ��� ����Ͽ� �ﰢ���� �׷�����.
	�� �Լ��� ȣ�� �� ���Ŀ� �ʷϻ� �ﰢ���� �׷�����.
*/
void CColorShaderClass::RenderShader(ID3D11DeviceContext* _devContext, int _idxCount)
{
	// ���� �Է� ���̾ƿ� ����
	_devContext->IASetInputLayout(m_pLayout);

	// �ﰢ���� �׸� ���� ���̴��� �ȼ� ���̴� ����
	_devContext->VSSetShader(m_pVetexShader, NULL, 0);
	_devContext->PSSetShader(m_pPixelShader, NULL, 0);

	// �ﰢ�� �׸���
	_devContext->DrawIndexed(_idxCount, 0, 0);

	return;
}