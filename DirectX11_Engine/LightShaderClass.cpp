//////////////////////////////////////////////////////////////////////////
// tutorial 6
// 조명, 모델에 빛이 비치는 것을 다룸.

#include "LightshaderClass.h"

CLightshaderClass::CLightshaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_sampleState = 0;
	m_mBuffer = 0;
	m_lBuffer = 0;
	m_cBuffer = 0;
}
CLightshaderClass::CLightshaderClass(const CLightshaderClass& other)
{

}
CLightshaderClass::~CLightshaderClass()
{

}

bool CLightshaderClass::Initialize(ID3D11Device* _device, HWND hWnd, int _TutorialNum)
{
	TutorialNum = _TutorialNum;

	if(_TutorialNum <= 8)
	{
		if(!InitializeShader(_device, hWnd, L"../DirectX11_Engine/HLSL/Light.vs", L"../DirectX11_Engine/HLSL/Light.ps"))
			return false;
	}
	else if(_TutorialNum ==9 )
	{
		if(!InitializeShader(_device, hWnd, L"../DirectX11_Engine/HLSL/Light.vs", L"../DirectX11_Engine/HLSL/Light-T9.ps"))
		{
			return false;
		}
	}	
	else if(_TutorialNum >= 10 && _TutorialNum <= 15)
	{
		if(!InitializeShader(_device, hWnd, L"../DirectX11_Engine/HLSL/Light-T10.vs", L"../DirectX11_Engine/HLSL/Light-T10.ps"))
		{
			return false;
		}
	}
	else if(_TutorialNum == 16)
	{
		if(!InitializeShader(_device, hWnd, L"../DirectX11_Engine/HLSL/Light-T10.vs", L"../DirectX11_Engine/HLSL/Light-T10.ps"))
			return false;
	}

	return true;
}

void CLightshaderClass::Shutdown()
{
	ShutdownShader();
	return;
}

bool CLightshaderClass::Render(ID3D11DeviceContext* _devContext, int idxCount, D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProjection, ID3D11ShaderResourceView *_resourceView, D3DXVECTOR3 _lightDesc, D3DXVECTOR4 _DiffuseColor)
{
	if(!SetShaderParameters(_devContext, mWorld, mView, mProjection, _resourceView, _lightDesc, _DiffuseColor))
	{
		return false;
	}

	RenderShader(_devContext, idxCount);

	return true;
}
bool CLightshaderClass::Render(ID3D11DeviceContext* _devContext, int idxCount, D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProjection, ID3D11ShaderResourceView *_resourceView, D3DXVECTOR3 _lightDesc, D3DXVECTOR4 _DiffuseColor, D3DXVECTOR4 _ambientColor)
{
	if(!SetShaderParameters(_devContext, mWorld, mView, mProjection, _resourceView, _lightDesc, _DiffuseColor, _ambientColor))
	{
		return false;
	}

	RenderShader(_devContext, idxCount);

	return true;
}
bool CLightshaderClass::Render(ID3D11DeviceContext* _devContext, int idxCount, D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProjection, ID3D11ShaderResourceView *_resourceView, D3DXVECTOR3 _lightDesc, D3DXVECTOR4 _DiffuseColor, D3DXVECTOR4 _ambientColor, D3DXVECTOR3 _camPosition, D3DXVECTOR4 _specColor, float _specPower)
{
	if(!SetShaderParameters(_devContext, mWorld, mView, mProjection, _resourceView, _lightDesc, _DiffuseColor, _ambientColor, _camPosition, _specColor, _specPower))
	{
		return false;
	}

	RenderShader(_devContext, idxCount);

	return true;
}

