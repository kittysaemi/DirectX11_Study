#include "Fps.h"


CFps::CFps(void)
{
}

CFps::CFps(const CFps& other)
{

}

CFps::~CFps(void)
{
}

void CFps::Initialize()
{
	m_nFps = 0;
	m_nCount = 0;
	m_nlStartTime = timeGetTime();
}

void CFps::Frame()
{
	// 매 프레임마다 호출 됨. 1씩 증가
	// 1초 이상 지난 경우, 프레임수 저장

	m_nCount++;

	if(timeGetTime() >= (m_nlStartTime + 1000))
	{
		m_nFps = m_nCount;
		m_nCount = 0;

		m_nlStartTime = timeGetTime();
	}
}

int CFps::GetFps()
{
	// 직전 1초간의 fps 값
	return m_nFps;
}