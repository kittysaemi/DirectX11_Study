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

	// 수직동기화 설정 저장.
	m_bVsyncEnable = vsync;

	/*
		Direct3D를 초기화하기 전에 그래픽카드/모니터의 주사율(새로고침 비율)을 알아야 한다. (컴퓨터마다 다름)
		이 비율의 분자/분모 값을 조회한 뒤 DirectX에 알려 주면 주사율을 계산한다.
		이 작업을 안하면, DirectX는 화면을 표시할 떄 버퍼 플립을 사용하지 않고 blit를 사용하게 되어 성능이 저하되고 디버그 출력에 거슬리는 에러 메세지를 남기게 된다.
	*/
	// DirectX 그래픽 인터페이스 팩토리 생성
	IDXGIFactory * _factory;
	hResult = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&_factory);
	if(FAILED(hResult))
	{
		OutputDebugStringW(L"Faild CreateDXGIFactory");
		return false;
	}

	// Factory 객체를 사용하여 첫번째 그래픽 카드 인터ㅔ이스에 대한 Adapter 생성
	IDXGIAdapter* _adapter;
	hResult = _factory->EnumAdapters(0, &_adapter);
	if(FAILED(hResult))
	{
		OutputDebugString(L"Faild EnumAdapters");
		return false;
	}

	// 출력(모니터)에 대한 첫번째 Adapter 나열
	IDXGIOutput* _adapterOutput;
	hResult = _adapter->EnumOutputs(0, &_adapterOutput);
	if(FAILED(hResult))
	{
		OutputDebugString(L"Faild EnumOutputs");
		return false;
	}

	// DXGI_FORMAT_R8G8B8A8_UNORM 모니터 출력 디스플레이 포맷에 맞는 모드이 개수 구하기
	unsigned int _numModes;
	hResult = _adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &_numModes, NULL);
	if(FAILED(hResult))
	{
		OutputDebugString(L"Faild GetDisplayModeList");
		return false;
	}

	// 가능한 모든 모니터와 그래픽 카드 조합을 저장할 리스틀 생성합니다.
	DXGI_MODE_DESC* _displaymodelist;
	_displaymodelist = new DXGI_MODE_DESC[_numModes];
	if(!_displaymodelist)
	{
		OutputDebugString(L"Faild create DXGI_MODE_DESC");
		return false;
	}

	// Display Mode에 대한 list 채우기
	hResult = _adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &_numModes, _displaymodelist);
	if(FAILED(hResult))
	{
		OutputDebugString(L"Faild GetDisplayModeList");
		return false;
	}

	// All Display Mode 에 대한 wdith, height 에 맞는 Display Mode Find
	// 모니터의 새로고침 비율의 분모와 분자 값 저장
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
		그래픽카드의 이름과 사용 가능한 그래픽 카드 메모리의 크기를 어댑터를 이용하여 받아온다.
	*/
	// 어댑터(그래픽카드)의 description 
	DXGI_ADAPTER_DESC _adapterDesc;
	hResult = _adapter->GetDesc(&_adapterDesc);
	if(FAILED(hResult))
	{
		OutputDebugString(L"Faild GetDesc");
		return false;
	}

	// 현재 그래픽 카드의 메모리 용량을 MByte 로 저장
	m_nVideoCardMemeory = (int)(_adapterDesc.DedicatedVideoMemory / 1024 / 1024 );

	// 그래픽카드의 이름을 char 문자열로 변환 저장
	unsigned int _strLen;
	if(wcstombs_s(&_strLen, m_sVideoCardDescription, 128, _adapterDesc.Description, 128) != 0)
	{
		OutputDebugString(L"Faild wcstombs_s");
		return false;
	}

	/*
		사용했던 구조체와 인터페이스 해제
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
		DirectX 초기화		
	*/
	/*
		SwapChain의 description struct Fill
		SwapChain 은 실제로 랜더링 한 곳이 기록되는 frontBufferd와 BackBuffer
		보통 랜더링 할때, 하나의 백버퍼만 사용하며, 그 위에 장면을 그린뒤, 프론트 버퍼와 바꿔치기(Swap)하여 유저의 화면에 보이게 된다.
	*/
	// SWAPCHAIN Description Init
	DXGI_SWAP_CHAIN_DESC _swapchainDesc;
	ZeroMemory(&_swapchainDesc, sizeof(_swapchainDesc));

	// one backbuffer use
	_swapchainDesc.BufferCount = 1;

	// backbuffer width, height 
	_swapchainDesc.BufferDesc.Width = scW;
	_swapchainDesc.BufferDesc.Height = scH;

	// Backbuffer , 32bit  서페이스 지정
	_swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	/*
		주사율 : 1초당 몇 장의 백버퍼를 프론트 버퍼와 바꿔치기하는지 나타내는 숫자
		vsync is true, 시스템에서 정한 새로고침 비율로 고정 (ex: 60Hz/1초에 60번 화면 그리기)
		vsync is false, 1초에 최대한 빠르게 화면 그리기
	*/

	// Backbuffer Refresh 비율 설정
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

	// BackBuff 용도 설정
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

	// Scanline Align, 스캔라이닝을 지정되지 않으므로(unspecified) 설정
	_swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	_swapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 출력된 이후의 Backbuffer 내용 버림
	_swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Add Option flag not use
	_swapchainDesc.Flags = 0;

	/*
		피쳐 레벨 변수 설정
		DirectX  버전 알림. 
	*/
	D3D_FEATURE_LEVEL _featureLevel;
	_featureLevel = D3D_FEATURE_LEVEL_11_0;

	/*
		Direct3D장치와 Direct3D 장치 컨텍스트는 모든 Direct3D 함수들의 인터페이스가 된다.
		* DirectX11을 지원하는 그래픽카드가 없다면, 장치와 장치 컨텍스트 생성 실패한다.
		  D3D_Driver_TYPE_HARDWARE -> D3D_DRVIER_TYPE_REFERENCE로 바꾸어서 그래픽 카드가 아닌 CPU에서 랜더링 처라하게 할수 있따.
		  속도는 1/1000정도로 느림.
	*/
	// Swapchain, Direct3D Device, Direct3D Device Context Create
	hResult = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &_featureLevel, 1, D3D11_SDK_VERSION, &_swapchainDesc, &m_dpSwapChain, &m_dpDevice, NULL, &m_dpDeviceContext);
	if(FAILED(hResult))
	{
		OutputDebugString(L"Faild D3D11CreateDeviceAndSwapChain");
		return false;
	}

	/*
		* 첫번째 그래픽 카드가 DirectX 11 과 호환되지 않는 경우, 첫번째는 DirectX 10을 지원하고 두번째가 DirectX 11을 지원할수도 있다.
		* 일부 하이브리드 그래픽카드의 경우 첫번째는 저전력의 인텍 그래픽카드, 두번째는 Nvidia로 동작할수도 있다.

		 - 기본 그래픽 카드만 사용하지 말아야 하고 기기의 모든 비디오 카드들을 나열하여 유저가 직접 가장 잘 맞는 그래픽 카드로 장치를 생성할 수 있도록 해야 한다.
	*/

	/*
		BackBufer의 pointor를 SwapChain에 연결.
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
		3D공간에서 올바르게 그리기 위한 작업.
		스텐실 버퍼 연결 ( 스텐실 버퍼 : 모션 블러, 볼류메트릭 그림자 등의 효과)
	*/
	// 깊이 버퍼의 description 초기화
	D3D11_TEXTURE2D_DESC _depthBufferDesc;
	ZeroMemory(&_depthBufferDesc, sizeof(_depthBufferDesc));

	// 깊이 버퍼의 descriptio 작성
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
		깊이/스텐실 버퍼 생성 (2D)
	*/

	// description을 사용하여 깊이 버퍼의 텍스처를 생성
	hResult = m_dpDevice->CreateTexture2D(&_depthBufferDesc, NULL, &m_dpTexture2DBuffer);
	if(FAILED(hResult))
	{
		OutputDebugString(L"faild, CreateTexture2D");
		return false;
	}

	// 스텐실 상태의 description 초기화
	 D3D11_DEPTH_STENCIL_DESC _depthStencilDesc;
	ZeroMemory(&_depthStencilDesc, sizeof(_depthStencilDesc));

	// 스텐실 상태의 description 작성
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

	// 깊이 - 스텐실 상태를 생성
	hResult = m_dpDevice->CreateDepthStencilState(&_depthStencilDesc, &m_dpDepthStencilState);
	if(FAILED(hResult))
	{
		OutputDebugString(L"faild, CreateDepthstencilstate");
		return false;
	}

	// 깊이 -스텐실 상태 설정
	m_dpDeviceContext->OMSetDepthStencilState(m_dpDepthStencilState, 1);

	/*
		깊이-스텐실 버퍼의 뷰에 대한 description.
		Direct3D가 깊이 버퍼를 깊이-스텐실 텍스쳐로 인식
	*/
	// 깊이-스텐실 뷰의 description 초기화
	 D3D11_DEPTH_STENCIL_VIEW_DESC _depthStencilViewDesc;
	ZeroMemory(&_depthStencilViewDesc, sizeof(_depthStencilViewDesc));

	// 깊-스텐실 뷰의 description 작성
	_depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	_depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	_depthStencilViewDesc.Texture2D.MipSlice = 0;

	// 깊이 - 스텐실 뷰 생성
	hResult = m_dpDevice->CreateDepthStencilView(m_dpTexture2DBuffer, &_depthStencilViewDesc, &m_dpDepthStencilView);
	if(FAILED(hResult))
	{
		OutputDebugString(L"faild, Createdepthstencilview");
		return false;
	}

	/*
		OMSetRenderTarget
		 : 렌더타겟뷰와 깊이-스텐실 뷰를 출력 렌더링 파이프라인에 바인딩시킨다.
		 파이프라인을 이용한 렌더링이 수행될 때, 백퍼버에 장면이 그려지게 된다.
		 백퍼에 그려진 것을 프론트 버퍼와 바꿔치기하여 유저의 모니터에 출력
	*/

	// 렌더타겟 뷰와 깊이-스텐실 버퍼를 각각 출력 파이프라인에 바인딩
	m_dpDeviceContext->OMSetRenderTargets(1, &m_dpRenderTargetView, m_dpDepthStencilView);

	/*
		추가적 기능 설정
		1. 래스터화기 상태(rasterizer state) : 도형이 어떻게 픽셀로 그려지는지에 대한 제어 / 화면을 와이어프레임모드로 그리거나 도형의 앞뒷면을 모두 그리도록 할수 있다.
		** DirectX 기본 래스터화기 상태가 있지만 직접 만들지 않는 이상 이에 대한 제어권이 없다.
	*/

	// 어떤 도형을 어떻게 그릴 것이닞 결정하는 래스터화기 description 작성
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

	 // 래스터화기 상태 
	 hResult = m_dpDevice->CreateRasterizerState(&_rasterDesc, &m_dpRasterizerState);
	 if(FAILED(hResult))
	 {
		 OutputDebugString(L"faild, CreateRasterizerState");
		 return false;
	 }

	 // 래스터화기 상태 결정
	 m_dpDeviceContext->RSSetState(m_dpRasterizerState);

	 /*
		뷰포트 : 렌더타겟 공간에서 클리핑 수행 가능.
	 */
	 // 랜더링을 위한 뷰포트 설정
	  D3D11_VIEWPORT _viewport; 
	_viewport.Width = (float)scW;
	_viewport.Height = (float)scH;
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;
	_viewport.TopLeftX = 0.0f;
	_viewport.TopLeftY = 0.0f;

	// 뷰포트 생성
	m_dpDeviceContext->RSSetViewports(1, &_viewport);

	/*
		투영행렬(projection matrix) 생성
		- 3D의 화면을 2D 뷰포트 공간으로 변환하도록 도와주는 것.
		행렬의 복사본을 만들어 셰이더에서 장면을 그릴 떄 사용할 수 있또록 해야 한다.
	*/

	// 투명 행렬 설정
	float _fieldOfView = (float)D3DX_PI / 4.0f;
	float _screenAspect = (float)scW / (float)scH;

	// 3D 렌더링을 위한 투영 행렬 생성
	D3DXMatrixPerspectiveFovLH(&m_matrixProjection, _fieldOfView, _screenAspect, scNear, scDepth);

	/*
		월드 행렬
		: 오브젝트들의 좌표를 3D 세계의 좌표로 변환하는 데 사용된다.
		3차원 공간에서의 회전/이동/크기 변환에도 사용된다.
		셰이더에 전달하여 사용할수 있도록 한다.
	*/
	// 월드 행렬을 단위 행렬로 초기화
	D3DXMatrixIdentity(&m_matrixWorld);

	/*
		뷰 행렬 생성
		: 현재 장면에서 우리가 어느 위치에서 어느 방향을 보고 있는가를 계산하는 데 이용된다.
		3D세계를 카메라라고 한다면 카메라에 대한 행렬이라고 할 수 있다.
	*/

	/*
		직교 투영 행렬 생성
		- 3D객체가 아니라 UI와 같은 2D의 요소들을 그리기 위해 사용된다.
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
// 매 프레임의 시작마다 3D화면을 그리기 시작할 떄 호출 된다.
// 버퍼를 빈 값으로 초기화하고 렌더링이 이루어지도록 준비한다.
void CD3DClass::BeginScene(float R, float G, float B, float A)
{
	// 버퍼를 어떤 색상으로 지울 것인지 설정
	float _color[4] = {R,G,B,A};

	// Backbuffer clear
	m_dpDeviceContext->ClearRenderTargetView(m_dpRenderTargetView, _color);

	// Depth buffer clear
	m_dpDeviceContext->ClearDepthStencilView(m_dpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}
//////////////////////////////////////////////////////////////////////////
// 매 프레임의 마지막에 스왑체인에게 백퍼에 그린 3D화면을 표시하도록 한다.
void CD3DClass::EndScene()
{
	// 랜더링 완료, Backbuffer show view
	if(m_bVsyncEnable)
		m_dpSwapChain->Present(1, 0);	// Refresh 비율 Fix
	else
		m_dpSwapChain->Present(0, 0);	// 가능한 한 빠르게 표시
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