// Filename: applicationclass.cpp

#include "applicationclass.h"
#include <string.h>
#include <format>
//#define SingleModel
#define MultiModel

ApplicationClass::ApplicationClass()
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_Models = 0;

	m_TextureShader = 0;
	m_Sprite = 0;
	m_Timer = 0;

	m_LightShader = 0;
	m_DirectionalLight = 0;

	m_Models = 0;
	m_modelCount = 0;

	m_Lights = 0;
}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{

}


ApplicationClass::~ApplicationClass()
{

}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	char spriteFilename[128];

	char modelFilename[128];
	char textureFilename[128];
	bool result;

	// Create and initialize Direct3D object.
	m_Direct3D = new D3DClass;

	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 5.0f, -12.0f);
	//m_Camera->SetRotation(20.0f, 0.0f, 0.0f);

#ifdef SingleModel
	// Set the file name of the model.
	strcpy_s(modelFilename, "../DirectXEngine/data/monkey.obj");//"../DirectXEngine/data/cube.txt");

	// Set the name of the texture file tha twe will be loading.
	strcpy_s(textureFilename, "../DirectXEngine/data/stone01.tga");

	// Create and initialize the model object.
	m_Model = new ModelClass;

	result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
#endif  // SingleModel

#ifdef MultiModel
	//const char* modelFilenames[1] = { "../DirectXEngine/data/plane.txt" }; //"../DirectXEngine/data/monkey.obj", "../DirectXEngine/data/cube.txt", "../DirectXEngine/data/sphere.txt",
	//const char* textureFilenames[1] = { "../DirectXEngine/data/stone01.tga" }; //"../DirectXEngine/data/stone02.tga", "../DirectXEngine/data/stone01.tga", "../DirectXEngine/data/stone01.tga",

	//m_modelCount = sizeof(modelFilenames) / sizeof(modelFilenames[0]);

	//m_Models = new ModelClass * [m_modelCount];
	//for (size_t i = 0; i < m_modelCount; i++)
	//{
	//	ModelClass* model = new ModelClass;

	//	strcpy_s(modelFilename, modelFilenames[i]);
	//	strcpy_s(textureFilename, textureFilenames[i]);


	//	result = model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename);
	//	model->ApplyTransformations(XMMatrixTranslation(0.0f, 0.0f, 0.0f),
	//		XMMatrixScaling(1.0f, 1.0f, 1.0f), 0.0f);

	//	if (!result)
	//	{
	//		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
	//		return false;
	//	}

	//	m_Models[i] = model;
	//}

#endif // MultiModel

	// Create and initialize the light shader object.
	m_LightShader = new LightShaderClass;

	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the light object.
	m_DirectionalLight = new LightClass;

	m_DirectionalLight->SetAmbientColor(0.25f, 0.25f, 0.25f, 1.0f);
	m_DirectionalLight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirectionalLight->SetDirection(0.0f, 1.0f, 1.0f);
	m_DirectionalLight->SetSpecularColor(0.75f, 0.75f, 0.75f, 1.0f);
	m_DirectionalLight->SetSpecularPower(16.0f);

	// Set the number of lights we will use.
	m_numLights = 5;

	// Create and initialize the light objects array.
	m_Lights = new LightClass[m_numLights];

	// Manually set the color and position of each light.
	m_Lights[0].SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);  // Red
	m_Lights[0].SetPosition(-3.0f, 1.0f, 3.0f);
	//m_Lights[0].SetPosition(0.0f, 1.0f, 0.0f);

	m_Lights[1].SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);  // Green
	m_Lights[1].SetPosition(3.0f, 1.0f, 3.0f);
	//m_Lights[1].SetPosition(0.0f, 1.0f, 0.0f);

	m_Lights[2].SetDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);  // Blue
	m_Lights[2].SetPosition(-3.0f, 1.0f, -3.0f);
	//m_Lights[2].SetPosition(0.0f, 1.0f, 0.0f);

	m_Lights[3].SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);  // White
	m_Lights[3].SetPosition(3.0f, 1.0f, -3.0f);

	m_Lights[4].SetDiffuseColor(0.5f, 0.0f, 0.5f, 1.0f);  // Purple
	m_Lights[4].SetPosition(0.0f, 1.0f, 0.0f);

	// Create and initialize the texture shader object.
	m_TextureShader = new TextureShaderClass;

	result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
	}

	// Set the sprite info file we will be using.
	strcpy_s(spriteFilename, "../DirectXEngine/data/sprite_data_01.txt");

	// Create and initialize the sprite object.
	m_Sprite = new SpriteClass;

	result = m_Sprite->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, spriteFilename, 50, 50);
	if (!result)
	{
		return false;
	}

	// Create and initialize the timer object.
	m_Timer = new TimerClass;

	result = m_Timer->Initialize();
	if (!result)
	{
		return false;
	}

	return true;
}


