#include "RenderTexture.h"


CRenderTexture::CRenderTexture(void)
{
	m_renderTargetTexture = 0;
	m_renderTargetView = 0;
	m_ShaderResourceView = 0;
}


CRenderTexture::~CRenderTexture(void)
{
}

bool CRenderTexture::Initialize(ID3D11Device * device, int texWidth, int texHeight)
{
	D3D11_TEXTURE2D_DESC tex2D_Desc;
	ZeroMemory(&tex2D_Desc, sizeof(tex2D_Desc));

	tex2D_Desc.Width = texWidth;
	tex2D_Desc.Height = texHeight;
	tex2D_Desc.MipLevels = 1;
	tex2D_Desc.ArraySize = 1;
	tex2D_Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tex2D_Desc.SampleDesc.Count = 1;
	tex2D_Desc.Usage = D3D11_USAGE_DEFAULT;
	tex2D_Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tex2D_Desc.CPUAccessFlags = 0;
	tex2D_Desc.MiscFlags = 0;

	HRESULT hResult = device->CreateTexture2D(&tex2D_Desc, NULL, &m_renderTargetTexture);
	if(FAILED(hResult))
		return false;

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = tex2D_Desc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	hResult = device->CreateRenderTargetView(m_renderTargetTexture, &renderTargetViewDesc, &m_renderTargetView);
	if(FAILED(hResult))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = tex2D_Desc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	hResult = device->CreateShaderResourceView(m_renderTargetTexture, &shaderResourceViewDesc, &m_ShaderResourceView);
	if(FAILED(hResult))
		return false;

	return true;
}

void CRenderTexture::Shutdown()
{
	if(m_ShaderResourceView)
	{
		m_ShaderResourceView->Release();
		m_ShaderResourceView = 0;
	}

	if(m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}
	if(m_renderTargetTexture)
	{
		m_renderTargetTexture->Release();
		m_renderTargetTexture = 0;
	}
}

void CRenderTexture::SetRenderTarget(ID3D11DeviceContext * devCon, ID3D11DepthStencilView * depthStencilView)
{
	devCon->OMSetRenderTargets(1, &m_renderTargetView, depthStencilView);
}

void CRenderTexture::ClearRenderTarget(ID3D11DeviceContext * devCont, ID3D11DepthStencilView * depthStencilVIew, float rgba[4])
{
	// BeginScene 함수와 같으나 백퍼가 아닌 m_renderTargetView에 클리어가 이루어진다.
	// 매 프레임 RTT가 일어나기 전에 호출

	devCont->ClearRenderTargetView(m_renderTargetView, rgba);

	devCont->ClearDepthStencilView(depthStencilVIew, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

ID3D11ShaderResourceView* CRenderTexture::GetShaderResourceVIew()
{
	return m_ShaderResourceView;
}