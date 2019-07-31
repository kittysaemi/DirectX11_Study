#include "Texturearray.h"


CTexturearray::CTexturearray(void)
{
	int nCnt = sizeof(m_TextureList) / sizeof(m_TextureList[0]);
	for(int i=0; i<nCnt; i++)
		m_TextureList[i] = 0;
}

CTexturearray::CTexturearray(const CTexturearray& otehr)
{

}

CTexturearray::~CTexturearray(void)
{
}

bool CTexturearray::Initialize(ID3D11Device * dev, WCHAR* fileList[])
{
	int nCnt = sizeof(m_TextureList) / sizeof(m_TextureList[0]);
	for(int i=0; i<nCnt; i++)
	{
		HRESULT hResult = D3DX11CreateShaderResourceViewFromFile(dev, fileList[i], NULL, NULL, &m_TextureList[i], NULL);

		if(FAILED(hResult))
			return false;
	}

	return true;
}

void CTexturearray::Shutdown()
{
	int nCnt = sizeof(m_TextureList) / sizeof(m_TextureList[0]);
	for(int i=0; i<nCnt; i++)
	{
		m_TextureList[i]->Release();
		m_TextureList[i] = 0;
	}
}

ID3D11ShaderResourceView** CTexturearray::GetTextureArray()
{
	return m_TextureList;
}