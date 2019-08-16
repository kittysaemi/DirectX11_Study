#include "SystemClass.h"


CSystemClass::CSystemClass(void)
{
	m_pGraphics = 0;
	m_pInput = 0;
	m_pSound = 0;

	m_pCpu = 0;
	m_pFps = 0;
	m_pTimer = 0;

	m_pPosition = 0;
}

CSystemClass::CSystemClass(const CSystemClass& other)
{

}

CSystemClass::~CSystemClass(void)
{
}

bool CSystemClass::Initialize()
{
	int screenX = 0, screenY = 0;
	InitializeWIndows(screenX, screenY);

	m_pInput = new CInputClass;
	if(!m_pInput)
		return false;

	// T13 Direct Input
	if(!m_pInput->Initialize(m_hinstance, m_hwnd, screenX, screenY))
		return false;

	m_pGraphics = new CGraphicsClass;
	if(!m_pGraphics)
		return false;

	if(!m_pGraphics->Initialize(screenX, screenY, m_hwnd))
		return false;

// 	m_pSound = new CSoundClass;
// 	if(!m_pSound)
// 		return false;
// 
// 	if(!m_pSound->Initialize(m_hwnd))
// 		return false;

	m_pFps = new CFps;
	if(!m_pFps)
		return false;
	m_pFps->Initialize();

	m_pCpu = new CCpu;
	if(!m_pCpu)
		return false;
	m_pCpu->Initialize();

	m_pTimer = new CTimer;
	if(!m_pTimer)
		return false;
	if(!m_pTimer->Initialize())
		return false;

	m_pPosition = new CPosition;
	if(!m_pPosition)
		return false;


	return true;
}

void CSystemClass::Shutdown()
{
	if(m_pPosition)
	{
		delete m_pPosition;
		m_pPosition = 0;
	}

	if(m_pTimer)
	{
		delete m_pTimer;
		m_pTimer = 0;
	}

	if(m_pCpu)
	{
		m_pCpu->Shutdown();
		delete m_pCpu;
		m_pCpu = 0;
	}

	if(m_pFps)
	{
		delete m_pFps;
		m_pFps = 0;
	}

	if(m_pSound)
	{
		m_pSound->Shutdown();
		delete m_pSound;
		m_pSound = 0;
	}

	if(m_pGraphics)
	{
		m_pGraphics->Shutdown();
		delete m_pGraphics;
		m_pGraphics = 0;
	}

	if(m_pInput)
	{
		m_pInput->Shutdown();
		delete m_pInput;
		m_pInput = 0;
	}

	ShutdownWindows();

	return;
}

void CSystemClass::Run()
{
	MSG msg;
	bool done= false;
	ZeroMemory(&msg, sizeof(MSG));

	while(!done)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(msg.message == WM_QUIT)
			done = true;
		else
		{
			if(!Frame())
			{
				done = true;
			}
		}
	}
	return;
}

bool CSystemClass::Frame()
{
	// Check if the user pressed escape and wants to quit.
	if(m_pInput->IsEscapePressed())
		return false;

	// cpu, timer, fps
	m_pTimer->Frame();
	m_pFps->Frame();
	m_pCpu->Frame();

	CGraphicsClass::SInputPosInfo info;
	info.nCPUper = m_pCpu->GetCpuPercentage();
	info.nCurTime = m_pTimer->GetTIme();
	info.nFPS = m_pFps->GetFps();


	// Sound
// 	if(m_pInput->IsPlayStatus(DIK_F1))	// Play
// 	{
// 		m_pSound->PlayWaveFile();
// 	}
// 	if(m_pInput->IsPlayStatus(DIK_F2))	// Pause
// 	{
// 		m_pSound->PauseWaveFIle();
// 	}
// 	if(m_pInput->IsPlayStatus(DIK_F3))	// Stop
// 	{
// 		m_pSound->StopWaveFile();
// 	}

	//m_pGraphics->SetPlayStatus(m_pSound->GetPlayStatus());

	// T13
	if(!m_pInput->Frame())
		return false;
	
	// DirectInput Info
	m_pInput->GetMouseLocation(info.nPosX, info.nPosY);
	info.sBuffer = m_pInput->GetKeyboardInputData();

	// T16 ÇÁ¸®½ºÅÒ 
	m_pPosition->SetFrameTIme(m_pTimer->GetTIme());
	m_pPosition->TurnLeft(m_pInput->IsLeftArrowPressed());
	m_pPosition->TurnRight(m_pInput->IsRightArrowPressed());
	m_pPosition->TurnTop(m_pInput->IsTopArrowPressed());
	m_pPosition->TurnBottom(m_pInput->IsBottomArrowPressed());
	m_pPosition->ZoomIn(m_pInput->IsCtrlDown());
	m_pPosition->ZoomOut(m_pInput->IsCtrlUp());

	m_pPosition->GetRotation(info.nRotationX, info.nRotationY, info.nRotationZ);

	
	// GUI 
	if(!m_pGraphics->Frame(info))
		return false;

	
	return true;
}
LRESULT CALLBACK CSystemClass::MessageHandler(HWND hwnd, UINT jumsg, WPARAM wparam, LPARAM lparam)
{
// 	switch (jumsg)
// 	{
// 	case WM_KEYDOWN:
// 		{
// 			if(m_pInput)
// 				m_pInput->KeyDown((unsigned int)wparam);
// 			return 0;
// 		}
// 	case WM_KEYUP:
// 		{
// 			if(m_pInput)
// 				m_pInput->KeyUp((unsigned int)wparam);
// 			return 0;
// 		}
// 	case WM_MOUSEMOVE:
// 		{
// 			
// 
// 			return 0;
// 		}
//	default:
//		return DefWindowProc(hwnd, jumsg, wparam, lparam);
//	}

	return DefWindowProc(hwnd, jumsg, wparam, lparam);
}

void CSystemClass::InitializeWIndows(int& screenW, int& screenH)
{
	WNDCLASSEX wc;
	DEVMODE dmSSeting;

	int pX = 0, pY = 0;

	ApplicationHandle = this;

	m_hinstance = GetModuleHandle(NULL);
	m_applicationName = L"DirectX11_Engine";

	wc.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	screenW = GetSystemMetrics(SM_CXSCREEN);
	screenH = GetSystemMetrics(SM_CYSCREEN);

	// if FullScreen
	if(FULL_SCREEN)
	{
		memset(&dmSSeting, 0, sizeof(dmSSeting));
		dmSSeting.dmSize = sizeof(dmSSeting);
		dmSSeting.dmPelsWidth = (unsigned long)screenW;
		dmSSeting.dmPelsHeight = (unsigned long)screenH;
		dmSSeting.dmBitsPerPel = 32;
		dmSSeting.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmSSeting, CDS_FULLSCREEN);

		pX = pY = 0;
	}
	else
	{

		screenW = 800;
		screenH = 600;

		pX = (GetSystemMetrics(SM_CXSCREEN) - screenW) / 2;
		pY = (GetSystemMetrics(SM_CYSCREEN) - screenH) / 2;

	}


	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_POPUP,
		pX, pY, screenW, screenH, NULL, NULL, m_hinstance, NULL);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	ShowCursor(true);

	return;

}

void CSystemClass::ShutdownWindows()
{
	ShowCursor(true);

	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	 DestroyWindow(m_hwnd);
	 m_hwnd = NULL;

	 UnregisterClass(m_applicationName, m_hinstance);
	 m_hinstance = NULL;

	 ApplicationHandle = NULL;

	 return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}