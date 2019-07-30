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
	// Cpu 사용량을 질의하기 위해 필요한 핸들을 초기화.
	// 질의(Query) : Cpu의 사용량 대신 시스템의 모든 cpu들의 사용량 총합을 합하여 돌려준다.
	// 일부 OS에서는 권한때문에 이 기능을 사용할 수 없을 수 있다.

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
	// 1초마다 cpu 사용량을 샘플링
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
	// 현재 cpu 사용량
	if(m_bCanReadCpu)
		return (int)m_lCPU_usage;
	
	return 0;
}