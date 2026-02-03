
//LIGHT CLASS//
#ifndef _LIGHTDATACLASS_H_
#define _LIGHTDATACLASS_H_

#include "d3dclass.h"
#include "LightClass.h"
#include "RenderTextureClass.h"

class LightData
{
public:
	LightData();
	~LightData();
	bool Initialize(LightClass* light, RenderTextureClass* renderTexture, RenderTextureClass* blackWhiteRenderTexture, float bias);
	void Shutdown();

public:
	LightClass* m_Light;
	RenderTextureClass* m_RenderTexture;
	RenderTextureClass* m_BlackWhiteRenderTexture;
	float m_bias;
	bool m_directionalLight;
};

#endif