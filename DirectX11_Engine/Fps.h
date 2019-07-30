//////////////////////////////////////////////////////////////////////////
// Tutorial 15
// 프로그램이 실행되는 동안의 초당 프레임 수(FPS, frames per second)를 기록.
// 초당 몇 frame을 그리는지에 대한 정보는 프로그램의 성능을 측정하는데 좋은 기준.
// 렌더링 속도
// 일반적 fps 60 -> 30밑으로 떨어지면 뚝뚝 끊기는 것처럼 보임.
// * 단순히 타이머가 있는 카운터 / 1초의 시간 동안 얼만큼의 프레임들이 그려지는지 세고 정기적으로 숫자를 갱신
// *VSYNC_ENABLED 사용여부에 따라 달라진다.

#pragma once
#ifndef _FPS_H_
#define _FPS_H_

#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <MMSystem.h>

class CFps
{
public:
	CFps(void);
	CFps(const CFps& other);
	~CFps(void);

	void Initialize();
	void Frame();
	int GetFps();

private:
	int m_nFps;
	int m_nCount;
	unsigned long m_nlStartTime;
};

#endif

