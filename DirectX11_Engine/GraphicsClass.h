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

// tutorial 11
#include "BitmapClass.h"

// tutorial 12
#include "Text.h"

//////////////////////////////////////////////////////////////////////////

const bool FULL_SCREEN = false;//true;
const bool VSYNC_ENABLED = true;//false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const int TUTORIALTYPE = 13;

class CGraphicsClass
{
public:
	struct SInputPosInfo
	{
		int nPosX;
		int nPosY;

		char * sBuffer;
	};


	CGraphicsClass(void);
	CGraphicsClass(const CGraphicsClass&);
	~CGraphicsClass(void);

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(SInputPosInfo data);

	// saemi
	void GetCardInfo();
	void SetPlayStatus(char * _Msg);

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

	// T11
	CBitmapClass * m_Bitmap;

	// T12
	CText * m_pText;

	// saemi
	float DirectionP[3];
	float BGColor[4];
	float LightColor[4];
	float AmbientColor[4];
	float SpecularColor[4];	// 반사색
	float SpecularPower;	// 반사강도

	SInputPosInfo m_pInputData;

	char m_sSoundStatusMsg[256];

};


#endif