bool CLightshaderClass::InitializeShader(ID3D11Device* _device, HWND _hWnd, WCHAR* _vsFileName, WCHAR* _psFileName)
{
	// Compile the vertex shader code.
	ID3D10Blob* _errMsg = 0;
	ID3D10Blob* _verShaderBuf = 0;
	HRESULT hResult = D3DX11CompileFromFile(_vsFileName, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &_verShaderBuf, &_errMsg, NULL);
	if(FAILED(hResult))
	{
		if(_errMsg)
			OutputShaderErrorMessage(_errMsg, _hWnd, _vsFileName);
		else
			OutputDebugStringA("Missing VS file");
		return false;
	}

	// Compile the pixel shader code.
	ID3D10Blob* _pixelShaderBuf = 0;
	hResult = D3DX11CompileFromFile(_psFileName, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &_pixelShaderBuf, &_errMsg, NULL);
	if(FAILED(hResult))
	{
		if(_errMsg)
			OutputShaderErrorMessage(_errMsg, _hWnd, _psFileName);
		else
			OutputDebugStringA("Missing PS file");

		return false;
	}

	// Create the vertex shader from the buffer.
	hResult = _device->CreateVertexShader(_verShaderBuf->GetBufferPointer(), _verShaderBuf->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(hResult))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	hResult = _device->CreatePixelShader(_pixelShaderBuf->GetBufferPointer(), _pixelShaderBuf->GetBufferSize(), NULL, &m_pixelShader);
	if(FAILED(hResult))
		return false;

	// Create the vertex input layout description.
	// THis setup needs to match the VertexType stucture in the ModelClass and in the shader.
	D3D11_INPUT_ELEMENT_DESC _polygonLayout[3];
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

	// 조명효과를 위한 세번째 요소
	// NORMAL, 법선 벡터의 x,y,z 부동소수점 변수를 담을 수 있는 DXGI_FORMAT_R32G32B32_FLOAT 
	// 이 구조는 HLSL 정점 셰이더가 입력으로 발아들일 입력과 잘 맞아 떨어지게 됩니다.
	_polygonLayout[2].SemanticName = "NORMAL";
	_polygonLayout[2].SemanticIndex = 0;
	_polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	_polygonLayout[2].InputSlot = 0;
	_polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	_polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	_polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	unsigned int nElenum = sizeof(_polygonLayout) / sizeof(_polygonLayout[0]);

	// Create the vertex input layout.
	hResult = _device->CreateInputLayout(_polygonLayout, nElenum, _verShaderBuf->GetBufferPointer(), _verShaderBuf->GetBufferSize(), &m_layout);
	if(FAILED(hResult))
		return false;

	_verShaderBuf->Release();
	_verShaderBuf = 0;
	_pixelShaderBuf->Release();
	_pixelShaderBuf = 0;

	// Create a texture sampler state description.
	D3D11_SAMPLER_DESC _samplerDesc;
	_samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	_samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	_samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	_samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	_samplerDesc.MipLODBias = 0.0f;
	_samplerDesc.MaxAnisotropy = 1;
	_samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	_samplerDesc.BorderColor[0] = 0;
	_samplerDesc.BorderColor[1] = 0;
	_samplerDesc.BorderColor[2] = 0;
	_samplerDesc.BorderColor[3] = 0;
	_samplerDesc.MinLOD = 0;
	_samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	hResult = _device->CreateSamplerState(&_samplerDesc, &m_sampleState);
	if(FAILED(hResult))
		return false;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	D3D11_BUFFER_DESC matrixBuf;
	matrixBuf.Usage = D3D11_USAGE_DYNAMIC;
	matrixBuf.ByteWidth = sizeof(MatrixBufferType);
	matrixBuf.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBuf.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBuf.MiscFlags = 0;
	matrixBuf.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class
	hResult = _device->CreateBuffer(&matrixBuf, NULL, &m_mBuffer);
	if(FAILED(hResult))
		return false;

	// 카메라 버퍼의 description을 작성하고 버퍼를 생성. 정점 셰이더에 카메라의 위치를 정할 수 있게 해주는 인터페이스 제공
	D3D11_BUFFER_DESC camBufDesc;
	camBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	camBufDesc.ByteWidth = sizeof(CameraBufferType);
	camBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	camBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	camBufDesc.MiscFlags = 0;
	camBufDesc.StructureByteStride = 0;

	// Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	hResult = _device->CreateBuffer(&camBufDesc, NULL, &m_cBuffer);
	if(FAILED(hResult))
		return false;



	// 조명의 색상과 방향을 다루는 조명 상수 버퍼의 desc 작성
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	D3D11_BUFFER_DESC lightBuf;
	lightBuf.Usage = D3D11_USAGE_DYNAMIC;
	
	if(TutorialNum < 9 ) lightBuf.ByteWidth = sizeof(LightBufferType);
	else if(TutorialNum == 9) lightBuf.ByteWidth = sizeof(LightBufferType_9);
	else if(TutorialNum >= 10) lightBuf.ByteWidth = sizeof(LightBufferType_10);

	lightBuf.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBuf.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBuf.MiscFlags = 0;
	lightBuf.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class
	hResult = _device->CreateBuffer(&lightBuf, NULL, &m_lBuffer);
	if(FAILED(hResult))
		return false;

	return true;
}

