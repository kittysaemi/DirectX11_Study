#include "CameraClass.h"


CCameraClass::CCameraClass(void)
{
	m_nfPosX = m_nfPosY = m_nfPosZ = m_nfRotX = m_nfRotY = m_nfRotZ = 0.0f;
}

CCameraClass::CCameraClass(const CCameraClass & other)
{

}

CCameraClass::~CCameraClass(void)
{
}

void CCameraClass::SetPosition(float x, float y, float z)
{
	m_nfPosX = x;
	m_nfPosY = y;
	m_nfPosZ = z;
	return;
}
void CCameraClass::SetRotation(float x, float y, float z)
{
	m_nfRotX = x;
	m_nfRotY = y;
	m_nfRotZ = z;
}

D3DXVECTOR3 CCameraClass::GetPosition()
{
	return D3DXVECTOR3(m_nfPosX, m_nfPosY, m_nfPosZ);
}
D3DXVECTOR3 CCameraClass::GetRotation()
{
	return D3DXVECTOR3(m_nfRotX, m_nfRotY, m_nfRotZ);
}

/*
	ī�޶��� ��ġ�� ȸ�� ������ �̿��Ͽ� �� ����� ����.
	up ���Ϳ� ��ġ, ȸ�� ���� ������ �ʱ�ȭ
	ȭ���� �������� �켱 ī�޶��� x,y,z���� ȸ�� ������ ī�޶��� ȸ���� ����.
	ȸ���� �� �̷������ ī�޶� 3D������ �ùٸ� ��ġ�� �Ű� ���´�.
	position�� lookAt �׸��� up�� ���� �߼����Ǿ��ٸ�, D3DXMatrixLookAtLH�Լ��� ����Ͽ� ���� ī�޶��� ��ġ�� ȸ�� ������ ǥ���ϴ� �� ����� �����.
*/
void CCameraClass::Render()
{
	// Setup the vector that points upwards.
	D3DXVECTOR3 _upWards;
	_upWards.x = 0.0f;
	_upWards.y = 1.0f;
	_upWards.z = 0.0f;

	// Setup the position of the camera in the world.
	D3DXVECTOR3 _positionCamWorld;
	_positionCamWorld.x = m_nfPosX;
	_positionCamWorld.y = m_nfPosY;
	_positionCamWorld.z = m_nfPosZ;

	// Setup where the camera is looking by default.
	D3DXVECTOR3 _CameralookAt;
	_CameralookAt.x = 0.0f;//0.0f;
	_CameralookAt.y = 0.0f;//0.0f;
	_CameralookAt.z = 1.0f;//1.0f; 

	// Set the yaw(Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	float RadianP = 0.0174532925f;
	float _Xaxis_pitch = m_nfRotX * RadianP;
	float _Yaxis_yaw = m_nfRotY * RadianP;
	float _Zaxis_roll = m_nfRotZ * RadianP;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMATRIX _rotationMatrix;
	D3DXMatrixRotationYawPitchRoll(&_rotationMatrix, _Yaxis_yaw,_Xaxis_pitch,_Zaxis_roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&_CameralookAt, &_CameralookAt, &_rotationMatrix);
	D3DXVec3TransformCoord(&_upWards, &_upWards, &_rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	_CameralookAt = _positionCamWorld + _CameralookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&m_ViewMatrix, &_positionCamWorld, &_CameralookAt, &_upWards);

	return;

}

void CCameraClass::GetViewMatrix(D3DXMATRIX & _matrix)
{
	_matrix = m_ViewMatrix;
	return;
}

