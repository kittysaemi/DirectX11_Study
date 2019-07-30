#include "GraphicsClass.h"


CGraphicsClass::CGraphicsClass(void)
{
	// T3
	m_D3D = 0;

	// T4
	m_Camera = 0;
	m_Model = 0;
	m_ColorShader = 0;

	// T5
	m_TextureShader = 0;

	// T6
	m_pLightShader = 0;
	m_pLight = 0;

	// T11
	m_Bitmap = 0;

	m_pText = 0;

	// saemi	
	DirectionP[0] = 1.0f;	
	DirectionP[1] = 1.0f;	
	DirectionP[2] = 1.0f;	
						// Yellow	// gray
	BGColor[0] = 0.0f;	// 0xff		// 0x5
	BGColor[1] = 0.0f;	// 0xff		// 0x6
	BGColor[2] = 0.0f;	// 0x00		// 0x5
	BGColor[3] = 1.0f;	
					//���	//���	// ���ֻ�
	LightColor[0] =	1.0f;	// 0.3f;	// 1.0f;
	LightColor[1] =	1.0f;		//	1.0f;	// 0.0f;
	LightColor[2] =	1.0f;		//0.3f;	// 1.0f;
	LightColor[3] =	1.0f;		//1.0f;	// 1.0f;

	// �ֺ���					//15%
	AmbientColor[0] = 0.2f;		//0.15f;
	AmbientColor[1] = 0.2f;		//0.15f;
	AmbientColor[2] = 0.2f;		//0.15f;
	AmbientColor[3] = 1.0f;

	// �ݻ��, �ݻ簭��
	SpecularColor[0] = 1.0f;
	SpecularColor[1] = 1.0f;
	SpecularColor[2] = 1.0f;
	SpecularColor[3] = 1.0f;
	SpecularPower = 32.0f;

	ZeroMemory(m_sSoundStatusMsg, sizeof(m_sSoundStatusMsg));

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


	if(TUTORIALTYPE < 11)
	{

		// set the initial position of the camera.
		m_Camera->SetPosition(0, 0, -10);

		// create the model object.
		m_Model = new CModelClass;
		if(!m_Model)
		{
			OutputDebugStringA("Could not Create Modelclass");
			return false;
		}


		// Initialize the model object.
		if(TUTORIALTYPE <= 4)
		{
			if(!m_Model->Initialize(m_D3D->GetDevice()))
			{
				return false;
			}
			// create the color shader object.
			m_ColorShader = new CColorShaderClass;
			if(!m_ColorShader)
			{
				OutputDebugStringA("Could not Create ColorShaderclass");
				return false;
			}

			// Initialize the color shader object.
			if(!m_ColorShader->Initialize(m_D3D->GetDevice(), hWnd))
			{
				OutputDebugStringA("Could not initialize the color shader object.");
				return false;
			}

			return true;
		}
		if(TUTORIALTYPE >= 5 && TUTORIALTYPE <= 6)
		{	
			if(!m_Model->Initialize(m_D3D->GetDevice(), L"../DirectX11_Engine/data/seafloor.dds"))	
			{
				OutputDebugStringA("Could not initialize the model object.");
				return false;
			}		
		}
		if(TUTORIALTYPE >= 7)
		{
			//if(!m_Model->Initialize(m_D3D->GetDevice(), "../DirectX11_Engine/data/Cube.txt", L"../DirectX11_Engine/data/wood.dds" ))
			//if(!m_Model->Initialize(m_D3D->GetDevice(), "../DirectX11_Engine/data/Moon.txt", L"../DirectX11_Engine/data/wood.dds" ))
			if(!m_Model->Initialize(m_D3D->GetDevice(), "../DirectX11_Engine/data/MoonV.txt", L"../DirectX11_Engine/data/MoonM.dds" ))
			{
				return false;
			}
		}

		if(TUTORIALTYPE == 5)
		{
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

		// T6
		// Create the light shader object.
		m_pLightShader = new CLightshaderClass;
		if(!m_pLightShader)
		{
			return false;
		}

		// Initialize the light shader object.
		if(!m_pLightShader->Initialize(m_D3D->GetDevice(), hWnd, TUTORIALTYPE))
		{
			return false;
		}

		// Create the light object.
		m_pLight = new CLightClass;
		if(!m_pLight)
		{
			return false;
		}


		m_Camera->SetPosition(0, 0, -300);
		// ������ ������ ���ֻ����� �ϰ� �� ������ z���� ���� ������ �մϴ�.
		// T9 - �ֺ��� �߰�
		m_pLight->SetAmbientColor(AmbientColor[0], AmbientColor[1], AmbientColor[2], AmbientColor[3]);

		// Initialize the light object.
		m_pLight->SetDiffuseColor(LightColor[0], LightColor[1], LightColor[2], LightColor[3]);
		m_pLight->SetDirection(DirectionP[0], DirectionP[1], DirectionP[2]);

		// T10 �ݻ籤
		m_pLight->SetSpecularColor(SpecularColor[0], SpecularColor[1], SpecularColor[2], SpecularColor[3]);
		m_pLight->SetSpecularPower(SpecularPower);
	}
	else if(TUTORIALTYPE == 11)
	{	
		// set the initial position of the camera.
		m_Camera->SetPosition(0, 0, -10);

		// create the texture shader object.
		m_TextureShader = new CTextureshaderclass;
		if(!m_TextureShader)
			return false;

		// Initalize the texture shader object.
		if(!m_TextureShader->Initialize(m_D3D->GetDevice(), hWnd))
			return false;

		m_Bitmap = new CBitmapClass;
		if(!m_Bitmap)
			return false;

		if(!m_Bitmap->Initialize(m_D3D->GetDevice(), scW, scH, L"../DirectX11_Engine/data/wood.dds", 1200, 800))
			return false;
	}
	else if(TUTORIALTYPE >= 12)
	{
		// set the initial position of the camera.
		m_Camera->SetPosition(0, 0, -1);
		m_Camera->Render();

		D3DXMATRIX baseViewMatrix;
		m_Camera->GetViewMatrix(baseViewMatrix);

		m_pText = new CText;
		if(!m_pText)
			return false;

		CText::TextPoint info;
		info.sW = scW;
		info.sH = scH;

		if(!m_pText->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hWnd, info, baseViewMatrix))
			return false;

	}
	else
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
	// T12
	if(m_pText)
	{
		m_pText->Shutdown();
		delete m_pText;
		m_pText = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// T11
	if(m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

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
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// T4
	if(m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}
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

bool CGraphicsClass::Frame(SInputPosInfo data)
{
	m_pInputData = data;

	if(!Render())
		return false;

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
	m_D3D->BeginScene(BGColor[0], BGColor[1], BGColor[2], BGColor[3]);	

	//////////////////////////////////////////////////////////////////////////
	// T4 Start
	
	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, projection matrices from the camera and d3d objects.
	D3DXMATRIX _viewMatrix;
	m_Camera->GetViewMatrix(_viewMatrix);
	D3DXMATRIX _worldMatrix;
	m_D3D->GetWorldMatrix(_worldMatrix);
	D3DXMATRIX _projectionMatrix;
	m_D3D->GetProjectionMatrix(_projectionMatrix);
	D3DXMATRIX _orthoMatrix;
	m_D3D->GetOtherMatrix(_orthoMatrix);


	if(TUTORIALTYPE < 11)
	{
		if(TUTORIALTYPE >= 6)
		{
			// Update the rotation variable each frame.
			static float rotation = 0.0f;

			rotation += (float)D3DX_PI * 0.001f;	// �ӵ� ���̱�
			//rotation += (float)D3DX_PI * 0.01f;
			if(rotation > 360.0f)
			{
				rotation -= 360.0f;
			}

			// ���� ����� ȸ������ŭ ȸ������ �� ����� �̿��Ͽ� �ﰢ���� �׸� �� �� ����ŭ ȸ���Ǿ� ���̰� �Ѵ�.
			// Rotate the world matrix by the rotation value so that the triangle will spin.
			D3DXMatrixRotationY(&_worldMatrix, rotation);
		}

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_Model->Render(m_D3D->GetDeviceContext());


		// Ʃ�丮�� ������ ����, ���� ��� ����
		if(TUTORIALTYPE <= 4)
		{
			// Render the model using the color shader.
			if(!m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), _worldMatrix, _viewMatrix, _projectionMatrix))
			{
				OutputDebugStringA(" Graphics Render / ColorShader Render Faild");
				return false;
			}
		}

		if(TUTORIALTYPE == 5 )
		{	
			// Render the model using the texture shader.
			if(!m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), _worldMatrix, _viewMatrix, _projectionMatrix, m_Model->GetTexture()))
			{
				return false;
			}
		}

		if(TUTORIALTYPE >= 6 && TUTORIALTYPE <= 8)
		{
			if(!m_pLightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), _worldMatrix, _viewMatrix, _projectionMatrix, m_Model->GetTexture(), m_pLight->GetDirection(), m_pLight->GetDiffuseColor()))
			{
				return false;
			}
		}

		if(TUTORIALTYPE == 9)
		{
			if(!m_pLightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), _worldMatrix, _viewMatrix, _projectionMatrix, m_Model->GetTexture(), m_pLight->GetDirection(), m_pLight->GetDiffuseColor(), m_pLight->GetAmbientColor() ))
			{
				return false;
			}
		}

		if(TUTORIALTYPE == 10)
		{
			if(!m_pLightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), _worldMatrix, _viewMatrix, _projectionMatrix, m_Model->GetTexture(), m_pLight->GetDirection(), m_pLight->GetDiffuseColor(), m_pLight->GetAmbientColor(), m_Camera->GetPosition(), m_pLight->GetSpecularColor(), m_pLight->GetSpecularPower()))
			{
				return false;
			}
		}

	}
	else if(TUTORIALTYPE >= 11)
	{
		D3DXMATRIX otherMatrix;
		m_D3D->GetOtherMatrix(otherMatrix);

		m_D3D->TurnZBufferOff();

		if(TUTORIALTYPE == 11)
		{

			if(!m_Bitmap->Render(m_D3D->GetDeviceContext(), m_pInputData.nPosX, m_pInputData.nPosY))
				return false;

			if(!m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), _worldMatrix, _viewMatrix, otherMatrix, m_Bitmap->GetTexture()))
				return false;

		}
		else if(TUTORIALTYPE >= 12)
		{
			m_D3D->TurnOnAlphaBlending();

			// FPS, timer, CPU
			m_pText->SetTimer(m_D3D->GetDeviceContext(), m_pInputData.nCurTime, 0);
			m_pText->SetFps(m_D3D->GetDeviceContext(), m_pInputData.nFPS, 1);
			m_pText->SetCPUpersentage(m_D3D->GetDeviceContext(), m_pInputData.nCPUper, 2);

			// Sound 
			m_pText->SetPlayStatusMsg(m_D3D->GetDeviceContext(), m_sSoundStatusMsg);

			// ���콺 ������ ���� 
			m_pText->SetMousePosition(m_pInputData.nPosX, m_pInputData.nPosY, m_D3D->GetDeviceContext());
			
			m_pText->SetKeyBoardInputData(m_D3D->GetDeviceContext(), m_pInputData.sBuffer);

			m_pText->Render(m_D3D->GetDeviceContext(), _worldMatrix, _orthoMatrix);


			m_Camera->SetPosition(0,0,-10);

			m_D3D->TurnOffAlphaBlending();
		}

		m_D3D->TurnZBufferOn();

	}
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

void CGraphicsClass::SetPlayStatus(char * _Msg)
{
	strcpy(m_sSoundStatusMsg, _Msg);
}