#include "Textureshaderclass.h"


CTextureshaderclass::CTextureshaderclass(void)
{
	m_pVertexShader = 0;
	m_pPixelShader = 0;
	m_pInputLayout = 0;
	m_pMBuffer = 0;
	m_sampleState = 0;
}

CTextureshaderclass::CTextureshaderclass(const CTextureshaderclass& other)
{

}


CTextureshaderclass::~CTextureshaderclass(void)
{
}

bool CTextureshaderclass::Initialize(ID3D11Device* _device, HWND _hWnd)
{
	// Initialize the vertex and pixel shaders.
	if(!InitializeShader(_device, _hWnd, L"../DirectX11_Engine/HLSL/Texture.vs", L"../DirectX11_Engine/HLSL/Texture.ps"))
	{
		OutputDebugStringA("Textureshaderclass Initalize faild");
		return false;
	}

	return true;
}

void CTextureshaderclass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();
	return;
}

bool CTextureshaderclass::Render(ID3D11DeviceContext* _deviceContext, int idxCount, D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProjection, ID3D11ShaderResourceView* texture )
{

	if(!SetShaderParameters(_deviceContext, mWorld, mView, mProjection, texture))
	{
		return false;
	}

	RenderShader(_deviceContext, idxCount);

	return true;
}

bool CTextureshaderclass::InitializeShader(ID3D11Device* _device, HWND hWnd, WCHAR* vsFileName, WCHAR* psFileName)
{
	// compile the vertex shader code.
	ID3D10Blob* _errMsg = 0;
	ID3D10Blob* _vertexShaderBuf = 0;
	HRESULT hResult = D3DX11CompileFromFile(vsFileName, NULL, NULL, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &_vertexShaderBuf, &_errMsg, NULL);

	if(FAILED(hResult))
	{
		if(_errMsg)
			OutputShaderErrorMessage(_errMsg, hWnd, vsFileName);
		else
			OutputDebugStringA("Missing Shader file");

		return false;
	}

	// compile the pixel shader code.
	  ID3D10Blob* _pixelShaderBuf = 0;
	  hResult = D3DX11CompileFromFile(psFileName, NULL, NULL, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &_pixelShaderBuf, &_errMsg, NULL);
	  if(FAILED(hResult))
	  {
		  if(_errMsg)
			  OutputShaderErrorMessage(_errMsg, hWnd, psFileName);
		  else
			  OutputDebugStringA("Missing Shader file");
		  return false;
	  }

	  hResult = _device->CreateVertexShader(_vertexShaderBuf->GetBufferPointer(), _vertexShaderBuf->GetBufferSize(), NULL, &m_pVertexShader);
	  if(FAILED(hResult))
	  {
		  OutputDebugStringA("Textureshaderclass, Faild, CreateVertexShader");
		  return false;
	  }

	  hResult = _device->CreatePixelShader(_pixelShaderBuf->GetBufferPointer(), _pixelShaderBuf->GetBufferSize(), NULL, &m_pPixelShader);
	  if(FAILED(hResult))
	  {
		  OutputDebugStringA("Textureshaderclass, Faild CreatepixelShader");
		  return false;
	  }

	// Create the vertex input layout description.
	  // This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	  D3D11_INPUT_ELEMENT_DESC _polygonLayout[2];
	  _polygonLayout[0].SemanticName = "POSITION";
	  _polygonLayout[0].SemanticIndex = 0;
	  _polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	  _polygonLayout[0].InputSlot = 0;
	  _polygonLayout[0].AlignedByteOffset = 0;
	  _polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	  _polygonLayout[0].InstanceDataStepRate = 0;

	  _polygonLayout[1].SemanticName = "TEXCOORD";
	  _polygonLayout[1].SemanticIndex = 0;
	  _polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	  _polygonLayout[1].InputSlot = 0;
	  _polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	  _polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	  _polygonLayout[1].InstanceDataStepRate = 0;

	  // Geta count of the elements in the layout.
	  unsigned int _nElements = sizeof(_polygonLayout) / sizeof(_polygonLayout[0]);

	  // Create the vertex input layout.
	  hResult = _device->CreateInputLayout(_polygonLayout, _nElements, _vertexShaderBuf->GetBufferPointer(), _vertexShaderBuf->GetBufferSize(), &m_pInputLayout);
	  if(FAILED(hResult))
	  {
		  return false;
	  }

	  // Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	  _vertexShaderBuf->Release();
	  _vertexShaderBuf = 0;
	  _pixelShaderBuf->Release();
	  _pixelShaderBuf = 0;

	  // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	  D3D11_BUFFER_DESC _MBufferDesc;
	  _MBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	  _MBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	  _MBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	  _MBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	  _MBufferDesc.MiscFlags = 0;
	  _MBufferDesc.StructureByteStride = 0;

	  // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	  hResult = _device->CreateBuffer(&_MBufferDesc, NULL, &m_pMBuffer);
	  if(FAILED(hResult))
	  {
		  return false;
	  }

	  /*
		샘플러 desc에서 가장 중요한 요소는 Filter이다.
		Filter는 최종 도형 표면에서 텍스쳐의 어느 픽셀이 사용되거나 혼합될것인지 결정하는 방법을 정한다.
		D3D11_FILTER_MIN_MAG_MIP_LINEAR : 처리량 면에서는 좀 무겁지만 가장 좋은 결과를 보여준다.
		                                  이것은 축소, 확대, 밉레벨 샘플링 시에 선형 보간(linear interpolation)을 사용한다는 것을 알렺누다.
		AddressU와 AddressV는 좌표가 언제나 0.0f와 1.0f 사이에 있또록 해 주는 Wrap으로 설정됨.
		이 범위를 나가는 값은 0.0f와 1.0ㄹ 사이의 값으로 재조정 된다.
	  */
	  // Create a texture sampler state description.
	  D3D11_SAMPLER_DESC _sampleDesc;
	  _sampleDesc.Filter = D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT; //D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	  _sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	  _sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	  _sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	  _sampleDesc.MipLODBias = 0.0f;
	  _sampleDesc.MaxAnisotropy = 1;
	  _sampleDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	  _sampleDesc.BorderColor[0] = 0;
	  _sampleDesc.BorderColor[1] = 0;
	  _sampleDesc.BorderColor[2] = 0;
	  _sampleDesc.BorderColor[3] = 0;
	  _sampleDesc.MinLOD = 0;
	  _sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;

	  // Create the texture sampler state.
	  hResult = _device->CreateSamplerState(&_sampleDesc, &m_sampleState);
	  if(FAILED(hResult))
	  {
		  return false;
	  }

	  return true;
}

