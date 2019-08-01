#include "AlphaMapShader.h"


CAlphaMapShader::CAlphaMapShader(void)
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_inputLayout = 0;
	m_pMBuffer = 0;
	m_samplerState = 0;
}


CAlphaMapShader::~CAlphaMapShader(void)
{
}

bool CAlphaMapShader::Initialize(ID3D11Device * dev, HWND hWnd)
{

	if(!InitializeShader(dev, hWnd, L"../DirectX11_Engine/HLSL/Alphamap.vs", L"../DirectX11_Engine/HLSL/Alphamap.ps"))
		return false;

	return true;
}

void CAlphaMapShader::Shutdown()
{
	ShutdownShader();
}

bool CAlphaMapShader::Render(ID3D11DeviceContext* devCon, int idxCnt, D3DXMATRIX wMatrix, D3DXMATRIX vMatrix, D3DXMATRIX pMatrix, ID3D11ShaderResourceView ** texturArray)
{
	if(!SetShaderParameters(devCon, wMatrix, vMatrix, pMatrix, texturArray))
		return false;

	RenderShader(devCon, idxCnt);

	return true;
}

bool CAlphaMapShader::InitializeShader(ID3D11Device * dev, HWND hWnd, WCHAR * vsFileName, WCHAR * psFileName)
{
	ID3D10Blob* errMsg = 0;
	ID3D10Blob* vShaderBuf = 0;
	HRESULT hResult = D3DX11CompileFromFile(vsFileName, NULL, NULL, "AlphaMapVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vShaderBuf, &errMsg, NULL);
	if(FAILED(hResult))
	{
		if(errMsg)
			OutputShaderErrorMessage(errMsg, hWnd, vsFileName);
		else
			OutputDebugStringA("Mssing vs Shader file");

		return false;
	}

	ID3D10Blob* pShaderBuf = 0;
	hResult = D3DX11CompileFromFile(psFileName, NULL, NULL, "AlphaMapPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pShaderBuf, &errMsg, NULL);
	if(FAILED(hResult))
	{
		if(errMsg)
			OutputShaderErrorMessage(errMsg, hWnd, psFileName);
		else
			OutputDebugStringA("Missing ps Shader file");

		return false;
	}

	hResult = dev->CreateVertexShader(vShaderBuf->GetBufferPointer(), vShaderBuf->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(hResult))
		return false;

	hResult = dev->CreatePixelShader(pShaderBuf->GetBufferPointer(), pShaderBuf->GetBufferSize(), NULL, &m_pixelShader);
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

	hResult = dev->CreateInputLayout(polygonLayout, nElements, vShaderBuf->GetBufferPointer(), vShaderBuf->GetBufferSize(), &m_inputLayout);
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

	hResult = dev->CreateBuffer(&mBufDesc, NULL, &m_pMBuffer);
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

	hResult = dev->CreateSamplerState(&sBufDesc, &m_samplerState);
	if(FAILED(hResult))
		return false;

	return true;
}

void CAlphaMapShader::ShutdownShader()
{
	if(m_samplerState)
	{
		m_samplerState->Release();
		m_samplerState = 0;
	}

	if(m_pMBuffer)
	{
		m_pMBuffer->Release();
		m_pMBuffer = 0;
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

void CAlphaMapShader::OutputShaderErrorMessage(ID3D10Blob * _errMsg, HWND hWnd, WCHAR * fileName)
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

bool CAlphaMapShader::SetShaderParameters(ID3D11DeviceContext * devCon, D3DXMATRIX wMatrix, D3DXMATRIX vMatrix, D3DXMATRIX pMatrix, ID3D11ShaderResourceView ** texturArray)
{
	D3DXMatrixTranspose(&wMatrix, &wMatrix);
	D3DXMatrixTranspose(&vMatrix, &vMatrix);
	D3DXMatrixTranspose(&pMatrix, &pMatrix);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hResult = devCon->Map(m_pMBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(hResult))
		return false;
		
	MatrixBufferType* ptrBufType = (MatrixBufferType*)mappedResource.pData;

	ptrBufType->world = wMatrix;
	ptrBufType->view = vMatrix;
	ptrBufType->projection = pMatrix;

	devCon->Unmap(m_pMBuffer, 0);

	unsigned int nbufNum = 0;

	devCon->VSSetConstantBuffers(nbufNum, 1, &m_pMBuffer);

	devCon->PSSetShaderResources(0, 3, texturArray);

	return true;
}

void CAlphaMapShader::RenderShader(ID3D11DeviceContext * devCon, int idxCnt)
{
	devCon->IASetInputLayout(m_inputLayout);

	devCon->VSSetShader(m_vertexShader, NULL, 0);
	devCon->PSSetShader(m_pixelShader, NULL, 0);

	devCon->PSSetSamplers(0, 1, &m_samplerState);

	devCon->DrawIndexed(idxCnt, 0, 0);
}