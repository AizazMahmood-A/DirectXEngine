////////////////////////////////////////////////////////////////////////////////
// Filename: shadow.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;

	matrix lightViewMatrix;
	matrix lightProjectionMatrix;

	matrix lightViewMatrix2;
	matrix lightProjectionMatrix2;

    matrix lightViewMatrix3;
	matrix lightProjectionMatrix3;

	matrix directionalLightViewMatrix;
	matrix directionalLightProjectionMatrix;
};

cbuffer LightPositionBuffer
{
	float3 lightPosition;
	float padding;
	float3 lightPosition2;
	float padding2;
    float3 lightPosition3;
    float padding3;
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 viewPosition : TEXCOORD1;
	float4 lightViewPosition : TEXCOORD2;
	float3 lightPos : TEXCOORD3;
	float4 lightViewPosition2 : TEXCOORD4;
	float3 lightPos2 : TEXCOORD5;
	float4 lightViewPosition3 : TEXCOORD6;
	float3 lightPos3 : TEXCOORD7;
	float4 directionalLightViewPosition : TEXCOORD8;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType ShadowVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Calculate the position of the vertice as viewed by the light source.
    output.lightViewPosition = output.position;//mul(input.position, worldMatrix);
	/*
    output.lightViewPosition = mul(output.lightViewPosition, lightViewMatrix);
    output.lightViewPosition = mul(output.lightViewPosition, lightProjectionMatrix);

	// Calculate the position of the vertice as viewed by the second light source.
    output.lightViewPosition2 = mul(input.position, worldMatrix);
    output.lightViewPosition2 = mul(output.lightViewPosition2, lightViewMatrix2);
    output.lightViewPosition2 = mul(output.lightViewPosition2, lightProjectionMatrix2);

	// Calculate the position of the vertice as viewed by the second light source.
    output.lightViewPosition3 = mul(input.position, worldMatrix);
    output.lightViewPosition3 = mul(output.lightViewPosition3, lightViewMatrix3);
    output.lightViewPosition3 = mul(output.lightViewPosition3, lightProjectionMatrix3);

	// Calculate the position of the vertice as viewed by the second light source.
    output.directionalLightViewPosition = mul(input.position, worldMatrix);
    output.directionalLightViewPosition = mul(output.directionalLightViewPosition, directionalLightViewMatrix);
    output.directionalLightViewPosition = mul(output.directionalLightViewPosition, directionalLightProjectionMatrix);
*/
	// Store the position of the vertice as viewed by the camera in a separate variable.
	output.viewPosition = output.position;

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

    // Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

    // Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix);

    output.lightPos = lightPosition.xyz - worldPosition.xyz;
    output.lightPos = normalize(output.lightPos);

    output.lightPos2 = lightPosition2.xyz - worldPosition.xyz;
    output.lightPos2 = normalize(output.lightPos2);

	output.lightPos3 = lightPosition3.xyz - worldPosition.xyz;
	output.lightPos3 = normalize(output.lightPos3);

    return output;
}