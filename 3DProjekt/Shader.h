#pragma once
#include "Header.h"
#include "objLoader.h"
#include "Camera.h"
class Shader
{
private:
	ID3DBlob* vs_blob_ptr = nullptr;
	ID3DBlob* ps_blob_ptr = nullptr;
	ID3DBlob* error_blob = nullptr;

	ID3D11VertexShader* vertex_shader_ptr = NULL;
	ID3D11GeometryShader* geometry_shader_ptr = NULL;
	ID3D11PixelShader* pixel_shader_ptr = NULL;

	ID3D11InputLayout* input_layout_ptr = NULL;

	ID3D11Buffer* constant_buffer_ptr = NULL;

	ID3D11ShaderResourceView* shader_resource_view_ptr = NULL;
	ID3D11Resource* texture = NULL;
	ID3D11SamplerState* samplerState = NULL;

	ObjLoader loader;
	
	MVP constantBufferMatrices;
	void _createConstantBuffer(ID3D11Device* device_ptr);
	void _createSamplerDesc(ID3D11Device* device_ptr);
public:
	Shader(LPCWSTR vertexPath, LPCWSTR fragmentPath, ID3D11Device* device_ptr, bool hasNormalMap = false);
	~Shader();

	void useShader(ID3D11DeviceContext* device_context_ptr);
	void updateShader(ID3D11DeviceContext* device_context_ptr, Camera* camera);
	void bindTexture(ID3D11DeviceContext* device_context_ptr);
	//Getters
	ID3D11InputLayout* getInputLayoutPtr() { return this->input_layout_ptr; }
	ID3D11ShaderResourceView* getShaderResourceViewPtr() { return shader_resource_view_ptr; }
	ID3D11Resource* getTexturePtr() { return texture; }
	ID3D11SamplerState* const * getSamplerStatePtr() { return &this->samplerState; }
	ObjLoader& getLoader() { return this->loader; }
	//Setters
	void setModelMatrix(DirectX::XMMATRIX model);
	void setViewMatrix(DirectX::XMMATRIX view);
	void setPerspectiveMatrix(DirectX::XMMATRIX perspective);
	void setMVPMatrix(DirectX::XMMATRIX mvp);
	void setShaderResourceView(ID3D11ShaderResourceView* srw);
	
};
