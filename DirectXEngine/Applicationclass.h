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
//#include "timerclass.h"
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

#include "FogShaderClass.h"
#include "ClipPlaneShaderClass.h"
#include "TranslateShaderClass.h"
#include "TransparentShaderClass.h"
#include "ReflectionShaderClass.h"
#include "RefractionShaderClass.h"
#include "WaterShaderClass.h"
#include "GlassShaderClass.h"
#include "DepthShaderClass.h"
#include "TimerClass.h"
#include "ParticleSystemClass.h"
#include "ParticleShaderClass.h"

#include "ProjectionShaderClass.h"
#include "ViewpointClass.h"

#include "LightClass.h"

#include "DepthShaderClass.h"
#include "ShadowShaderClass.h"
#include "LightData.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;

//const float SCREEN_DEPTH = 1000.0f;
//const float SCREEN_NEAR = 0.3f;

const float SCREEN_DEPTH = 100.0f;
const float SCREEN_NEAR = 1.0f;
const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;

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
	//bool RenderReflectionToTexture(float);
	//bool RenderRefractionToTexture();
	bool RenderDepthToTexture(RenderTextureClass*, LightClass*);
	//bool RenderDepthToTexture2();

private:
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	//ModelClass* m_Model2;

	//MultiTextureShaderClass* m_MultiTextureShader;
	//LightMapShaderClass* m_LightMapShader;
	//AlphaMapShaderClass* m_AlphaMapShader;
	//NormalMapShaderClass* m_NormalMapShader;
	//SpecMapShaderClass* m_SpecMapShader;

	TextureShaderClass* m_TextureShader;
	SpriteClass* m_Sprite;
	TimerClass* m_Timer;

	//LightShaderClass* m_LightShader;
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

	RenderTextureClass *m_RenderTexture, *m_RenderTexture2, *m_RenderTexture3;
	//DisplayPlaneClass* m_DisplayPlane;

	//FogShaderClass* m_FogShader;
	//ClipPlaneShaderClass* m_ClipPlaneShader;
	//TranslateShaderClass* m_TranslateShader;
	//TransparentShaderClass* m_TransparentShader;

	//ReflectionShaderClass* m_ReflectionShader;

	ModelClass *m_GroundModel, *m_CubeModel, *m_SphereModel;//, *m_WallModel, *m_BathModel, *m_WaterModel;
	//LightClass* m_Light;

	//RenderTextureClass* m_RefractionTexture, *m_ReflectionTexture;

	//LightShaderClass* m_LightShader;
	//RefractionShaderClass* m_RefractionShader;
	//WaterShaderClass* m_WaterShader;

	//float m_waterHeight, m_waterTranslation;

	//ModelClass* m_WindowModel;
	//GlassShaderClass* m_GlassShader;
	DepthShaderClass* m_DepthShader;
	ShadowShaderClass* m_ShadowShader;

	ParticleShaderClass* m_ParticleShader;
	ParticleSystemClass* m_ParticleSystem;

	ProjectionShaderClass* m_ProjectionShader;
	TextureClass* m_ProjectionTexture;
	ViewpointClass* m_Viewpoint;
	LightClass *m_Light, *m_Light2;

	LightData *m_LightData;
	int m_LightDataSize;

	float m_shadowMapBias;
};

#endif