#include "D3DClass.h"


CD3DClass::CD3DClass(void)
{
	m_dpSwapChain = 0;
	m_dpDevice =0;
	m_dpDeviceContext =0;
	m_dpRenderTargetView = 0;
	m_dpTexture2DBuffer = 0;
	m_dpDepthStencilState =0;
	m_dpDepthStencilView = 0;
	m_dpRasterizerState = 0;
}
CD3DClass::CD3DClass(const CD3DClass& other)
{

}


CD3DClass::~CD3DClass(void)
{
}


bool CD3DClass::Initialize(int scW, int scH, bool vsync, HWND hWnd, bool fullscreenMode, float scDepth, float scNear)
{
	HRESULT hResult = false;

	// ��������ȭ ���� ����.
	m_bVsyncEnable = vsync;

	/*
		Direct3D�� �ʱ�ȭ�ϱ� ���� �׷���ī��/������� �ֻ���(���ΰ�ħ ����)�� �˾ƾ� �Ѵ�. (��ǻ�͸��� �ٸ�)
		�� ������ ����/�и� ���� ��ȸ�� �� DirectX�� �˷� �ָ� �ֻ����� ����Ѵ�.
		�� �۾��� ���ϸ�, DirectX�� ȭ���� ǥ���� �� ���� �ø��� ������� �ʰ� blit�� ����ϰ� �Ǿ� ������ ���ϵǰ� ����� ��¿� �Ž����� ���� �޼����� ����� �ȴ�.
	*/
	// DirectX �׷��� �������̽� ���丮 ����
	IDXGIFactory * _factory;
	hResult = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&_factory);
	if(FAILED(hResult))
	{
		OutputDebugStringW(L"Faild CreateDXGIFactory");
		return false;
	}

	// Factory ��ü�� ����Ͽ� ù��° �׷��� ī�� ���ͤ��̽��� ���� Adapter ����
	IDXGIAdapter* _adapter;
	hResult = _factory->EnumAdapters(0, &_adapter);
	if(FAILED(hResult))
	{
		OutputDebugString(L"Faild EnumAdapters");
		return false;
	}

	// ���(�����)�� ���� ù��° Adapter ����
	IDXGIOutput* _adapterOutput;
	hResult = _adapter->EnumOutputs(0, &_adapterOutput);
	if(FAILED(hResult))
	{
		OutputDebugString(L"Faild EnumOutputs");
		return false;
	}

	// DXGI_FORMAT_R8G8B8A8_UNORM ����� ��� ���÷��� ���˿� �´� ����� ���� ���ϱ�
	unsigned int _numModes;
	hResult = _adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &_numModes, NULL);
	if(FAILED(hResult))
	{
		OutputDebugString(L"Faild GetDisplayModeList");
		return false;
	}

	// ������ ��� ����Ϳ� �׷��� ī�� ������ ������ ����Ʋ �����մϴ�.
	DXGI_MODE_DESC* _displaymodelist;
	_displaymodelist = new DXGI_MODE_DESC[_numModes];
	if(!_displaymodelist)
	{
		OutputDebugString(L"Faild create DXGI_MODE_DESC");
		return false;
	}

	// Display Mode�� ���� list ä���
	hResult = _adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &_numModes, _displaymodelist);
	if(FAILED(hResult))
	{
		OutputDebugString(L"Faild GetDisplayModeList");
		return false;
	}

	// All Display Mode �� ���� wdith, height �� �´� Display Mode Find
	// ������� ���ΰ�ħ ������ �и�� ���� �� ����
	unsigned int _nNumerator = 0, _nDenominator = 0;
	for(unsigned int i=0; i<_numModes; i++)
	{
		if(_displaymodelist[i].Width == (unsigned int)scW)
		{
			if(_displaymodelist[i].Height == (unsigned int)scH)
			{
				_nNumerator = _displaymodelist[i].RefreshRate.Numerator;
				_nDenominator = _displaymodelist[i].RefreshRate.Denominator;
			}
		}
	}

	/*
		�׷���ī���� �̸��� ��� ������ �׷��� ī�� �޸��� ũ�⸦ ����͸� �̿��Ͽ� �޾ƿ´�.
	*/
	// �����(�׷���ī��)�� description 
	DXGI_ADAPTER_DESC _adapterDesc;
	hResult = _adapter->GetDesc(&_adapterDesc);
	if(FAILED(hResult))
	{
		OutputDebugString(L"Faild GetDesc");
		return false;
	}

	// ���� �׷��� ī���� �޸� �뷮�� MByte �� ����
	m_nVideoCardMemeory = (int)(_adapterDesc.DedicatedVideoMemory / 1024 / 1024 );

	// �׷���ī���� �̸��� char ���ڿ��� ��ȯ ����
	unsigned int _strLen;
	if(wcstombs_s(&_strLen, m_sVideoCardDescription, 128, _adapterDesc.Description, 128) != 0)
	{
		OutputDebugString(L"Faild wcstombs_s");
		return false;
	}

	/*
		����ߴ� ����ü�� �������̽� ����
	*/
	delete [] _displaymodelist;
	_displaymodelist = 0;

	_adapterOutput->Release();
	_adapterOutput = 0;

	_adapter->Release();
	_adapter = 0;

	_factory->Release();
	_factory = 0;

	/*
		DirectX �ʱ�ȭ		
	*/
	/*
		SwapChain�� description struct Fill
		SwapChain �� ������ ������ �� ���� ��ϵǴ� frontBufferd�� BackBuffer
		���� ������ �Ҷ�, �ϳ��� ����۸� ����ϸ�, �� ���� ����� �׸���, ����Ʈ ���ۿ� �ٲ�ġ��(Swap)�Ͽ� ������ ȭ�鿡 ���̰� �ȴ�.
	*/
	// SWAPCHAIN Description Init
	DXGI_SWAP_CHAIN_DESC _swapchainDesc;
	ZeroMemory(&_swapchainDesc, sizeof(_swapchainDesc));

	// one backbuffer use
	_swapchainDesc.BufferCount = 1;

	// backbuffer width, height 
	_swapchainDesc.BufferDesc.Width = scW;
	_swapchainDesc.BufferDesc.Height = scH;

	// Backbuffer , 32bit  �����̽� ����
	_swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	/*
		�ֻ��� : 1�ʴ� �� ���� ����۸� ����Ʈ ���ۿ� �ٲ�ġ���ϴ��� ��Ÿ���� ����
		vsync is true, �ý��ۿ��� ���� ���ΰ�ħ ������ ���� (ex: 60Hz/1�ʿ� 60�� ȭ�� �׸���)
		vsync is false, 1�ʿ� �ִ��� ������ ȭ�� �׸���
	*/

	// Backbuffer Refresh ���� ����
	if(m_bVsyncEnable)
	{
		_swapchainDesc.BufferDesc.RefreshRate.Numerator = _nNumerator;
		_swapchainDesc.BufferDesc.RefreshRate.Denominator = _nDenominator;
	}
	else
	{
		_swapchainDesc.BufferDesc.RefreshRate.Numerator = 0;
		_swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// BackBuff �뵵 ����
	_swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Rendering Windows Handle set
	_swapchainDesc.OutputWindow = hWnd;

	// MutilSampling Off
	_swapchainDesc.SampleDesc.Count =1;
	_swapchainDesc.SampleDesc.Quality = 0;

	// Window Mode or FullScreen Mode Set
	if(fullscreenMode)
		_swapchainDesc.Windowed = false;
	else
		_swapchainDesc.Windowed = true;

	// Scanline Align, ��ĵ���̴��� �������� �����Ƿ�(unspecified) ����
	_swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	_swapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// ��µ� ������ Backbuffer ���� ����
	_swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Add Option flag not use
	_swapchainDesc.Flags = 0;

	/*
		���� ���� ���� ����
		DirectX  ���� �˸�. 
	*/
	D3D_FEATURE_LEVEL _featureLevel;
	_featureLevel = D3D_FEATURE_LEVEL_11_0;

	/*
		Direct3D��ġ�� Direct3D ��ġ ���ؽ�Ʈ�� ��� Direct3D �Լ����� �������̽��� �ȴ�.
		* DirectX11�� �����ϴ� �׷���ī�尡 ���ٸ�, ��ġ�� ��ġ ���ؽ�Ʈ ���� �����Ѵ�.
		  D3D_Driver_TYPE_HARDWARE -> D3D_DRVIER_TYPE_REFERENCE�� �ٲپ �׷��� ī�尡 �ƴ� CPU���� ������ ó���ϰ� �Ҽ� �ֵ�.
		  �ӵ��� 1/1000������ ����.
	*/
	// Swapchain, Direct3D Device, Direct3D Device Context Create
	hResult = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &_featureLevel, 1, D3D11_SDK_VERSION, &_swapchainDesc, &m_dpSwapChain, &m_dpDevice, NULL, &m_dpDeviceContext);
	if(FAILED(hResult))
	{
		OutputDebugString(L"Faild D3D11CreateDeviceAndSwapChain");
		return false;
	}

	/*
		* ù��° �׷��� ī�尡 DirectX 11 �� ȣȯ���� �ʴ� ���, ù��°�� DirectX 10�� �����ϰ� �ι�°�� DirectX 11�� �����Ҽ��� �ִ�.
		* �Ϻ� ���̺긮�� �׷���ī���� ��� ù��°�� �������� ���� �׷���ī��, �ι�°�� Nvidia�� �����Ҽ��� �ִ�.

		 - �⺻ �׷��� ī�常 ������� ���ƾ� �ϰ� ����� ��� ���� ī����� �����Ͽ� ������ ���� ���� �� �´� �׷��� ī��� ��ġ�� ������ �� �ֵ��� �ؾ� �Ѵ�.
	*/

	/*
		BackBufer�� pointor�� SwapChain�� ����.
	*/
	// get Backbuffer pointor
	 ID3D11Texture2D* _backBufferPtr;
	hResult = m_dpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&_backBufferPtr);
	if(FAILED(hResult))
	{
		OutputDebugString(L"Faild, SwapChian GetBuffer");
		return false;
	}

	// Backbuffer pointor, RanderTarget View Create
	hResult = m_dpDevice->CreateRenderTargetView(_backBufferPtr, NULL, &m_dpRenderTargetView);
	if(FAILED(hResult))
	{
		OutputDebugString(L"Faild, CreateRenderTargetView");
		return false;
	}

	// BackBuffer Pointor Release
	_backBufferPtr->Release();
	_backBufferPtr = 0;

	/*
		Depth Buffer description Strcut Create
		3D�������� �ùٸ��� �׸��� ���� �۾�.
		���ٽ� ���� ���� ( ���ٽ� ���� : ��� ��, ������Ʈ�� �׸��� ���� ȿ��)
	*/
	// ���� ������ description �ʱ�ȭ
	D3D11_TEXTURE2D_DESC _depthBufferDesc;
	ZeroMemory(&_depthBufferDesc, sizeof(_depthBufferDesc));

	// ���� ������ descriptio �ۼ�
	_depthBufferDesc.Width = scW;
	_depthBufferDesc.Height = scH;
	_depthBufferDesc.MipLevels = 1;
	_depthBufferDesc.ArraySize = 1;
	_depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	_depthBufferDesc.SampleDesc.Count = 1;
	_depthBufferDesc.SampleDesc.Quality = 0;
	_depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	_depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	_depthBufferDesc.CPUAccessFlags = 0;
	_depthBufferDesc.MiscFlags = 0;

	/*
		����/���ٽ� ���� ���� (2D)
	*/

	// description�� ����Ͽ� ���� ������ �ؽ�ó�� ����
	hResult = m_dpDevice->CreateTexture2D(&_depthBufferDesc, NULL, &m_dpTexture2DBuffer);
	if(FAILED(hResult))
	{
		OutputDebugString(L"faild, CreateTexture2D");
		return false;
	}

	// ���ٽ� ������ description �ʱ�ȭ
	 D3D11_DEPTH_STENCIL_DESC _depthStencilDesc;
	ZeroMemory(&_depthStencilDesc, sizeof(_depthStencilDesc));

	// ���ٽ� ������ description �ۼ�
	_depthStencilDesc.DepthEnable = true;
	_depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	_depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	_depthStencilDesc.StencilEnable = true;
	_depthStencilDesc.StencilReadMask = 0xFF;
	_depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	_depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	_depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	_depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	_depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	_depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	_depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	_depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	_depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// ���� - ���ٽ� ���¸� ����
	hResult = m_dpDevice->CreateDepthStencilState(&_depthStencilDesc, &m_dpDepthStencilState);
	if(FAILED(hResult))
	{
		OutputDebugString(L"faild, CreateDepthstencilstate");
		return false;
	}

	// ���� -���ٽ� ���� ����
	m_dpDeviceContext->OMSetDepthStencilState(m_dpDepthStencilState, 1);

	/*
		����-���ٽ� ������ �信 ���� description.
		Direct3D�� ���� ���۸� ����-���ٽ� �ؽ��ķ� �ν�
	*/
	// ����-���ٽ� ���� description �ʱ�ȭ
	 D3D11_DEPTH_STENCIL_VIEW_DESC _depthStencilViewDesc;
	ZeroMemory(&_depthStencilViewDesc, sizeof(_depthStencilViewDesc));

	// ��-���ٽ� ���� description �ۼ�
	_depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	_depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	_depthStencilViewDesc.Texture2D.MipSlice = 0;

	// ���� - ���ٽ� �� ����
	hResult = m_dpDevice->CreateDepthStencilView(m_dpTexture2DBuffer, &_depthStencilViewDesc, &m_dpDepthStencilView);
	if(FAILED(hResult))
	{
		OutputDebugString(L"faild, Createdepthstencilview");
		return false;
	}

	/*
		OMSetRenderTarget
		 : ����Ÿ�ٺ�� ����-���ٽ� �並 ��� ������ ���������ο� ���ε���Ų��.
		 ������������ �̿��� �������� ����� ��, ���۹��� ����� �׷����� �ȴ�.
		 ���ۿ� �׷��� ���� ����Ʈ ���ۿ� �ٲ�ġ���Ͽ� ������ ����Ϳ� ���
	*/

	// ����Ÿ�� ��� ����-���ٽ� ���۸� ���� ��� ���������ο� ���ε�
	m_dpDeviceContext->OMSetRenderTargets(1, &m_dpRenderTargetView, m_dpDepthStencilView);

	/*
		�߰��� ��� ����
		1. ������ȭ�� ����(rasterizer state) : ������ ��� �ȼ��� �׷��������� ���� ���� / ȭ���� ���̾������Ӹ��� �׸��ų� ������ �յ޸��� ��� �׸����� �Ҽ� �ִ�.
		** DirectX �⺻ ������ȭ�� ���°� ������ ���� ������ �ʴ� �̻� �̿� ���� ������� ����.
	*/

	// � ������ ��� �׸� ���̈� �����ϴ� ������ȭ�� description �ۼ�
	 D3D11_RASTERIZER_DESC _rasterDesc;
	 _rasterDesc.AntialiasedLineEnable = false;
	 _rasterDesc.CullMode = D3D11_CULL_BACK;
	 _rasterDesc.DepthBias = 0;
	 _rasterDesc.DepthBiasClamp = 0.0f;
	 _rasterDesc.DepthClipEnable = true;
	 _rasterDesc.FillMode = D3D11_FILL_SOLID;
	 _rasterDesc.FrontCounterClockwise = false;
	 _rasterDesc.MultisampleEnable = false;
	 _rasterDesc.ScissorEnable = false;
	 _rasterDesc.SlopeScaledDepthBias = 0.0f;

	 // ������ȭ�� ���� 
	 hResult = m_dpDevice->CreateRasterizerState(&_rasterDesc, &m_dpRasterizerState);
	 if(FAILED(hResult))
	 {
		 OutputDebugString(L"faild, CreateRasterizerState");
		 return false;
	 }

	 // ������ȭ�� ���� ����
	 m_dpDeviceContext->RSSetState(m_dpRasterizerState);

	 /*
		����Ʈ : ����Ÿ�� �������� Ŭ���� ���� ����.
	 */
	 // �������� ���� ����Ʈ ����
	  D3D11_VIEWPORT _viewport; 
	_viewport.Width = (float)scW;
	_viewport.Height = (float)scH;
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;
	_viewport.TopLeftX = 0.0f;
	_viewport.TopLeftY = 0.0f;

	// ����Ʈ ����
	m_dpDeviceContext->RSSetViewports(1, &_viewport);

	/*
		�������(projection matrix) ����
		- 3D�� ȭ���� 2D ����Ʈ �������� ��ȯ�ϵ��� �����ִ� ��.
		����� ���纻�� ����� ���̴����� ����� �׸� �� ����� �� �ֶǷ� �ؾ� �Ѵ�.
	*/

	// ���� ��� ����
	float _fieldOfView = (float)D3DX_PI / 4.0f;
	float _screenAspect = (float)scW / (float)scH;

	// 3D �������� ���� ���� ��� ����
	D3DXMatrixPerspectiveFovLH(&m_matrixProjection, _fieldOfView, _screenAspect, scNear, scDepth);

	/*
		���� ���
		: ������Ʈ���� ��ǥ�� 3D ������ ��ǥ�� ��ȯ�ϴ� �� ���ȴ�.
		3���� ���������� ȸ��/�̵�/ũ�� ��ȯ���� ���ȴ�.
		���̴��� �����Ͽ� ����Ҽ� �ֵ��� �Ѵ�.
	*/
	// ���� ����� ���� ��ķ� �ʱ�ȭ
	D3DXMatrixIdentity(&m_matrixWorld);

	/*
		�� ��� ����
		: ���� ��鿡�� �츮�� ��� ��ġ���� ��� ������ ���� �ִ°��� ����ϴ� �� �̿�ȴ�.
		3D���踦 ī�޶��� �Ѵٸ� ī�޶� ���� ����̶�� �� �� �ִ�.
	*/

	/*
		���� ���� ��� ����
		- 3D��ü�� �ƴ϶� UI�� ���� 2D�� ��ҵ��� �׸��� ���� ���ȴ�.
	*/

	D3DXMatrixOrthoLH(&m_matrixOther, (float)scW, (float)scH, scNear, scDepth);


	return true;
}

