#include "InputClass.h"


CInputClass::CInputClass(void)
{
	m_pDirectInput = 0;
	m_pKeyboard = 0;
	m_pMouse = 0;

	for(int i=0; i<256; i++)
		m_cKeyState[i] = ' ';
}

CInputClass::CInputClass(const CInputClass& other)
{

}

CInputClass::~CInputClass(void)
{
}

// void CInputClass::Initialize()
// {
// 	for(int i=0; i<256; i++)
// 		m_keys[i] = false;
// }

// void CInputClass::KeyDown(unsigned int key)
// {
// 	m_keys[key] = true;
// }
// 
// void CInputClass::KeyUp(unsigned int key)
// {
// 	m_keys[key] = false;
// }
// bool CInputClass::IsKeyDown(unsigned int key)
// {
// 	return m_keys[key];
// }

bool CInputClass::Initialize(HINSTANCE hInstance, HWND hWnd, int scW, int scH)
{
	m_nScreenSize.nX = scW;
	m_nScreenSize.nY = scH;

	m_nMousePos.nX = 0;
	m_nMousePos.nY = 0;

	// Initialize the main direct input interface
	HRESULT hReslut = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
	if(FAILED(hReslut))
		return false;

	// Initialize the direct input interface for the keyboard.
	hReslut = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL);
	if(FAILED(hReslut))
		return false;

	// set the data foramt. In this case sine it is a keyboard we can use the predefined data foramt.
	hReslut = m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hReslut))
		return false;

	/*
		키볻의 협력 레벨(cooperative level)을 정하는 것은 이 장치가 무엇을 하는지, 어떻게 사용될 것인지를 결정한다.
		DISCL_EXCLUSIVE플래그로 다른 프로그램들과 공유하지 않는다고 알린다 (배제상태). 그러면 오직 이 어플리케이션에서만 입력을 감지 할 수 있다.
		다른 어플리케이션에서 키보드의 입력을 접근하게 하고 싶다면 DISCL_NONEXCLUSIVE를 사용하면 된다. 그러면 스크린샷을 찍고, 다른 어플리케이션도 키보드로 제어 할 수 있따.
		비 배제 상태이고 풀스크린이 아니라면 장치가 다른 윈도우로 포커스가 이동했는지, 다시 포커스를 얻어 장치를 사용할 수 있게 되었는지 확인해야 한다.
	*/

	// Set the cooperative level of the keyboard to not share with other programs.
	hReslut = m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if(FAILED(hReslut))
		return false;

	// Now acquire the keyboard.
	hReslut = m_pKeyboard->Acquire();
	if(FAILED(hReslut))
		return false;

	// Initialize the direct input interface for the mouse.
	hReslut = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	if(FAILED(hReslut))
		return false;

	// Set the data format for the mouse using the pre-defind mouse data format.
	hReslut = m_pMouse->SetDataFormat(&c_dfDIMouse);
	if(FAILED(hReslut))
		return false;

	// Set the cooperative level of the mouse to share with other programs.
	hReslut = m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if(FAILED(hReslut))
		return false;

	// Acquire the mouse.
	hReslut = m_pMouse->Acquire();
	if(FAILED(hReslut))
		return false;

	return true;
}

void CInputClass::Shutdown()
{
	if(m_pMouse)
	{
		m_pMouse->Unacquire();
		m_pMouse->Release();
		m_pMouse = 0;
	}

	if(m_pKeyboard)
	{
		m_pKeyboard->Unacquire();
		m_pKeyboard->Release();
		m_pKeyboard = 0;
	}

	if(m_pDirectInput)
	{
		m_pDirectInput->Release();
		m_pDirectInput = 0;
	}
}

bool CInputClass::Frame()
{
	// 장치의 현재 상태를 읽어 앞서 만든 상태 버퍼에 기록.
	// 각 장치의 상태를 읽은 뒤 변경사항을 처리 하게 한다.

	if(!ReadKeyboard())
		return false;

	if(!ReadMouse())
		return false;

	ProcessInput();

	return true;
}

