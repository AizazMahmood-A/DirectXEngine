////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "lightclass.h"


LightClass::LightClass()
{
    IsOrtho = false;
    m_depthMap = 0;
}


LightClass::LightClass(const LightClass& other)
{
}


LightClass::~LightClass()
{

}

bool LightClass::InitializeShadowMaps(HWND hwnd, ID3D11Device* device, int textureWidth, int textureHeight, float screenDepth, float screenNear, int format)
{
    // Create and initialize the render to texture object.
    m_depthMap = new RenderTextureClass;

    bool result = m_depthMap->Initialize(device, textureWidth, textureHeight, screenDepth, screenNear, 1);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the render texture object.", L"Error", MB_OK);
        return false;
    }

    return true;
}

RenderTextureClass* LightClass::GetShadowMap()
{
    return m_depthMap;
}

void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
    m_ambientColor = XMFLOAT4(red, green, blue, alpha);
    return;
}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
    m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
    return;
}


void LightClass::SetDirection(float x, float y, float z)
{
    m_direction = XMFLOAT3(x, y, z);
    return;
}

void LightClass::SetSpecularColor(float red, float green, float blue, float alpha)
{
    m_specularColor = XMFLOAT4(red, green, blue, alpha);
    return;
}


void LightClass::SetSpecularPower(float power)
{
    m_specularPower = power;
    return;
}

void LightClass::SetPosition(float x, float y, float z)
{
    m_position = XMFLOAT3(x, y, z);
    return;
}

void LightClass::SetLookAt(float x, float y, float z)
{
    m_lookAt = XMFLOAT3(x, y, z);
    return;
}

XMFLOAT4 LightClass::GetAmbientColor()
{
    return m_ambientColor;
}

XMFLOAT4 LightClass::GetDiffuseColor()
{
    return m_diffuseColor;
}

XMFLOAT3 LightClass::GetDirection()
{
    return m_direction;
}

XMFLOAT4 LightClass::GetSpecularColor()
{
    return m_specularColor;
}

float LightClass::GetSpecularPower()
{
    return m_specularPower;
}

XMFLOAT3 LightClass::GetPosition()
{
    return m_position;
}

void LightClass::GenerateViewMatrix()
{
    XMFLOAT3 up;
    XMVECTOR positionVector, lookAtVector, upVector;

    // Setup the vectpr that points upwards.
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;

    // Load the XMFLOAT3 into XMVECTOR.
    positionVector = XMLoadFloat3(&m_position);
    lookAtVector = XMLoadFloat3(&m_lookAt);
    upVector = XMLoadFloat3(&up);

    // Create the view matrix from the three vectors.
    m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

    return;
}

void LightClass::GenerateProjectionMatrix(float screenDepth, float screenNear)
{
    float fieldOfView, screenAspect;


    // Setup field of view and screen aspect for a square light source.
    fieldOfView = 3.14159265358979323846f / 2.0f;
    screenAspect = 1.0f;

    // Create the projection matrix for the light.
    m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

    return;
}

void LightClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
    viewMatrix = m_viewMatrix;
    return;
}

void LightClass::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
    projectionMatrix = m_projectionMatrix;
    return;
}

void LightClass::GenerateOrthoMatrix(float width, float depthPlane, float nearPlane)
{

    // Create the orthographic for the light
    m_orthoMatrix = XMMatrixOrthographicLH(width, width, nearPlane, depthPlane);
    IsOrtho = true;
    return;
}

void LightClass::GetOrthoMatrix(XMMATRIX& orthoMatrix)
{
    orthoMatrix = m_orthoMatrix;
    return;
}