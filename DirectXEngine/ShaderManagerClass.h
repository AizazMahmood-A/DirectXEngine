////////////////////////////////////////////////////////////////////////////////
// Filename: shadermanagerclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SHADERMANAGERCLASS_H_
#define _SHADERMANAGERCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureshaderclass.h"
#include "lightshaderclass.h"
#include "normalmapshaderclass.h"
#include "SpecmapShaderClass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ShaderManagerClass
////////////////////////////////////////////////////////////////////////////////
class ShaderManagerClass
{
public:
    ShaderManagerClass();
    ShaderManagerClass(const ShaderManagerClass&);
    ~ShaderManagerClass();

    bool Initialize(ID3D11Device*, HWND);
    void Shutdown();

    bool RenderTextureShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);
    bool RenderLightShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, CameraClass*, LightClass*, XMFLOAT4[], XMFLOAT4[]);
    bool RenderNormalMapShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4);
    bool RenderSpecMapShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*,
        XMFLOAT3, XMFLOAT4, XMFLOAT3, XMFLOAT4, float);

private:
    TextureShaderClass* m_TextureShader;
    LightShaderClass* m_LightShader;
    NormalMapShaderClass* m_NormalMapShader;
    SpecMapShaderClass* m_SpecMapShaderClass;

};
#endif