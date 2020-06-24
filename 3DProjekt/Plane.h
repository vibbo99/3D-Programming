#pragma once
#include "Header.h"
#include  "Shader.h"
#include "Camera.h"

using namespace DirectX;
class Plane
{
private:
	XMFLOAT3 position;
	XMVECTOR posVec;
	std::vector<XMFLOAT3> vertices;
	std::vector<XMFLOAT2> texCoords;
	std::vector<XMFLOAT3> normals;

	Material material;

	ID3D11Resource* texture = NULL;
	ID3D11Resource* normalTexture = NULL;
	ID3D11ShaderResourceView* textureResource = NULL;
	ID3D11ShaderResourceView* normalResource = NULL;
	ID3D11Buffer* vertex_buffer_ptr = NULL;
	ID3D11Buffer* material_buffer_ptr = NULL;

	Shader* shader;

	Camera* camera;
	ID3D11Device* device_ptr;
	ID3D11DeviceContext* device_context_ptr;

	VertexDataNormalMap vertex_data_array[6];

	UINT vertex_stride = 0;
	UINT vertex_offset = 0;
	UINT vertex_count = 0;

	void calcNormal(XMFLOAT3& tangent, XMFLOAT3& bitangent, XMFLOAT3& normal);
	void calcTangentBitangent(VertexDataNormalMap vertex1, VertexDataNormalMap vertex2, VertexDataNormalMap vertex3, XMFLOAT3& tangent, XMFLOAT3& bitangent);
	void calcModelVectors(std::vector<XMFLOAT3> vertices, std::vector<XMFLOAT2> texCoords, std::vector<XMFLOAT3> normals, VertexDataNormalMap vertexStruct[]);
	void setMaterialBuffer();
public:
	//Plane();
	Plane(float x, float y, float z, LPCWSTR texturePath, LPCWSTR normalPath, Camera* camera,
		ID3D11Device* device_ptr, ID3D11DeviceContext* device_context_ptr);
	Plane();
	//Pos Rot Scale
	void setPosition(float x, float y, float z);
	
	void rotateX(float x);
	void rotateY(float x);
	void rotateZ(float x);

	void scaleSize(float x, float y, float z);

	void draw(ID3D11Buffer* light_constant_buffer, ID3D11Buffer* lightNr_constant_buffer,
		ID3D11Buffer* cam_pos_buffer);

	///Getters
	XMFLOAT3 getPositionXMFLOAT3();
	XMVECTOR getPositionXMVECTOR();

	XMVECTOR getNormalInfo();
};
