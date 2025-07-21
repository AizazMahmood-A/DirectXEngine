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

	//m_MultiTextureShader = 0;
	//m_LightMapShader = 0;
	//m_AlphaMapShader = 0;
	//m_NormalMapShader = 0;
	//m_SpecMapShader = 0;

	m_TextureShader = 0;
	m_Sprite = 0;
	m_Timer = 0;

	m_LightShader = 0;
	m_DirectionalLight = 0;

	m_Models = 0;
	m_modelCount = 0;

	m_Lights = 0;

	m_FontShader = 0;
	m_Font = 0;

	m_Fps = 0;
	m_FpsString = 0;
	m_MouseStrings = 0;

	m_ShaderManager = 0;

	m_RenderCountString = 0;
	m_ModelList = 0;
	m_Position = 0;
	m_Frustum = 0;

	m_RenderTexture = 0;
	m_DisplayPlane = 0;
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
	char modelFilename[128], textureFilename1[128], textureFilename2[128], textureFilename3[128];
	char textureFilename[128];
	char renderString[32];
	char mouseString1[32], mouseString2[32], mouseString3[32];

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
	m_Camera->SetPosition(0.0f, 0.0f, -8.0f);
	m_Camera->SetRotation(0.0f, 0.0f, 0.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(m_baseViewMatrix);

	// Create and initialize the light object.
	m_DirectionalLight = new LightClass;

	m_DirectionalLight->SetAmbientColor(0.25f, 0.25f, 0.25f, 1.0f);
	m_DirectionalLight->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirectionalLight->SetDirection(0.0f, 1.0f, 1.0f);
	m_DirectionalLight->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirectionalLight->SetSpecularPower(16.0f);

	// Set the number of lights we will use.
	m_numLights = 5;

	// Create and initialize the light objects array.
	m_Lights = new LightClass[m_numLights];

	// Manually set the color and position of each light.
	m_Lights[0].SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);  // Red
	m_Lights[0].SetPosition(-3.0f, 1.0f, 3.0f);

	m_Lights[1].SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);  // Green
	m_Lights[1].SetPosition(3.0f, 1.0f, 3.0f);

	m_Lights[2].SetDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);  // Blue
	m_Lights[2].SetPosition(-3.0f, 1.0f, -3.0f);

	m_Lights[3].SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);  // White
	m_Lights[3].SetPosition(3.0f, 1.0f, -3.0f);

	m_Lights[4].SetDiffuseColor(0.5f, 0.0f, 0.5f, 1.0f);  // Purple
	m_Lights[4].SetPosition(0.0f, 1.0f, 0.0f);

	// Set the file name of the model.
	strcpy_s(modelFilename, "../DirectXEngine/model.txt");//data/monkey.obj");

	// Set the file name of the textures.
	strcpy_s(textureFilename1, "../DirectXEngine/data/stone01.tga");
	strcpy_s(textureFilename2, "../DirectXEngine/data/normal01.tga");
	strcpy_s(textureFilename3, "../DirectXEngine/data/spec02.tga");

	// Create and initialize the model object.
	m_Model = new ModelClass;

	result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename1, textureFilename2, textureFilename3);
	if (!result)
	{
		return false;
	}

	// Create and Initialize the normal map shader object.
	m_ShaderManager = new ShaderManagerClass;

	result = m_ShaderManager->Initialize(m_Direct3D->GetDevice(), hwnd);
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

	// Set the initial mouse string
	strcpy_s(mouseString1, "Mouse X: 0");
	strcpy_s(mouseString2, "Mouse Y: 0");
	strcpy_s(mouseString3, "Mouse Button : No");

	// Create and initialize the text object for the mouse string.
	m_MouseStrings = new TextClass[3];

	result = m_MouseStrings[0].Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, m_Font, mouseString1, 10, 35, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = m_MouseStrings[1].Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, m_Font, mouseString2, 10, 55, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	result = m_MouseStrings[2].Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, m_Font, mouseString3, 10, 85, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	// Set the initial render count string.
	strcpy_s(renderString, "Render Count: 0");

	// Create and initialize the text object for the render count string.
	m_RenderCountString = new TextClass;

	result = m_RenderCountString->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 32, m_Font, renderString, 10, 105, 1.0f, 1.0f, 1.0f);
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

	result = m_RenderTexture->Initialize(m_Direct3D->GetDevice(), 256, 256, SCREEN_DEPTH, SCREEN_NEAR, 1);
	if (!result)
	{
		return false;
	}

	// Create and initialize the display plane object.
	m_DisplayPlane = new DisplayPlaneClass;

	result = m_DisplayPlane->Initialize(m_Direct3D->GetDevice(), 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	// Create and initialize the model list object.
	m_ModelList = new ModelListClass;
	m_ModelList->Initialize(25);

	// Create the position class object.
	m_Position = new PositionClass;

	// Create the frustum class object.
	m_Frustum = new FrustumClass;

	return true;
}