void CLightshaderClass::ShutdownShader()
{
	if(m_cBuffer)
	{
		m_cBuffer->Release();
		m_cBuffer = 0;
	}

	if(m_lBuffer)
	{
		m_lBuffer->Release();
		m_lBuffer = 0;
	}

	if(m_mBuffer)
	{
		m_mBuffer->Release();
		m_mBuffer = 0;
	}

	if(m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	if(m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	if(m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	if(m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
}


void CLightshaderClass::OutputShaderErrorMessage(ID3D10Blob* _errMsg, HWND hWnd, WCHAR* _fileName)
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

bool CLightshaderClass::SetShaderParameters(ID3D11DeviceContext* _devContext, D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProjection, ID3D11ShaderResourceView *_resourceView, D3DXVECTOR3 _lightDesc, D3DXVECTOR4 _DiffuseColor )
{
	// Transpose the matrices to prepare them for the shader.
	D3DXMatrixTranspose(&mWorld, &mWorld);
	D3DXMatrixTranspose(&mView, &mView);
	D3DXMatrixTranspose(&mProjection, &mProjection);

	// Lock the constant buffer so it can be written to.
	D3D11_MAPPED_SUBRESOURCE _mapResource;
	HRESULT hResult = _devContext->Map(m_mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &_mapResource);
	if(FAILED(hResult))
		return false;

	// Get a pointer to the data in the constant buffer.
	MatrixBufferType* ptrMatrix = (MatrixBufferType*)_mapResource.pData;

	// Copy the matrices into the constant buffer.
	ptrMatrix->world = mWorld;
	ptrMatrix->view = mView;
	ptrMatrix->projection = mProjection;

	// UnLock the constant buffer.
	_devContext->Unmap(m_mBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	unsigned int bufNum = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	_devContext->VSSetConstantBuffers(bufNum, 1, &m_mBuffer);

	// Set Shader texture resource in the pixel shader.
	_devContext->PSSetShaderResources(0, 1, &_resourceView);

	// Set shader texture resource in the pixel shader.
	hResult = _devContext->Map(m_lBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &_mapResource);
	if(FAILED(hResult))
		return false;

	// Get a pointer to the data in the constant buffer.
	LightBufferType* ptrLight = (LightBufferType*)_mapResource.pData;

	// Copy the lighting variables into the constant buffer.
	ptrLight->diffuseColor = _DiffuseColor;
	ptrLight->lightDirection = _lightDesc;
	ptrLight->_padding = 0.0f;

	// Unlock the constant buffer.
	_devContext->Unmap(m_lBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufNum = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	_devContext->PSSetConstantBuffers(bufNum, 1, &m_lBuffer);

	return true;
	

}
bool CLightshaderClass::SetShaderParameters(ID3D11DeviceContext* _devContext, D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProjection, ID3D11ShaderResourceView *_resourceView, D3DXVECTOR3 _lightDesc, D3DXVECTOR4 _DiffuseColor, D3DXVECTOR4 _ambientColor )
{
	// Transpose the matrices to prepare them for the shader.
	D3DXMatrixTranspose(&mWorld, &mWorld);
	D3DXMatrixTranspose(&mView, &mView);
	D3DXMatrixTranspose(&mProjection, &mProjection);

	// Lock the constant buffer so it can be written to.
	D3D11_MAPPED_SUBRESOURCE _mapResource;
	HRESULT hResult = _devContext->Map(m_mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &_mapResource);
	if(FAILED(hResult))
		return false;

	// Get a pointer to the data in the constant buffer.
	MatrixBufferType* ptrMatrix = (MatrixBufferType*)_mapResource.pData;

	// Copy the matrices into the constant buffer.
	ptrMatrix->world = mWorld;
	ptrMatrix->view = mView;
	ptrMatrix->projection = mProjection;

	// UnLock the constant buffer.
	_devContext->Unmap(m_mBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	unsigned int bufNum = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	_devContext->VSSetConstantBuffers(bufNum, 1, &m_mBuffer);

	// Set Shader texture resource in the pixel shader.
	_devContext->PSSetShaderResources(0, 1, &_resourceView);

	// Set shader texture resource in the pixel shader.
	hResult = _devContext->Map(m_lBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &_mapResource);
	if(FAILED(hResult))
		return false;

	// Get a pointer to the data in the constant buffer.
	LightBufferType_9* ptrLight = (LightBufferType_9*)_mapResource.pData;

	// Copy the lighting variables into the constant buffer.
	ptrLight->ambientColor = _ambientColor;
	ptrLight->diffuseColor = _DiffuseColor;
	ptrLight->lightDirection = _lightDesc;
	ptrLight->_padding = 0.0f;

	// Unlock the constant buffer.
	_devContext->Unmap(m_lBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufNum = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	_devContext->PSSetConstantBuffers(bufNum, 1, &m_lBuffer);

	return true;


}
bool CLightshaderClass::SetShaderParameters(ID3D11DeviceContext* _devContext, D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProjection, ID3D11ShaderResourceView *_resourceView, D3DXVECTOR3 _lightDesc, D3DXVECTOR4 _DiffuseColor, D3DXVECTOR4 _ambientColor, D3DXVECTOR3 _camPosition, D3DXVECTOR4 _specColor, float _specPower )
{
	// Transpose the matrices to prepare them for the shader.
	D3DXMatrixTranspose(&mWorld, &mWorld);
	D3DXMatrixTranspose(&mView, &mView);
	D3DXMatrixTranspose(&mProjection, &mProjection);

	// Lock the constant buffer so it can be written to.
	D3D11_MAPPED_SUBRESOURCE _mapResource;
	HRESULT hResult = _devContext->Map(m_mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &_mapResource);
	if(FAILED(hResult))
		return false;

	// Get a pointer to the data in the constant buffer.
	MatrixBufferType* ptrMatrix = (MatrixBufferType*)_mapResource.pData;

	// Copy the matrices into the constant buffer.
	ptrMatrix->world = mWorld;
	ptrMatrix->view = mView;
	ptrMatrix->projection = mProjection;

	// UnLock the constant buffer.
	_devContext->Unmap(m_mBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	unsigned int bufNum = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	_devContext->VSSetConstantBuffers(bufNum, 1, &m_mBuffer);

	//카메라 위치값
	hResult = _devContext->Map(m_cBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &_mapResource);
	if(FAILED(hResult))
		return false;

	CameraBufferType* ptrCamBuf = (CameraBufferType*)_mapResource.pData;
	ptrCamBuf->cameraPosition = _camPosition;
	ptrCamBuf->padding = 0.0f;
	_devContext->Unmap(m_cBuffer, 0);

	bufNum = 1;
	_devContext->VSSetConstantBuffers(bufNum, 1, &m_cBuffer);


	// Set Shader texture resource in the pixel shader.
	_devContext->PSSetShaderResources(0, 1, &_resourceView);

	// Set shader texture resource in the pixel shader.
	hResult = _devContext->Map(m_lBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &_mapResource);
	if(FAILED(hResult))
		return false;

	// Get a pointer to the data in the constant buffer.
	LightBufferType_10* ptrLight = (LightBufferType_10*)_mapResource.pData;

	// Copy the lighting variables into the constant buffer.
	ptrLight->ambientColor = _ambientColor;
	ptrLight->diffuseColor = _DiffuseColor;
	ptrLight->lightDirection = _lightDesc;
	ptrLight->specularColor = _specColor;
	ptrLight->specularPower = _specPower;

	// Unlock the constant buffer.
	_devContext->Unmap(m_lBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufNum = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	_devContext->PSSetConstantBuffers(bufNum, 1, &m_lBuffer);

	return true;


}

void CLightshaderClass::RenderShader(ID3D11DeviceContext* _devContext, int idxCount)
{
	// Set the vertex input layout.
	_devContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	_devContext->VSSetShader(m_vertexShader, NULL, 0);
	_devContext->PSSetShader(m_pixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	_devContext->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangle.
	_devContext->DrawIndexed(idxCount, 0, 0);

	return ;
}
