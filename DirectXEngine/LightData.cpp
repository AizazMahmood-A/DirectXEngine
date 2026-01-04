#include "LightData.h"

LightData::LightData()
{
	m_Light = 0;
	m_RenderTexture = 0;
	m_bias = 0;
}

LightData::~LightData()
{

}

bool LightData::Initialize(LightClass* light, RenderTextureClass* renderTexture, float bias)
{
	if (!light)
	{
		return false;
	}

	if (!renderTexture)
	{
		return false;
	}

	m_Light = light;
	m_RenderTexture = renderTexture;
	m_bias = bias;

	return true;
}

void LightData::Shutdown()
{

}