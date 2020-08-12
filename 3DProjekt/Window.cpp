#include "Window.h"



void Window::_createSwapChain()
{
	DXGI_SWAP_CHAIN_DESC swap_chain_descr = { 0 };

	swap_chain_descr.BufferDesc.RefreshRate.Numerator = 0;
	swap_chain_descr.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_descr.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swap_chain_descr.SampleDesc.Count = 1;
	swap_chain_descr.SampleDesc.Quality = 0;
	//| DXGI_USAGE_SHADER_INPUT for shader
	swap_chain_descr.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swap_chain_descr.BufferCount = 1;
	swap_chain_descr.OutputWindow = this->hwnd;
	swap_chain_descr.Windowed = true;
	swap_chain_descr.BufferDesc.Width = WIDTH;
	swap_chain_descr.BufferDesc.Height = HEIGHT;

	D3D_FEATURE_LEVEL feature_level;
	UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined (DEBUG) || defined (_DEBUG)
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		flags,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swap_chain_descr,
		&this->swap_chain_ptr,
		&this->device_ptr,
		&feature_level,
		&this->device_context_ptr);

	assert(S_OK == hr && this->swap_chain_ptr && this->device_ptr && this->device_context_ptr);

	ID3D11Texture2D* framebuffer;
	hr = this->swap_chain_ptr->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&framebuffer);
	assert(SUCCEEDED(hr));

	hr = this->device_ptr->CreateRenderTargetView(framebuffer, 0, &this->render_target_view_ptr);
	assert(SUCCEEDED(hr));

	framebuffer->Release();
}

void Window::_createDepthBuffer()
{
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = WIDTH;
	descDepth.Height = HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	HRESULT hr = device_ptr->CreateTexture2D(&descDepth, NULL, &pDepthStencilTex);
	if (FAILED(hr))
		assert(SUCCEEDED(hr));
}

void Window::_createDepthState()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = 0;
	dsvDesc.Texture2D.MipSlice = 0;


	// Create depth stencil state
	ID3D11DepthStencilState* pDSState;
	device_ptr->CreateDepthStencilState(&dsDesc, &pDSState);

	device_ptr->CreateDepthStencilView(pDepthStencilTex, &dsvDesc, &pDepthStencilView);

	device_context_ptr->OMSetDepthStencilState(pDSState, 1);
}

void Window::_createRSCullNone()
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = true;
	rasterizerDesc.DepthBias = false;
	rasterizerDesc.DepthBiasClamp = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.ScissorEnable = true;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.AntialiasedLineEnable = false;

	HRESULT hr = device_ptr->CreateRasterizerState(&rasterizerDesc, &RSCullNone);

	assert(SUCCEEDED(hr));
}

//Glow
void Window::_createNoGlowTexture()
{
	ID3D11Texture2D* noGlowTexture;

	D3D11_TEXTURE2D_DESC textureDesc;

	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = WIDTH;
	textureDesc.Height = HEIGHT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	HRESULT hr = this->device_ptr->CreateTexture2D(&textureDesc, NULL, &noGlowTexture);
	assert(SUCCEEDED(hr) && "Texture failed!");

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	hr = this->device_ptr->CreateRenderTargetView(noGlowTexture, &renderTargetViewDesc, &noGlowRTV);
	assert(SUCCEEDED(hr) && "RTV failed!");

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	//0 so that it uses textures desc
	hr = this->device_ptr->CreateShaderResourceView(noGlowTexture, &shaderResourceViewDesc, &this->noGlowSRV);
	assert(SUCCEEDED(hr) && "SRV failed!");
}

