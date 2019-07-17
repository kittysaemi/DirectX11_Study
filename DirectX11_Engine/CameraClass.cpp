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
	카메라의 위치와 회전 정보를 이용하여 뷰 행렬을 갱신.
	up 백터와 위치, 회전 등의 변수를 초기화
	화면의 원점에서 우선 카메라의 x,y,z축의 회전 정도로 카메라의 회전을 수행.
	회전이 잘 이루어지면 카메라를 3D세계의 올바른 위치로 옮겨 놓는다.
	position과 lookAt 그리고 up의 값이 잘설정되었다면, D3DXMatrixLookAtLH함수를 사용하여 현재 카메라의 위치와 회전 정도를 표현하는 뷰 행려를 만든다.
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

