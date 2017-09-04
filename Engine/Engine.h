#pragma once

#include <Windows.h>
#include <Camera.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")


class Engine
{
public:
	Engine(HINSTANCE hInstance, int width, int height);
	virtual ~Engine();

	int run();

private:
	HWND window;
	int mWidth;
	int mHeight;
	HINSTANCE hInstance;

	ID3D11Device* mDevice;
	ID3D11DeviceContext* mContext;
	ID3D11Debug* mDebugDevice;
	IDXGISwapChain* mSwapChain;
	ID3D11RenderTargetView* mBackBufferRTV;

	void initializeWindow();
	HRESULT createSwapChain();
};