void CTextureshaderclass::ShutdownShader()
{
	// Release the Things.
	if(m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}
	if(m_pMBuffer)
	{
		m_pMBuffer->Release();
		m_pMBuffer = 0;
	}
	if(m_pInputLayout)
	{
		m_pInputLayout->Release();
		m_pInputLayout = 0;
	}
	if(m_pPixelShader)
	{
		m_pPixelShader->Release();
		m_pPixelShader = 0;
	}
	if(m_pVertexShader)
	{
		m_pVertexShader->Release();
		m_pVertexShader = 0;
	}

	return;
}

void CTextureshaderclass::OutputShaderErrorMessage(ID3D10Blob* _errMsg, HWND _hWnd, WCHAR* _fileName)
{
	// Get a pointer to the error message text buffer.
	char * strErr;
	strErr = (char *)(_errMsg->GetBufferPointer());

	// Get the length of the message.
	unsigned long bufSize = _errMsg->GetBufferSize();

	// Oepn a file to write the error message to.
	ofstream fileOut;
	fileOut.open("shader-error.txt");

	// Write out the error message.
	for(unsigned long i =0; i<bufSize; i++)
		fileOut << strErr[i];

	fileOut.close();

	_errMsg->Release();
	_errMsg = 0;

	
	return;
}

// 텍스쳐는 반드시 버퍼에 랜더링이 일어나기 전에 설정되어 있어야 한다.
bool CTextureshaderclass::SetShaderParameters(ID3D11DeviceContext* _deviceContext, D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProjection, ID3D11ShaderResourceView* textrue)
{
	// Transpose the matrices to prepare them for the shader.
	D3DXMatrixTranspose(&mWorld, &mWorld);
	D3DXMatrixTranspose(&mView, &mView);
	D3DXMatrixTranspose(&mProjection, &mProjection);

	// Lock the constant buffer so it can be writeen to.
	D3D11_MAPPED_SUBRESOURCE _mappedSubResource;
	HRESULT hResult = _deviceContext->Map(m_pMBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &_mappedSubResource);
	if(FAILED(hResult))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	MatrixBufferType* ptrData = (MatrixBufferType*)_mappedSubResource.pData;

	// Copy the matrices into the constant buffer.
	ptrData->world = mWorld;
	ptrData->view = mView;
	ptrData->projection = mProjection;

	// unLock the constant buffer.
	_deviceContext->Unmap(m_pMBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	unsigned int nbufNumber;
	nbufNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	_deviceContext->VSSetConstantBuffers(nbufNumber, 1, &m_pMBuffer);

	// Set shader texture resource in the pixel shader.
	_deviceContext->PSSetShaderResources(0, 1, &textrue);

	return true;
}

void CTextureshaderclass::RenderShader(ID3D11DeviceContext* _deviceContext, int _idxCount)
{
	// set the vertex input layout.
	_deviceContext->IASetInputLayout(m_pInputLayout);

	// set the vertex and pixel shaders that will be used to render this triangle.
	_deviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	_deviceContext->PSSetShader(m_pPixelShader, NULL , 0);

	// set the sampler state in the pixel shader.
	_deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangle.
	_deviceContext->DrawIndexed(_idxCount, 0, 0);

	return;
}