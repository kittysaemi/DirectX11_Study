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
	// �ý����� ���ػ� Ÿ�̸Ӹ� �����ϴ��� Ȯ���ϰ�, Ÿ�̸��� �ֱ⸦ ������ �� ���� ������ �и��ʸ��� ī���Ϳ��� ����̳� ƽ�� �Ͼ���� ����Ѵ�.
	// ��� �� ������ �����Ӱ� �ð��� ����� �� �ִ�.

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
	// ���� ���α׷����� �� �������� ȣ�� �ؾ� �ϴ� �Լ�.
	// �������� �ð��� ���̸� ����ؼ� ������ ���� �ð� ������ �˼� �ִ�.
	
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