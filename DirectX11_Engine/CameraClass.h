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
	struct CameraPosition
	{
		float X;
		float Y;
		float Z;
	};
	struct CameraRotation
	{
		float X;
		float Y;
		float Z;
	};

	CameraPosition m_nfPos;
	CameraRotation m_nfRot; 
	D3DXMATRIX m_ViewMatrix;
};

#endif
