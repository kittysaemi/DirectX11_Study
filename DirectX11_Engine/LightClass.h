//////////////////////////////////////////////////////////////////////////
// Tutorial 6
// ���� ����� ������ ������ �����ϴ� Ŭ����


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

	void SetDiffuseColor(float r, float g, float b, float a);
	void SetDirection(float x, float y, float z);

	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();

private:
	D3DXVECTOR4 m_diffuseColor;
	D3DXVECTOR3 m_direction;
};

#endif


