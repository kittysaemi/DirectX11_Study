//////////////////////////////////////////////////////////////////////////
/* Tutorial 4

	뷰 행렬을 다루는 클래스.
	현재 월드에서의 카메라의 위치, 보는 방향을 제어하며 이 위치를 필요한 셰이더에 전달한다.
*/


#pragma once
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

#include <D3DX10math.h>

class CCameraClass
{
public:
	CCameraClass(void);
	CCameraClass(const CCameraClass & other);
	~CCameraClass(void);

	// 카메라의 위치와 회전을 정하는데 사용
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	// 카메라의 위치화 회전 정보를 return
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX & _matrix);

private:
	float m_nfPosX;
	float m_nfPosY;
	float m_nfPosZ;
	float m_nfRotX;
	float m_nfRotY;
	float m_nfRotZ;
	D3DXMATRIX m_ViewMatrix;
};

#endif
