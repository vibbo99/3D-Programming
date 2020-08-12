#include "Skybox.h"


Skybox::Skybox(std::wstring texturePath, Camera* camera,
	ID3D11Device* device_ptr, ID3D11DeviceContext* device_context_ptr)
{
	ID3D11Texture2D* texture;

	this->shader = new Shader(L"SkyBoxVS.hlsl", L"SkyBoxPS.hlsl", device_ptr);

	this->camera = camera;
	this->device_ptr = device_ptr;
	this->device_context_ptr = device_context_ptr;

	char* SkyboxPath = new char[126]{ "SkyBox2.obj" };
	char* mtlPath = new char[126];
	shader->getLoader().LoadObj(SkyboxPath, vertices, texCoords, normals, mtlPath);

	char* texPath = new char[126];
	
	//Fix this to get size from vertices later
	// std vector "gör pekare till data"
	VertexData vertex_data_array[36];
	for (int i = 0; i < vertices.size(); i++)
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
	vertex_count = vertices.size();

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

	ID3D11ShaderResourceView* srw = shader->getShaderResourceViewPtr();

	HRESULT hr = CreateDDSTextureFromFile(device_ptr, texturePath.c_str(), (ID3D11Resource**)&texture, &srw);

	D3D11_TEXTURE2D_DESC skymapTextureDesc;
	texture->GetDesc(&skymapTextureDesc);
	D3D11_SHADER_RESOURCE_VIEW_DESC textureShaderResourceViewDesc;
	textureShaderResourceViewDesc.Format = skymapTextureDesc.Format;
	textureShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	textureShaderResourceViewDesc.TextureCube.MipLevels = skymapTextureDesc.MipLevels;
	textureShaderResourceViewDesc.TextureCube.MostDetailedMip = 0;

	hr = device_ptr->CreateShaderResourceView(texture, &textureShaderResourceViewDesc, &srw);
	this->shader->setShaderResourceView(srw);

	D3D11_DEPTH_STENCIL_DESC skyBoxDepthDesc;
	ZeroMemory(&skyBoxDepthDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	skyBoxDepthDesc.DepthEnable = true;
	skyBoxDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	skyBoxDepthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	hr = device_ptr->CreateDepthStencilState(&skyBoxDepthDesc, &skybox_depth_stencil_state_ptr);


}


void Skybox::draw(ID3D11Buffer* light_constant_buffer, ID3D11Buffer* lightNr_constant_buffer,
	ID3D11Buffer* cam_pos_buffer)
{
	XMFLOAT3 cameraPos = this->camera->getPositionFloat3();

	//Position
	this->shader->setModelMatrix(XMMatrixTranslation(
		cameraPos.x, cameraPos.y, cameraPos.z));
	
	//1. Refresh constant buffers
	shader->updateShader(device_context_ptr, camera);
	//2. Input Assembly
	device_context_ptr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device_context_ptr->IASetInputLayout(shader->getInputLayoutPtr());
	device_context_ptr->IASetVertexBuffers(0, 1, &vertex_buffer_ptr, &vertex_stride, &vertex_offset);
	//3. Vertex Shader
	shader->useShader(device_context_ptr);
	ID3D11ShaderResourceView* srw = shader->getShaderResourceViewPtr();
	//4. Pixel Shader
	device_context_ptr->PSSetShaderResources(0, 1, &srw);
	//5. Output Merger

	device_context_ptr->OMSetDepthStencilState(skybox_depth_stencil_state_ptr, 0);
	device_context_ptr->Draw(vertex_count, 0);
}
