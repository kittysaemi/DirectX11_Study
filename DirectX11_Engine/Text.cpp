#include "Text.h"


CText::CText(void)
{
	m_pFont = 0;
	m_pFontShader = 0;

	int nC = sizeof(m_sentence) / sizeof(m_sentence[0]);
	for(int i=0; i<nC; i++)
		m_sentence[i] = 0;

}
CText::CText(const CText& other)
{

}
CText::~CText(void)
{
}

bool CText::Initialize(ID3D11Device * _device, ID3D11DeviceContext * _devContext, HWND _hWnd, ScreenSizeInfo _screenSize, D3DXMATRIX _baseViewMatrix)
{
	m_nScreenSize.screenWidth = _screenSize.screenWidth;
	m_nScreenSize.screenHeight = _screenSize.screenHeight;
	m_baseViewMatrix = _baseViewMatrix;

	m_pFont = new CFontClass;
	if(!m_pFont)
		return false;

	if(!m_pFont->Initialize(_device, "../DirectX11_Engine/data/fontdata.txt", L"../DirectX11_Engine/data/font.dds"))
		return false;

	m_pFontShader = new CFontShader;
	if(!m_pFontShader)
		return false;

	if(!m_pFontShader->Initialize(_device, _hWnd))
		return false;

	char * strList[20] = {"First", "Second", "Third", "Fourth", "Fifth", "Sixth", "Seventh", "Eight times", "Nine times", "Heat burning"};
	float colorRed[10] = {0xCC, 0xFF, 0xFF, 0x00, 0x33, 0x00, 0x99, 0x66, 0x33, 0xFF};
	float colorGreen[10] = {0x00, 0x99, 0xFF, 0xCC, 0x00, 0x00, 0x00, 0x33, 0xFF, 0x66};
	float colorBlue[10] = {0x00, 0x00, 0x00, 0x00, 0xFF, 0x66, 0x99, 0x00, 0xCC, 0x99};


	int nC = sizeof(m_sentence) / sizeof(m_sentence[0]);
	for(int i=0; i<nC; i++)
	{
		FontColor fColor;
		fColor.red = colorRed[i];
		fColor.green = colorGreen[i];
		fColor.blue = colorBlue[i];

		if(!InitializeSentence(&m_sentence[i], strlen(strList[i]), _device))
			return false;
		if(!UpdateSentence(m_sentence[i], strList[i], 50, 100 + ( i * 20 ), fColor, _devContext ))
			return false;
	}

	return true;
}

void CText::Shutdown()
{
	int nC = sizeof(m_sentence) / sizeof(m_sentence[0]);
	for(int i=0; i<nC; i++)
		ReleaseSentence(&m_sentence[i]);

	if(m_pFontShader)
	{
		m_pFontShader->Shutdown();
		delete m_pFontShader;
		m_pFontShader = 0;
	}
	if(m_pFont)
	{
		m_pFont->Shutdown();
		delete m_pFont;
		m_pFont = 0;
	}
}
bool CText::Render(ID3D11DeviceContext * _devContext, D3DXMATRIX _worldMatrix, D3DXMATRIX _orthoMatrix)
{
	int nC = sizeof(m_sentence) / sizeof(m_sentence[0]);
	for(int i=0; i<nC; i++)
		if(!RenderSetence(_devContext, m_sentence[i], _worldMatrix, _orthoMatrix))
			return false;

	return true;
}

