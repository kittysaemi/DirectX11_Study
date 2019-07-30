//////////////////////////////////////////////////////////////////////////
// Tutorial 16
// ������ ����/������ ��ư�� ������ �þ� ������ �����ϴ� ���
// ī�޶��� ��ġ�� ����ϰ� �����ϴ� ��
// ��, ��, �̿ܿ� �ٸ� �������� �־� Ȯ�� ����
// �����ϴ� ������ �߿��� �ε巯�� ī�޶� �̵� ȿ���� ���� �����̳� ���ӵ� ���� �ȴ�.

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

	void ZoomIn(bool keydown); // Ȯ��
	void ZoomOut(bool keydown); // ���

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
