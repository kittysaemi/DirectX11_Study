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
	//m_TextureShader = 0;

	// T6
	m_pLightShader = 0;
	m_pLight = 0;

	// saemi			// Yellow	// gray
	backColorR = 0.5f;	// 0xff		// 0x5
	backColorG = 0.6f;	// 0xff		// 0x6
	backColorB = 0.0f;	// 0x00		// 0x5
	backColorA = 1.0f;	

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
// 	m_TextureShader = new CTextureshaderclass;
// 	if(!m_TextureShader)
// 	{
// 		return false;
// 	}
// 
// 	// Initalize the texture shader object.
// 	if(!m_TextureShader->Initialize(m_D3D->GetDevice(), hWnd))
// 	{
// 		return false;
// 	}

	// T6
	// Create the light shader object.
	m_pLightShader = new CLightshaderClass;
	if(!m_pLightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	if(!m_pLightShader->Initialize(m_D3D->GetDevice(), hWnd))
	{
		return false;
	}

	// Create the light object.
	m_pLight = new CLightClass;
	if(!m_pLight)
	{
		return false;
	}

	// ������ ������ ���ֻ����� �ϰ� �� ������ z���� ���� ������ �մϴ�.
	// Initialize the light object.
	m_pLight->SetDiffuseColor(1.0f, 0.0f, 1.0f, 1.0f);
	m_pLight->SetDirection(0.0f, 0.0f, 1.0f);


	return true;

}
void CGraphicsClass::Shutdown()
{
	// The TextureShaderClass object is also released in the shutdown function.
	// Release the thing object.

	//////////////////////////////////////////////////////////////////////////
	// T6
	if(m_pLight)
	{
		delete m_pLight;
		m_pLight = 0;
	}
	if(m_pLightShader)
	{
		m_pLightShader->Shutdown();
		delete m_pLightShader;
		m_pLightShader = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// T5
// 	if(m_TextureShader)
// 	{
// 		m_TextureShader->Shutdown();
// 		delete m_TextureShader;
// 		m_TextureShader = 0;
// 	}

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
	// T6
	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.01f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	if(!Render(rotation))
	{

		return false;
	}
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// T3
//	if(!Render())
//		return false;
	//////////////////////////////////////////////////////////////////////////
	return true;
}
/*
	Tutorial 4
	Initialize �Լ����� ������ ī�޶��� ��ġ�� ���� �� ����� ����� ���� ī�޶��� Render �Լ��� ȣ���Ѵ�.
	�� ����� ��������� �װ��� ���纻�� ���� �� �� �ִ�.
	���� D3DClass ��ü�κ��� ���� ��İ� ���� ����� ������ �´�.
	�׸����� ModelClass�� Render �Լ��� �����Ͽ� �׷��� ���������ο� �ﰢ�� ���� �׸����� �Ѵ�. 
	�غ��� ������� ���̴��� ȣ���Ͽ� ���̴��� �� ������ ������ ��ġ��Ű�� ���� �� ����� ����Ͽ� �������� �׷�����.
	�ﰢ���� ���۾ �׷�����.
	�� �׸��Ⱑ �Ϸ�Ǹ� EndScene�� ȣ���Ͽ� ȭ�鿡 ǥ���Ѵ�.
*/
bool CGraphicsClass::Render()
{
	//////////////////////////////////////////////////////////////////////////
	// T3
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(backColorR, backColorG, backColorB, backColorA);	

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
// 	if(!m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), _worldMatrix, _viewMatrix, _projectionMatrix, m_Model->GetTexture()))
// 	{
// 		return false;
// 	}
	//////////////////////////////////////////////////////////////////////////


	// Present the rendered scene to the screen.
	m_D3D->EndScene();
	//////////////////////////////////////////////////////////////////////////
	return true;
}
bool CGraphicsClass::Render(float _rotation)
{
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(backColorR, backColorG, backColorB, backColorA);	

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, projection matrices from the camera and d3d objects.
	D3DXMATRIX _viewMatrix;
	m_Camera->GetViewMatrix(_viewMatrix);
	D3DXMATRIX _worldMatrix;
	m_D3D->GetWorldMatrix(_worldMatrix);
	D3DXMATRIX _projectionMatrix;
	m_D3D->GetProjectionMatrix(_projectionMatrix);

	// ���� ����� ȸ������ŭ ȸ������ �� ����� �̿��Ͽ� �ﰢ���� �׸� �� �� ����ŭ ȸ���Ǿ� ���̰� �Ѵ�.
	// Rotate the world matrix by the rotation value so that the triangle will spin.
	D3DXMatrixRotationY(&_worldMatrix, _rotation);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext());

	// ���� ���̴��� �ﰢ���� �׸��� ���� ȣ�� �ȴ�.
	// �� ���� ��ü�� Render �Լ��� ������ ����� ������ �Ѱ��־� ���̴��� �� ���� ���� �� �ְ� �Ѵ�.
	// Render the model using the light shader.
	if(!m_pLightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), _worldMatrix, _viewMatrix, _projectionMatrix, m_Model->GetTexture(), m_pLight->GetDirection(), m_pLight->GetDiffuseColor()))
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

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