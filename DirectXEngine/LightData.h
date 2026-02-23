
//LIGHT CLASS//
#ifndef _LIGHTDATACLASS_H_
#define _LIGHTDATACLASS_H_

#include "d3dclass.h"
#include "LightClass.h"
#include "RenderTextureClass.h"
#include <vector>
#include <memory>
#include "RenderTextureArrayClass.h"

class LightData
{
public:
	LightData();
	~LightData();
	bool Initialize(float bias);
	float GetShadowBias();

	bool InitializeShadowMap(ID3D11Device* device, HWND hwnd, int textureWidth, int textureHeight, float screenDepth, float screenNear, int format, int maxLights);
	RenderTextureArrayClass* GetDepthMaps();
	RenderTextureArrayClass* GetShadowMaps();

	bool RegisterPointLight(LightClass* light);
	std::vector<LightClass*>& GetAllPointLights();

	bool RegisterDirectionalLight(LightClass* light);
	LightClass* GetDirectionalLight();
	void Shutdown();

private:
	std::vector<LightClass*> m_PointLights;
	std::vector<LightClass*> m_SpotLights;
	LightClass* m_DirectionalLight;
	RenderTextureArrayClass* m_DepthMapArray;
	RenderTextureArrayClass* m_ShadowMapArray;
	//RenderTextureClass* m_RenderTexture;
	//RenderTextureClass* m_BlackWhiteRenderTexture;
	float m_bias;
};

#endif