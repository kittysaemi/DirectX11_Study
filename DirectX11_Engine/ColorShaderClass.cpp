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
	// 정점 셰이더와 픽셀 셰이더및 그와 관련된 것들을 반환.
	ShutdownShader();

	return;
}

bool CColorShaderClass::Render(ID3D11DeviceContext* _devContext, int _idxCount, D3DXMATRIX _world, D3DXMATRIX _view, D3DXMATRIX _projection)
{
	// 렌더링에 사용 할 셰이더의 인자를 입력하여 설정
	if(!SetShaderParameters(_devContext, _world, _view, _projection))
	{
		OutputDebugStringA("Faild, SetShaderParameters fun call");
		return false;
	}

	// 셰이더를 이용하여 준비된 버퍼를 그린다.
	// HLSL 셰이더를 이용한 녹색 삼각형 그리기 수행
	RenderShader(_devContext, _idxCount);

	return true;


}

bool CColorShaderClass::InitializeShader(ID3D11Device* _device, HWND _hWnd, WCHAR* _vsFileName, WCHAR* _psFileName)
{
	// 정점 셰이터 컴파일
	ID3D10Blob* _vertexSBuffer = 0; 
	ID3D10Blob* _errMessage = 0; 
	HRESULT hResult = D3DX11CompileFromFile(_vsFileName, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &_vertexSBuffer, &_errMessage, NULL);

	if(FAILED(hResult))
	{
		// 셰이더가 컴파일에 실패하면, 에러 메세지 기록
		if(_errMessage)
			OutputShaderErrorMesage(_errMessage, _hWnd, _vsFileName);
		else
			OutputDebugStringA("Missing Vertex Shader File");

		return false;
	}

	// 픽셀 셰이더 컴파일
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

	// 버퍼로부터 정점 셰이더를 생성
	hResult = _device->CreateVertexShader(_vertexSBuffer->GetBufferPointer(), _vertexSBuffer->GetBufferSize(), NULL, &m_pVetexShader);
	if(FAILED(hResult))
	{
		OutputDebugStringA("Faild, CreateVertexShader");
		return false;
	}

	// 버퍼로부터 픽셀 셰이더를 생성
	hResult = _device->CreatePixelShader(_pixelSBuffer->GetBufferPointer(), _pixelSBuffer->GetBufferSize(), NULL, &m_pPixelShader);
	if(FAILED(hResult))
	{
		OutputDebugStringA("Faild, CreatePixelShader");
		return false;
	}

	/*
		셰이더에서 사용할 정점 데이터의 레이아웃 생성
		이 셰이더에서는 위치 벡터와 색상 벡터를 사용하므로 레이아웃에 각각의 벡터의 크기를 포함하는 두 레이아웃을 만든다.
		SemanticName은 이 요소가 레이아웃에서 어떻게 사용되는지 알려주므로 레이아웃에서 가장 먼저 채워져야 할 항목이다.
		POSITION을 먼저, COLOR를 두번째로 처리한다.
		레이아웃에서 중요한 부분은 Format이다.
		위치 벡터에 대해서는 DXGI_FORMAT_R32G32B32_FLOAT를 사용하고 색상 벡터에 대해서는 DXGI_FORMAT_R32G32B32A32_FLOAT를 사용한다.
		버퍼에 데이터가 어떻게 배열되는지 알려주는 AlignedByteOffset 이다.
		이 레이아웃에서는 처음 12byte를 위치 벡터에 사용하고 다음 16byte를 색상으로 사용할 것임을 알려줘야 하는데, AlignedByteOffset이 각 요소가 어디서 시작하느지 보여준다. 여기서 직접 값을 입력하기보다 D3D11_APPEND_ALIGNED_ELEMENT를 지정하여 자동으로 알아내도록 한다
	*/

	 // 정점 입력 레이아웃 description을 작성한다.
	// 이 설정은 ModelClass와 셰이더에 있는 VertexType과 일치해야 한다.
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
		D3D 디바이스를 사용하여 입력 레이아웃 생성 / 지역변수 할당 해제
	*/

	// 레이아웃의 요소 갯수
	unsigned int _nElements = sizeof(_polygonLayout) / sizeof(_polygonLayout[0]);

	// 정점 입력 레이아웃 생성
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
		상수 버퍼(constant buffer)
		매 프레임마다 상수 버퍼를 업데이트하기 떄문에 버퍼의 사용은 동적이 될 필요가 있다.
		BindFlags로 상수 버퍼를 이 버퍼로 한다는 것을 설정한다.
		CPUAccessFlags도 용도에 맞추어야 함으로, D3D11_CPU_ACCESS_WRITE로 설정한다.
	*/

	// 정점 셰이더에 있는 행렬 상수 버퍼의 description을 작성
	D3D11_BUFFER_DESC _matrixBufferDesc; 
	_matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	_matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	_matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	_matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	_matrixBufferDesc.MiscFlags = 0;
	_matrixBufferDesc.StructureByteStride = 0;

	// 상수 버퍼 포인터를 만들어 이 클래스에서 정점 셰이더 상수 버퍼에 접근할수 있게 한다.
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
	// ErrMessage를 담고 있는 문자열 버퍼의 포인터를 가져온다.
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
	// 행렬을 transpose하여 셰이더에서 사용할 수 있게 한다.
	D3DXMatrixTranspose(&world, &world);
	D3DXMatrixTranspose(&view, &view);
	D3DXMatrixTranspose(&projection, &projection);

	// 상수 버퍼 Lock
	 D3D11_MAPPED_SUBRESOURCE _mapResource; 
	HRESULT hResult = _devContext->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &_mapResource);
	if(FAILED(hResult))
	{
		OutputDebugStringA("Faild, SetShaderParameters fun, MAP()");
		return false;
	}

	// 상수 버퍼의 데이터에 대한 포인터 가져온다.
	 MatrixBufferType* _pMBtypePointor = (MatrixBufferType*)_mapResource.pData;

	 // 상수 버퍼에 행렬을 복사한다.
	 _pMBtypePointor->world = world;
	 _pMBtypePointor->view = view;
	 _pMBtypePointor->projection = projection;

	 // 상수 버퍼 Lock 해제
	 _devContext->Unmap(m_pBuffer, 0);

	 // 정점 셰이더에서의 상수 버퍼의 위치를 설정한다.
	 unsigned int _nBufferNumber = 0;
	 _devContext->VSSetConstantBuffers(_nBufferNumber, 1, &m_pBuffer);

	 return true;
}

/*
	SetShaderParameters함수보다 먼저 호출되어 셰이더의 인자들을 올바르게 셋팅한다.
	입력 어셈블러에 입력 레이아웃 연결.
	GPU 정점 버퍼의 자료구조를 알게 된다.
	정점 버퍼를 그리기 위한 정점 셰이더와 픽셀 셰이더 설정.
	셰이더가 설정되면 D3D 디바이스 컨텍스트에서 DirectX 11의 DrawIndexed함수를 사용하여 삼각형을 그려낸다.
	이 함수가 호출 된 이후에 초록색 삼각형이 그려진다.
*/
void CColorShaderClass::RenderShader(ID3D11DeviceContext* _devContext, int _idxCount)
{
	// 정점 입력 레이아웃 설정
	_devContext->IASetInputLayout(m_pLayout);

	// 삼각형을 그릴 정점 셰이더와 픽셀 셰이더 설정
	_devContext->VSSetShader(m_pVetexShader, NULL, 0);
	_devContext->PSSetShader(m_pPixelShader, NULL, 0);

	// 삼각형 그리기
	_devContext->DrawIndexed(_idxCount, 0, 0);

	return;
}