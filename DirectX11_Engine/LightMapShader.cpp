#include "LightMapShader.h"


CLightMapShader::CLightMapShader(void)
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_inputLayout = 0;
	m_mBuffer = 0;
	m_sampleState = 0;
}

CLightMapShader::CLightMapShader(const CLightMapShader& other)
{

}

CLightMapShader::~CLightMapShader(void)
{
}

bool CLightMapShader::Initialize(ID3D11Device * _dev, HWND _hWnd)
{
	if(!InitializeShader(_dev, _hWnd, L"../DirectX11_Engine/HLSL/LightMap.vs", L"../DirectX11_Engine/HLSL/LightMap.ps"))
		return false;

	return true;
}

void CLightMapShader::Shutdown()
{
	ShutdownShader();
}

bool CLightMapShader::Render(ID3D11DeviceContext * _devCon, int idxCnt, D3DXMATRIX wMatrix, D3DXMATRIX vMatrix, D3DXMATRIX pMatrix, ID3D11ShaderResourceView ** textureArray)
{
	if(!SetShaderParameters(_devCon, wMatrix, vMatrix, pMatrix, textureArray))
		return false;

	RenderShader(_devCon, idxCnt);

	return true;
}

bool CLightMapShader::InitializeShader(ID3D11Device * _dev, HWND _hWnd, WCHAR* vsFileName, WCHAR* psFIleName)
{
	ID3D10Blob * errMsg;
	ID3D10Blob * vShaderBuf;
	HRESULT hResult = D3DX11CompileFromFile(vsFileName, NULL, NULL, "LightMapVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vShaderBuf, &errMsg, NULL);
	if(FAILED(hResult))
	{
		if(errMsg)
			OutputShaderErrorMessage(errMsg, _hWnd, vsFileName);
		else
			OutputDebugStringA("Missing vs Shader FIle");
		return false;
	}
	
	ID3D10Blob * pShaderBuf;
	hResult = D3DX11CompileFromFile(psFIleName, NULL, NULL, "LightMapPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pShaderBuf, &errMsg, NULL);
	if(FAILED(hResult))
	{
		if(errMsg)
			OutputShaderErrorMessage(errMsg, _hWnd, psFIleName);
		else
			OutputDebugStringA("Missing ps Shader FIle");

		return false;
	}

	hResult = _dev->CreateVertexShader(vShaderBuf->GetBufferPointer(), vShaderBuf->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(hResult))
		return false;

	hResult = _dev->CreatePixelShader(pShaderBuf->GetBufferPointer(), pShaderBuf->GetBufferSize(), NULL, &m_pixelShader);
	if(FAILED(hResult))
		return false;
	
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	unsigned int nElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	hResult = _dev->CreateInputLayout(polygonLayout, nElements, vShaderBuf->GetBufferPointer(), vShaderBuf->GetBufferSize(), &m_inputLayout);
	if(FAILED(hResult))
		return false;

	vShaderBuf->Release();
	vShaderBuf = 0;

	pShaderBuf->Release();
	pShaderBuf = 0;


	D3D11_BUFFER_DESC mBufDesc;
	mBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	mBufDesc.ByteWidth = sizeof(MatrixBufferType);
	mBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	mBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mBufDesc.MiscFlags = 0;
	mBufDesc.StructureByteStride = 0;

	hResult = _dev->CreateBuffer(&mBufDesc, NULL, &m_mBuffer);
	if(FAILED(hResult))
		return false;

	D3D11_SAMPLER_DESC SampleDesc;
	SampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SampleDesc.MipLODBias = 0.0f;
	SampleDesc.MaxAnisotropy = 1;
	SampleDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SampleDesc.BorderColor[0] = 0;
	SampleDesc.BorderColor[1] = 0;
	SampleDesc.BorderColor[2] = 0;
	SampleDesc.BorderColor[3] = 0;
	SampleDesc.MinLOD = 0;
	SampleDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hResult = _dev->CreateSamplerState(&SampleDesc, &m_sampleState);
	if(FAILED(hResult))
		return false;

	return true;
}

void CLightMapShader::ShutdownShader()
{
	if(m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	if(m_mBuffer)
	{
		m_mBuffer->Release();
		m_mBuffer = 0;
	}

	if(m_inputLayout)
	{
		m_inputLayout->Release();
		m_inputLayout = 0;
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

void CLightMapShader::OutputShaderErrorMessage(ID3D10Blob* _errMsg, HWND hWnd, WCHAR* errFileName)
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

}

bool CLightMapShader::SetShaderParameters(ID3D11DeviceContext * devCon, D3DXMATRIX wMatrix, D3DXMATRIX vMatrix, D3DXMATRIX pMatrix, ID3D11ShaderResourceView ** textureArray)
{
	D3DXMatrixTranspose(&wMatrix, &wMatrix);
	D3DXMatrixTranspose(&vMatrix, &vMatrix);
	D3DXMatrixTranspose(&pMatrix, &pMatrix);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hResult = devCon->Map(m_mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(hResult))
		return false;
	
	MatrixBufferType * ptrMBuf = (MatrixBufferType *)mappedResource.pData;

	ptrMBuf->world = wMatrix;
	ptrMBuf->view = vMatrix;
	ptrMBuf->projection = pMatrix;

	devCon->Unmap(m_mBuffer, 0);
	
	unsigned int nbufNum = 0;

	devCon->VSSetConstantBuffers(nbufNum, 1, &m_mBuffer);

	devCon->PSSetShaderResources(0, 2, textureArray);

	return true;
}

void CLightMapShader::RenderShader(ID3D11DeviceContext * devCon, int idxCnt)
{
	devCon->IASetInputLayout(m_inputLayout);

	devCon->VSSetShader(m_vertexShader, NULL, 0);
	devCon->PSSetShader(m_pixelShader, NULL, 0);

	devCon->PSSetSamplers(0, 1, &m_sampleState);

	devCon->DrawIndexed(idxCnt, 0, 0);
}