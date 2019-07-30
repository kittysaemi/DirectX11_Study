//////////////////////////////////////////////////////////////////////////
// Tutorial 15
// CPU ���� ��� / �� �� ���� CPU ��뷮�� �ѷ� ����

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


