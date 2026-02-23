////////////////////////////////////////////////////////////////////////////////
// Filename: rendertextureclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _RENDERTEXTUREARRAYCLASS_H_
#define _RENDERTEXTUREARRAYCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
//#include "LightShaderClass.h"
using namespace DirectX;
const int NUM_LIGHTS = 2;
////////////////////////////////////////////////////////////////////////////////
// Class name: RenderTextureClass
////////////////////////////////////////////////////////////////////////////////
class RenderTextureArrayClass
{
public:
	RenderTextureArrayClass();
	RenderTextureArrayClass(const RenderTextureArrayClass&);
	~RenderTextureArrayClass();

	bool Initialize(ID3D11Device*, int, int, float, float, int, int arraySize);
	void Shutdown();

	void SetRenderTarget(ID3D11DeviceContext*, int index);
	void ClearRenderTarget(ID3D11DeviceContext*, float, float, float, float, int index);
	ID3D11ShaderResourceView* GetShaderResourceView();
	//void SetShaderResourceView(ID3D11ShaderResourceView* view);

	void GetProjectionMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	int GetTextureWidth();
	int GetTextureHeight();

private:
	int m_textureWidth, m_textureHeight;
	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11RenderTargetView* m_renderTargetView[NUM_LIGHTS] = { nullptr };
	ID3D11ShaderResourceView* m_shaderResourceView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView[NUM_LIGHTS] = { nullptr };
	D3D11_VIEWPORT m_viewport;
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_orthoMatrix;
};

#endif
