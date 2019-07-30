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
		Ű���� ���� ����(cooperative level)�� ���ϴ� ���� �� ��ġ�� ������ �ϴ���, ��� ���� �������� �����Ѵ�.
		DISCL_EXCLUSIVE�÷��׷� �ٸ� ���α׷���� �������� �ʴ´ٰ� �˸��� (��������). �׷��� ���� �� ���ø����̼ǿ����� �Է��� ���� �� �� �ִ�.
		�ٸ� ���ø����̼ǿ��� Ű������ �Է��� �����ϰ� �ϰ� �ʹٸ� DISCL_NONEXCLUSIVE�� ����ϸ� �ȴ�. �׷��� ��ũ������ ���, �ٸ� ���ø����̼ǵ� Ű����� ���� �� �� �ֵ�.
		�� ���� �����̰� Ǯ��ũ���� �ƴ϶�� ��ġ�� �ٸ� ������� ��Ŀ���� �̵��ߴ���, �ٽ� ��Ŀ���� ��� ��ġ�� ����� �� �ְ� �Ǿ����� Ȯ���ؾ� �Ѵ�.
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
	// ��ġ�� ���� ���¸� �о� �ռ� ���� ���� ���ۿ� ���.
	// �� ��ġ�� ���¸� ���� �� ��������� ó�� �ϰ� �Ѵ�.

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

	// Ű������ ���¸� ����.
	// m_cKeyState : ��� Ű�� ���� ���� ����(push, nomarl)

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
	// ��������� ���� ����Ǵ� ��.

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