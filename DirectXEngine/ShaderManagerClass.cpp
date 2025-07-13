////////////////////////////////////////////////////////////////////////////////
// Filename: shadermanagerclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "ShaderManagerClass.h"

ShaderManagerClass::ShaderManagerClass()
{
	m_TextureShader = 0;
	m_LightShader = 0;
	m_NormalMapShader = 0;
}

ShaderManagerClass::ShaderManagerClass(const ShaderManagerClass& other)
{

}

ShaderManagerClass::~ShaderManagerClass()
{

}

bool ShaderManagerClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	// Create and initialize the texture shader object.
	m_TextureShader = new TextureShaderClass;

	result = m_TextureShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	// Create and initialize the light shader object.
	m_LightShader = new LightShaderClass;

	result = m_LightShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	// Create and initialize the normal map shader object.
	m_NormalMapShader = new NormalMapShaderClass;

	result = m_NormalMapShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	// Create and initialize the spec map shader object.
	m_SpecMapShaderClass = new SpecMapShaderClass;

	result = m_SpecMapShaderClass->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void ShaderManagerClass::Shutdown()
{
	// Release the normal map shader object.
	if (m_NormalMapShader)
	{
		m_NormalMapShader->Shutdown();
		delete m_NormalMapShader;
		m_NormalMapShader = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	return;
}

bool ShaderManagerClass::RenderTextureShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	bool result;

	result = m_TextureShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture);
	if (!result)
	{
		return false;
	}
	return true;
}

bool ShaderManagerClass::RenderLightShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView* texture, CameraClass* camera, LightClass* directionalLight, XMFLOAT4 diffuseColor[], XMFLOAT4 lightPosition[])
{
	bool result;

	result = m_LightShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, camera, directionalLight, diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ShaderManagerClass::RenderNormalMapShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView* colorTexture, ID3D11ShaderResourceView* normalTexture, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor)
{
	bool result;

	result = m_NormalMapShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, colorTexture, normalTexture, lightDirection, diffuseColor);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ShaderManagerClass::RenderSpecMapShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView* texture1, ID3D11ShaderResourceView* texture2, ID3D11ShaderResourceView* texture3,
	XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor, XMFLOAT3 cameraPosition, XMFLOAT4 specularColor, float specularPower)
{
	bool result;

	result = m_SpecMapShaderClass->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture1, texture2, texture3, lightDirection, diffuseColor, cameraPosition, specularColor, specularPower);
	if (!result)
	{
		return false;
	}

	return true;
}
