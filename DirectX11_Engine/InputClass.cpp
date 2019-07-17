#include "InputClass.h"


CInputClass::CInputClass(void)
{
}

CInputClass::CInputClass(const CInputClass& other)
{

}

CInputClass::~CInputClass(void)
{
}

void CInputClass::Initialize()
{
	for(int i=0; i<256; i++)
		m_keys[i] = false;
}

void CInputClass::KeyDown(unsigned int key)
{
	m_keys[key] = true;
}

void CInputClass::KeyUp(unsigned int key)
{
	m_keys[key] = false;
}
bool CInputClass::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}

