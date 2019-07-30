//////////////////////////////////////////////////////////////////////////
// Tutorial 15
// ���α׷��� ����Ǵ� ������ �ʴ� ������ ��(FPS, frames per second)�� ���.
// �ʴ� �� frame�� �׸������� ���� ������ ���α׷��� ������ �����ϴµ� ���� ����.
// ������ �ӵ�
// �Ϲ��� fps 60 -> 30������ �������� �Ҷ� ����� ��ó�� ����.
// * �ܼ��� Ÿ�̸Ӱ� �ִ� ī���� / 1���� �ð� ���� ��ŭ�� �����ӵ��� �׷������� ���� ���������� ���ڸ� ����
// *VSYNC_ENABLED ��뿩�ο� ���� �޶�����.

#pragma once
#ifndef _FPS_H_
#define _FPS_H_

#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <MMSystem.h>

class CFps
{
public:
	CFps(void);
	CFps(const CFps& other);
	~CFps(void);

	void Initialize();
	void Frame();
	int GetFps();

private:
	int m_nFps;
	int m_nCount;
	unsigned long m_nlStartTime;
};

#endif

