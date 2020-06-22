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
#include "mousePicking.h"

ID3D11Buffer* light_constant_buffer_ptr = NULL;
ID3D11Buffer* lightNr_constant_buffer_ptr = NULL;
ID3D11Buffer* camera_pos_buffer = NULL;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	Window window(hInstance, pCmdLine, nCmdShow);
	Camera camera;


	camera.setPosition(0, 0, 0);
	camera.setupMouse(window.getHwind());

	mousePicking* mousePicker;

	mousePicker = new mousePicking(&camera);

	mousePicker->getWorld();


	/*Point_Light pl(XMFLOAT3(0.0f, 2.0f, 0.0f), window.getDevicePtr(), window.getDeviceContextPtr(), &camera);
	pl.setScale(0.5, 0.5, 0.5);*/
	Cube cube(4.0f, 0.0f, 4.0f, LPCWSTR(L"groundTexture.jpg"), &camera,  window.getDevicePtr(), window.getDeviceContextPtr());

	//SkyBoxCube skyBoxCube(0.0f, 0.0f , 0.0f, LPCWSTR(L"Skybox_test.jpg"), &camera, window.getDevicePtr(), window.getDeviceContextPtr());
	
	Plane planeObject(0.0f, -2.0f, 0.0f, L"texture2.png", L"texture2_normal.png", &camera, window.getDevicePtr(), window.getDeviceContextPtr());
	
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

	ID3D11Resource* texture;
	ID3D11ShaderResourceView* srw;
	ObjLoader loader;
	loader.LoadTexture(L"brickwall_normal", window.getDevicePtr(), &texture, &srw);
	

	//planeObject.scaleSize(10, 1, 10);
	while (!quitWindow)
	{

		camera.processKeyboard(0.5);
		//camera.processMouse(0.5);

		//Test mouse-picking




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
		
		window.getDeviceContextPtr()->ClearRenderTargetView(*window.getRenderTargetViewPtr(), background_colour);
		window.clearDepthStencil();
		window.getDeviceContextPtr()->RSSetViewports(1, window.getViewportPtr());
		window.getDeviceContextPtr()->OMSetRenderTargets(1, window.getRenderTargetViewPtr(), window.getDepthStencilViewPtr());
		

		
		//Draw cube object
		cube.draw(light_constant_buffer_ptr, lightNr_constant_buffer_ptr, camera_pos_buffer);

		//skyBoxCube.draw(light_constant_buffer_ptr, lightNr_constant_buffer_ptr, camera_pos_buffer);

		//Draw object plane
		//planeObject.setPosition(0, 0, 0);
		//planeObject.rotateZ(-0.5);
		planeObject.draw(light_constant_buffer_ptr, lightNr_constant_buffer_ptr, camera_pos_buffer);

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
