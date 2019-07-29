#pragma once

//////////////////////////////////////////////////////////////////////////
// tutorial 2

//////////////////////////////////////////////////////////////////////////
// tutorial 13
// DirectInput 추가

//////////////////////////////////////////////////////////////////////////
// tutorial 14
// DirectSound 추가

#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include "GraphicsClass.h"
#include "InputClass.h"
#include "SoundClass.h"

class CSystemClass
{
public:
	CSystemClass(void);
	CSystemClass(const CSystemClass&);
	~CSystemClass(void);

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWIndows(int&, int&);
	void ShutdownWindows();

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	
	CInputClass * m_pInput;
	CGraphicsClass * m_pGraphics;
	CSoundClass * m_pSound;

};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static CSystemClass* ApplicationHandle = 0;

#endif
