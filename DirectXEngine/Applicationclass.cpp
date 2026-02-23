// Filename: applicationclass.cpp

#include "applicationclass.h"
#include <string.h>
#include <format>
//#define SingleModel
//#define MultiModel

ApplicationClass::ApplicationClass()
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_Models = 0;

	m_TextureShader = 0;
	m_Sprite = 0;
	m_Timer = 0;

	m_DirectionalLight = 0;

	m_Models = 0;
	m_modelCount = 0;

	m_FontShader = 0;
	m_Font = 0;

	m_Fps = 0;
	m_FpsString = 0;
	m_MouseBitmap = 0;

	m_ShaderManager = 0;

	m_GroundModel = 0;
	m_CubeModel = 0;

	m_ParticleSystem = 0;
	m_ParticleShader = 0;

	m_ProjectionShader = 0;
	m_ProjectionTexture = 0;
	m_Viewpoint = 0;

	m_DepthShader = 0;
	//m_ShadowShader = 0;

	m_LightData = 0;

	m_RenderTexture = 0;
	m_FullScreenWindow = 0;

	m_Blur = 0;
	m_BlurShader = 0;
	m_depthToBWShadowMap = 0;

	m_TreeTrunkModel = 0;
	m_TreeLeafModel = 0;
	m_TransparentDepthShader = 0;
	m_screenWidth = 0;
	m_screenHeight = 0;
	m_DeferredBuffers = 0;
	m_LightShader = 0;
	m_DeferredShader = 0;
}

ApplicationClass::ApplicationClass(const ApplicationClass& other)
{

}

