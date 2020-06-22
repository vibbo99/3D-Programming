#pragma once
#include "Header.h"

class Window
{
private:
	
	HWND hwnd;
	ID3D11Device* device_ptr = NULL;
	ID3D11DeviceContext* device_context_ptr = NULL;
	IDXGISwapChain* swap_chain_ptr = NULL;
	ID3D11RenderTargetView* render_target_view_ptr = NULL;
	ID3D11ShaderResourceView* shaderRV = NULL;

	D3D11_VIEWPORT* viewport = NULL;

	ID3D11Texture2D* pDepthStencilTex = NULL;
	ID3D11DepthStencilView* pDepthStencilView = NULL;

	// SkyBox
	ID3D11Buffer* sphereIndexBuffer;
	ID3D11Buffer* sphereVertBuffer;
	ID3D11VertexShader* SKYMAP_VS;
	ID3D11PixelShader* SKYMAP_PS;
	ID3D10Blob* SKYMAP_VS_Buffer;
	ID3D10Blob* SKYMAP_PS_Buffer;
	ID3D11ShaderResourceView* smrv;
	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSCullNone;


	bool quitWindow = false;
	void _createSwapChain();
	void _createDepthBuffer();
	void _createDepthState();
public:
	Window(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow);
	~Window();

	//GETTERS
	HWND getHwind() { return this->hwnd; }
	ID3D11Device* getDevicePtr(){ return this->device_ptr; }
	ID3D11DeviceContext* getDeviceContextPtr() { return this->device_context_ptr; }
	ID3D11RenderTargetView* const* getRenderTargetViewPtr() { return  &this->render_target_view_ptr; }
	IDXGISwapChain* getSwapChainPtr() { return this->swap_chain_ptr; }
	D3D11_VIEWPORT* getViewportPtr() { return this->viewport; }
	ID3D11DepthStencilView* getDepthStencilViewPtr() { return this->pDepthStencilView; }

	//SkyBox
	ID3D11Buffer* getSphereIndexBuffer() { return this->sphereIndexBuffer; }
	ID3D11Buffer* getSphereVertBuffer() { return this->sphereVertBuffer; }
	ID3D11VertexShader* getSKYMAP_VS() { return this->SKYMAP_VS; }
	ID3D11PixelShader* getSKYMAP_PS() { return this->SKYMAP_PS; }
	ID3D10Blob* getSKYMAP_VSBuffer() { return this->SKYMAP_VS_Buffer; }
	ID3D10Blob* getSKYMAP_PS_Buffer() { return this->SKYMAP_PS_Buffer; }
	ID3D11ShaderResourceView* getSMRV() { return this->smrv; }
	ID3D11DepthStencilState* getDSLessEqual() { return this->DSLessEqual; }
	ID3D11RasterizerState* getRSCullNone() { return this->RSCullNone; }


	//SETTERS



	//MISC
	void clearDepthStencil();
	bool update();

};