bool CText::InitializeSentence(SentenceType** _sentenceData, int _maxLen, ID3D11Device* _device)
{
	(*_sentenceData) = new SentenceType;
	if(!(*_sentenceData))
		return false;

	(*_sentenceData)->pVertBuf = 0;
	(*_sentenceData)->pIdxBuf = 0;

	(*_sentenceData)->nMaxLen = _maxLen;

	(*_sentenceData)->nVertCount = 6 * _maxLen;
	(*_sentenceData)->nIdxCount = (*_sentenceData)->nVertCount;

	VertexType * vertices = new VertexType[(*_sentenceData)->nVertCount];
	if(!vertices)
		return false;

	unsigned long * indices = new unsigned long[(*_sentenceData)->nIdxCount];
	if(!indices)
		return false;

	memset(vertices, 0, (sizeof(VertexType) * (*_sentenceData)->nVertCount));

	for(int i=0; i<(*_sentenceData)->nIdxCount; i++)
		indices[i] = i;

	D3D11_BUFFER_DESC verBufDesc;
	verBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	verBufDesc.ByteWidth = sizeof(VertexType) * (*_sentenceData)->nVertCount;
	verBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	verBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	verBufDesc.MiscFlags = 0;
	verBufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vetData;
	vetData.pSysMem = vertices;
	vetData.SysMemPitch = 0;
	vetData.SysMemSlicePitch = 0;

	HRESULT hResult = _device->CreateBuffer(&verBufDesc, &vetData, &(*_sentenceData)->pVertBuf);
	if(FAILED(hResult))
		return false;

	D3D11_BUFFER_DESC idxBufDesc;
	idxBufDesc.Usage = D3D11_USAGE_DEFAULT;
	idxBufDesc.ByteWidth = sizeof(unsigned long) * (*_sentenceData)->nIdxCount;
	idxBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	idxBufDesc.CPUAccessFlags = 0;
	idxBufDesc.MiscFlags = 0;
	idxBufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA idxData;
	idxData.pSysMem = indices;
	idxData.SysMemPitch = 0;
	idxData.SysMemSlicePitch = 0;

	hResult = _device->CreateBuffer(&idxBufDesc, &idxData, &(*_sentenceData)->pIdxBuf);
	if(FAILED(hResult))
		return false;

	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

bool CText::UpdateSentence(SentenceType* _sentence, char* _strText, int _posX, int _posY, FontColor _fColor, ID3D11DeviceContext* _devContext)
{
	_sentence->nfRed = _fColor.red;
	_sentence->nfGreen = _fColor.green;
	_sentence->nfBlue = _fColor.blue;

	int nLetter = (int)strlen(_strText);

	if(nLetter > _sentence->nMaxLen)
		return false;
	
	VertexType * vertices = new VertexType[_sentence->nVertCount];
	if(!vertices)
		return false;

	memset(vertices, 0, (sizeof(VertexType) * _sentence->nVertCount));

	float drawX = (float)(((m_nScreenSize.screenWidth / 2) * -1) + _posX);
	float drawY = (float)((m_nScreenSize.screenHeight / 2) - _posY);
	
	m_pFont->BuildVertexArray((void*)vertices, _strText, drawX, drawY);

	D3D11_MAPPED_SUBRESOURCE _mapResource;
	HRESULT hResult = _devContext->Map(_sentence->pVertBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &_mapResource);
	if(FAILED(hResult))
		return false;
	
	VertexType * ptrVertices = (VertexType *)_mapResource.pData;
	
	memcpy(ptrVertices, (void*)vertices, (sizeof(VertexType) * _sentence->nVertCount));

	_devContext->Unmap(_sentence->pVertBuf, 0);

	delete [] vertices;
	vertices = 0;

	return true;
}

void CText::ReleaseSentence(SentenceType** _sentence)
{
	if(*_sentence)
	{
		if((*_sentence)->pVertBuf)
		{
			(*_sentence)->pVertBuf->Release();
			(*_sentence)->pVertBuf = 0;
		}

		if((*_sentence)->pIdxBuf)
		{
			(*_sentence)->pIdxBuf->Release();
			(*_sentence)->pIdxBuf = 0;
		}

		delete (*_sentence);
		(*_sentence) = 0;
	}
}

bool CText::RenderSetence(ID3D11DeviceContext* _devContext, SentenceType * _sentence, D3DXMATRIX _worldMatrix, D3DXMATRIX _orthoMatrix)
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	_devContext->IASetVertexBuffers(0, 1, &_sentence->pVertBuf, &stride, &offset);
	_devContext->IASetIndexBuffer(_sentence->pIdxBuf, DXGI_FORMAT_R32_UINT, 0);
	_devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DXVECTOR4 pixelColor = D3DXVECTOR4(_sentence->nfRed, _sentence->nfGreen, _sentence->nfBlue, 1.0f);

	if(!m_pFontShader->Render(_devContext, _sentence->nIdxCount, _worldMatrix, m_baseViewMatrix, _orthoMatrix, m_pFont->GetTexture(), pixelColor))
		return false;

	return true;
}