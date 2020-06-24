#include "Plane.h"

void Plane::calcNormal(XMFLOAT3& tangent, XMFLOAT3& bitangent, XMFLOAT3& normal)
{
	float lenght;

	normal.x = (tangent.y * bitangent.z) - (tangent.z * bitangent.y);
	normal.y = (tangent.z * bitangent.x) - (tangent.x * bitangent.z);
	normal.z = (tangent.x * bitangent.y) - (tangent.y * bitangent.x);

	lenght = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	normal.x = normal.x / lenght;
	normal.y = normal.y / lenght;
	normal.z = normal.z / lenght;
}

void Plane::calcTangentBitangent(VertexDataNormalMap vertex1, VertexDataNormalMap vertex2, VertexDataNormalMap vertex3, XMFLOAT3& tangent, XMFLOAT3& bitangent)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;

	vector1[0] = vertex2.pX - vertex1.pX;
	vector1[1] = vertex2.pY - vertex1.pY;
	vector1[2] = vertex2.pZ - vertex1.pZ;

	vector2[0] = vertex3.pX - vertex1.pX;
	vector2[1] = vertex3.pY - vertex1.pY;
	vector2[2] = vertex3.pZ - vertex1.pZ;

	tuVector[0] = vertex2.u - vertex1.u;
	tvVector[0] = vertex2.v - vertex1.v;

	tuVector[1] = vertex3.u - vertex1.u;
	tvVector[1] = vertex3.v - vertex1.v;

	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	bitangent.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	bitangent.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	bitangent.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;


	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	length = sqrt((bitangent.x * bitangent.x) + (bitangent.y * bitangent.y) + (bitangent.z * bitangent.z));

	bitangent.x = bitangent.x / length;
	bitangent.y = bitangent.y / length;
	bitangent.z = bitangent.z / length;

}

void Plane::calcModelVectors(std::vector<XMFLOAT3> vertices, std::vector<XMFLOAT2> texCoords, std::vector<XMFLOAT3> normals, VertexDataNormalMap vertexStruct[])
{
	int faceCount, i, index;
	VertexDataNormalMap vertex1, vertex2, vertex3;
	XMFLOAT3 tangent, bitangent, normal;
	int vertexCount = vertices.size();

	faceCount = vertexCount / 3;

	index = 0;

	for (i = 0; i < faceCount; i++)
	{
		vertex1.pX = vertices[index].x;
		vertex1.pY = vertices[index].y;
		vertex1.pZ = vertices[index].z;
		vertex1.u = texCoords[index].x;
		vertex1.v = texCoords[index].y;
		vertex1.nX = normals[index].x;
		vertex1.nY = normals[index].y;
		vertex1.nZ = normals[index].z;
		index++;

		vertex2.pX = vertices[index].x;
		vertex2.pY = vertices[index].y;
		vertex2.pZ = vertices[index].z;
		vertex2.u = texCoords[index].x;
		vertex2.v = texCoords[index].y;
		vertex2.nX = normals[index].x;
		vertex2.nY = normals[index].y;
		vertex2.nZ = normals[index].z;
		index++;

		vertex3.pX = vertices[index].x;
		vertex3.pY = vertices[index].y;
		vertex3.pZ = vertices[index].z;
		vertex3.u = texCoords[index].x;
		vertex3.v = texCoords[index].y;
		vertex3.nX = normals[index].x;
		vertex3.nY = normals[index].y;
		vertex3.nZ = normals[index].z;
		index++;


		calcTangentBitangent(vertex1, vertex2, vertex3, tangent, bitangent);
		calcNormal(tangent, bitangent, normal);

		vertexStruct[index - 1].nX = normal.x;
		vertexStruct[index - 1].nY = normal.y;
		vertexStruct[index - 1].nZ = normal.z;
		vertexStruct[index - 1].tX = tangent.x;
		vertexStruct[index - 1].tY = tangent.y;
		vertexStruct[index - 1].tZ = tangent.z;
		vertexStruct[index - 1].bX = bitangent.x;
		vertexStruct[index - 1].bY = bitangent.y;
		vertexStruct[index - 1].bZ = bitangent.z;

		vertexStruct[index - 2].nX = normal.x;
		vertexStruct[index - 2].nY = normal.y;
		vertexStruct[index - 2].nZ = normal.z;
		vertexStruct[index - 2].tX = tangent.x;
		vertexStruct[index - 2].tY = tangent.y;
		vertexStruct[index - 2].tZ = tangent.z;
		vertexStruct[index - 2].bX = bitangent.x;
		vertexStruct[index - 2].bY = bitangent.y;
		vertexStruct[index - 2].bZ = bitangent.z;

		vertexStruct[index - 3].nX = normal.x;
		vertexStruct[index - 3].nY = normal.y;
		vertexStruct[index - 3].nZ = normal.z;
		vertexStruct[index - 3].tX = tangent.x;
		vertexStruct[index - 3].tY = tangent.y;
		vertexStruct[index - 3].tZ = tangent.z;
		vertexStruct[index - 3].bX = bitangent.x;
		vertexStruct[index - 3].bY = bitangent.y;
		vertexStruct[index - 3].bZ = bitangent.z;


	}
}

void Plane::setMaterialBuffer()
{
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(Material);
	desc.Usage = D3D11_USAGE_DYNAMIC; //D3D11_USAGE_DEFAULT
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &material;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	HRESULT hr = device_ptr->CreateBuffer(&desc, &initData, &material_buffer_ptr);

	if (FAILED(hr))
		assert(FAILED(hr));
}


Plane::Plane()
{
}

