//////////////////////////////////////////////////////////////////////////
// Tutorial 16
// ÇÁ·¯½ºÅÒ ÄÃ¸µ ±â´É Ä¸½¶È­
// 

#pragma once
#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include <D3DX10math.h>

class CFrustum
{
public:
	CFrustum(void);
	CFrustum(const CFrustum& otehr);
	~CFrustum(void);

	void ConstructFrustum(float scDepth, D3DXMATRIX projectionMatrix, D3DXMATRIX viewMatrix );

	bool CheckPoint(float x, float y, float z);
	bool CheckCube(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);

private:
	D3DXPLANE m_pPlanes[6];
};

#endif
