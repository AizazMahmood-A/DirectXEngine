// Filename: applicationclass.h

#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_

//////////////
// INCLUDES //
//////////////
#include "d3dclass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "FontShaderClass.h"
#include "FontClass.h"
#include "TextClass.h"

#include "LightShaderClass.h"
#include "lightClass.h"

#include "LightShaderClass.h"
#include "lightClass.h"

#include "TextureShaderClass.h"
#include "SpriteClass.h"
#include "timerclass.h"
#include "FPSClass.h"
#include "Inputclass.h"

#include "MultitextureShaderClass.h"
#include "LightmapShaderClass.h"
#include "alphamapshaderclass.h"
#include "NormalMapShaderClass.h"

#include "SpecmapShaderClass.h"

#include "ShaderManagerClass.h"

#include "FrustumClass.h"
#include "PositionClass.h"
#include "ModelList.Class.h"

#include "RenderTextureClass.h"
#include "DisplayPlaneClass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
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
	bool Frame(InputClass*);

private:
	bool Render();
	bool UpdateFPS();
	bool UpdateMouseStrings(int, int, bool);
	bool UpdateRenderCountString(int);
	bool RenderSceneToTexture(float);

private:
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;

	//MultiTextureShaderClass* m_MultiTextureShader;
	//LightMapShaderClass* m_LightMapShader;
	//AlphaMapShaderClass* m_AlphaMapShader;
	//NormalMapShaderClass* m_NormalMapShader;
	//SpecMapShaderClass* m_SpecMapShader;

	TextureShaderClass* m_TextureShader;
	SpriteClass* m_Sprite;
	TimerClass* m_Timer;

	LightShaderClass* m_LightShader;
	LightClass* m_DirectionalLight;

	ModelClass** m_Models;
	size_t m_modelCount;

	LightClass* m_Lights;
	int m_numLights;

	FontShaderClass* m_FontShader;
	FontClass* m_Font;
	FPSClass* m_Fps;
	TextClass* m_FpsString;
	int m_previousFps;
	TextClass* m_MouseStrings;

	ShaderManagerClass* m_ShaderManager;

	TextClass* m_RenderCountString;
	ModelListClass* m_ModelList;
	PositionClass* m_Position;
	FrustumClass* m_Frustum;
	XMMATRIX m_baseViewMatrix;

	RenderTextureClass* m_RenderTexture;
	DisplayPlaneClass* m_DisplayPlane;
};

#endif