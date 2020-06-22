#include "Shader.h"

void Shader::_createConstantBuffer(ID3D11Device* device_ptr)
{
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(MVP); //DirectX::XMMATRIX
	cbDesc.Usage = D3D11_USAGE_DYNAMIC; //D3D11_USAGE_DEFAULT
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &constantBufferMatrices; //&transformMatrix
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT hr = device_ptr->CreateBuffer(&cbDesc, &InitData, &constant_buffer_ptr);

	if (FAILED(hr))
		assert(SUCCEEDED(hr));
}

void Shader::_createSamplerDesc(ID3D11Device* device_ptr)
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device_ptr->CreateSamplerState(&samplerDesc, &samplerState);
}

Shader::Shader(LPCWSTR vertexPath, LPCWSTR fragmentPath, ID3D11Device* device_ptr, bool hasNormalMap)
{
	if (!hasNormalMap)
	{
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		flags |= D3DCOMPILE_DEBUG;
#endif

		// For the Vertex Shader


		HRESULT hr = D3DCompileFromFile(
			vertexPath,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"vs_main",
			"vs_5_0",
			flags,
			0,
			&vs_blob_ptr,
			&error_blob
		);

		assert(SUCCEEDED(hr));

		hr = device_ptr->CreateVertexShader(
			vs_blob_ptr->GetBufferPointer(),
			vs_blob_ptr->GetBufferSize(),
			NULL,
			&vertex_shader_ptr
		);

		assert(SUCCEEDED(hr));

		//CHANGE LATER!!!!!!!!!!!!!!
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 3 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 5 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		hr = device_ptr->CreateInputLayout(
			inputElementDesc,
			ARRAYSIZE(inputElementDesc),
			vs_blob_ptr->GetBufferPointer(),
			vs_blob_ptr->GetBufferSize(),
			&input_layout_ptr
		);
		assert(SUCCEEDED(hr));
		vs_blob_ptr->Release();

		// For the Pixel Shader
		if (error_blob) error_blob->Release();
		error_blob = nullptr;

		hr = D3DCompileFromFile(
			fragmentPath,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"ps_main",
			"ps_5_0",
			flags,
			0,
			&ps_blob_ptr,
			&error_blob
		);

		if (FAILED(hr))
		{
			if (error_blob)
			{
				OutputDebugStringA((char*)error_blob->GetBufferPointer());
				error_blob->Release();
			}
			if (ps_blob_ptr)
				ps_blob_ptr->Release();
			assert(false);
		}
		assert(SUCCEEDED(hr));

		hr = device_ptr->CreatePixelShader(
			ps_blob_ptr->GetBufferPointer(),
			ps_blob_ptr->GetBufferSize(),
			NULL, &pixel_shader_ptr
		);
		assert(SUCCEEDED(hr));
		ps_blob_ptr->Release();



		this->_createConstantBuffer(device_ptr);
		this->_createSamplerDesc(device_ptr);
	}
	else
	{
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		flags |= D3DCOMPILE_DEBUG;
#endif

		// For the Vertex Shader


		HRESULT hr = D3DCompileFromFile(
			vertexPath,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"vs_main",
			"vs_5_0",
			flags,
			0,
			&vs_blob_ptr,
			&error_blob
		);

		assert(SUCCEEDED(hr));

		hr = device_ptr->CreateVertexShader(
			vs_blob_ptr->GetBufferPointer(),
			vs_blob_ptr->GetBufferSize(),
			NULL,
			&vertex_shader_ptr
		);

		assert(SUCCEEDED(hr));

		//CHANGE LATER!!!!!!!!!!!!!!
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 3 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 5 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 8 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{"BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 11 * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		hr = device_ptr->CreateInputLayout(
			inputElementDesc,
			ARRAYSIZE(inputElementDesc),
			vs_blob_ptr->GetBufferPointer(),
			vs_blob_ptr->GetBufferSize(),
			&input_layout_ptr
		);
		assert(SUCCEEDED(hr));
		vs_blob_ptr->Release();

		// For the Pixel Shader
		if (error_blob) error_blob->Release();
		error_blob = nullptr;

		hr = D3DCompileFromFile(
			fragmentPath,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"ps_main",
			"ps_5_0",
			flags,
			0,
			&ps_blob_ptr,
			&error_blob
		);

		if (FAILED(hr))
		{
			if (error_blob)
			{
				OutputDebugStringA((char*)error_blob->GetBufferPointer());
				error_blob->Release();
			}
			if (ps_blob_ptr)
				ps_blob_ptr->Release();
			assert(false);
		}
		assert(SUCCEEDED(hr));

		hr = device_ptr->CreatePixelShader(
			ps_blob_ptr->GetBufferPointer(),
			ps_blob_ptr->GetBufferSize(),
			NULL, &pixel_shader_ptr
		);
		assert(SUCCEEDED(hr));
		ps_blob_ptr->Release();



		this->_createConstantBuffer(device_ptr);
		this->_createSamplerDesc(device_ptr);
	}
}

Shader::~Shader()
{
	vertex_shader_ptr->Release();
	if (geometry_shader_ptr != NULL)
	{
		geometry_shader_ptr->Release();
	}
	pixel_shader_ptr->Release();
}

void Shader::useShader(ID3D11DeviceContext* device_context_ptr)
{
	
	device_context_ptr->VSSetConstantBuffers(0, 1, &constant_buffer_ptr); //move this to cube/plane classes later
	
	//Set the vertex shader
	device_context_ptr->VSSetShader(vertex_shader_ptr, NULL, 0);
	//Set the geo shader
	if (geometry_shader_ptr != NULL)
	{
		device_context_ptr->GSSetShader(geometry_shader_ptr, NULL, 0);
	}
	//Set the pixel shader
	device_context_ptr->PSSetShader(pixel_shader_ptr, NULL, 0);
}

void Shader::updateShader(ID3D11DeviceContext* device_context_ptr, Camera* camera)
{
	//Move matrix calculations to object instead of shader later
	this->constantBufferMatrices.view = camera->getViewMatrix();
	this->constantBufferMatrices.perspective = camera->getProjectionMatrix();
	XMMATRIX mvp = this->constantBufferMatrices.model;
	mvp *= camera->getViewMatrix();
	mvp *= camera->getProjectionMatrix();
	this->constantBufferMatrices.modelViewPerspective = mvp;

	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	ZeroMemory(&mappedSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	device_context_ptr->Map(this->constant_buffer_ptr, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
	memcpy(mappedSubResource.pData, &this->constantBufferMatrices, sizeof(MVP));
	device_context_ptr->Unmap(this->constant_buffer_ptr, 0);

}

void Shader::bindTexture(ID3D11DeviceContext* device_context_ptr)
{
	//device_context_ptr->PSSetShaderResources(0, 1, shader_resource_view_ptr);
}

void Shader::setModelMatrix(DirectX::XMMATRIX model)
{
	constantBufferMatrices.model = model;
}

void Shader::setViewMatrix(DirectX::XMMATRIX view)
{
	constantBufferMatrices.view = view;
}

void Shader::setPerspectiveMatrix(DirectX::XMMATRIX perspective)
{
	constantBufferMatrices.perspective = perspective;
}

void Shader::setMVPMatrix(DirectX::XMMATRIX mvp)
{
	constantBufferMatrices.modelViewPerspective = mvp;
}

void Shader::setShaderResourceView(ID3D11ShaderResourceView* srw)
{
	this->shader_resource_view_ptr = srw;
}
