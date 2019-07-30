//////////////////////////////////////////////////////////////////////////
// Tutorial 15
// CPU 사용률 기록 / 매 초 마다 CPU 사용량의 총량 측정

#pragma once
#ifndef _CPU_H_
#define _CPU_H_

#pragma comment(lib, "pdh.lib")

#include <Pdh.h>

class CCpu
{
public:
	CCpu(void);
	CCpu(const CCpu& otehr);
	~CCpu(void);

	void Initialize();
	void Shutdown();
	void Frame();
	int GetCpuPercentage();

private:
	bool m_bCanReadCpu;
	HQUERY m_qHandle;
	HCOUNTER m_cHandle;
	unsigned long m_nlLastTime;
	long m_lCPU_usage;

};

#endif


