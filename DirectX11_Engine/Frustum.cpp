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
	// Graphics에 의해 매 프레임마다 호출.
	
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
	// 하나의 점이 시야 프러스텀 내부에 있는지 확인
	// 가장 일반적이지만 올바른 환경에서 사용되면 또한 가장 효율적이기도 하다.
	// 점의 좌표를 입력으로 받고 모든 여섯 개의 평면 안에 점이 있는지를 확인한다.
	// 리턴 값은 포함 여부

	for(int i=0; i<6; i++)
	{
		if(D3DXPlaneDotCoord(&m_pPlanes[i], &D3DXVECTOR3(x,y,z)) < 0.0f)
			return false;
	}

	return true;
}

bool CFrustum::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
	// 정육면체의 8개의 꼭지점 중 어느 것이라도 시야 프러스텀 내에 있는지 확인
	// 입력으로는 육면체의 중점과 반경만 있으면 되며 이를 이용하여 여덟개의 꼭지점 위치를 계산할 수 있다.
	// 꼭지점 하나라도 프러스텀 내부에 있다면 true

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
	// 구의 중심이 시야 프러스텀의 여섯평면에 반경 이내만큼 존재하는지를 확인.
	// 어느 것이라도 바깥에 있으면 구체는 보이지 않는 것이고 함수는 false를 리턴할 것이다.
	// 모든 평면의 안쪽에 있다면 구체는 보이는 것이므로 true

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