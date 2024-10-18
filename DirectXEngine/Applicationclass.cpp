// Filename: applicationclass.cpp

#include "applicationclass.h"
#include <string.h>
//#define SingleModel
#define MultiModel

ApplicationClass::ApplicationClass()
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_Models = 0;
	m_LightShader = 0;
	m_Light = 0;
}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{

}


ApplicationClass::~ApplicationClass()
{

}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
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
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
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
	const char* modelFilenames[3] = { "../DirectXEngine/data/monkey.obj", "../DirectXEngine/data/cube.txt", "../DirectXEngine/data/sphere.txt" };
	const char* textureFilenames[3] = { "../DirectXEngine/data/stone02.tga", "../DirectXEngine/data/stone01.tga", "../DirectXEngine/data/stone01.tga" };

	m_modelCount = sizeof(modelFilenames) / sizeof(modelFilenames[0]);

	m_Models = new ModelClass * [m_modelCount];
	for (size_t i = 0; i < m_modelCount; i++)
	{
		ModelClass* model = new ModelClass;

		strcpy_s(modelFilename, modelFilenames[i]);
		strcpy_s(textureFilename, textureFilenames[i]);


		result = model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename);
		model->ApplyTransformations(XMMatrixTranslation(-3.0f + i * 3, 0.0f, 0.0f),
			XMMatrixScaling(1.0f, 1.0f, 1.0f), 0.0f);

		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_Models[i] = model;
	}

#endif // MultiModel







	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the light shader object.
	m_LightShader = new LightShaderClass;

	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the light object.
	m_Light = new LightClass;

	m_Light->SetAmbientColor(0.25f, 0.25f, 0.25f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(0.75f, 0.75f, 0.75f, 1.0f);
	m_Light->SetSpecularPower(16.0f);

	return true;
}


void ApplicationClass::Shutdown()
{
	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
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
	static float rotation = 0.0f;
	bool result;

	// Update rotation variable each frame.
	rotation -= 0.0174532925f * 0.1f;
	if (rotation < 0.0f)
	{
		rotation += 360.0f;
	}

	// Render the scene.
	result = Render(rotation);
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

bool ApplicationClass::Render(float rotation)
{

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, rotationMatrix, translateMatrix, scaleMatrix, srMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
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
	// Apply transforms.
	// Multiply them together to create the final world transformation matrix.
	//worldMatrix = ApplyTransformations(XMMatrixTranslation(-3.0f, 0.0f, 0.0f),
	//	XMMatrixScaling(1.0f, 1.0f, 1.0f),
	//	XMMatrixRotationY(rotation));

	for (size_t i = 0; i < m_modelCount; i++)
	{
		m_Models[i]->ApplyTransformations(m_Models[i]->GetPosition(), m_Models[i]->GetScale(), rotation);
		worldMatrix = m_Models[i]->GetTransform();

		m_Models[i]->Render(m_Direct3D->GetDeviceContext());
		// Render the model using the color shader.
		result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Models[i]->GetTexture(),
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

		if (!result)
		{
			return false;
		}
}
#endif // MultiModel

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}