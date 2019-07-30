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
	// �� �����Ӹ��� ȣ�� ��. 1�� ����
	// 1�� �̻� ���� ���, �����Ӽ� ����

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
	// ���� 1�ʰ��� fps ��
	return m_nFps;
}