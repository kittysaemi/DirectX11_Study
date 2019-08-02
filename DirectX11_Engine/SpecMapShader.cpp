#include "SpecMapShader.h"


CSpecMapShader::CSpecMapShader(void)
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_inputLayout = 0;
	m_mBuffer = 0;
	m_sampleState = 0;
	m_lBuffer = 0;
	m_cBuffer = 0;
}


CSpecMapShader::~CSpecMapShader(void)
{
}

bool CSpecMapShader::Initialize(ID3D11Device* device, HWND hWnd)
{
	if(!InitializeShader(device, hWnd, L"../DirectX11_Engine/HLSL/Specmap.vs", L"../DirectX11_Engine/HLSL/Specmap.ps"))
		return false;

	return true;
}

void CSpecMapShader::Shutdown()
{
	ShutdownShader();
}

bool CSpecMapShader::Render(ID3D11DeviceContext* deviceCon, int idxCnt, D3DXMATRIX wMatrix, D3DXMATRIX vMatrix, D3DXMATRIX pMatrix, ID3D11ShaderResourceView ** texturArray, D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower)
{
	if(!SetShaderParameters(deviceCon, wMatrix, vMatrix, pMatrix, texturArray, lightDirection, diffuseColor, cameraPosition, specularColor, specularPower))
		return false;

	RenderShader(deviceCon, idxCnt);

	return true;
}

bool CSpecMapShader::InitializeShader(ID3D11Device* device, HWND hWnd, WCHAR * vsFIlename, WCHAR * psFilename)
{
	ID3D10Blob * errMsg = 0;
	ID3D10Blob * vShaderBuf = 0;
	HRESULT hResult = D3DX11CompileFromFile(vsFIlename, NULL, NULL, "SpecMapVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vShaderBuf, &errMsg, NULL);
	if(FAILED(hResult))
	{
		if(errMsg)
			OutputShaderErrorMessage(errMsg, hWnd, vsFIlename);
		else
			OutputDebugStringA("Missing vs Shader FIle");
		return false;
	}

	ID3D10Blob * pShaderBuf = 0;
	hResult = D3DX11CompileFromFile(psFilename, NULL, NULL, "SpecMapPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pShaderBuf, &errMsg, NULL);
	if(FAILED(hResult))
	{
		if(errMsg)
			OutputShaderErrorMessage(errMsg, hWnd, psFilename);
		else
			OutputDebugStringA("Missing ps Shader File");
		return false;
	}

	hResult = device->CreateVertexShader(vShaderBuf->GetBufferPointer(), vShaderBuf->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(hResult))
		return false;

	hResult = device->CreatePixelShader(pShaderBuf->GetBufferPointer(), pShaderBuf->GetBufferSize(), NULL, &m_pixelShader);
	if(FAILED(hResult))
		return false;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
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

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "TANGENT";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	polygonLayout[4].SemanticName = "BINORMAL";
	polygonLayout[4].SemanticIndex = 0;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[4].InputSlot = 0;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[4].InstanceDataStepRate = 0;

	unsigned int nElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	hResult = device->CreateInputLayout(polygonLayout, nElements, vShaderBuf->GetBufferPointer(), vShaderBuf->GetBufferSize(), &m_inputLayout);
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

	hResult = device->CreateBuffer(&mBufDesc, NULL, &m_mBuffer);
	if(FAILED(hResult))
		return false;

	D3D11_SAMPLER_DESC sDesc;
	sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sDesc.MipLODBias = 0.0f;
	sDesc.MaxAnisotropy = 1;
	sDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sDesc.BorderColor[0] = 0;
	sDesc.BorderColor[1] = 0;
	sDesc.BorderColor[2] = 0;
	sDesc.BorderColor[3] = 0;
	sDesc.MinLOD = 0;
	sDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hResult - device->CreateSamplerState(&sDesc, &m_sampleState);
	if(FAILED(hResult))
		return false;

	D3D11_BUFFER_DESC lBufDesc;
	lBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	lBufDesc.ByteWidth = sizeof(LightBufferType);
	lBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lBufDesc.MiscFlags = 0;
	lBufDesc.StructureByteStride = 0;

	hResult = device->CreateBuffer(&lBufDesc, NULL, &m_lBuffer);
	if(FAILED(hResult))
		return false;

	D3D11_BUFFER_DESC cBufDesc;
	cBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	cBufDesc.ByteWidth = sizeof(CameraBufferType);
	cBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cBufDesc.MiscFlags = 0;
	cBufDesc.StructureByteStride = 0;

	hResult = device->CreateBuffer(&cBufDesc, NULL, &m_cBuffer);
	if(FAILED(hResult))
		return false;

	return true;
}

void CSpecMapShader::ShutdownShader()
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

void CSpecMapShader::OutputShaderErrorMessage(ID3D10Blob* _errMsg, HWND hWnd, WCHAR* errFileName)
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

bool CSpecMapShader::SetShaderParameters(ID3D11DeviceContext * deviceCon, D3DXMATRIX wMatrix, D3DXMATRIX vMatrix, D3DXMATRIX pMatrix, ID3D11ShaderResourceView ** texturArray, D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 sepecularColor, float specularPower)
{
	D3DXMatrixTranspose(&wMatrix, &wMatrix);
	D3DXMatrixTranspose(&vMatrix, &vMatrix);
	D3DXMatrixTranspose(&pMatrix, &pMatrix);

	D3D11_MAPPED_SUBRESOURCE mapped;
	HRESULT hResult = deviceCon->Map(m_mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	if(FAILED(hResult))
		return false;

	MatrixBufferType * ptrMBufType = (MatrixBufferType *)mapped.pData;

	ptrMBufType->world = wMatrix;
	ptrMBufType->view = vMatrix;
	ptrMBufType->projection = pMatrix;

	deviceCon->Unmap(m_mBuffer, 0);

	unsigned int nBufNums = 0;

	deviceCon->VSSetConstantBuffers(nBufNums, 1, &m_mBuffer);

	deviceCon->PSSetShaderResources(0, 3, texturArray);

	hResult = deviceCon->Map(m_lBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	if(FAILED(hResult))
		return false;

	LightBufferType * ptrLBufType = (LightBufferType*)mapped.pData;

	ptrLBufType->diffuseColor = diffuseColor;
	ptrLBufType->lightDirection = lightDirection;
	ptrLBufType->specularColor = sepecularColor;
	ptrLBufType->specularPower = specularPower;

	deviceCon->Unmap(m_lBuffer, 0);

	nBufNums = 0;

	deviceCon->PSSetConstantBuffers(nBufNums, 1, &m_lBuffer);

	hResult = deviceCon->Map(m_cBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	if(FAILED(hResult))
		return false;
	
	CameraBufferType * ptrCBufType = (CameraBufferType *)mapped.pData;

	ptrCBufType->cameraPosition = cameraPosition;

	deviceCon->Unmap(m_cBuffer, 0);

	nBufNums = 1;

	deviceCon->VSSetConstantBuffers(nBufNums, 1, &m_cBuffer);

	return true;
}

void CSpecMapShader::RenderShader(ID3D11DeviceContext * deviceCon, int idxCnt)
{
	deviceCon->IASetInputLayout(m_inputLayout);

	deviceCon->VSSetShader(m_vertexShader, NULL, 0);
	deviceCon->PSSetShader(m_pixelShader, NULL, 0);

	deviceCon->PSSetSamplers(0, 1, &m_sampleState);

	deviceCon->DrawIndexed(idxCnt, 0, 0);
}