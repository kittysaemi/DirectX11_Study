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
			DrawPositionCalculation(pLeft, pTop, drawX, drawY, nletterIdx, ptrVertex[idx].position, ptrVertex[idx].texture);		idx++;
			DrawPositionCalculation(pRight, pBottom, drawX, drawY, nletterIdx, ptrVertex[idx].position, ptrVertex[idx].texture);	idx++;
			DrawPositionCalculation(pLeft, pBottom, drawX, drawY, nletterIdx, ptrVertex[idx].position, ptrVertex[idx].texture);		idx++;

			DrawPositionCalculation(pLeft, pTop, drawX, drawY, nletterIdx, ptrVertex[idx].position, ptrVertex[idx].texture);		idx++;
			DrawPositionCalculation(pRight, pTop, drawX, drawY, nletterIdx, ptrVertex[idx].position, ptrVertex[idx].texture);		idx++;
			DrawPositionCalculation(pRight, pBottom, drawX, drawY, nletterIdx, ptrVertex[idx].position, ptrVertex[idx].texture);	idx++;

			drawX = drawX + m_structFont[nletterIdx].nSize + 1.0f;
		}
	}
	
}

void CFontClass::DrawPositionCalculation(HorizontalType _hType, VerticalType _vType,float drawX, float drawY, int letterIdx, D3DXVECTOR3 &positon, D3DXVECTOR2 &texture)
{
	float dX = drawX, dY = drawY, dZ = 0.0f;
	float uvX = 0.0f, uvY = 0.0f;

	switch (_hType)
	{
	case pLeft:
		dX = drawX;
		uvX = m_structFont[letterIdx].nLeft;
		break;
	case pRight:
		dX = drawX + m_structFont[letterIdx].nSize;
		uvX = m_structFont[letterIdx].nRight;
		break;
	}
	switch(_vType)
	{
	case pTop:
		dY = drawY;
		uvY = 0.0f;
		break;
	case pBottom:
		dY = drawY - 16;
		uvY = 1.0f;
		break;
	}

	positon = D3DXVECTOR3(dX, dY, dZ);
	texture = D3DXVECTOR2(uvX, uvY);
}