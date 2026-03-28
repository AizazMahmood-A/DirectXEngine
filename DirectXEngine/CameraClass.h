////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

//////////////
// INCLUDES //
//////////////
#include <directxmath.h>
using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void GetPosition(XMVECTOR&);

	void Render();
	void GetViewMatrix(XMMATRIX&);

	void RenderReflection(float);
	void GetReflectionViewMatrix(XMMATRIX&);

	void RenderBaseViewMatrix();
	void GetBaseViewMatrix(XMMATRIX& viewMatrix);

	const XMVECTOR& GetForwardVector();
	const XMVECTOR& GetUpVector();
	const XMVECTOR& GetRightVector();

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	XMMATRIX m_viewMatrix;
	XMMATRIX m_reflectionViewMatrix;
	XMMATRIX m_baseViewMatrix;

	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR m_forwardVec;
	XMVECTOR m_upVec;
	XMVECTOR m_rightVec;
};

#endif