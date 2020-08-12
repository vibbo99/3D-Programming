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
#include "intersectionChecker.h"
#include "Skybox.h"

#include <fcntl.h>
#include <io.h>
#include <iostream>

void RedirectIOToConsole()
{
	AllocConsole();
	HANDLE stdHandle;
	int hConsole;
	FILE* fp;
	stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	hConsole = _open_osfhandle((long)stdHandle, _O_TEXT);
	fp = _fdopen(hConsole, "w");

	freopen_s(&fp, "CONOUT$", "w", stdout);

	printf("Hello console on\n");
	std::cout << "Windows 10" << std::endl;
}


ID3D11Buffer* light_constant_buffer_ptr = NULL;
ID3D11Buffer* lightNr_constant_buffer_ptr = NULL;
ID3D11Buffer* camera_pos_buffer = NULL;


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	Window window(hInstance, pCmdLine, nCmdShow);
	Camera camera;
	XMFLOAT3 mousePos;

	XMFLOAT3 cubePosition;

	//Mouse
	Mouse::ButtonStateTracker mouseCheck;
	std::unique_ptr<DirectX::Mouse> mouse = std::make_unique<DirectX::Mouse>();
	mouse->SetWindow(window.getHwind());

	//Plane intersection variables
	int nrOfPlanes = 0;
	XMVECTOR planeNormals[50];
	XMVECTOR planePositions[50];
	float planeDistance = 0.f;

	float lowestValue;

	Plane planeObjects[50];

	int planeDelay = 200;

	//Cursor test

	int setCursorPosOnce = 2;
	

	//Sphere intersection variables
	int nrOfSpheres = 0;
	XMVECTOR sphereCenter[50];
	float sphereRadius[50];

	Sphere sphereObjects[50];

	//If you want to display console information

	RedirectIOToConsole();
	
	camera.setPosition(0, 0, 0);
	camera.setupMouse(window.getHwind());

	mousePicking* mousePicker;

	mousePicker = new mousePicking(&camera);

	intersectionChecker intersectionCheck;


	/*Point_Light pl(XMFLOAT3(0.0f, 2.0f, 0.0f), window.getDevicePtr(), window.getDeviceContextPtr(), &camera);
	pl.setScale(0.5, 0.5, 0.5);*/
	Cube cube(4.0f, 0.0f, 4.0f, LPCWSTR(L"groundTexture.jpg"), &camera,  window.getDevicePtr(), window.getDeviceContextPtr());

	Cube cubeWhite(4.0f, 0.0f, 4.0f, LPCWSTR(L"whiteTexture.jpg"), &camera, window.getDevicePtr(), window.getDeviceContextPtr());

	Skybox skyBox(L"skymap.DDS", &camera, window.getDevicePtr(), window.getDeviceContextPtr());
	

	//SkyBoxCube skyBoxCube(0.0f, 0.0f , 0.0f, LPCWSTR(L"Skybox_test.jpg"), &camera, window.getDevicePtr(), window.getDeviceContextPtr());
	planeObjects[0] = Plane(20.0f, -2.0f, 2.0f, L"texture2.png", L"texture2_normal.png", &camera, window.getDevicePtr(), window.getDeviceContextPtr());

	planeNormals[nrOfPlanes] = planeObjects[0].getNormalInfo();
	planePositions[nrOfPlanes] = planeObjects[0].getPositionXMVECTOR();
	nrOfPlanes++;

	//planeObjects[0].rotateX(XMConvertToRadians(-90));
	
	Sphere sphereObject(0.0f, 0.0f, 0.0f, LPCWSTR(L"groundTexture.jpg"), &camera, window.getDevicePtr(), window.getDeviceContextPtr());

	sphereObjects[0] = Sphere(0.0f, 0.0f, 0.0f, LPCWSTR(L"groundTexture.jpg"), &camera, window.getDevicePtr(), window.getDeviceContextPtr());

	sphereCenter[nrOfSpheres] = sphereObjects[0].getCENTERXMVECTOR();
	sphereRadius[nrOfSpheres] = 0.005f;

	nrOfSpheres++;


	//Viktors inverterade sphere
	//SkyBoxSphere SkySphere(0.0f, 0.0f, 0.0f, LPCWSTR(L"skymap.dds"), &camera, window.getDevicePtr(), window.getDeviceContextPtr());

	Point_Light* plArr[5];
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
	plArr[4] = new Point_Light(XMFLOAT3(2.f, 0.f, 0.f), window.getDevicePtr(), window.getDeviceContextPtr(), &camera);
	plArr[4]->setScale(0.5, 0.5, 0.5);
	plArr[4]->setDiffuse(XMFLOAT4(0.f, 1.f, 0.f, 1.f));
	
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
		//set rasterizeState, lasts until change

		//Mouse things
		auto theMouse = mouse->GetState();
		
		camera.processKeyboard(0.5);


		if (theMouse.positionMode == Mouse::MODE_RELATIVE)
		{
			camera.processMouse(theMouse.x, theMouse.y);				
		}

		if (theMouse.rightButton)
		{
			std::cout << "MOUSE CLICK" << std::endl;
			//Last frame where is mousePos, how much movement since last frame
			mouse->SetMode(Mouse::MODE_RELATIVE);
		}

		else
		{
			//For picking, example.
			mouse->SetMode(Mouse::MODE_ABSOLUTE);
		}
		
		

		//Test mouse-picking

		
		//SetPhysicalCursorPos((int)WIDTH / 2, (int)HEIGHT / 2);
		

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


		
		//inserted mouse x & y
		mousePicker->update(&camera, theMouse.x, theMouse.y);

		mousePos = mousePicker->getRayDirFloat3();


		///////INTERSECTION/////////
		

		mouseCheck.Update(theMouse);


		if (mouseCheck.leftButton == Mouse::ButtonStateTracker::PRESSED)
		{
			if (intersectionCheck.cubeCheck(0.5f, cube.getPositionXMFLOAT3(), mousePicker->getRayPos(), mousePicker->getRayDir()))
			{
				//std::cout << "Hit" << std::endl;
				cubePosition = cube.getPositionXMFLOAT3();
				cube.setPosition(cubePosition.x , cubePosition.y , cubePosition.z + 0.1);
			}
			else
			{
				//std::cout << "Not hit" << std::endl;
			}
		}
		
		//for (int i = 0; i < nrOfSpheres; i++)
		//{
		//	//lowestValue 
		//	lowestValue = intersectionCheck.collisionCheck(i, mousePicker->getRayPos(), planeDistance, mousePicker->getRayDir(), planeNormals, "Sphere", sphereCenter, sphereRadius);

		//	//std::cout << std::to_string(mousePos.x) << ", " << std::to_string(mousePos.y) << ", " << std::to_string(mousePos.z) << std::endl;
		//	

		//	if (lowestValue >= 0)
		//	{
		//		std::cout << "Ray hit sphere" << std::endl;
		//	
		//	
		//	}
		//	else
		//	{
		//		std::cout << "Not hit" << std::endl;
		//	}
		//}
		////////////////////////////////////



		
		

		HRESULT hr = window.getDevicePtr()->CreateBuffer(&camDesc, &camData, &camera_pos_buffer);

		if (FAILED(hr))
			assert(FAILED(hr));
		
		window.getDeviceContextPtr()->ClearRenderTargetView(*window.getRenderTargetViewPtr(), background_colour);
		window.getDeviceContextPtr()->ClearRenderTargetView(*window.getNoGlowRTV_ptr(), background_colour);
		window.clearDepthStencil();
		window.getDeviceContextPtr()->RSSetViewports(1, window.getViewportPtr());
		window.getDeviceContextPtr()->OMSetRenderTargets(1, window.getNoGlowRTV_ptr(), window.getDepthStencilViewPtr());
		//window.getDeviceContextPtr()->RSSetState(window.getRSCullNone());
		

		
		//Draw cube object
		cube.draw(light_constant_buffer_ptr, lightNr_constant_buffer_ptr, camera_pos_buffer);


		


		//skyBoxCube.draw(light_constant_buffer_ptr, lightNr_constant_buffer_ptr, camera_pos_buffer);

		//Draw object plane
		//planeObject.setPosition(0, 0, 0);
		//planeObject.rotateZ(-0.5);

		//planeObject.draw(light_constant_buffer_ptr, lightNr_constant_buffer_ptr, camera_pos_buffer);

		for (int i = 0; i < nrOfPlanes; i++)
		{
			planeObjects[i].draw(light_constant_buffer_ptr, lightNr_constant_buffer_ptr, camera_pos_buffer);
		}
		
		//Draw sphere
		sphereObject.draw(light_constant_buffer_ptr, lightNr_constant_buffer_ptr, camera_pos_buffer);

		//Viktors inverterade sphere
		//SkySphere.draw(light_constant_buffer_ptr, lightNr_constant_buffer_ptr, camera_pos_buffer);


		//Draw point light
		for (int i = 0; i < 4; i++) {
			plArr[i]->draw();
		}

		skyBox.draw(nullptr, nullptr, camera_pos_buffer);

		window.glowPass();

		window.getSwapChainPtr()->Present(1, 0);
		quitWindow = window.update();

		camera_pos_buffer->Release();
	}

	return 0;
}
