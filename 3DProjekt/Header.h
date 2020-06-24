#pragma once
#pragma comment(lib,"user32")
#pragma comment(lib,"gdi32")
#pragma comment(lib,"d3d11")
#pragma comment(lib,"d3dcompiler.lib")

#ifndef UNICODE
#define UNICODE
#endif 
#define D3D_COMPILE_STANDARD_FILE_INCLUDE ((ID3DInclude*)(UINT_PTR)1)
#define WIN32_LEAN_AND_MEAN

#include <WICTextureLoader.h>
#include <stdio.h>
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <assert.h>
#include <string>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <DirectXMath.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <vector>

const float HEIGHT = 720;
const float WIDTH = 1280;
struct MVP
{
	DirectX::XMMATRIX model;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX perspective;
	DirectX::XMMATRIX modelViewPerspective;
};
const float background_colour[4] = {
	0.f, 0.f, 0.f, 0.f};

struct VertexData {
	float pX, pY, pZ;
	float u, v;
	float nX, nY, nZ;
};
struct VertexDataNormalMap
{
	float pX, pY, pZ;
	float u, v;
	float nX, nY, nZ;
	float tX, tY, tZ;
	float bX, bY, bZ;
};
struct outputData {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;
};
struct Point_Light_Struct
{
	DirectX::XMFLOAT4 pos;
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
	DirectX::XMFLOAT4 attenuation;
};
struct Material
{
	DirectX::XMFLOAT4 m_ambient;
	DirectX::XMFLOAT4 m_diffuse;
	DirectX::XMFLOAT4 m_specular;
	DirectX::XMFLOAT4 m_otherInfo;
};
struct TextureOffset
{
	float texOffset;
	DirectX::XMFLOAT3 pad;
};

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_MOUSEMOVE:
		
		DirectX::Mouse::ProcessMessage(uMsg, wParam, lParam);
		break;

	case WM_MBUTTONDOWN:
		DirectX::Mouse::ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_KEYDOWN:
	{
		DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
		break;
	}
	case WM_KEYUP:
		DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);



		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 4));

		EndPaint(hwnd, &ps);
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



