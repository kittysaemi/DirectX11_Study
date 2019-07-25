#include "FontClass.h"


CFontClass::CFontClass(void)
{
	m_structFont = 0;
	m_pTexture = 0;
}
CFontClass::CFontClass(const CFontClass& other)
{

}
CFontClass::~CFontClass(void)
{
}

bool CFontClass::Initialize(ID3D11Device * _device, char * _idxFIleName, WCHAR * _texFIleName)
{
	if(!LoadFontData(_idxFIleName))
		return false;

	if(!LoadTexture(_device, _texFIleName))
		return false;

	return true;
}

void CFontClass::Shutdown()
{
	ReleaseTexture();

	ReleaseFontData();
}

bool CFontClass::LoadFontData(char * _idxFileName)
{
	int FontCount = 95;
	m_structFont = new FontType[FontCount];
	if(!m_structFont)
		return false;

	ifstream fileIn;
	fileIn.open(_idxFileName);
	if(fileIn.fail())
		return false;

	for(int i=0; i<FontCount; i++)
	{
		char Temp;
		fileIn.get(Temp);
		while (Temp != ' ')
		{
			fileIn.get(Temp);
		}
		fileIn.get(Temp);
		while (Temp != ' ')
		{
			fileIn.get(Temp);
		}

		fileIn >> m_structFont[i].nLeft;
		fileIn >> m_structFont[i].nRight;
		fileIn >> m_structFont[i].nSize;
	}

	fileIn.close();

	return true;
}

void CFontClass::ReleaseFontData()
{
	if(m_structFont)
	{
		delete [] m_structFont;
		m_structFont = 0;
	}
}

bool CFontClass::LoadTexture(ID3D11Device * _device, WCHAR * _texFileName)
{
	m_pTexture = new CTextureClass;
	if(!m_pTexture)
		return false;

	if(!m_pTexture->Initialize(_device, _texFileName))
		return false;

	return true;
}

void CFontClass::ReleaseTexture()
{
	if(m_pTexture)
	{
		m_pTexture->Shutdown();
		delete m_pTexture;
		m_pTexture = 0;
	}
}

ID3D11ShaderResourceView * CFontClass::GetTexture()
{
	return m_pTexture->GetTexture();
}

void CFontClass::BuildVertexArray(void* buffer, char * strData, float drawX, float drawY)
{
	VertexType * ptrVertex = (VertexType*)buffer;
	int nLetter = (int)strlen(strData);
	int idx = 0;
	int nletterIdx = 0;

	for(int i=0; i<nLetter; i++)
	{
		nletterIdx = ((int)strData[i]) - 32;

		if(nletterIdx == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			ptrVertex[idx].position = D3DXVECTOR3(drawX, drawY, 0.0f);
			ptrVertex[idx++].texture = D3DXVECTOR2(m_structFont[nletterIdx].nLeft, 0.0f);

			ptrVertex[idx].position = D3DXVECTOR3((drawX + m_structFont[nletterIdx].nSize), (drawY - 16), 0.0f);
			ptrVertex[idx++].texture = D3DXVECTOR2(m_structFont[nletterIdx].nRight, 1.0f);

			ptrVertex[idx].position = D3DXVECTOR3(drawX, (drawY - 16), 0.0f);
			ptrVertex[idx++].texture = D3DXVECTOR2(m_structFont[nletterIdx].nLeft, 1.0f);

			ptrVertex[idx].position = D3DXVECTOR3(drawX, drawY, 0.0f);
			ptrVertex[idx++].texture = D3DXVECTOR2(m_structFont[nletterIdx].nLeft, 0.0f);

			ptrVertex[idx].position = D3DXVECTOR3((drawX + m_structFont[nletterIdx].nSize), drawY, 0.0f);
			ptrVertex[idx++].texture = D3DXVECTOR2(m_structFont[nletterIdx].nRight, 0.0f);

			ptrVertex[idx].position = D3DXVECTOR3((drawX + m_structFont[nletterIdx].nSize), (drawY - 16), 0.0f);
			ptrVertex[idx++].texture = D3DXVECTOR2(m_structFont[nletterIdx].nRight, 1.0f);

			drawX = drawX + m_structFont[nletterIdx].nSize + 1.0f;
		}
	}
	
}