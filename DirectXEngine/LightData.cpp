#include "LightData.h"

LightData::LightData()
{
	m_DirectionalLight = 0;
	m_bias = 0;
}

LightData::~LightData()
{

}

bool LightData::Initialize(float bias)
{
	m_bias = bias;

	return true;
}

float LightData::GetShadowBias()
{
	return m_bias;
}

bool LightData::InitializeShadowMap(ID3D11Device* device, HWND hwnd, int textureWidth, int textureHeight, float screenDepth, float screenNear, int format, int maxLights)
{
	// Create and initialize the render to texture object.
	m_DepthMapArray = new RenderTextureArrayClass;

	bool result = m_DepthMapArray->Initialize(device, textureWidth, textureHeight, screenDepth, screenNear, 1, maxLights);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the depth render texture array.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the render to texture object.
	m_ShadowMapArray = new RenderTextureArrayClass;

	result = m_ShadowMapArray->Initialize(device, textureWidth, textureHeight, screenDepth, screenNear, 1, maxLights);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the shadow render texture array.", L"Error", MB_OK);
		return false;
	}
	return true;
}

RenderTextureArrayClass* LightData::GetDepthMaps()
{
	return m_DepthMapArray;
}

RenderTextureArrayClass* LightData::GetShadowMaps()
{
	return m_ShadowMapArray;
}

bool LightData::RegisterPointLight(LightClass* light)
{
	if (!light) return false;

	m_PointLights.push_back(light);

	return true;
}

std::vector<LightClass*>& LightData::GetAllPointLights()
{
	return m_PointLights;
}

bool LightData::RegisterDirectionalLight(LightClass* light)
{
	if (!light) return false;

	m_DirectionalLight = light;

	return true;
}

LightClass* LightData::GetDirectionalLight()
{
	return m_DirectionalLight;
}

void LightData::Shutdown()
{
	m_PointLights.clear();

	if (m_DepthMapArray)
	{
		m_DepthMapArray->Shutdown();
		m_DepthMapArray = 0;
	}

	if (m_ShadowMapArray)
	{
		m_ShadowMapArray->Shutdown();
		m_ShadowMapArray = 0;
	}
}