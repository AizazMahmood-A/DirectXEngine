// Filename: applicationclass.h

#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_

//////////////
// INCLUDES //
//////////////
#include "d3dclass.h"
#include "CameraClass.h"
#include "ModelClass.h"

#include "LightShaderClass.h"
#include "lightClass.h"

#include "LightShaderClass.h"
#include "lightClass.h"

#include "TextureShaderClass.h"
#include "SpriteClass.h"
#include "timerclass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render(float);

private:
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;

	TextureShaderClass* m_TextureShader;
	SpriteClass* m_Sprite;
	TimerClass* m_Timer;

	LightShaderClass* m_LightShader;
	LightClass* m_DirectionalLight;

	ModelClass** m_Models;
	size_t m_modelCount;

	LightClass* m_Lights;
	int m_numLights;
};

#endif