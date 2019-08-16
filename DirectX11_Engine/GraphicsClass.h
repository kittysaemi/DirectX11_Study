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

// tutorial 16
// 프러스텀 컬링 
#include "ModelList.h"
#include "Frustum.h"

// tutorial 17
#include "MultiTextureShader.h"

// tutorial 18
#include "LightMapShader.h"

// tutorial 19
#include "AlphaMapShader.h"

// tutorial 20
#include "BumpMapShader.h"

// tutorial 21
#include "SpecMapShader.h"

// tutorial 22
#include "DebugWindow.h"
#include "RenderTexture.h"

//////////////////////////////////////////////////////////////////////////

const bool FULL_SCREEN = true;//true;
const bool VSYNC_ENABLED = false;//false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const int TUTORIALTYPE = 22;

class CGraphicsClass
{
public:
	struct SInputPosInfo
	{
		SInputPosInfo()
		{
			nPosX = nPosY = nCPUper = nFPS = 0;
			nCurTime = nRotationY = nRotationX = 0.0f;
			nRotationZ = -10.0f;
		}
		SInputPosInfo(const SInputPosInfo& data)
		{
			nPosX = data.nPosX;
			nPosY = data.nPosY;
			nRotationY = data.nRotationY;
			nRotationX = data.nRotationX;
			nRotationZ = data.nRotationZ;

			sBuffer = data.sBuffer;

			nCurTime = data.nCurTime;
			nCPUper = data.nCPUper;
			nFPS = data.nFPS;
		}

		int nPosX;
		int nPosY;
		float nRotationY;
		float nRotationX;
		float nRotationZ;

		char * sBuffer;

		float nCurTime;
		int nCPUper;
		int nFPS;
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
	bool RenderToTexture();
	bool RenderScene();

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

	// T16
	CModelList * m_pModelList;
	CFrustum * m_pFrustum;

	// T17
	CMultiTextureShader * m_pMultiTextureShader;

	// T18
	CLightMapShader * m_pLightMapShader;

	// T19
	CAlphaMapShader * m_pAlphaMapShader;

	// T20
	CBumpMapShader * m_pBumpMapShader;

	// T21
	CSpecMapShader * m_pSpecMapShader;

	// T22
	CRenderTexture * m_pRenderTexture;
	CDebugWindow * m_pDebugWindow;

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
