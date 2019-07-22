#include "LightClass.h"


CLightClass::CLightClass(void)
{
}

CLightClass::CLightClass(const CLightClass& otehr)
{

}

CLightClass::~CLightClass(void)
{
}

void CLightClass::SetDiffuseColor(float r, float g, float b, float a)
{
	m_diffuseColor = D3DXVECTOR4(r,g,b,a);
	return;
}

void CLightClass::SetDirection(float x, float y, float z)
{
	m_direction = D3DXVECTOR3(x,y,z);
	return;
}
void CLightClass::SetAmbientColor(float r, float g, float b, float a)
{
	m_ambientColor = D3DXVECTOR4(r,g,b,a);
	return;
}

D3DXVECTOR3 CLightClass::GetDirection(){return m_direction;}
D3DXVECTOR4 CLightClass::GetDiffuseColor(){return m_diffuseColor;}
D3DXVECTOR4 CLightClass::GetAmbientColor(){return m_ambientColor;}