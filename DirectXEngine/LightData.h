
//LIGHT CLASS//
#ifndef _LIGHTDATACLASS_H_
#define _LIGHTDATACLASS_H_

#include "LightClass.h"
#include "RenderTextureClass.h"

class LightData
{
public:
	LightData();
	~LightData();
	bool Initialize(LightClass*, RenderTextureClass*, float);
	void Shutdown();

public:
	LightClass* m_Light;
	RenderTextureClass* m_RenderTexture;
	float m_bias;
};

#endif