Plane::Plane(float x, float y, float z, LPCWSTR texturePath, LPCWSTR normalPath, Camera* camera, ID3D11Device* device_ptr, ID3D11DeviceContext* device_context_ptr)
{
	//this->shader = new Shader(L"NormalMappingVertexShader.hlsl", L"NormalMappingPixelShader.hlsl", device_ptr, true);
	this->shader = new Shader(L"NormalMappingVertexShader.hlsl", L"NormalMappingPixelShader.hlsl", device_ptr, true);
	this->setPosition(x, y, z);

	this->camera = camera;
	this->device_ptr = device_ptr;
	this->device_context_ptr = device_context_ptr;

	if (texturePath != L"noTex")
	{
		shader->getLoader().LoadTexture(texturePath, device_ptr, &this->texture, &textureResource);
		//shader->setShaderResourceView(textureResource);
	}
	if (normalPath != L"noTex")
	{
		shader->getLoader().LoadTexture(normalPath, device_ptr, &this->normalTexture, &normalResource);
		//shader->setShaderResourceView(textureResource);
	}
	char* planePath = new char[126]{ "TriPlane.obj" };
	char* mtlPath = new char[126]; // { "TriPlaneWMat.mtl" }
	shader->getLoader().LoadObj(planePath, vertices, texCoords, normals, mtlPath);
	char* texPath = new char[126];

	shader->getLoader().LoadMtl(mtlPath, material.m_diffuse, material.m_ambient, material.m_specular, texPath);
	material.m_otherInfo.y = 1;

	
	setMaterialBuffer();
	
	for (int i = 0; i < 6; i++)
	{
		//Copy vertices
		vertex_data_array[i].pX = vertices[i].x;
		vertex_data_array[i].pY = vertices[i].y;
		vertex_data_array[i].pZ = vertices[i].z;

		//Copy UVs
		vertex_data_array[i].u = texCoords[i].x;
		vertex_data_array[i].v = texCoords[i].y;

		//Copy normals
		vertex_data_array[i].nX = normals[i].x;
		vertex_data_array[i].nY = normals[i].y;
		vertex_data_array[i].nZ = normals[i].z;
	}

	calcModelVectors(vertices, texCoords, normals, vertex_data_array);
	


	vertex_stride = sizeof(VertexDataNormalMap);
	vertex_offset = 0;
	vertex_count = sizeof(vertex_data_array);

	D3D11_BUFFER_DESC vertex_buff_descr = {};
	vertex_buff_descr.ByteWidth = sizeof(vertex_data_array);
	vertex_buff_descr.Usage = D3D11_USAGE_DEFAULT;
	vertex_buff_descr.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sr_data = { 0 };
	sr_data.pSysMem = vertex_data_array;

	HRESULT hrCreateVBuffer = device_ptr->CreateBuffer(
		&vertex_buff_descr,
		&sr_data,
		&vertex_buffer_ptr
	);

	assert(SUCCEEDED(hrCreateVBuffer));
}

void Plane::setPosition(float x, float y, float z)
{
	this->position = XMFLOAT3(x, y, z);
	this->posVec = XMLoadFloat3(&this->position);
	this->shader->setModelMatrix(XMMatrixTranslation(
		this->position.x, this->position.y, this->position.z));
}


void Plane::rotateX(float x)
{
	shader->setModelMatrix(XMMatrixRotationX(x));
}

void Plane::rotateY(float x)
{
	shader->setModelMatrix(XMMatrixRotationY(x));
}

void Plane::rotateZ(float x)
{
	shader->setModelMatrix(XMMatrixRotationZ(x));
}

void Plane::scaleSize(float x, float y, float z)
{
	shader->setModelMatrix(XMMatrixScaling(x, y, z) * XMMatrixTranslationFromVector(this->posVec));
	//FXMMATRIX world = XMMatrixScaling(x, y, z) * XMMatrixTranslationFromVector(this->posVec);
	//XMMATRIX XMworld = XMMatrixInverse(&XMMatrixDeterminant(world), world);
	//XMworld = XMMatrixTranspose(XMworld);
	//shader->setModelMatrix(XMworld);
}



void Plane::draw(ID3D11Buffer* light_constant_buffer, ID3D11Buffer* lightNr_constant_buffer,
	ID3D11Buffer* cam_pos_buffer)
{
	//1. Refresh constant buffers
	shader->updateShader(device_context_ptr, camera);
	//2. Input Assembly
	device_context_ptr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device_context_ptr->IASetInputLayout(shader->getInputLayoutPtr());
	device_context_ptr->IASetVertexBuffers(0, 1, &vertex_buffer_ptr, &vertex_stride, &vertex_offset);
	//3. Vertex Shader
	shader->useShader(device_context_ptr);
	//4. Pixel Shader
	//ID3D11ShaderResourceView* srw = shader->getShaderResourceViewPtr();
	if (textureResource != NULL)
	{
		device_context_ptr->PSSetShaderResources(0, 1, &textureResource);

	}
	if (normalResource != NULL)
	{
		device_context_ptr->PSSetShaderResources(1, 1, &normalResource);
	}
	if (light_constant_buffer != NULL)
	{
		device_context_ptr->PSSetConstantBuffers(0, 1, &light_constant_buffer);
		device_context_ptr->PSSetConstantBuffers(1, 1, &lightNr_constant_buffer);
		device_context_ptr->PSSetConstantBuffers(2, 1, &cam_pos_buffer);
	}
	//device_context_ptr->PSSetConstantBuffers(3, 1, &material_buffer_ptr);
	//5. Output Merger

	device_context_ptr->Draw(vertex_count, 0);
}

XMFLOAT3 Plane::getPositionXMFLOAT3()
{
	return this->position;
}

XMVECTOR Plane::getPositionXMVECTOR()
{
	return this->posVec;
}

XMVECTOR Plane::getNormalInfo()
{
	return XMVectorSet(normals[0].x, normals[0].y, normals[0].z, 1.0f);
}