ApplicationClass::~ApplicationClass()
{

}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	char fpsString[32];
	char spriteFilename[128];
	char modelFilename[128], modelFilename2[128], textureFilename1[128], textureFilename2[128], textureFilename3[128], textureFilename4[128];
	char textureFilename[128];
	char renderString[32];
	char mouseString1[32], mouseString2[32], mouseString3[32];

	bool result;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

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
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();
	m_Camera->RenderBaseViewMatrix();
	m_Camera->GetBaseViewMatrix(m_baseViewMatrix);

	m_Camera->SetRotation(35.0f, 0.0f, 0.0f);
	m_Camera->SetPosition(0.0f, 8.0f, -25.0f);
	//m_Camera->SetRotation(20.0f, 0.0f, 0.0f);
	m_Camera->Render();
	//m_Camera->SetPosition(0.0f, 0.0f, 0.0f);

	// Create and initialize the light object.
	m_DirectionalLight = new LightClass;

	m_DirectionalLight->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_DirectionalLight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirectionalLight->SetDirection(0.0f, -1.0f, 0.5f);
	m_DirectionalLight->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirectionalLight->SetSpecularPower(16.0f);
	m_DirectionalLight->SetLookAt(0.0f, 0.0f, 0.0f);
	m_DirectionalLight->GenerateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);

	// Set the number of lights we will use.
	m_numLights = 5;

	// Create and Initialize the normal map shader object.
	m_ShaderManager = new ShaderManagerClass;

	result = m_ShaderManager->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		return false;
	}

	// Create and initialize the tree trunk model object.
	m_TreeTrunkModel = new ModelClass;

	strcpy_s(modelFilename, "../DirectXEngine/data/trunk001.txt");
	strcpy_s(textureFilename, "../DirectXEngine/data/trunk001.tga");

	result = m_TreeTrunkModel->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename, textureFilename, textureFilename);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the tree trunk model object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the tree leaf model object.
	m_TreeLeafModel = new ModelClass;

	strcpy_s(modelFilename, "../DirectXEngine/data/leaf001.txt");
	strcpy_s(textureFilename, "../DirectXEngine/data/leaf001.tga");

	result = m_TreeLeafModel->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename, textureFilename, textureFilename);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the tree leaf model object.", L"Error", MB_OK);
		return false;
	}

	strcpy_s(modelFilename, "../DirectXEngine/data/sphere.txt");
	strcpy_s(textureFilename, "../DirectXEngine/data/ice.tga");

	// Create and initialize the ground model object.
	m_SphereModel = new ModelClass;
	result = m_SphereModel->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename, textureFilename, textureFilename);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sphere model object.", L"Error", MB_OK);
		return false;
	}

	strcpy_s(modelFilename, "../DirectXEngine/data/plane.txt");
	strcpy_s(textureFilename, "../DirectXEngine/data/metal001.tga");

	// Create and initialize the ground model object.
	m_GroundModel = new ModelClass;

	result = m_GroundModel->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename, textureFilename, textureFilename);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the ground model object.", L"Error", MB_OK);
		return false;
	}

	strcpy_s(modelFilename, "../DirectXEngine/data/monkey.obj");
	strcpy_s(textureFilename, "../DirectXEngine/data/wall01.tga");//stone01.tga");

	// Create and initialize the cube model object.
	m_CubeModel = new ModelClass;

	result = m_CubeModel->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename, textureFilename, textureFilename);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the cube model object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the projection shader object.
	m_ProjectionShader = new ProjectionShaderClass;

	result = m_ProjectionShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the projection shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the projection texture object.
	m_ProjectionTexture = new TextureClass;

	strcpy_s(textureFilename, "../DirectXEngine/data/grate.tga");

	result = m_ProjectionTexture->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), textureFilename);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the projection texture object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the view port object.
	m_Viewpoint = new ViewpointClass;

	m_Viewpoint->SetPosition(2.0f, 5.0f, -2.0f);
	m_Viewpoint->SetLookAt(0.0f, 0.0f, 0.0f);
	m_Viewpoint->SetProjectionParameters((3.14159265358979323846f / 2.0f), 1.0f, 0.1f, 100.0f); //(3.14159265358979323846f / 2.0f)
	m_Viewpoint->GenerateViewMatrix();
	m_Viewpoint->GenerateProjectionMatrix();

	// Create and initialize the timer object.
	m_Timer = new TimerClass;

	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Create and initializr the font shader object.
	m_FontShader = new FontShaderClass;

	result = m_FontShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the font object.
	m_Font = new FontClass;

	result = m_Font->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), 0);
	if (!result)
	{
		return false;
	}

	// Create and initialize the fps object.
	m_Fps = new FPSClass();

	m_Fps->Initialize();

	// Set the initial fps and fps string.
	m_previousFps = -1;
	strcpy_s(fpsString, "Fps 0");

	// Create and initialize the text object for the fps string.
	m_FpsString = new TextClass();

	result = m_FpsString->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, m_Font, fpsString, 10, 10, 0.0f, 1.0f, 0.0f);
	if (!result)
	{
		return false;
	}

	// Create and initialize the mouse bitmap object.
	m_MouseBitmap = new BitmapClass;

	strcpy_s(textureFilename, "../DirectXEngine/data/mouse.tga");

	result = m_MouseBitmap->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, textureFilename, 50, 50);
	if (!result)
	{
		return false;
	}

	// Create and initialize the texture shader object.
	m_TextureShader = new TextureShaderClass;

	result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the render to texture object.
	m_RenderTexture = new RenderTextureClass;

	result = m_RenderTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH, 0);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the render texture object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the full screen ortho window object.
	m_FullScreenWindow = new OrthoWindowClass;

	result = m_FullScreenWindow->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the full screen ortho window object.", L"Error", MB_OK);
		return false;
	}

	// Set the size to sample down to.
	int downSampleWidth = SHADOWMAP_WIDTH / 2;
	int downSampleHeight = SHADOWMAP_HEIGHT / 2;

	// Create and initialize the blur object.
	m_Blur = new BlurClass;

	result = m_Blur->Initialize(m_Direct3D, downSampleWidth, downSampleHeight, SCREEN_NEAR, SCREEN_DEPTH, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the blur object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the blur shader object.
	m_BlurShader = new BlurShaderClass;

	result = m_BlurShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the blur shader object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the depth shader object.
	m_DepthShader = new DepthShaderClass;

	result = m_DepthShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the depth shader object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the shadow shader object.
	//m_ShadowShader = new ShadowShaderClass;

	//result = m_ShadowShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the shadow shader object.", L"Error", MB_OK);
	//	return false;
	//}

	m_depthToBWShadowMap = new DepthToBWShadowMap;

	result = m_depthToBWShadowMap->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the depth to BW shader object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the transparent depth shader object.
	m_TransparentDepthShader = new TransparentDepthShaderClass;

	result = m_TransparentDepthShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the transparent depth shader object.", L"Error", MB_OK);
		return false;
	}

	m_LightShader = new LightShaderClass;

	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the deferred buffers object.
	m_DeferredBuffers = new DeferredBuffersClass;

	result = m_DeferredBuffers->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the deferred buffers object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the deferred shader object.
	m_DeferredShader = new DeferredShaderClass;

	result = m_DeferredShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the deferred shader object.", L"Error", MB_OK);
		return false;
	}

	// Set the shadow map bias to fix the floating point precision issues (shadow acne/lines artifacts).
	m_shadowMapBias = 0.0022f;
	m_LightData = new LightData;
	m_LightData->Initialize(m_shadowMapBias);
	m_LightData->InitializeShadowMap(m_Direct3D->GetDevice(), hwnd, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR, 1, NUM_LIGHTS);

	LightClass* light = new LightClass;
	light->InitializeShadowMaps(hwnd, m_Direct3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR, 1);
	light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	light->SetDiffuseColor(0.2f, 0.2f, 0.75f, 1.0f);
	light->SetPosition(0, 1.0f, -5.0f);
	light->SetLookAt(0.0f, 0.0f, 0.0f);
	light->GenerateViewMatrix();
	light->GenerateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);
	m_LightData->RegisterPointLight(light);

	light = new LightClass;
	light->InitializeShadowMaps(hwnd, m_Direct3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR, 1);
	light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	light->SetDiffuseColor(0.2f, 0.75f, 0.2f, 1.0f);
	light->SetPosition(0, 1.0f, -20.0f);
	light->SetLookAt(0.0f, 0.0f, 0.0f);
	light->GenerateViewMatrix();
	light->GenerateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);
	m_LightData->RegisterPointLight(light);

	light = new LightClass;
	light->InitializeShadowMaps(hwnd, m_Direct3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR, 1);
	light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	light->SetDiffuseColor(1.0f, 1.0f, 0.78f, 1.0f);
	light->SetPosition(-5.0f, 3.0f, 5.0f);
	light->SetLookAt(0.0f, 0.0f, 0.0f);
	light->GenerateViewMatrix();
	light->GenerateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);
	m_LightData->RegisterPointLight(light);

	light = new LightClass;
	light->InitializeShadowMaps(hwnd, m_Direct3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR, 1);
	light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	light->SetDiffuseColor(1.0f, 1.0f, 0.78f, 1.0f);
	light->SetPosition(12.0f, 1.0f, -5.0f);
	light->SetLookAt(0.0f, 0.0f, 0.0f);
	light->GenerateViewMatrix();
	light->GenerateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);
	m_LightData->RegisterPointLight(light);

	light = new LightClass;
	light->InitializeShadowMaps(hwnd, m_Direct3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR, 1);
	light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	light->SetDiffuseColor(0.75f, 0.75f, 1.0f, 1.0f);
	light->SetDirection(0.0f, -0.5f, 1.0f);
	light->SetPosition(0.0, 8.0f, -0.1f);
	light->SetLookAt(0.0f, 0.0f, 0.0f);
	light->GenerateViewMatrix();
	light->GenerateOrthoMatrix(30.0f, SCREEN_DEPTH, SCREEN_NEAR);
	m_LightData->RegisterDirectionalLight(light);

	return true;
}

