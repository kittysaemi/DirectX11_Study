//////////////////////////////////////////////////////////////////////////
// Tutorial 15
// ���ػ� Ÿ�̸�
// ���� �� �� ������ ���� ��Ȯ�� �ð��� ��� ���ػ� Ÿ�̸� Ŭ����
// ������Ʈ�� �̵��� ���� �������� ǥ�� �ð������ ����ȭ�̴�.
// ���� ����� ���� ������ ��� ������ �� �ִ��� �����ִ� �ڵ�
// �����Ӱ� ������ 1�ʿ� ���� %�� �ٲ㼭 ��ü�� �׸�ŭ �����̵��� �ϴ� ��쿡 ���� ���ȴ�.

#pragma once
#ifndef _Timer_H_
#define _Timer_H_

#include <Windows.h>

class CTimer
{
public:
	CTimer(void);
	CTimer(const CTimer& otehr);
	~CTimer(void);

	bool Initialize();
	void Frame();

	float GetTIme();

private:
	INT64 m_Frequency;
	float m_TicksPerMs;
	INT64 m_StartTIme;
	float m_FrameTime;

};

#endif
