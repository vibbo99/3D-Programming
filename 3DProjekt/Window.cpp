#include "Window.h"



void Window::_createSwapChain()
{
	RECT winRect;
	GetClientRect(this->hwnd, &winRect);

	DXGI_SWAP_CHAIN_DESC swap_chain_descr = { 0 };

	swap_chain_descr.BufferDesc.RefreshRate.Numerator = 0;
	swap_chain_descr.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_descr.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swap_chain_descr.SampleDesc.Count = 1;
	swap_chain_descr.SampleDesc.Quality = 0;
	swap_chain_descr.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_descr.BufferCount = 1;
	swap_chain_descr.OutputWindow = this->hwnd;
	swap_chain_descr.Windowed = true;
	swap_chain_descr.BufferDesc.Width = winRect.right;
	swap_chain_descr.BufferDesc.Height = winRect.bottom;

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
	RECT winRect;
	GetClientRect(this->hwnd, &winRect);

	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = winRect.right;
	descDepth.Height = winRect.bottom;
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

	RECT winRect;
	GetClientRect(this->hwnd, &winRect);

	this->viewport = new D3D11_VIEWPORT{
		0.0f,
		0.0f,
		(FLOAT)(winRect.right - winRect.left),
		(FLOAT)(winRect.bottom - winRect.top),
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
