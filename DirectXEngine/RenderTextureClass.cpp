////////////////////////////////////////////////////////////////////////////////
// Filename: rendertextureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "RenderTextureClass.h"

RenderTextureClass::RenderTextureClass()
{
	m_renderTargetTexture = 0;
	m_renderTargetView = 0;
	m_shaderResourceView = 0;
	m_depthStencilBuffer = 0;
	m_depthStencilView = 0;
}

RenderTextureClass::RenderTextureClass(const RenderTextureClass& other)
{

}

RenderTextureClass::~RenderTextureClass()
{

}

bool RenderTextureClass::Initialize(ID3D11Device* device, int textureWidth, int textureHeight, float screenDepth, float screenNear, int format)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
}