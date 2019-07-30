//////////////////////////////////////////////////////////////////////////
// Tutorial 15
// 고해상도 타이머
// 실행 중 매 프레임 간의 정확한 시간을 재는 고해상도 타이머 클래스
// 오브젝트의 이동을 위해 프레임의 표준 시간대와의 동기화이다.
// 직접 써먹을 일은 없지만 어떻게 적용할 수 있는지 보여주는 코드
// 프레임간 간격을 1초에 대한 %로 바꿔서 물체를 그만큼 움직이도록 하는 경우에 많이 사용된다.

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
