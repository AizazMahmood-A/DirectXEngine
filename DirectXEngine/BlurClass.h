////////////////////////////////////////////////////////////////////////////////
// Filename: blurclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BLURCLASS_H_
#define _BLURCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "CameraClass.h"
#include "RenderTextureClass.h"
#include "OrthoWindowClass.h"
#include "TextureShaderClass.h"
#include "BlurShaderClass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: BlurClass
////////////////////////////////////////////////////////////////////////////////
class BlurClass
{
public:
    BlurClass();
    BlurClass(const BlurClass&);
    ~BlurClass();

    bool Initialize(D3DClass*, int, int, float, float, int, int);
    void Shutdown();

    bool BlurTexture(D3DClass*, CameraClass*, RenderTextureClass*, TextureShaderClass*, BlurShaderClass*);

private:
    RenderTextureClass* m_DownSampleTexture1, * m_DownSampleTexture2;
    OrthoWindowClass* m_DownSampleWindow, * m_UpSampleWindow;
    int m_downSampleWidth, m_downSampleHeight;
};

#endif