void ApplicationClass::Shutdown()
{

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the deferred shader object.
	if (m_DeferredShader)
	{
		m_DeferredShader->Shutdown();
		delete m_DeferredShader;
		m_DeferredShader = 0;
	}

	// Release the deferred buffers object.
	if (m_DeferredBuffers)
	{
		m_DeferredBuffers->Shutdown();
		delete m_DeferredBuffers;
		m_DeferredBuffers = 0;
	}

	if (m_MouseBitmap)
	{
		m_MouseBitmap->Shutdown();
		delete m_MouseBitmap;
		m_MouseBitmap = 0;
	}

	if (m_TreeTrunkModel)
	{
		m_TreeTrunkModel->Shutdown();
		delete m_TreeTrunkModel;
		m_TreeTrunkModel = 0;
	}

	if (m_TreeLeafModel)
	{
		m_TreeLeafModel->Shutdown();
		delete m_TreeLeafModel;
		m_TreeLeafModel = 0;
	}

	if (m_TransparentDepthShader)
	{
		m_TransparentDepthShader->Shutdown();
		delete m_TransparentDepthShader;
		m_TransparentDepthShader = 0;
	}

	if (m_depthToBWShadowMap)
	{
		m_depthToBWShadowMap->Shutdown();
		delete m_depthToBWShadowMap;
		m_depthToBWShadowMap = 0;
	}

	// Release the blur shader object.
	if (m_BlurShader)
	{
		m_BlurShader->Shutdown();
		delete m_BlurShader;
		m_BlurShader = 0;
	}

	// Release the blur object.
	if (m_Blur)
	{
		m_Blur->Shutdown();
		delete m_Blur;
		m_Blur = 0;
	}

	// Release the full screen ortho window object.
	if (m_FullScreenWindow)
	{
		m_FullScreenWindow->Shutdown();
		delete m_FullScreenWindow;
		m_FullScreenWindow = 0;
	}

	// Release the render texture object.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	if (m_LightData)
	{
		for (int i = 0; i < m_LightDataSize; i++)
		{
			m_LightData[i].Shutdown();
		}
		m_LightData = 0;
	}

	// Release the depth shader object.
	if (m_DepthShader)
	{
		m_DepthShader->Shutdown();
		delete m_DepthShader;
		m_DepthShader = 0;
	}

	// Release the view point object.
	if (m_Viewpoint)
	{
		delete m_Viewpoint;
		m_Viewpoint = 0;
	}

	// Release the projection texture object.
	if (m_ProjectionTexture)
	{
		m_ProjectionTexture->Shutdown();
		delete m_ProjectionTexture;
		m_ProjectionTexture = 0;
	}

	// Release the projection shader object.
	if (m_ProjectionShader)
	{
		m_ProjectionShader->Shutdown();
		delete m_ProjectionShader;
		m_ProjectionShader = 0;
	}

	// Release the cube model object.
	if (m_CubeModel)
	{
		m_CubeModel->Shutdown();
		delete m_CubeModel;
		m_CubeModel = 0;
	}

	// Release the particle shader object.
	if (m_ParticleShader)
	{
		m_ParticleShader->Shutdown();
		delete m_ParticleShader;
		m_ParticleShader = 0;
	}

	// Release the particle system object.
	if (m_ParticleSystem)
	{
		m_ParticleSystem->Shutdown();
		delete m_ParticleSystem;
		m_ParticleSystem = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the ground model object.
	if (m_GroundModel)
	{
		m_GroundModel->Shutdown();
		delete m_GroundModel;
		m_GroundModel = 0;
	}

	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the shader manager object.
	if (m_ShaderManager)
	{
		m_ShaderManager->Shutdown();
		delete m_ShaderManager;
		m_ShaderManager = 0;
	}

	// Release the text object for the fps string.
	if (m_FpsString)
	{
		m_FpsString->Shutdown();
		delete m_FpsString;
		m_FpsString = 0;
	}

	// Release the fps object.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	// Release the font object.
	if (m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}

	// Release the font shader object.
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

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

	// Release the light object.
	if (m_DirectionalLight)
	{
		delete m_DirectionalLight;
		m_DirectionalLight = 0;
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

	// Release the ground model object.
	if (m_GroundModel)
	{
		m_GroundModel->Shutdown();
		delete m_GroundModel;
		m_GroundModel = 0;
	}

	// Release the sphere model object.
	if (m_SphereModel)
	{
		m_SphereModel->Shutdown();
		delete m_SphereModel;
		m_SphereModel = 0;
	}

	// Release the cube model object.
	if (m_CubeModel)
	{
		m_CubeModel->Shutdown();
		delete m_CubeModel;
		m_CubeModel = 0;
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

static float rotation = 360.0f;

bool ApplicationClass::Frame(InputClass* Input)
{
	static float lightPositionX = -5.0f;

	int mouseX, mouseY;
	bool result, mouseDown, keyDown, intersect;
	char testString[32];
	float frameTime, cameraRotationY;

	// Check if the user pressed escape and wants to exit the application.
	if (Input->IsEscapePressed())
	{
		return false;
	}

	// Get the location of the mouse from the input object.
	Input->GetMouseLocation(mouseX, mouseY);

	// Update the location of the mouse cursor on th screen.
	m_MouseBitmap->SetRenderLocation(mouseX, mouseY);

	// Update the system stats.
	m_Timer->Frame();

	// Get the current frame time.
	frameTime = m_Timer->GetTime();

	// Update the frame per second each frame.
	result = UpdateFPS();
	if (!result)
	{
		return false;
	}

	// Update the rotation variable each frame.
	rotation -= 0.0174532925f * 0.0025f;
	if (rotation <= 0.0f)
	{
		rotation += 360.0f;
	}

	static float lightAngle = 270.0f;
	float radians;
	static float lightPosX = 10.0f;

	// Update the position of the light each frame.
	lightPosX -= 0.03f * frameTime * 100.0f;

	// Update the angle of the light each frame.
	lightAngle -= 0.03f * frameTime * 100.0f;
	if (lightPosX < -10.0f)
	{
		lightAngle = 270.0f;

		// Reset the light position also.
		lightPosX = 9.0f;
	}
	radians = lightAngle * 0.0174532925f;



	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		LightClass* light = m_LightData->GetAllPointLights()[i];
		result = RenderDepthToTexture(light->GetShadowMap(), light);
		if (!result)
		{
			return false;
		}


		result = RenderBlackAndWhiteShadows(m_LightData->GetShadowMaps(), light, light->GetShadowMap(), i);
		if (!result)
		{
			return false;
		}


		//result = m_Blur->BlurTexture(m_Direct3D, m_Camera, light->m_BlackWhiteRenderTexture, m_TextureShader, m_BlurShader);
		//if (!result)
		//{
		//	return false;
		//}
	}

	// Render deferred buffer.
	result = RenderSceneDeferredBuffer(rotation);
	if (!result)
	{
		return false;
	}

	// Render the scene.
	result = Render();
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

bool ApplicationClass::RenderDepthToTexture(RenderTextureClass* renderTexture, LightClass* light)
{
	XMMATRIX translateMatrix, lightViewMatrix, lightProjectionMatrix;
	bool result;

	// Set the render target to be the render to texture.  Also clear the render to texture.
	renderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());
	renderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Get the view and orthographic matrices from the light object.
	light->GetViewMatrix(lightViewMatrix);
	if (light->IsOrtho)
	{
		light->GetOrthoMatrix(lightProjectionMatrix);
	}
	else
	{
		light->GetProjectionMatrix(lightProjectionMatrix);
	}

	// Setup the translation matrix for the tree model.
	translateMatrix = XMMatrixTranslation(0.0f, -1.0f, -5.0f);
	translateMatrix = ApplyTransformations(translateMatrix,
		XMMatrixScaling(3.0f, 3.0f, 3.0f), 
		XMMatrixRotationX(0.0f));

	// Render the tree trunk model using the depth shader.
	m_GroundModel->Render(m_Direct3D->GetDeviceContext());

	result = m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_GroundModel->GetIndexCount(), translateMatrix, lightViewMatrix, lightProjectionMatrix);
	if (!result)
	{
		return false;
	}

	translateMatrix = XMMatrixTranslation(0.0f, 0.0f, -5.0f);
	translateMatrix = ApplyTransformations(translateMatrix, XMMatrixScaling(1.020f, 1.020f, 1.020f), XMMatrixRotationY(rotation));

	// Render the tree leaf model using the transparent depth shader.
	m_CubeModel->Render(m_Direct3D->GetDeviceContext());

	result = m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_CubeModel->GetIndexCount(), translateMatrix, lightViewMatrix, lightProjectionMatrix);
	if (!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.  Also reset the viewport back to the original.
	m_Direct3D->SetBackBufferRenderTarget();
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::RenderBlackAndWhiteShadows(RenderTextureArrayClass* blackWhiteRenderTexture, LightClass* light, RenderTextureClass* depthTexture, int index)
{
	XMMATRIX translateMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix;
	bool result;

	// Set the render target to be render to texture. Also clear the render to texture.
	blackWhiteRenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), index);
	blackWhiteRenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f, index);

	// Get the view matrix from the camera, and get the projection matrix from the Direct3D object.
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Get the View and orthographic matrices from the light object.
	light->GetViewMatrix(lightViewMatrix);
	if (light->IsOrtho)
	{
		light->GetOrthoMatrix(lightProjectionMatrix);
	}
	else
	{
		light->GetProjectionMatrix(lightProjectionMatrix);
	}


	// Setup the translation matrix for the tree model.
	translateMatrix = XMMatrixTranslation(0.0f, -1.0f, -5.0f);
	translateMatrix = ApplyTransformations(translateMatrix,
		XMMatrixScaling(3.0f, 3.0f, 3.0f),
		XMMatrixRotationX(0.0f));

	// Render the tree trunk model using the depth shader.
	m_GroundModel->Render(m_Direct3D->GetDeviceContext());

	result = m_depthToBWShadowMap->Render(m_Direct3D->GetDeviceContext(), m_GroundModel->GetIndexCount(), translateMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix,
		depthTexture->GetShaderResourceView(), light->GetPosition(), m_shadowMapBias);
	if (!result)
	{
		return false;
	}

	translateMatrix = XMMatrixTranslation(0.0f, 0.0f, -5.0f);
	translateMatrix = ApplyTransformations(translateMatrix, XMMatrixScaling(1.020f, 1.020f, 1.020f), XMMatrixRotationY(rotation));

	// Render the tree leaf model using the transparent depth shader.
	m_CubeModel->Render(m_Direct3D->GetDeviceContext());

	result = m_depthToBWShadowMap->Render(m_Direct3D->GetDeviceContext(), m_CubeModel->GetIndexCount(), translateMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix,
		depthTexture->GetShaderResourceView(), light->GetPosition(), m_shadowMapBias);
	if (!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore. also reset the viewport back to the original.
	m_Direct3D->SetBackBufferRenderTarget();
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::RenderSceneToTexture(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Set the render target to be the render texture and clear it.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Get the matrices.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_RenderTexture->GetProjectionMatrix(projectionMatrix);

	// Setup the translation matrix for the cube model.
	worldMatrix = XMMatrixTranslation(-2.0f, 2.0f, 0.0f);
	worldMatrix = ApplyTransformations(worldMatrix, XMMatrixScaling(0.015f, 0.015f, 0.015f), XMMatrixRotationX(0.0f));
	// Render the cube model using the shadow shader.
	m_CubeModel->Render(m_Direct3D->GetDeviceContext());
	//lightPosition = XMFLOAT3(m_Light->GetPosition().x, m_Light->GetPosition().y, m_Light->GetPosition().z);

	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_CubeModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(0));// , m_LightData, m_LightDataSize);

	if (!result)
	{
		return false;
	}

	// Setup the translation matrix for the sphere model.
	worldMatrix = XMMatrixTranslation(2.0f, 2.0f, 0.0f);
	//worldMatrix = ApplyTransformations(worldMatrix, XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixRotationX(0.0f));

	// Render the sphere model using the shadow shader.
	m_SphereModel->Render(m_Direct3D->GetDeviceContext());
	//lightPosition = XMFLOAT3(m_Light->GetPosition().x, m_Light->GetPosition().y, m_Light->GetPosition().z);

	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_SphereModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_SphereModel->GetTexture(0));//, m_LightData, m_LightDataSize);

	if (!result)
	{
		return false;
	}

	// Setup the translation matrix for the ground model.
	worldMatrix = XMMatrixTranslation(0.0f, 1.0f, 0.0f);

	// Render the ground model using the shadow shader.
	m_GroundModel->Render(m_Direct3D->GetDeviceContext());

	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_GroundModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_GroundModel->GetTexture(0));//, m_LightData, m_LightDataSize);

	if (!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.  And reset the viewport back to the original.
	m_Direct3D->SetBackBufferRenderTarget();
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::RenderSceneDeferredBuffer(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Set the render buffers to be the render target.
	m_DeferredBuffers->SetRenderTargets(m_Direct3D->GetDeviceContext());

	// Clear the render buffers.
	m_DeferredBuffers->ClearRenderTargets(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Get the matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	worldMatrix = XMMatrixTranslation(0.0f, -1.0f, -5.0f);
	worldMatrix = ApplyTransformations(worldMatrix, XMMatrixScaling(3.0f, 3.0f, 3.0f), XMMatrixRotationX(0.0f));

	m_GroundModel->Render(m_Direct3D->GetDeviceContext());
	result = m_DeferredShader->Render(m_Direct3D->GetDeviceContext(), m_GroundModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_GroundModel->GetTexture(0));
	if (!result)
	{
		return false;
	}
	
	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, -5.0f);
	worldMatrix = ApplyTransformations(worldMatrix, XMMatrixScaling(1.020f, 1.020f, 1.020f), XMMatrixRotationY(rotation));

	m_CubeModel->Render(m_Direct3D->GetDeviceContext());
	result = m_DeferredShader->Render(m_Direct3D->GetDeviceContext(), m_CubeModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(0));
	if (!result)
	{
		return false;
	}
	
	// Reset the render target back to the original back buffer and not the render buffers anymore.  And reset the viewport back to the original.
	m_Direct3D->SetBackBufferRenderTarget();
	m_Direct3D->ResetViewport();
	
	return true;
}

bool ApplicationClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, reflectionMatrix, lightViewMatrix, lightProjectionMatrix, lightViewMatrix2, lightProjectionMatrix2; // , rotationMatrix, translateMatrix, scaleMatrix, srMatrix;
	XMMATRIX orthoMatrix;

	XMFLOAT4 diffuseColors[5];
	XMFLOAT3 lightPositions[5];
	XMFLOAT3 lightPosition;

	int modelCount, renderCount, i;
	float positionX, positionY, positionZ, radius, blendAmount, refractionScale;
	bool renderModel, result;
	float fogColor, fogStart, fogEnd;
	float brightness;

	XMFLOAT4 clipPlane;

	// Setup a clipping plane.
	clipPlane = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);

	// Set the color of the fog to grey.
	fogColor = 0.5f;

	// Set the start and end of the fog.
	fogStart = 0.0f;
	fogEnd = 10.0f;

	// Set the blending amount to 50%
	blendAmount = 0.5f;

	// Set the refraction scale for the glass shader.
	refractionScale = 0.1f;

	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(fogColor, fogColor, fogColor, 1.0f);

	// Get the world, view, projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// Set the light brightness.
	brightness = 1.5f;
	/*
	// Initialize the count of models that have been rendered.
	renderCount = 0;
	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	worldMatrix = ApplyTransformations(worldMatrix, XMMatrixScaling(0.015f, 0.015f, 0.015f), XMMatrixRotationY(rotation));
	// Render the cube model using the shadow shader.
	m_CubeModel->Render(m_Direct3D->GetDeviceContext());

	result = m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), m_CubeModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(0), m_LightData, m_LightDataSize);
	if (!result)
	{
		return false;
	}
	*/
	/*
	worldMatrix = XMMatrixTranslation(-2.0f, 1.0f, -3.0f);
	worldMatrix = ApplyTransformations(worldMatrix, XMMatrixScaling(0.015f, 0.015f, 0.015f), XMMatrixRotationX(0.0f));
	// Render the cube model using the shadow shader.
	m_CubeModel->Render(m_Direct3D->GetDeviceContext());

	result = m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), m_CubeModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_CubeModel->GetTexture(0), m_LightData, m_LightDataSize);
	if (!result)
	{
		return false;
	}

	// Setup the translation matrix for the sphere model.
	worldMatrix = XMMatrixTranslation(2.0f, 2.0f, 0.0f);

	// Render the sphere model using the shadow shader.
	m_SphereModel->Render(m_Direct3D->GetDeviceContext());

	result = m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), m_SphereModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_SphereModel->GetTexture(0), m_LightData, m_LightDataSize);

	if (!result)
	{
		return false;
	}

	// Setup the translation matrix for the ground model.
	worldMatrix = XMMatrixTranslation(0.0f, 1.0f, 0.0f);
	worldMatrix = ApplyTransformations(worldMatrix, XMMatrixScaling(2.0f, 2.0f, 2.0f), XMMatrixRotationX(0.0f));

	// Render the ground model using the shadow shader.
	m_GroundModel->Render(m_Direct3D->GetDeviceContext());

	result = m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), m_GroundModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_GroundModel->GetTexture(0), m_LightData, m_LightDataSize);

	if (!result)
	{
		return false;
	}

	// Setup the translation matrix for the tree model.
	worldMatrix = ApplyTransformations(XMMatrixTranslation(0.0f, 1.0f, 0.0f),
		XMMatrixScaling(0.1f, 0.1f, 0.1f), XMMatrixRotationX(0.0f));

	// Render the tree trunk model using the depth shader.
	m_TreeTrunkModel->Render(m_Direct3D->GetDeviceContext());

	result = m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), m_TreeTrunkModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_TreeTrunkModel->GetTexture(0), m_LightData, m_LightDataSize);
	if (!result)
	{
		return false;
	}

	// Turn on alpha blending for the transparency to work.
	m_Direct3D->EnableAlphaBlending();

	// Enable alpha transparency before drawing the leaf model.
	// Render the tree leaf model using the transparent depth shader.
	m_TreeLeafModel->Render(m_Direct3D->GetDeviceContext());

	result = m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), m_TreeLeafModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_TreeLeafModel->GetTexture(0), m_LightData, m_LightDataSize);
	if (!result)
	{
		return false;
	}
	*/

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Turn off the Z buffer to begin all 2D rendering and enable alpha blending.
	m_Direct3D->TurnZBufferOff();

	// Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_FullScreenWindow->Render(m_Direct3D->GetDeviceContext());

	// Render the full screen ortho window using the deferred light shader and the render buffers.
	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_FullScreenWindow->GetIndexCount(), 
		worldMatrix, m_baseViewMatrix, orthoMatrix, projectionMatrix,
		m_DeferredBuffers->GetShaderResourceView(0), 
		m_DeferredBuffers->GetShaderResourceView(1), 
		m_DeferredBuffers->GetShaderResourceView(2),
		m_DeferredBuffers->GetShaderResourceView(3),
		m_LightData, m_Camera);

	if (!result)
	{
		return false;
	}


	m_Direct3D->EnableAlphaBlending();
	// Render the fps text string using the font shader.
	m_FpsString->Render(m_Direct3D->GetDeviceContext());

	result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_FpsString->GetIndexCount(), worldMatrix, m_baseViewMatrix, orthoMatrix,
		m_Font->GetTexture(), m_FpsString->GetPixelColor());
	if (!result)
	{
		return false;
	}

	// Render the mouse cursor using the texture shader.
	result = m_MouseBitmap->Render(m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_MouseBitmap->GetIndexCount(), worldMatrix, m_baseViewMatrix, orthoMatrix, m_MouseBitmap->GetTexture());
	if (!result)
	{
		return false;
	}

	// Enable the Z buffer and disable alpha blending now that 2D rendering is complete.
	m_Direct3D->TurnZBufferOn();
	m_Direct3D->DisableAlphaBlending();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}

