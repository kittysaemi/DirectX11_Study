//////////////////////////////////////////////////////////////////////////
// Tutorial 16
// 유저가 왼쪽/오른쪽 버튼을 누를때 시야 방향을 조절하는 기능
// 카메라의 위치를 계산하고 유지하는 일
// 좌, 우, 이외에 다른 움직임을 넣어 확장 가능
// 구현하는 움직임 중에는 부드러운 카메라 이동 효과를 위한 가속이나 감속도 포함 된다.

#pragma once
#ifndef _POSITION_H_
#define _POSITION_H_

#include <math.h>

class CPosition
{
public:
	CPosition(void);
	CPosition(const CPosition& otehr);
	~CPosition(void);

	void SetFrameTIme(float time);
	void GetRotation(float& x, float& y, float& z);

	void TurnLeft(bool keydown);
	void TurnRight(bool keydown);

	void TurnTop(bool keydown);
	void TurnBottom(bool keydown);

	void ZoomIn(bool keydown); // 확대
	void ZoomOut(bool keydown); // 축소

private:
	float m_nfFrameTIme;
	float m_nfRotationX;
	float m_nfRotationY;
	float m_nfRotationZ;
	float m_nfLeftTurnSpeed;
	float m_nfRightTurnSpeed;
	float m_nfTopTurnSpeed;
	float m_nfBottomTurnSpeed;
	float m_nfZoomInSpeed;
	float m_nfZoomOutSpeed;
	
};

#endif
