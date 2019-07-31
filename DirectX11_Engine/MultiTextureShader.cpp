#include "MultiTextureShader.h"


CMultiTextureShader::CMultiTextureShader(void)
{
	m_pVertextShader = 0;
	m_pPixelShader = 0;
	m_pInputLayout = 0;
	m_pBuffer = 0;
	m_pSamplerState = 0;
}

CMultiTextureShader::CMultiTextureShader(const CMultiTextureShader& other)
{

}

CMultiTextureShader::~CMultiTextureShader(void)
{
}

bool CMultiTextureShader::Initialize(ID3D11Device * _dev, HWND _hWnd)
{
	if(!InitializeShader(_dev, _hWnd, L"../DirectX11_Engine/HLSL/Multitexture.vs", L"../DirectX11_Engine/HLSL/Multitexture.ps"))
		return false;

	return true;
}

void CMultiTextureShader::Shutdown()
{
	ShutdownShader();
}

bool CMultiTextureShader::Render(ID3D11DeviceContext * _devContext, int idxCnt, D3DXMATRIX wMatrix, D3DXMATRIX vMatrix, D3DXMATRIX pMatrix, ID3D11ShaderResourceView ** textureArray)
{
	if(!SetShaderParameters(_devContext, wMatrix,vMatrix,pMatrix, textureArray))
		return false;

	RenderShader(_devContext, idxCnt);

	return true;
}

bool CMultiTextureShader::InitializeShader(ID3D11Device* _dev, HWND _hWnd, WCHAR* vsFileName, WCHAR * psFileName)
{
	ID3D10Blob * errMsgBuf;
	ID3D10Blob * vertexShaderbuf;
	HRESULT hResult = D3DX11CompileFromFile(vsFileName, NULL, NULL, "MultiTextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderbuf, &errMsgBuf, NULL);
	if(FAILED(hResult))
	{
		if(errMsgBuf)
			OutputShaderErrorMessage(errMsgBuf, _hWnd, vsFileName);
		else
			OutputDebugStringA("Missing vs Shader File");

		return false;
	}

	ID3D10Blob * idxShaderBuf;
	hResult = D3DX11CompileFromFile(psFileName, NULL, NULL, "MultiTexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &idxShaderBuf, &errMsgBuf, NULL);
	if(FAILED(hResult))
	{
		if(errMsgBuf)
			OutputShaderErrorMessage(errMsgBuf, _hWnd, psFileName);
		else
			OutputDebugStringA("Missing ps Shader File");

		return false;
	}

	hResult = _dev->CreateVertexShader(vertexShaderbuf->GetBufferPointer(), vertexShaderbuf->GetBufferSize(), NULL, &m_pVertextShader);
	if(FAILED(hResult))
		return false;

	hResult = _dev->CreatePixelShader(idxShaderBuf->GetBufferPointer(), idxShaderBuf->GetBufferSize(), NULL, &m_pPixelShader);
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

	hResult = _dev->CreateInputLayout(polygonLayout, nElements, vertexShaderbuf->GetBufferPointer(), vertexShaderbuf->GetBufferSize(), &m_pInputLayout);
	if(FAILED(hResult))
		return false;

	vertexShaderbuf->Release();
	vertexShaderbuf = 0;

	idxShaderBuf->Release();
	idxShaderBuf = 0;

	D3D11_BUFFER_DESC mBufDesc;
	mBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	mBufDesc.ByteWidth = sizeof(MatrixBufferType);
	mBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	mBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mBufDesc.MiscFlags = 0;
	mBufDesc.StructureByteStride = 0;

	hResult = _dev->CreateBuffer(&mBufDesc, NULL, &m_pBuffer);
	if(FAILED(hResult))
		return false;

	D3D11_SAMPLER_DESC sBufDesc;
	sBufDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sBufDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sBufDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sBufDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sBufDesc.MipLODBias = 0.0f;
	sBufDesc.MaxAnisotropy = 1;
	sBufDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sBufDesc.BorderColor[0] = 0;
	sBufDesc.BorderColor[1] = 0;
	sBufDesc.BorderColor[2] = 0;
	sBufDesc.BorderColor[3] = 0;
	sBufDesc.MinLOD = 0;
	sBufDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hResult = _dev->CreateSamplerState(&sBufDesc, &m_pSamplerState);
	if(FAILED(hResult))
		return false;

	return true;
}

void CMultiTextureShader::ShutdownShader()
{
	if(m_pSamplerState)
	{
		m_pSamplerState->Release();
		m_pSamplerState = 0;
	}

	if(m_pBuffer)
	{
		m_pBuffer->Release();
		m_pBuffer = 0;
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

	if(m_pVertextShader)
	{
		m_pVertextShader->Release();
		m_pVertextShader = 0;
	}
}

void CMultiTextureShader::OutputShaderErrorMessage(ID3D10Blob* _errMsg, HWND hWnd, WCHAR* shaderFileName)
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

bool CMultiTextureShader::SetShaderParameters(ID3D11DeviceContext* devContext, D3DXMATRIX wMatrix, D3DXMATRIX vMatrix, D3DXMATRIX pMatrix, ID3D11ShaderResourceView ** tetureArray)
{
	D3DXMatrixTranspose(&wMatrix, &wMatrix);
	D3DXMatrixTranspose(&vMatrix, &vMatrix);
	D3DXMatrixTranspose(&pMatrix, &pMatrix);

	D3D11_MAPPED_SUBRESOURCE mappedR;
	HRESULT hResult = devContext->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedR);
	if(FAILED(hResult))
		return false;

	MatrixBufferType * ptrMBufType = (MatrixBufferType*)mappedR.pData;

	ptrMBufType->world = wMatrix;
	ptrMBufType->view = vMatrix;
	ptrMBufType->projection = pMatrix;

	devContext->Unmap(m_pBuffer, 0);
	
	unsigned int nBufNumber = 0;

	devContext->VSSetConstantBuffers(nBufNumber, 1, &m_pBuffer);

	devContext->PSSetShaderResources(0, 2, tetureArray);

	return true;
}

void CMultiTextureShader::RenderShader(ID3D11DeviceContext* devContext, int idxCnt)
{
	devContext->IASetInputLayout(m_pInputLayout);

	devContext->VSSetShader(m_pVertextShader, NULL, 0);
	devContext->PSSetShader(m_pPixelShader, NULL, 0);

	devContext->PSSetSamplers(0, 1, &m_pSamplerState);

	devContext->DrawIndexed(idxCnt, 0, 0);
}