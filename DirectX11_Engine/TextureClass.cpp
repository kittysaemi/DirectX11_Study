#include "TextureClass.h"


CTextureClass::CTextureClass(void)
{
	m_pShaderResourceViewTexture = 0;
}

CTextureClass::CTextureClass(const CTextureClass& other)
{

}


CTextureClass::~CTextureClass(void)
{
}

bool CTextureClass::Initialize(ID3D11Device* _device, WCHAR* _filename)
{
	HRESULT hResult;

	// Laod the texture in.
	hResult = D3DX11CreateShaderResourceViewFromFile(_device, _filename, NULL, NULL, &m_pShaderResourceViewTexture, NULL);
	if(FAILED(hResult))
	{
		OutputDebugStringA("Faild, TextureClass Init");
		return false;
	}

	return true;
}

void CTextureClass::Shutdown()
{
	// Release the resource.
	if(m_pShaderResourceViewTexture)
	{
		m_pShaderResourceViewTexture->Release();
		m_pShaderResourceViewTexture = 0;
	}

	return;
}

ID3D11ShaderResourceView* CTextureClass::GetTexture()
{
	return m_pShaderResourceViewTexture;
}