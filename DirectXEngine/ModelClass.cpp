////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "ModelClass.h"
#include <sstream>
#include <algorithm>
#include "Utils.h"

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_model = 0;
	m_modelIndices = 0;
}

ModelClass::ModelClass(const ModelClass& other)
{

}

ModelClass::~ModelClass()
{

}

bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* modelFilename, char* textureFilename)
{
	bool result;

	// Load in the model data.
	if (Utils::hasExtension(modelFilename, "txt"))
	{
		result = LoadModel(modelFilename);
	}
	else if (Utils::hasExtension(modelFilename, "obj"))
	{
		result = LoadObjModel(modelFilename);
	}
	else
	{
		return false;
	}

	// Initialize the vertex and index buffer.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];

	// Create the index array.
	indices = new unsigned long[m_indexCount];

	// Load the vertex array
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	// Load Indicies array
	for (i = 0; i < m_indexCount; i++)
	{
		indices[i] = m_modelIndices[i];
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;

	// Create and initialize the texture object.
	m_Texture = new TextureClass;

	result = m_Texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
	return;
}

bool ModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;

	// Open the model file.
	fin.open(filename);

	// if it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	m_modelIndices = new unsigned long[m_indexCount];

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	for (i = 0; i < m_indexCount; i++)
	{
		m_modelIndices[i] = i;
	}

	// Close the model file.
	fin.close();

	return true;
}

bool ModelClass::LoadObjModel(char* filename)
{
	ifstream fin;
	std::string line;

	// Open the model file.
	fin.open(filename);

	// if it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	// Parse Obj Model
	struct Vector3
	{
		float x;
		float y;
		float z;
	};

	struct Vector2
	{
		float x;
		float y;
	};

	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Vector2> uvs;
	std::vector<ModelType> models;
	std::vector<unsigned long> indicies;

	getline(fin, line);
	getline(fin, line);
	getline(fin, line);
	getline(fin, line);
	int index = 0;

	while (getline(fin, line))
	{
		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;

		if (prefix == "v") {  // Vertex position
			Vector3 vec3;
			iss >> vec3.x >> vec3.y >> vec3.z;
			vertices.push_back(vec3);
		}
		else if (prefix == "vt") {  // Texture coordinates
			Vector2 vec2;
			iss >> vec2.x >> vec2.y;
			uvs.push_back(vec2);
		}
		else if (prefix == "vn") {  // Normal vectors
			Vector3 vec3;
			iss >> vec3.x >> vec3.y >> vec3.z;
			normals.push_back(vec3);
		}
		else if (prefix == "f") {	// Face Indicies
			std::string faceString;
			std::vector<Vector3> faces;

			while (iss >> faceString) {
				std::replace(faceString.begin(), faceString.end(), '/', ' ');
				std::istringstream vtnStream(faceString);

				Vector3 vec3;
				vtnStream >> vec3.x >> vec3.y >> vec3.z;
				vec3.x--, vec3.y--, vec3.z--;
				faces.push_back(vec3);
			}
			std::reverse(faces.begin(), faces.end());
			if (faces.size() == 3)
			{
				for (int i = 0; i < faces.size(); ++i) {
					ModelType model;
					Vector3 face = faces[i];

					Vector3 vertex = vertices[(int)face.x];
					Vector2 uv = uvs[(int)face.y];
					Vector3 normal = normals[(int)face.z];

					model.x = vertex.x;
					model.y = vertex.y;
					model.z = -vertex.z;

					model.tu = uv.x;
					model.tv = uv.y;

					model.nx = normal.x;
					model.ny = normal.y;
					model.nz = -normal.z;

					models.push_back(model);
					indicies.push_back(index++);
				}
			}
			else if (faces.size() == 4)
			{
				for (int i = 0; i < faces.size(); ++i) {
					ModelType model;
					Vector3 face = faces[i];

					Vector3 vertex = vertices[(int)face.x];
					Vector2 uv = uvs[(int)face.y];
					Vector3 normal = normals[(int)face.z];

					model.x = vertex.x;
					model.y = vertex.y;
					model.z = -vertex.z;

					model.tu = uv.x;
					model.tv = uv.y;

					model.nx = normal.x;
					model.ny = normal.y;
					model.nz = -normal.z;

					models.push_back(model);
				}

				indicies.push_back(index);
				indicies.push_back(index + 1);
				indicies.push_back(index + 2);

				indicies.push_back(index);
				indicies.push_back(index + 2);
				indicies.push_back(index + 3);
				index += 4;
			}
		}
	}

	// Set index count to be equal to vertex count.
	m_vertexCount = models.size();
	m_indexCount = indicies.size();

	// Create the model using vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	m_modelIndices = new unsigned long[m_indexCount];

	// Assign values to properties
	for (int i = 0; i < m_vertexCount; i++)
	{
		m_model[i] = models[i];
	}
	for (int i = 0; i < m_indexCount; i++)
	{
		m_modelIndices[i] = indicies[i];
	}

	fin.close();
	return true;
}

void ModelClass::ApplyTransformations(XMMATRIX translation, XMMATRIX scale, float rotation)
{
	m_position = translation;
	m_scale = scale;;
	m_rotation = rotation;
}

XMMATRIX ModelClass::GetTransform()
{
	XMMATRIX worldMatrix, srMatrix;
	srMatrix = XMMatrixMultiply(m_scale, XMMatrixRotationY(m_rotation));
	worldMatrix = XMMatrixMultiply(srMatrix, m_position);
	return worldMatrix;
}

XMMATRIX ModelClass::GetPosition()
{
	return m_position;
}

XMMATRIX ModelClass::GetScale()
{
	return m_scale;
}

float ModelClass::GetRotation()
{
	return m_rotation;
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}
	if (m_modelIndices)
	{
		delete[] m_modelIndices;
		m_modelIndices = 0;
	}
	return;
}

