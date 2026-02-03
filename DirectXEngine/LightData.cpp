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

bool LightData::Initialize(LightClass* light, RenderTextureClass* renderTexture, RenderTextureClass* blackWhiteRenderTexture, float bias)
{
	if (!light)
	{
		return false;
	}

	if (!renderTexture)
	{
		return false;
	}

	if (!blackWhiteRenderTexture)
	{
		return false;
	}

	m_Light = light;
	m_RenderTexture = renderTexture;
	m_BlackWhiteRenderTexture = blackWhiteRenderTexture;
	m_bias = bias;

	return true;
}

void LightData::Shutdown()
{
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	if (m_BlackWhiteRenderTexture)
	{
		m_BlackWhiteRenderTexture->Shutdown();
		delete m_BlackWhiteRenderTexture;
		m_BlackWhiteRenderTexture = 0;
	}
}