bool ApplicationClass::TestIntersection(int mouseX, int mouseY)
{
	XMMATRIX projectionMatrix, viewMatrix, inverseViewMatrix, worldMatrix, inverseWorldMatrix;
	XMFLOAT4X4 pMatrix, iViewMatrix;
	XMVECTOR direction, origin, rayOrigin, rayDirection;
	XMFLOAT3 cameraDirection, cameraOrigin, rayOri, rayDir;
	float pointX, pointY;
	bool intersect;

	// Move the mouse cursor coordinates into the -1 to +1 range.
	pointX = ((2.0f * (float)mouseX) / (float)m_screenWidth) - 1.0f;
	pointY = (((2.0f * (float)mouseY) / (float)m_screenHeight) - 1.0f) * -1.0f;

	// Adjust the points using the projection matrix to account for the aspect ratio of the viewport.
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	XMStoreFloat4x4(&pMatrix, projectionMatrix);
	pointX = pointX / pMatrix._11;
	pointY = pointY / pMatrix._22;

	// Get the inverse of the view matrix.
	m_Camera->GetViewMatrix(viewMatrix);
	inverseViewMatrix = XMMatrixInverse(NULL, viewMatrix);
	XMStoreFloat4x4(&iViewMatrix, inverseViewMatrix);

	// Calculate the direction of the picking ray in view space.
	cameraDirection.x = (pointX * iViewMatrix._11) + (pointY * iViewMatrix._21) + iViewMatrix._31;
	cameraDirection.y = (pointX * iViewMatrix._12) + (pointY * iViewMatrix._22) + iViewMatrix._32;
	cameraDirection.z = (pointX * iViewMatrix._13) + (pointY * iViewMatrix._23) + iViewMatrix._33;
	direction = XMLoadFloat3(&cameraDirection);

	// Get the origin of the picking ray which is the position of the camera.
	cameraOrigin = m_Camera->GetPosition();
	origin = XMLoadFloat3(&cameraOrigin);

	// Get the world matrix and translate to the location of the sphere.
	worldMatrix = XMMatrixTranslation(-2.0f, 1.0f, -3.0f);//-5.0f, 1.0f, 5.0f);

	// Now get the inverse of the translated world matrix.
	inverseWorldMatrix = XMMatrixInverse(NULL, worldMatrix);

	// Now transform the ray origin and the ray direction from view space to world space.
	rayOrigin = XMVector3TransformCoord(origin, inverseWorldMatrix);
	rayDirection = XMVector3TransformNormal(direction, inverseWorldMatrix);

	// Normalize the ray direction.
	rayDirection = XMVector3Normalize(rayDirection);

	// Convert the ray origin and direction XMVECTOR to a XMFLOAT3 type.
	XMStoreFloat3(&rayOri, rayOrigin);
	XMStoreFloat3(&rayDir, rayDirection);

	// Now perform the ray-sphere intersection test.
	intersect = RaySphereIntersect(rayOri, rayDir, 1.0f);

	return intersect;
}