void ApplicationClass::Shutdown()
{

	// Release the display plane object.
	if (m_DisplayPlane)
	{
		m_DisplayPlane->Shutdown();
		delete m_DisplayPlane;
		m_DisplayPlane = 0;
	}

	// Release the render to texture object.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	// Release the frustum class object.
	if (m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = 0;
	}

	// Release the position object.
	if (m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the model list object.
	if (m_ModelList)
	{
		m_ModelList->Shutdown();
		delete m_ModelList;
		m_ModelList = 0;
	}

	// Release the text objects for the render count string.
	if (m_RenderCountString)
	{
		m_RenderCountString->Shutdown();
		delete m_RenderCountString;
		m_RenderCountString = 0;
	}

	// Release the shader manager object.
	if (m_ShaderManager)
	{
		m_ShaderManager->Shutdown();
		delete m_ShaderManager;
		m_ShaderManager = 0;
	}

	// Release the text objects for the mouse strings.
	if (m_MouseStrings)
	{
		m_MouseStrings[0].Shutdown();
		m_MouseStrings[1].Shutdown();
		m_MouseStrings[2].Shutdown();

		delete[] m_MouseStrings;
		m_MouseStrings = 0;
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


static float rotation = 360.0f;
bool ApplicationClass::Frame(InputClass* Input)
{
	int mouseX, mouseY;
	bool result, mouseDown, keyDown;

	float frameTime, cameraRotationY;

	// Check if the user pressed escape and wants to exit the application.
	if (Input->IsEscapePressed())
	{
		return false;
	}

	// Get the location of the mouse from the input object.
	Input->GetMouseLocation(mouseX, mouseY);

	// Check if the mouse has been pressed.
	mouseDown = Input->IsMousePressed();

	// Update the mouse strings each frame.
	result = UpdateMouseStrings(mouseX, mouseY, mouseDown);
	if (!result)
	{
		return false;
	}

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

	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(m_Timer->GetTime());

	// Check if the left or right arrow key has been presse, if so rotate the camera accordingly.
	keyDown = Input->IsLeftArrowPressed();
	m_Position->TurnLeft(keyDown);

	keyDown = Input->IsRightArrowPressed();
	m_Position->TurnRight(keyDown);

	// Get the current view point rotation.
	m_Position->GetRotation(cameraRotationY);

	// Set the rotation of the camera.
	m_Camera->SetRotation(0.0f, cameraRotationY, 0.0f);
	m_Camera->Render();

	// Update the rotation variable each frame.
	rotation -= 0.0174532925f * 0.0025f;
	if (rotation <= 0.0f)
	{
		rotation += 360.0f;
	}

	// Render the scene to a render texture.
	result = RenderSceneToTexture(rotation);
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

bool ApplicationClass::RenderSceneToTexture(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Set the render target to be the render texture and clear it.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.5f, 0.5f, 1.0f, 1.0f);

	// Set the position of the camera for viewing the cube.
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	m_Camera->Render();

	// Get the matrices.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_RenderTexture->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the cube will spin.
	worldMatrix = XMMatrixRotationY(rotation);

	// Render the model using the texture shader.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(0));
	if (!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.  And reset the viewport back to the original.
	m_Direct3D->SetBackBufferRenderTarget();
	m_Direct3D->ResetViewport();

	return true;
}

XMMATRIX ApplyTransformations(XMMATRIX translation, XMMATRIX scale, XMMATRIX rotation)
{
	XMMATRIX worldMatrix, srMatrix;
	srMatrix = XMMatrixMultiply(scale, rotation);
	worldMatrix = XMMatrixMultiply(srMatrix, translation);
	return worldMatrix;
}

bool ApplicationClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix; // , rotationMatrix, translateMatrix, scaleMatrix, srMatrix;
	XMMATRIX orthoMatrix;

	XMFLOAT4 diffuseColors[5], lightPositions[5];

	int modelCount, renderCount, i;
	float positionX, positionY, positionZ, radius;
	bool renderModel, result;

	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	//m_Direct3D->GetWorldMatrix(UIWorldMatrix);
	//m_Camera->GetViewMatrix(UIViewMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// Construct the frustum.
	m_Frustum->ConstructFrustum(viewMatrix, projectionMatrix, SCREEN_DEPTH);

	// Get the number of models that will be rendered.
	modelCount = m_ModelList->GetModelCount();

	// Initialize the count of models that have been rendered.
	renderCount = 0;

	// Go through all the models and render them only if they can be seen by the camera view.
	for (i = 0; i < modelCount; i++)
	{
		// Get the position and color of the sphere model at this index.
		m_ModelList->GetData(i, positionX, positionY, positionZ);

		// Set the radius of the sphere to 1.0 since this is already known.
		radius = 1.0f;

		// Check if the sphere model is in the view frustum.
		renderModel = m_Frustum->CheckCube(positionX, positionY, positionZ, radius);

		// If it can be seen then render it, if not skip this model and check the next sphere.
		if (renderModel)
		{
			// Move the model to the location it should be rendered at.
			worldMatrix = XMMatrixTranslation(positionX, positionY, positionZ);

			// Render the model using the light shader.
			m_Model->Render(m_Direct3D->GetDeviceContext());

			result = m_ShaderManager->RenderNormalMapShader(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_Model->GetTexture(0), m_Model->GetTexture(1), m_DirectionalLight->GetDirection(), m_DirectionalLight->GetDiffuseColor());
			if (!result)
			{
				return false;
			}

			// Since this model was rendered then increase the count for this frame.
			renderCount++;
		}
	}

	// Update the render count text.
	result = UpdateRenderCountString(renderCount);
	if (!result)
	{
		return false;
	}

	// Get the light properties.
	for (i = 0; i < m_numLights; i++)
	{
		// Create the diffuse color array from the four light colors.
		diffuseColors[i] = m_Lights[i].GetDiffuseColor();

		// Create the light position array from the four light positions.
		lightPositions[i] = m_Lights[i].GetPosition();
	}

	// Setup matrices - Top display plane.
	worldMatrix = XMMatrixTranslation(0.0f, 1.5f, 0.0f);

	// Render the display plane using the texture shader and render texture resource.
	m_DisplayPlane->Render(m_Direct3D->GetDeviceContext());

	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_DisplayPlane->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_RenderTexture->GetShaderResourceView());
	if (!result)
	{
		return false;
	}

	// Setup matrices - Bottom left display plane.
	worldMatrix = XMMatrixTranslation(-1.5f, -1.5f, 0.0f);

	// Render the display plane using the texture shader and the render texture resource.
	m_DisplayPlane->Render(m_Direct3D->GetDeviceContext());

	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_DisplayPlane->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_RenderTexture->GetShaderResourceView());
	if (!result)
	{
		return false;
	}

	// Setup matrices - Bottom right display plane.
	worldMatrix = XMMatrixTranslation(1.5f, -1.5f, 0.0f);

	// Render the display plane using the texture shader and the render texture resource.
	m_DisplayPlane->Render(m_Direct3D->GetDeviceContext());

	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_DisplayPlane->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_RenderTexture->GetShaderResourceView());
	if (!result)
	{
		return false;
	}

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	// Turn off the Z buffer to begin all 2D rendering and enable alpha blending.
	m_Direct3D->TurnZBufferOff();
	m_Direct3D->EnableAlphaBlending();

	// Render the fps text string using the font shader.
	m_FpsString->Render(m_Direct3D->GetDeviceContext());

	result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_FpsString->GetIndexCount(), worldMatrix, m_baseViewMatrix, orthoMatrix,
		m_Font->GetTexture(), m_FpsString->GetPixelColor());
	if (!result)
	{
		return false;
	}

	// Render the mouse text strings using the font shader.
	for (i = 0; i < 3; i++)
	{
		m_MouseStrings[i].Render(m_Direct3D->GetDeviceContext());

		result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_MouseStrings[i].GetIndexCount(), worldMatrix, m_baseViewMatrix, orthoMatrix,
			m_Font->GetTexture(), m_MouseStrings[i].GetPixelColor());

		if (!result)
		{
			return false;
		}
	}

	// Render the render count text string using the font shader.
	m_RenderCountString->Render(m_Direct3D->GetDeviceContext());

	result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_RenderCountString->GetIndexCount(), worldMatrix, m_baseViewMatrix, orthoMatrix,
		m_Font->GetTexture(), m_RenderCountString->GetPixelColor());
	if (!result)
	{
		return false;
	}

	// Enable the Z buffer and disable alpha blending now that 2D rendering is complete.
	m_Direct3D->TurnZBufferOn();
	m_Direct3D->DisableAlphaBlending();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	//std::cout << m_baseViewMatrix << std::endl;

	return true;
}