Window::Window(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow)
{
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window.

	hwnd = CreateWindowEx(
		0,                         // Optional window styles.
		CLASS_NAME,                // Window class
		L"3D Projekt",             // Window text
		WS_OVERLAPPEDWINDOW,       // Window style
		CW_USEDEFAULT,             // Pos X
		CW_USEDEFAULT,             // Pos Y
		WIDTH,			           // Width
		HEIGHT,			           // Height
		NULL,					   // Parent window    
		NULL,					   // Menu
		hInstance,				   // Instance handle
		NULL					   // Additional application data
	);
	assert(hwnd);

	ShowWindow(hwnd, nCmdShow);

	this->_createSwapChain();
	this->_createDepthBuffer();
	this->_createDepthState();
	this->_createRSCullNone();

	//Glow
	_createNoGlowTexture();
	this->glowShader = new Shader(L"glowVS.hlsl", L"glowPS.hlsl", device_ptr);

	D3D11_SAMPLER_DESC samplerStateDesc;
	samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.MinLOD = (-FLT_MAX);
	samplerStateDesc.MaxLOD = (FLT_MAX);
	samplerStateDesc.MipLODBias = 0.0f;
	samplerStateDesc.MaxAnisotropy = 1;
	samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerStateDesc.BorderColor[0] = 1.f;
	samplerStateDesc.BorderColor[1] = 1.f;
	samplerStateDesc.BorderColor[2] = 1.f;
	samplerStateDesc.BorderColor[3] = 1.f;

	HRESULT hr = device_ptr->CreateSamplerState(&samplerStateDesc, &this->samplerState);
	assert(SUCCEEDED(hr));


	//Using this to remove alpha blending
	D3D11_BLEND_DESC blendState;
	ZeroMemory(&blendState, sizeof(D3D11_BLEND_DESC));
	blendState.RenderTarget[0].BlendEnable = FALSE;
	blendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	device_ptr->CreateBlendState(&blendState, &this->blendState);

	//setting states
	device_context_ptr->PSSetSamplers(0, 1, &this->samplerState);

	float blend[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	UINT sampleMask = 0xffffffff;

	device_context_ptr->OMSetBlendState(this->blendState, blend, sampleMask);

	//Quad
	UINT vertex_stride = sizeof(VertexData);
	UINT vertex_offset = 0;
	UINT vertex_count = 6;

	std::vector<VertexData> vertices = 
	{
		{
			-1.f,  1.f, 0.f,
			0.f, 0.f,
			0.f, 0.f, 0.f
		},
		{
			1.f, -1.f, 0.f,
			1.f, 1.f,
			0.f, 0.f, 0.f
		},
		{
			-1.f, -1.f, 0.f,
			0.f, 1.f,
			0.f, 0.f, 0.f
		},
		{
			-1.f,  1.f, 0.f,
			0.f, 0.f,
			0.f, 0.f, 0.f
		},
		{
			1.f,  1.f, 0.f,
			1.f, 0.f,
			0.f, 0.f, 0.f
		},
		{
			1.f, -1.f, 0.f,
			1.f, 1.f,
			0.f, 0.f, 0.f
		}
	};
	

	D3D11_BUFFER_DESC vertex_buff_descr = {};
	vertex_buff_descr.ByteWidth = vertex_stride * vertex_count;
	vertex_buff_descr.Usage = D3D11_USAGE_DEFAULT;
	vertex_buff_descr.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sr_data = { 0 };
	sr_data.pSysMem = vertices.data();

	HRESULT hrCreateVBuffer = device_ptr->CreateBuffer(
		&vertex_buff_descr,
		&sr_data,
		&quadVertBuffer
	);

	assert(SUCCEEDED(hrCreateVBuffer));

	RECT winRect;
	GetClientRect(this->hwnd, &winRect);
	//Lock mouse to screen
	//ClipCursor(&winRect);
	

	this->viewport = new D3D11_VIEWPORT{
		0.0f,
		0.0f,
		(FLOAT)(WIDTH - winRect.left),
		(FLOAT)(HEIGHT - winRect.top),
		0.0f,
		1.0f
	};
}

Window::~Window()
{
	this->device_ptr->Release();
	this->device_context_ptr->Release();
	this->swap_chain_ptr->Release();
	this->render_target_view_ptr->Release();
	this->pDepthStencilView->Release();
	this->pDepthStencilTex->Release();

	// SkyBox
	this->sphereIndexBuffer->Release();
	this->sphereVertBuffer->Release();
	this->SKYMAP_VS->Release();
	this->SKYMAP_PS->Release();
	this->SKYMAP_VS_Buffer->Release();
	this->SKYMAP_PS_Buffer->Release();
	this->smrv->Release();
	this->DSLessEqual->Release();
	this->RSCullNone->Release();

	//Glow
	this->noGlowRTV->Release();
	this->noGlowSRV->Release();

	this->samplerState->Release();
	this->blendState->Release();
}

void Window::glowPass()
{	
	UINT vertex_stride = sizeof(VertexData);
	UINT vertex_offset = 0;
	UINT vertex_count = 6;

	//Null since we don't need depth to create glow, it's drawn on a quad on the screen. NDC-space/clip-space, 2D, -1 to 1
	device_context_ptr->OMSetRenderTargets(1, &render_target_view_ptr, NULL);
	device_context_ptr->PSSetShaderResources(0, 1, &noGlowSRV);

	//Set shaders (ps, vs)
	glowShader->useShader(device_context_ptr);

	//Render quad
	device_context_ptr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device_context_ptr->IASetInputLayout(glowShader->getInputLayoutPtr());
	device_context_ptr->IASetVertexBuffers(0, 1, &quadVertBuffer, &vertex_stride, &vertex_offset);
	
	device_context_ptr->Draw(vertex_count, 0);
	//Unbinding noGlow from shaderResource so it doesn't interfer with itself
	ID3D11ShaderResourceView* nullPtrSRV = nullptr;
	device_context_ptr->PSSetShaderResources(0, 1, &nullPtrSRV);
}

void Window::clearDepthStencil()
{
	device_context_ptr->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

bool Window::update()
{
	//device_context_ptr->ClearRenderTargetView(this->render_target_view_ptr, background_colour);
	//device_context_ptr->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	MSG msg = { };
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (msg.message == WM_QUIT)
	{
		quitWindow = true;
	}



	//NOTHING MORE BELOW THIS LINE
	//this->swap_chain_ptr->Present(1, 0);

	return quitWindow;
}