#include <stdio.h>
bool CInputClass::ReadKeyboard()
{
	if(m_pKeyboard == NULL)
		return false;

	ZeroMemory(m_cKeyState, sizeof(m_cKeyState));

	// 키보드의 상태를 저장.
	// m_cKeyState : 모든 키에 대해 현재 상태(push, nomarl)

	HRESULT hResult = m_pKeyboard->GetDeviceState(sizeof(m_cKeyState), (LPVOID)&m_cKeyState);
	if(FAILED(hResult))
	{
		hResult = m_pKeyboard->Acquire();
		while(hResult == DIERR_INPUTLOST)
			hResult = m_pKeyboard->Acquire();

		if(hResult == DIERR_NOTACQUIRED)
			m_pKeyboard->Acquire();
		else
			return false;
	}

	return true;
}

bool CInputClass::ReadMouse()
{
	HRESULT hReulst = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_dMouseState);
	if(FAILED(hReulst))
	{
		if((hReulst == DIERR_INPUTLOST) || (hReulst == DIERR_NOTACQUIRED))
			m_pMouse->Acquire();
		else
			return false;
	}

	return true;
}

void CInputClass::ProcessInput()
{
	// 변경사항이 실제 적용되는 곳.

	m_nMousePos.nX += m_dMouseState.lX;
	m_nMousePos.nY += m_dMouseState.lY;

	if(m_nMousePos.nX < 0) m_nMousePos.nX = 0;
	if(m_nMousePos.nY < 0) m_nMousePos.nY = 0;

	if(m_nMousePos.nX > m_nScreenSize.nX) m_nMousePos.nX = m_nScreenSize.nX;
	if(m_nMousePos.nY > m_nScreenSize.nY) m_nMousePos.nY = m_nScreenSize.nY;
}

bool CInputClass::IsEscapePressed()
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if(m_cKeyState[DIK_ESCAPE] & 0x80)
		return true;

	return false;
}
bool CInputClass::IsRightArrowPressed()
{
	if(m_cKeyState[DIK_LEFT] & 0x80)
		return true;
	return false;
}
bool CInputClass::IsLeftArrowPressed()
{
	if(m_cKeyState[DIK_RIGHT] & 0x80)
		return true;
	return false;
}
bool CInputClass::IsTopArrowPressed()
{
	if(m_cKeyState[DIK_UP] & 0x80)
		return true;
	return false;
}
bool CInputClass::IsBottomArrowPressed()
{
	if(m_cKeyState[DIK_DOWN] & 0x80)
		return true;
	return false;
}
bool CInputClass::IsCtrlUp()
{
	if(m_cKeyState[DIK_ADD] & 0x80)
		return true;
	return false;
}

bool CInputClass::IsCtrlDown()
{
	if(m_cKeyState[DIK_SUBTRACT] & 0x80)
		return true;
	return false;
}

bool CInputClass::IsPlayStatus(UINT _type)
{
	if(m_cKeyState[_type] & 0x80)
		return true;
	return false;
}


void CInputClass::GetMouseLocation(int &posX, int &posY)
{
	posX = m_nMousePos.nX;
	posY = m_nMousePos.nY;
}


char* CInputClass::GetKeyboardInputData()
{
	char strSanCode[20];
	
	ZeroMemory(m_strKeyboardInputData, sizeof(m_strKeyboardInputData));
	ZeroMemory(strSanCode, sizeof(strSanCode));
	
	for(int i=0; i<256; i++)
	{
		if(m_cKeyState[i] & 0x80)
		{
			sprintf(strSanCode, "0x%02x ", i);
			strcat(m_strKeyboardInputData, strSanCode);
		}
	}
	if(strcmp(m_strKeyboardInputData, "" ) != 0)
	{
		OutputDebugStringA(m_strKeyboardInputData);
		OutputDebugStringA("\r\n");

	}
	
	return (char*)m_strKeyboardInputData;
}