bool ApplicationClass::UpdateRenderCountString(int renderCount)
{
	char tempString[16], finalString[32];
	bool result;


	// Convert the render count integer to string format.
	sprintf_s(tempString, "%d", renderCount);

	// Setup the render count string.
	strcpy_s(finalString, "Render Count: ");
	strcat_s(finalString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = m_RenderCountString->UpdateText(m_Direct3D->GetDeviceContext(), m_Font, finalString, 10, 105, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	return true;
}


bool ApplicationClass::UpdateMouseStrings(int mouseX, int mouseY, bool mouseDown)
{
	char tempString[16], finalString[32];
	bool result;

	// Convert the mouse X integer to string format.
	sprintf_s(tempString, "%d", mouseX);

	// Setup the mouse X string.
	strcpy_s(finalString, "Mouse X: ");
	strcat_s(finalString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = m_MouseStrings[0].UpdateText(m_Direct3D->GetDeviceContext(), m_Font, finalString, 10, 35, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	// Convert the mouse Y integer to string format.
	sprintf_s(tempString, "%d", mouseY);

	// Setup the mouse Y string.
	strcpy_s(finalString, "Mouse Y: ");
	strcat_s(finalString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = m_MouseStrings[1].UpdateText(m_Direct3D->GetDeviceContext(), m_Font, finalString, 10, 55, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	// Setup the mouse button string.
	if (mouseDown)
	{
		strcpy_s(finalString, "Mouse Button: Yes");
	}
	else
	{
		strcpy_s(finalString, "Mouse Button: No");
	}

	// Update the sentence vertex buffer with the new string information.
	result = m_MouseStrings[2].UpdateText(m_Direct3D->GetDeviceContext(), m_Font, finalString, 10, 85, 1.0f, 1.0f, 1.0f);
	if (!result)
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