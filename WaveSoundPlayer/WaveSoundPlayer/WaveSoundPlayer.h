
// WaveSoundPlayer.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CWaveSoundPlayerApp:
// �� Ŭ������ ������ ���ؼ��� WaveSoundPlayer.cpp�� �����Ͻʽÿ�.
//

class CWaveSoundPlayerApp : public CWinApp
{
public:
	CWaveSoundPlayerApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CWaveSoundPlayerApp theApp;