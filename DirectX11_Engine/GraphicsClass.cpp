#include "GraphicsClass.h"


CGraphicsClass::CGraphicsClass(void)
{
	// T3
	m_D3D = 0;

	// T4
	m_Camera = 0;
	m_Model = 0;
	//m_ColorShader = 0;

	// T5
	m_TextureShader = 0;
}

CGraphicsClass::CGraphicsClass(const CGraphicsClass& other)
{

}

CGraphicsClass::~CGraphicsClass(void)
{
}

bool CGraphicsClass::Initialize(int scW, int scH, HWND hWnd)
{
	//////////////////////////////////////////////////////////////////////////
	// T3
	// Create the Direct3D object.
	m_D3D = new CD3DClass;
	if(!m_D3D)
	{
		OutputDebugString(L"CD3DClass Create Faild");
		return false;
	}

	// initialize the Direct3D object.
	bool hResult = m_D3D->Initialize(scW, scH, VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR );
	if(!hResult)
	{
		OutputDebugString(L"Could not initialize Direct3D");
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	// T4

	// create the camera object
	m_Camera = new CCameraClass;
	if(!m_Camera)
	{
		OutputDebugStringA("Could not Create Cameraclass");
		return false;
	}

	// set the initial position of the camera.
	// Cam Move
	float CamX = 0.0f, CamY = 0.0f, CamZ = 1.0f;
	CamZ = -10.0f;


	m_Camera->SetPosition(CamX, CamY, CamZ);


	// create the model object.
	m_Model = new CModelClass;
	if(!m_Model)
	{
		OutputDebugStringA("Could not Create Modelclass");
		return false;
	}
	
	// Initialize the model object.
	if(!m_Model->Initialize(m_D3D->GetDevice(), L"../DirectX11_Engine/data/seafloor.dds"))		// T5
//	if(!m_Model->Initialize(m_D3D->GetDevice()))	// T4
	{
		OutputDebugStringA("Could not initialize the model object.");
		return false;
	}
	// create the color shader object.
// 	m_ColorShader = new CColorShaderClass;
// 	if(!m_ColorShader)
// 	{
// 		OutputDebugStringA("Could not Create ColorShaderclass");
// 		return false;
// 	}
// 
// 	// Initialize the color shader object.
// 	if(!m_ColorShader->Initialize(m_D3D->GetDevice(), hWnd))
// 	{
// 		OutputDebugStringA("Could not initialize the color shader object.");
// 		return false;
// 	}
	
	// T5
	// create the texture shader object.
	m_TextureShader = new CTextureshaderclass;
	if(!m_TextureShader)
	{
		return false;
	}

	// Initalize the texture shader object.
	if(!m_TextureShader->Initialize(m_D3D->GetDevice(), hWnd))
	{
		return false;
	}

	return true;

}
void CGraphicsClass::Shutdown()
{
	// The TextureShaderClass object is also released in the shutdown function.
	// Release the thing object.

	//////////////////////////////////////////////////////////////////////////
	// T5
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// T4
// 	if(m_ColorShader)
// 	{
// 		m_ColorShader->Shutdown();
// 		delete m_ColorShader;
// 		m_ColorShader = 0;
// 	}
	if(m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// T3
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
	//////////////////////////////////////////////////////////////////////////

	return;
}

bool CGraphicsClass::Frame()
{
	//////////////////////////////////////////////////////////////////////////
	// T3
	if(!Render())
		return false;
	//////////////////////////////////////////////////////////////////////////
	return true;
}
/*
	Tutorial 4
	Initialize 함수에서 지정한 카메라의 위치를 토대로 뷰 행렬을 만들기 위해 카메라의 Render 함수를 호출한다.
	뷰 행렬이 만들어지면 그것의 복사본을 가져 올 수 있다.
	또한 D3DClass 객체로부터 월드 행렬과 투영 행렬을 복사해 온다.
	그리고나서 ModelClass의 Render 함수를 ㅗ출하여 그래픽 파이프라인에 삼각형 모델을 그리도록 한다. 
	준비한 정점들로 셰이더를 호출하여 셰이더는 모델 정보와 정점을 배치시키기 위한 세 행렬을 사용하여 정점들을 그려낸다.
	삼각형이 백퍼어에 그려진다.
	씬 그리기가 완료되면 EndScene을 호출하여 화면에 표시한다.
*/
bool CGraphicsClass::Render()
{
	//////////////////////////////////////////////////////////////////////////
	// T3
	// Clear the buffers to begin the scene.
		m_D3D->BeginScene(0x5, 0x6, 0x5, 1.0f);	
	//m_D3D->BeginScene(0xff, 0xff, 0, 1.0f); // Yellow

	//////////////////////////////////////////////////////////////////////////
	// T4 Start
	
	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, projection matrices from the camera and d3d objects.
	D3DXMATRIX _viewMatrix;
	m_Camera->GetViewMatrix(_viewMatrix);
	D3DXMATRIX _projectionMatrix;
	m_D3D->GetProjectionMatrix(_projectionMatrix);
	D3DXMATRIX _worldMatrix;
	m_D3D->GetWorldMatrix(_worldMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
 	m_Model->Render(m_D3D->GetDeviceContext());

// 	// Render the model using the color shader.
// 	if(!m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), _worldMatrix, _viewMatrix, _projectionMatrix))
//  	{
// 		OutputDebugStringA(" Graphics Render / ColorShader Render Faild");
// 		return false;
//	}

	// T4 End
	//////////////////////////////////////////////////////////////////////////
	// T5 Start
	// Render the model using the texture shader.
	if(!m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), _worldMatrix, _viewMatrix, _projectionMatrix, m_Model->GetTexture()))
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////


	// Present the rendered scene to the screen.
	m_D3D->EndScene();
	//////////////////////////////////////////////////////////////////////////
	return true;
}

// T3 - saemi
#include <stdio.h>
void CGraphicsClass::GetCardInfo()
{
	if(m_D3D)
	{
		char carName[128];
		ZeroMemory(carName, sizeof(carName));
		int memSize;
		m_D3D->GetVideoCardInfo(carName, memSize);

		char logMessage[1024];
		::memset(logMessage, 0, 1024);
		sprintf_s(logMessage, "CardName : %s \r\n MemorySize : %d", carName, memSize);
		
		OutputDebugStringA(logMessage);
	}
}