bool ApplicationClass::RaySphereIntersect(XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, float radius)
{
	float a, b, c, discriminant;


	// Calculate the a, b, and c coefficients.
	a = (rayDirection.x * rayDirection.x) + (rayDirection.y * rayDirection.y) + (rayDirection.z * rayDirection.z);
	b = ((rayDirection.x * rayOrigin.x) + (rayDirection.y * rayOrigin.y) + (rayDirection.z * rayOrigin.z)) * 2.0f;
	c = ((rayOrigin.x * rayOrigin.x) + (rayOrigin.y * rayOrigin.y) + (rayOrigin.z * rayOrigin.z)) - (radius * radius);

	// Find the discriminant.
	discriminant = (b * b) - (4 * a * c);

	// if discriminant is negative the picking ray missed the sphere, otherwise it intersected the sphere.
	if (discriminant < 0.0f)
	{
		return false;
	}

	return true;
}

bool ApplicationClass::UpdateFPS()
{
	int fps;
	char tempString[16], finalString[16];
	float red, green, blue;
	bool result;

	//Update the fps each frame.
	m_Fps->Frame();

	// Get the current fps.
	fps = m_Fps->GetFps();

	// Check if the fps from the previous frame was the same, if so don't need to update the text string.
	if (m_previousFps == fps)
	{
		return true;
	}

	// Store the fps for checking next frame.
	m_previousFps = fps;

	// Truncate the fps to below 100,000.
	if (fps > 9999)
	{
		fps = 99999;
	}

	// Convert the fps integer to string format.
	sprintf_s(tempString, "%d", fps);

	// Setup the fps string.
	strcpy_s(finalString, "Fps: ");
	strcat_s(finalString, tempString);

	// If fps is 60 or above set the fps color to green.
	if (fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 60 set the fps color to yellow.
	if (fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// If fps is below 30 set the fps color to red.
	if (fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	// Update the sentence vertex buffer with the new string information.
	result = m_FpsString->UpdateText(m_Direct3D->GetDeviceContext(), m_Font, finalString, 10, 10, red, green, blue);
	if (!result)
	{
		return false;
	}

	return true;
}


//bool ApplicationClass::RenderRefractionToTexture()
//{
//	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
//	XMFLOAT4 clipPlane;
//	bool result;
//
//	// Setup a clipping plane based on the height of the water to clip everything above it.
//	clipPlane = XMFLOAT4(0.0f, -1.0f, 0.0f, m_waterHeight + 0.1f);
//
//	// Set the render target to be the refraction render to texture and clear it.
//	m_RefractionTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());
//	m_RefractionTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
//
//	// Generate the view matrix based on the camera's position.
//	m_Camera->Render();
//
//	// Get the world, view and projection matrices from the camera and d3d objects.
//	m_Direct3D->GetWorldMatrix(worldMatrix);
//	m_Camera->GetViewMatrix(viewMatrix);
//	m_Direct3D->GetProjectionMatrix(projectionMatrix);
//
//	// Translate to where the bath model will be rendered.
//	worldMatrix = XMMatrixTranslation(0.0f, 2.0f, 0.0f);
//
//	// Render the bath model using the refraction shader.
//	m_BathModel->Render(m_Direct3D->GetDeviceContext());
//
//	result = m_RefractionShader->Render(m_Direct3D->GetDeviceContext(), m_BathModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_BathModel->GetTexture(0),
//		m_DirectionalLight->GetDirection(), m_DirectionalLight->GetAmbientColor(), m_DirectionalLight->GetDiffuseColor(), clipPlane);
//	if (!result)
//	{
//		return false;
//	}
//
//	// Reset the render target back to the original back buffer and not the render to texture anymore.  And reset the viewport back to the original.
//	m_Direct3D->SetBackBufferRenderTarget();
//	m_Direct3D->ResetViewport();
//
//	return true;
//}
//
//bool ApplicationClass::RenderReflectionToTexture(float rotation)
//{
//	XMMATRIX worldMatrix, reflectionViewMatrix, projectionMatrix;
//	bool result;
//
//	// Set the render target to be the render to texture and clear it.
//	m_ReflectionTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());
//	m_ReflectionTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
//
//	// Use the camera to render the reflection and create a reflection view matrix.
//	m_Camera->RenderReflection(m_waterHeight);
//
//	// Get the camera reflection view matrix instead of the normal view matrix.
//	m_Camera->GetReflectionViewMatrix(reflectionViewMatrix);
//
//	// Get the world and projection matrices from the d3d object.
//	m_Direct3D->GetWorldMatrix(worldMatrix);
//	m_Direct3D->GetProjectionMatrix(projectionMatrix);
//
//	// Translate to where the wall model will be rendered.
//	worldMatrix = XMMatrixTranslation(0.0f, 6.0f, 8.0f);
//
//	// Render the wall model using the light shader and the reflection view matrix.
//	m_WallModel->Render(m_Direct3D->GetDeviceContext());
//
//	int i = 0;
//	XMFLOAT4 diffuseColors[5], lightPositions[5];
//
//	for (i = 0; i < m_numLights; i++)
//	{
//		// Create the diffuse color array from the four light colors.
//		diffuseColors[i] = m_Lights[i].GetDiffuseColor();
//
//		// Create the light position array from the four light positions.
//		lightPositions[i] = m_Lights[i].GetPosition();
//	}
//
//	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_WallModel->GetIndexCount(), 
//		worldMatrix, reflectionViewMatrix, projectionMatrix, m_WallModel->GetTexture(0),
//		m_Camera, m_DirectionalLight, diffuseColors, lightPositions);
//
//	if (!result)
//	{
//		return false;
//	}
//
//	// Reset the render target back to the original back buffer and not the render to texture anymore. And reset the viewport back to the original.
//	m_Direct3D->SetBackBufferRenderTarget();
//	m_Direct3D->ResetViewport();
//
//	return true;
//}