#include "Sphere.h"

Sphere::Sphere(float x, float y, float z, LPCWSTR texturePath, Camera* camera, ID3D11Device* device_ptr, ID3D11DeviceContext* device_context_ptr)
{
	LPCWSTR pixelPath;
	if (texturePath == LPCWSTR(L"noTex"))
	{
		pixelPath = LPCWSTR(L"WhitePixelShader.hlsl");
	}
	else
		pixelPath = LPCWSTR(L"PixelShader.hlsl");

	this->shader = new Shader(L"VertexShader.hlsl", pixelPath, device_ptr);
	this->setPosition(x, y, z);

	this->camera = camera;
	this->device_ptr = device_ptr;
	this->device_context_ptr = device_context_ptr;

	if (texturePath != LPCWSTR(L"noTex"))
	{
		ID3D11ShaderResourceView* srw = shader->getShaderResourceViewPtr();
		shader->getLoader().LoadTexture(texturePath, device_ptr, &this->texture, &srw);
		this->shader->setShaderResourceView(srw);
	}


	char* spherePath = new char[126]{ "TriSphere.obj" };
	char* mtlPath = new char[126];

	shader->getLoader().LoadObj(spherePath, vertices, texCoords, normals, mtlPath);

	//Fix this to get size from vertices later
	VertexData vertex_data_array[2280];
	//std::vector<VertexData> vertex_data_array(2280);
	for (int i = 0; i < 2280; i++)
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

	vertex_stride = sizeof(VertexData);
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

Sphere::Sphere()
{
}

void Sphere::setPosition(float x, float y, float z)
{
	this->position = XMFLOAT3(x, y, z);
	this->posVec = XMLoadFloat3(&this->position);
	this->shader->setModelMatrix(XMMatrixTranslation(
		this->position.x, this->position.y, this->position.z));
}

void Sphere::scaleSize(float x, float y, float z)
{
	shader->setModelMatrix(XMMatrixScaling(x, y, z) * XMMatrixTranslationFromVector(this->posVec));

}

XMVECTOR Sphere::getCENTERXMVECTOR()
{
	return posVec;
}

void Sphere::draw(ID3D11Buffer* light_constant_buffer, ID3D11Buffer* lightNr_constant_buffer,
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
	ID3D11ShaderResourceView* srw = shader->getShaderResourceViewPtr();
	if (srw != NULL)
	{
		device_context_ptr->PSSetShaderResources(0, 1, &srw);
	}
	if (light_constant_buffer != NULL) {
		device_context_ptr->PSSetConstantBuffers(0, 1, &light_constant_buffer);
		device_context_ptr->PSSetConstantBuffers(1, 1, &lightNr_constant_buffer);
		device_context_ptr->PSSetConstantBuffers(2, 1, &cam_pos_buffer);
	}
	//5. Output Merger
	device_context_ptr->Draw(vertex_count, 0);
}
