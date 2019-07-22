//////////////////////////////////////////////////////////////////////////
// Tutorial 6
// 빛의 방향과 색상을 가지고 유지하는 클래스


#pragma once
#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_

#include <D3DX10math.h>

class CLightClass
{
public:
	CLightClass(void);
	CLightClass(const CLightClass& otehr);
	~CLightClass(void);

	void SetAmbientColor(float r, float g, float b, float a);
	void SetDiffuseColor(float r, float g, float b, float a);
	void SetDirection(float x, float y, float z);

	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();
	D3DXVECTOR4 GetAmbientColor();

private:
	D3DXVECTOR4 m_ambientColor;
	D3DXVECTOR4 m_diffuseColor;
	D3DXVECTOR3 m_direction;
};

#endif


