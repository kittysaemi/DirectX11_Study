#pragma once

//////////////////////////////////////////////////////////////////////////
// tutorial 2

#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_


class CInputClass
{
public:
	CInputClass(void);
	CInputClass(const CInputClass&);
	~CInputClass(void);

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool m_keys[256];
};



#endif
