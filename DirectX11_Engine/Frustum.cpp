#include "Frustum.h"


CFrustum::CFrustum(void)
{
	ZeroMemory(m_pPlanes, sizeof(m_pPlanes[0]));
}

CFrustum::CFrustum(const CFrustum& otehr)
{

}

CFrustum::~CFrustum(void)
{
}

void CFrustum::ConstructFrustum(float scDepth, D3DXMATRIX projectionMatrix, D3DXMATRIX viewMatrix )
{
	// Graphics�� ���� �� �����Ӹ��� ȣ��.
	
	// Calculate the minimum Z distance in the frustum.
	float zMinimum = -projectionMatrix._43 / projectionMatrix._33;
	float r = scDepth / (scDepth - zMinimum);
	projectionMatrix._33 = r;
	projectionMatrix._43 = -r * zMinimum;

	// Create the frustum matrix from the view matrix and updated projection matrix.
	D3DXMATRIX matrix;
	D3DXMatrixMultiply(&matrix, &viewMatrix, &projectionMatrix);


	float a[6] = {	/*near*/	matrix._14 + matrix._13,	/*far*/		matrix._14 - matrix._13, 
					/*left*/	matrix._14 + matrix._11,	/*right*/	matrix._14 - matrix._11, 
					/*top*/		matrix._14 - matrix._12,	/*bottom*/	matrix._14 + matrix._12 };
	float b[6] = {	/*near*/	matrix._24 + matrix._23,	/*far*/		matrix._24 - matrix._23, 
					/*left*/	matrix._24 + matrix._21,	/*right*/	matrix._24 - matrix._21, 
					/*top*/		matrix._24 - matrix._22,	/*bottom*/	matrix._24 + matrix._22 };
	float c[6] = {	/*near*/	matrix._34 + matrix._33,	/*far*/		matrix._34 - matrix._33, 
					/*left*/	matrix._34 + matrix._31,	/*right*/	matrix._34 - matrix._31, 
					/*top*/		matrix._34 - matrix._32,	/*bottom*/	matrix._34 + matrix._32 };
	float d[6] = {	/*near*/	matrix._44 + matrix._43,	/*far*/		matrix._44 - matrix._43, 
					/*left*/	matrix._44 + matrix._41,	/*right*/	matrix._44 - matrix._41, 
					/*top*/		matrix._44 - matrix._42,	/*bottom*/	matrix._44 + matrix._42 };

	// Calculate (near, far, left, right, top, bottom) plane of frustum.
	for(int i=0; i<6; i++)
	{
		m_pPlanes[i].a = a[i];
		m_pPlanes[i].b = b[i];
		m_pPlanes[i].c = c[i];
		m_pPlanes[i].d = d[i];

		D3DXPlaneNormalize(&m_pPlanes[i], &m_pPlanes[i]);
	}
}

bool CFrustum::CheckPoint(float x, float y, float z)
{
	// �ϳ��� ���� �þ� �������� ���ο� �ִ��� Ȯ��
	// ���� �Ϲ��������� �ùٸ� ȯ�濡�� ���Ǹ� ���� ���� ȿ�����̱⵵ �ϴ�.
	// ���� ��ǥ�� �Է����� �ް� ��� ���� ���� ��� �ȿ� ���� �ִ����� Ȯ���Ѵ�.
	// ���� ���� ���� ����

	for(int i=0; i<6; i++)
	{
		if(D3DXPlaneDotCoord(&m_pPlanes[i], &D3DXVECTOR3(x,y,z)) < 0.0f)
			return false;
	}

	return true;
}

bool CFrustum::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
	// ������ü�� 8���� ������ �� ��� ���̶� �þ� �������� ���� �ִ��� Ȯ��
	// �Է����δ� ����ü�� ������ �ݰ游 ������ �Ǹ� �̸� �̿��Ͽ� �������� ������ ��ġ�� ����� �� �ִ�.
	// ������ �ϳ��� �������� ���ο� �ִٸ� true

	for(int i = 0; i < 6; i++)
	{
		if(D3DXPlaneDotCoord(&m_pPlanes[i], &D3DXVECTOR3((xCenter - radius), (yCenter - radius), (zCenter - radius ))) >= 0.0f)
			continue;

		if(D3DXPlaneDotCoord(&m_pPlanes[i], &D3DXVECTOR3((xCenter + radius), (yCenter - radius), (zCenter - radius ))) >= 0.0f)
			continue;

		if(D3DXPlaneDotCoord(&m_pPlanes[i], &D3DXVECTOR3((xCenter - radius), (yCenter + radius), (zCenter - radius ))) >= 0.0f)
			continue;

		if(D3DXPlaneDotCoord(&m_pPlanes[i], &D3DXVECTOR3((xCenter + radius), (yCenter + radius), (zCenter - radius ))) >= 0.0f)
			continue;

		if(D3DXPlaneDotCoord(&m_pPlanes[i], &D3DXVECTOR3((xCenter - radius), (yCenter - radius), (zCenter + radius ))) >= 0.0f)
			continue;

		if(D3DXPlaneDotCoord(&m_pPlanes[i], &D3DXVECTOR3((xCenter + radius), (yCenter - radius), (zCenter + radius ))) >= 0.0f)
			continue;

		if(D3DXPlaneDotCoord(&m_pPlanes[i], &D3DXVECTOR3((xCenter - radius), (yCenter + radius), (zCenter + radius ))) >= 0.0f)
			continue;

		if(D3DXPlaneDotCoord(&m_pPlanes[i], &D3DXVECTOR3((xCenter + radius), (yCenter + radius), (zCenter + radius ))) >= 0.0f)
			continue;

		return false;
	}

	return true;
}

bool CFrustum::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	// ���� �߽��� �þ� ���������� ������鿡 �ݰ� �̳���ŭ �����ϴ����� Ȯ��.
	// ��� ���̶� �ٱ��� ������ ��ü�� ������ �ʴ� ���̰� �Լ��� false�� ������ ���̴�.
	// ��� ����� ���ʿ� �ִٸ� ��ü�� ���̴� ���̹Ƿ� true

	for(int i=0; i<6; i++)
	{
		if(D3DXPlaneDotCoord(&m_pPlanes[i], &D3DXVECTOR3(xCenter, yCenter, zCenter)) < -radius)
			return false;
	}

	return true;
}

bool CFrustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	for(int i=0; i<6; i++)
	{
		if(D3DXPlaneDotCoord(&m_pPlanes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter - ySize), (zCenter - zSize))) >= 0.0f)
			continue;
		if(D3DXPlaneDotCoord(&m_pPlanes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter - ySize), (zCenter - zSize))) >= 0.0f)
			continue;
		if(D3DXPlaneDotCoord(&m_pPlanes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter + ySize), (zCenter - zSize))) >= 0.0f)
			continue;
		if(D3DXPlaneDotCoord(&m_pPlanes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter - ySize), (zCenter + zSize))) >= 0.0f)
			continue;
		if(D3DXPlaneDotCoord(&m_pPlanes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter + ySize), (zCenter - zSize))) >= 0.0f)
			continue;
		if(D3DXPlaneDotCoord(&m_pPlanes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter - ySize), (zCenter + zSize))) >= 0.0f)
			continue;
		if(D3DXPlaneDotCoord(&m_pPlanes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter + ySize), (zCenter + zSize))) >= 0.0f)
			continue;
		if(D3DXPlaneDotCoord(&m_pPlanes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter + ySize), (zCenter + zSize))) >= 0.0f)
			continue;

		return false;
	}

	return true;
}