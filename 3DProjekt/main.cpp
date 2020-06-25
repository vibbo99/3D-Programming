#include "Header.h"
#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "objLoader.h"
#include "Point_Light.h"
#include "Cube.h"
#include "Plane.h"
#include "Sphere.h"
#include "SkyBoxCube.h"
#include "SkyBoxSphere.h"
#include "Timer.h"

ID3D11Buffer* light_constant_buffer_ptr = NULL;
ID3D11Buffer* lightNr_constant_buffer_ptr = NULL;
ID3D11Buffer* camera_pos_buffer = NULL;
ID3D11Buffer* uvOffset_buffer_ptr = NULL;
ID3D11SamplerState* m_sampleState = NULL;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	Window window(hInstance, pCmdLine, nCmdShow);
	Camera camera;
	camera.setPosition(0, 0, 0);
	camera.setupMouse(window.getHwind());

	Timer m_timer;
	float m_deltaTime = 0;
	TextureOffset texOffset;
	texOffset.texOffset = 0;
	

	/*Point_Light pl(XMFLOAT3(0.0f, 2.0f, 0.0f), window.getDevicePtr(), window.getDeviceContextPtr(), &camera);
	pl.setScale(0.5, 0.5, 0.5);*/
	Cube cube(4.0f, 0.0f, 4.0f, LPCWSTR(L"groundTexture.jpg"), &camera,  window.getDevicePtr(), window.getDeviceContextPtr());

	//SkyBoxCube skyBoxCube(0.0f, 0.0f , 0.0f, LPCWSTR(L"Skybox_test.jpg"), &camera, window.getDevicePtr(), window.getDeviceContextPtr());
	
	Plane planeObject(0.0f, -2.0f, 0.0f, L"texture2.png", L"texture2_normal.png", &camera, window.getDevicePtr(), window.getDeviceContextPtr());

	Plane waterPlane(3.0f, -6.0f, 3.0f, L"blue_texture.png", L"water_normal.png", &camera, window.getDevicePtr(), window.getDeviceContextPtr(), true);
	
	Sphere sphereObject(-4.0f, 0.0f, 4.0f, LPCWSTR(L"groundTexture.jpg"), &camera, window.getDevicePtr(), window.getDeviceContextPtr());

	SkyBoxSphere SkySphere(0.0f, 0.0f, 0.0f, LPCWSTR(L"skymap.dds"), &camera, window.getDevicePtr(), window.getDeviceContextPtr());

	Point_Light* plArr[4];
	plArr[0] = new Point_Light(XMFLOAT3(4.0f, 1.5f, 2.0f), window.getDevicePtr(), window.getDeviceContextPtr(), &camera);
	plArr[0]->setScale(0.5, 0.5, 0.5);
	plArr[1] = new Point_Light(XMFLOAT3(0.0f, 25.0f, 25.0f), window.getDevicePtr(), window.getDeviceContextPtr(), &camera);
	plArr[1]->setScale(0.5, 0.5, 0.5);
	plArr[1]->setAttenuation(1.0f, 0.007f, 0.0002f);
	plArr[2] = new Point_Light(XMFLOAT3(0.0f, 25.0f, -25.0f), window.getDevicePtr(), window.getDeviceContextPtr(), &camera);
	plArr[2]->setScale(0.5, 0.5, 0.5);
	plArr[2]->setAttenuation(1.0f, 0.007f, 0.0002f);
	plArr[3] = new Point_Light(XMFLOAT3(0.0f, 40.0f, 0.0f), window.getDevicePtr(), window.getDeviceContextPtr(), &camera);
	plArr[3]->setScale(0.5, 0.5, 0.5);
	plArr[3]->setAttenuation(1.0, 0.0014, 0.000007);
	
	bool quitWindow = false;
	if ( window.getHwind() == NULL)
	{
		return 0;
	}

	ShowWindow(window.getHwind(), nCmdShow);
	// ---
	//Setprojection
	camera.setProjectionValues(70.0f, static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 1000.0f);

	// Fill lightNr_constant_buffer_ptr
	XMFLOAT4 nrOfLights = XMFLOAT4(4.0f, 0.0f, 0.0f, 1.0f);
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(XMFLOAT4);
	desc.Usage = D3D11_USAGE_DYNAMIC; //D3D11_USAGE_DEFAULT
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &nrOfLights;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	HRESULT hr = window.getDevicePtr()->CreateBuffer(&desc, &initData, &lightNr_constant_buffer_ptr);

	if (FAILED(hr))
		assert(FAILED(hr));

	// Fill light_constant_buffer_ptr
	Point_Light_Struct lStruct[4];
	for (int i = 0; i < 4; i++)
	{
		lStruct[i] = plArr[i]->getAsStruct();
	}
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(Point_Light_Struct[10]);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC; //D3D11_USAGE_DEFAULT
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &lStruct;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	hr = window.getDevicePtr()->CreateBuffer(&cbDesc, &InitData, &light_constant_buffer_ptr);

	if (FAILED(hr))
		assert(FAILED(hr));

	// Fill uvOffset_buffer_ptr
	D3D11_BUFFER_DESC uvOffsetDesc;
	uvOffsetDesc.ByteWidth = sizeof(TextureOffset);
	uvOffsetDesc.Usage = D3D11_USAGE_DYNAMIC; //D3D11_USAGE_DEFAULT
	uvOffsetDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	uvOffsetDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	uvOffsetDesc.MiscFlags = 0;
	uvOffsetDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA uvOffsetData;
	uvOffsetData.pSysMem = &texOffset;
	uvOffsetData.SysMemPitch = 0;
	uvOffsetData.SysMemSlicePitch = 0;
	
	hr = window.getDevicePtr()->CreateBuffer(&uvOffsetDesc, &uvOffsetData, &uvOffset_buffer_ptr);
	if (FAILED(hr))
		assert(FAILED(hr));

	// water normal samplerstate
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

	// Create the texture sampler state.
	hr = window.getDevicePtr()->CreateSamplerState(&samplerDesc, &m_sampleState);


	ID3D11Resource* texture;
	ID3D11ShaderResourceView* srw;
	ObjLoader loader;
	loader.LoadTexture(L"brickwall_normal", window.getDevicePtr(), &texture, &srw);
	

	//planeObject.scaleSize(10, 1, 10);
	while (!quitWindow)
	{

		camera.processKeyboard(0.5);
		camera.processMouse(0.5);

		m_deltaTime = (float)m_timer.timeElapsed();
		m_timer.restart();

		// Fill camera_pos_buffer
		D3D11_BUFFER_DESC camDesc;
		camDesc.ByteWidth = sizeof(XMFLOAT4);
		camDesc.Usage = D3D11_USAGE_DYNAMIC; //D3D11_USAGE_DEFAULT
		camDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		camDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		camDesc.MiscFlags = 0;
		camDesc.StructureByteStride = 0;

		XMFLOAT3 camPos = camera.getPositionFloat3();
		D3D11_SUBRESOURCE_DATA camData;
		camData.pSysMem = &XMFLOAT4(camPos.x, camPos.y, camPos.z, 1.0f);
		camData.SysMemPitch = 0;
		camData.SysMemSlicePitch = 0;

		HRESULT hr = window.getDevicePtr()->CreateBuffer(&camDesc, &camData, &camera_pos_buffer);

		if (FAILED(hr))
			assert(FAILED(hr));
		
		// TEXOffset
		//float normalizedTime = ((m_deltaTime - 0) / (999999 - 0));
		texOffset.texOffset += (0.1f * m_deltaTime);
		if (texOffset.texOffset > 1.0f)
			texOffset.texOffset = -1.0f;

		// update constant buffer "texture"
		D3D11_MAPPED_SUBRESOURCE mapSubresource;
		hr = window.getDeviceContextPtr()->Map(uvOffset_buffer_ptr, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapSubresource);
		assert(SUCCEEDED(hr) && "Error, failed to map constant buffer!");
		
		CopyMemory(mapSubresource.pData, &texOffset, sizeof(TextureOffset));

		window.getDeviceContextPtr()->Unmap(uvOffset_buffer_ptr, 0);







		// Render window
		window.getDeviceContextPtr()->ClearRenderTargetView(*window.getRenderTargetViewPtr(), background_colour);
		window.clearDepthStencil();
		window.getDeviceContextPtr()->RSSetViewports(1, window.getViewportPtr());
		window.getDeviceContextPtr()->OMSetRenderTargets(1, window.getRenderTargetViewPtr(), window.getDepthStencilViewPtr());

		window.getDeviceContextPtr()->PSSetSamplers(0, 1, &m_sampleState);
		
		//Draw cube object
		cube.draw(light_constant_buffer_ptr, lightNr_constant_buffer_ptr, camera_pos_buffer);

		//skyBoxCube.draw(light_constant_buffer_ptr, lightNr_constant_buffer_ptr, camera_pos_buffer);

		//Draw object plane
		//planeObject.setPosition(0, 0, 0);
		//planeObject.rotateZ(-0.5);
		planeObject.draw(light_constant_buffer_ptr, lightNr_constant_buffer_ptr, camera_pos_buffer);

		waterPlane.draw(light_constant_buffer_ptr, lightNr_constant_buffer_ptr, camera_pos_buffer, uvOffset_buffer_ptr);


		//Draw sphere
		sphereObject.draw(light_constant_buffer_ptr, lightNr_constant_buffer_ptr, camera_pos_buffer);
		SkySphere.draw(light_constant_buffer_ptr, lightNr_constant_buffer_ptr, camera_pos_buffer);


		//Draw point light
		for (int i = 0; i < 4; i++) {
			plArr[i]->draw();
		}

		window.getSwapChainPtr()->Present(1, 0);
		quitWindow = window.update();

		camera_pos_buffer->Release();
	}

	return 0;
}