void ApplicationClass::Shutdown()
{

	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the sprite object.
	if (m_Sprite)
	{
		m_Sprite->Shutdown();
		delete m_Sprite;
		m_Sprite = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the light objects.
	if (m_Lights)
	{
		delete[] m_Lights;
		m_Lights = 0;
	}

	// Release the light object.
	if (m_DirectionalLight)
	{
		delete m_DirectionalLight;
		m_DirectionalLight = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	if (m_Models)
	{
		for (size_t i = 0; i < m_modelCount; i++)
		{
			m_Models[i]->Shutdown();
		}
		delete[] m_Models;
		m_Models = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D;
	}

	return;
}


bool ApplicationClass::Frame()
{
	float frameTime;
	//static float rotation = 0.0f;
	bool result;

	// Update the system stats.
	m_Timer->Frame();

	// Get the current frame time.
	frameTime = m_Timer->GetTime();

	// Update the sprite object using the frame time.
	m_Sprite->Update(frameTime);

	// Update rotation variable each frame.
	//rotation -= 0.0174532925f * 0.1f;
	//if (rotation < 0.0f)
	//{
	//	rotation += 360.0f;
	//}

	// Render the scene.
	result = Render(frameTime);
	if (!result)
	{
		return false;
	}

	return true;
}

XMMATRIX ApplyTransformations(XMMATRIX translation, XMMATRIX scale, XMMATRIX rotation)
{
	XMMATRIX worldMatrix, srMatrix;
	srMatrix = XMMatrixMultiply(scale, rotation);
	worldMatrix = XMMatrixMultiply(srMatrix, translation);
	return worldMatrix;
}

bool ApplicationClass::Render(float frameTime)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, rotationMatrix, translateMatrix, scaleMatrix, srMatrix;
	XMMATRIX UIWorldMatrix, UIViewMatrix, orthoMatrix;

	XMFLOAT4 diffuseColors[5], lightPositions[5];
	int i;

	bool result;

	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, projection matrices from the camera and d3d objects.
	//m_Direct3D->GetWorldMatrix(worldMatrix);
	//m_Camera->GetViewMatrix(viewMatrix);
	//m_Direct3D->GetProjectionMatrix(projectionMatrix);

	m_Direct3D->GetWorldMatrix(UIWorldMatrix);
	m_Camera->GetViewMatrix(UIViewMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// Get the light properties.
	for (i = 0; i < m_numLights; i++)
	{
		// Create the diffuse color array from the four light colors.
		diffuseColors[i] = m_Lights[i].GetDiffuseColor();

		// Create the light position array from the four light positions.
		lightPositions[i] = m_Lights[i].GetPosition();
	}

#ifdef SingleModel
	// Apply transforms.
	// Multiply them together to create the final world transformation matrix.
	worldMatrix = ApplyTransformations(XMMatrixTranslation(-3.0f, 0.0f, 0.0f),
		XMMatrixScaling(1.0f, 1.0f, 1.0f),
		XMMatrixRotationY(rotation));

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// Render the model using the color shader.
	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result)
	{
		return false;
	}
#endif // SingleModel

#ifdef MultiModel
	//for (size_t i = 0; i < m_modelCount; i++)
	//{
		//m_Models[i]->ApplyTransformations(m_Models[i]->GetPosition(), m_Models[i]->GetScale(), 0);
	//	worldMatrix = m_Models[i]->GetTransform();

		//m_Models[i]->Render(m_Direct3D->GetDeviceContext());
	//	// Render the model using the color shader.
	//	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Models[i]->GetTexture(),
	//		m_Camera, m_DirectionalLight, diffuseColors, lightPositions);

	//	if (!result)
	//	{
	//		return false;
	//	}
	//}
#endif // MultiModel

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Put the sprite vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Sprite->Render(m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Render the sprite with the texture shader.
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Sprite->GetIndexCount(), UIWorldMatrix, UIViewMatrix, orthoMatrix, m_Sprite->GetTexture());
	if (!result)
	{
		return false;
	}

	//m_Bitmap->SetSize(100, 100);

	// Turn on Z Buffer for 3D rendering.
	m_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}