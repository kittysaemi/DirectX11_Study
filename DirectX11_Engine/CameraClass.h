//////////////////////////////////////////////////////////////////////////
/* Tutorial 4

	�� ����� �ٷ�� Ŭ����.
	���� ���忡���� ī�޶��� ��ġ, ���� ������ �����ϸ� �� ��ġ�� �ʿ��� ���̴��� �����Ѵ�.
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

	// ī�޶��� ��ġ�� ȸ���� ���ϴµ� ���
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	// ī�޶��� ��ġȭ ȸ�� ������ return
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
