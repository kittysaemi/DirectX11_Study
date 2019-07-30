#include "Timer.h"


CTimer::CTimer(void)
{
}

CTimer::CTimer(const CTimer& otehr)
{

}

CTimer::~CTimer(void)
{
}

bool CTimer::Initialize()
{
	// 시스템이 고해상도 타이머를 지원하는지 확인하고, 타이머의 주기를 받으면 이 값을 가지고 밀리초마다 카운터에서 몇번이나 틱이 일어나는지 계산한다.
	// 계산 된 값으로 프레임간 시간을 계산할 수 있다.

	// Check to see if this system supports high performance timers.
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
	if(m_Frequency == 0)
		return false;

	// Find out how many times the frequency counter ticks every millisecond.
	m_TicksPerMs = (float)(m_Frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_StartTIme);

	return true;
}

void CTimer::Frame()
{
	// 메인 프로그램에서 매 루프마다 호출 해야 하는 함수.
	// 루프마다 시간의 차이를 계싼해서 프레임 간의 시간 간격을 알수 있다.
	
	INT64 curTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);

	float timeDiff = (float)(curTime - m_StartTIme);

	m_FrameTime = timeDiff / m_TicksPerMs;

	m_StartTIme = curTime;	

}

float CTimer::GetTIme()
{
	return m_FrameTime;
}