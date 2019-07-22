#pragma once

//////////////////////////////////////////////////////////////////////////
// tutorial 2

#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

//////////////////////////////////////////////////////////////////////////
// tutorial 2 -> 3 Chanage
//#include <Windows.h>
#include "D3DClass.h"

// tutorial 4 
#include "CameraClass.h"
#include "ModelClass.h"
#include "ColorShaderClass.h" 

// tutorial 5
#include "Textureshaderclass.h"

// tutorial 6
#include "LightshaderClass.h"	
#include "LightClass.h"

//////////////////////////////////////////////////////////////////////////

const bool FULL_SCREEN = true;//true;
const bool VSYNC_ENABLED = false;//false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const int TUTORIALTYPE = 9;

class CGraphicsClass
{
public:
	CGraphicsClass(void);
	CGraphicsClass(const CGraphicsClass&);
	~CGraphicsClass(void);

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

	// saemi
	void GetCardInfo();

private:
	bool Render();

	// tutorial 3 - T3
	CD3DClass* m_D3D;

	// tutorial 4 - T4
	CCameraClass* m_Camera;
	CModelClass* m_Model;
	CColorShaderClass * m_ColorShader; 

	// T5
	CTextureshaderclass * m_TextureShader; 

	// T6
	CLightshaderClass* m_pLightShader;
	CLightClass* m_pLight;

	// saemi
	float BGColor[4];
	float LightColor[4];
	float AmbientColor[4];
};


#endif
