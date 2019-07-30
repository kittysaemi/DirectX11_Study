#include "Cpu.h"


CCpu::CCpu(void)
{
}

CCpu::CCpu(const CCpu& otehr)
{

}

CCpu::~CCpu(void)
{
}

void CCpu::Initialize()
{
	// Cpu ��뷮�� �����ϱ� ���� �ʿ��� �ڵ��� �ʱ�ȭ.
	// ����(Query) : Cpu�� ��뷮 ��� �ý����� ��� cpu���� ��뷮 ������ ���Ͽ� �����ش�.
	// �Ϻ� OS������ ���Ѷ����� �� ����� ����� �� ���� �� �ִ�.

	// Initialize the flag indicating whether this object can read the system cpu usage or not.
	m_bCanReadCpu = true;

	// Create a query object to poll cpu usage.
	if(PdhOpenQuery(NULL, 0, &m_qHandle) != ERROR_SUCCESS)
		m_bCanReadCpu = false;

	// Set query object to poll all cpus in the system.
	if(PdhAddCounter(m_qHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &m_cHandle) != ERROR_SUCCESS)
		m_bCanReadCpu = false;

	m_nlLastTime = GetTickCount();
	m_lCPU_usage = 0; 
}

void CCpu::Shutdown()
{
	if(m_bCanReadCpu)
	{
		PdhCloseQuery(m_qHandle);
	}
}

void CCpu::Frame()
{
	// 1�ʸ��� cpu ��뷮�� ���ø�
	if(m_bCanReadCpu)
	{
		if((m_nlLastTime + 1000) < GetTickCount())
		{
			m_nlLastTime = GetTickCount();
			
			PdhCollectQueryData(m_qHandle);

			PDH_FMT_COUNTERVALUE value;
			PdhGetFormattedCounterValue(m_cHandle, PDH_FMT_LONG, NULL, &value);

			m_lCPU_usage = value.longValue;
		}
	}
}

int CCpu::GetCpuPercentage()
{
	// ���� cpu ��뷮
	if(m_bCanReadCpu)
		return (int)m_lCPU_usage;
	
	return 0;
}