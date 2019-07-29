#pragma once

//////////////////////////////////////////////////////////////////////////
// tutorial 2

//////////////////////////////////////////////////////////////////////////
// totorial 13

#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

// 버전 선언
#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

class CInputClass
{
public:

	struct S_POINT
	{
		int nX;
		int nY;
	};
	
	CInputClass(void);
	CInputClass(const CInputClass&);
	~CInputClass(void);

	//void Initialize();
	bool Initialize(HINSTANCE hInstance, HWND hWnd, int scW, int scH);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int &posX, int &posY);
	char* GetKeyboardInputData();

	// Sound
	bool IsPlayStatus(UINT _type);


private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

	IDirectInput8 * m_pDirectInput;
	IDirectInputDevice8 * m_pKeyboard;
	IDirectInputDevice8 * m_pMouse;

	char m_strKeyboardInputData[256*5+1];
	unsigned char m_cKeyState[256];
	DIMOUSESTATE m_dMouseState;

	S_POINT m_nScreenSize;
	S_POINT m_nMousePos;

// 	void KeyDown(unsigned int);
// 	void KeyUp(unsigned int);
// 	bool IsKeyDown(unsigned int);
// private:
// 	bool m_keys[256];
};



#endif