void CD3DClass::Shutdown()
{
	if(m_dpSwapChain)
		m_dpSwapChain->SetFullscreenState(FALSE, NULL);

	if(m_dpRasterizerState)
	{
		m_dpRasterizerState->Release();
		m_dpRasterizerState = 0;
	}

	if(m_dpDepthStencilView)
	{
		m_dpDepthStencilView->Release();
		m_dpDepthStencilView = 0;
	}

	if(m_dpDepthStencilState)
	{
		m_dpDepthStencilState->Release();
		m_dpDepthStencilState = 0;
	}

	if(m_dpTexture2DBuffer)
	{
		m_dpTexture2DBuffer->Release();
		m_dpTexture2DBuffer = 0;
	}

	if(m_dpRenderTargetView)
	{
		m_dpRenderTargetView->Release();
		m_dpRenderTargetView = 0;
	}

	if(m_dpDeviceContext)
	{
		m_dpDeviceContext->Release();
		m_dpDeviceContext = 0;
	}

	if(m_dpDevice)
	{
		m_dpDevice->Release();
		m_dpDevice = 0;
	}

	if(m_dpSwapChain)
	{
		m_dpSwapChain->Release();
		m_dpSwapChain = 0;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
// �� �������� ���۸��� 3Dȭ���� �׸��� ������ �� ȣ�� �ȴ�.
// ���۸� �� ������ �ʱ�ȭ�ϰ� �������� �̷�������� �غ��Ѵ�.
void CD3DClass::BeginScene(float R, float G, float B, float A)
{
	// ���۸� � �������� ���� ������ ����
	float _color[4] = {R,G,B,A};

	// Backbuffer clear
	m_dpDeviceContext->ClearRenderTargetView(m_dpRenderTargetView, _color);

	// Depth buffer clear
	m_dpDeviceContext->ClearDepthStencilView(m_dpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}
//////////////////////////////////////////////////////////////////////////
// �� �������� �������� ����ü�ο��� ���ۿ� �׸� 3Dȭ���� ǥ���ϵ��� �Ѵ�.
void CD3DClass::EndScene()
{
	// ������ �Ϸ�, Backbuffer show view
	if(m_bVsyncEnable)
		m_dpSwapChain->Present(1, 0);	// Refresh ���� Fix
	else
		m_dpSwapChain->Present(0, 0);	// ������ �� ������ ǥ��
}
ID3D11Device* CD3DClass::GetDevice()
{
	return m_dpDevice;
}
ID3D11DeviceContext* CD3DClass::GetDeviceContext()
{
	return m_dpDeviceContext;
}
void CD3DClass::GetProjectionMatrix(D3DXMATRIX& matrix)
{
	matrix = m_matrixProjection;
	return;
}

void CD3DClass::GetWorldMatrix(D3DXMATRIX& matrix)
{
	matrix = m_matrixWorld;
	return;
}

void CD3DClass::GetOtherMatrix(D3DXMATRIX& matrix)
{
	matrix = m_matrixOther;
	return;
}

void CD3DClass::GetVideoCardInfo(char* cardName, int& mem)
{
	strcpy_s(cardName, 128, m_sVideoCardDescription);
	mem = m_nVideoCardMemeory;
	return;
}