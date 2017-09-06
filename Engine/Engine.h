#pragma once

#include <Windows.h>
#include <Camera.h>
#include <Game.h>

class Engine
{
public:
	Engine(HINSTANCE hInstance, int width, int height);
	virtual ~Engine();

	int run();

private:
//	Logic::Game game;
	HWND window;
	int mWidth;
	int mHeight;
	HINSTANCE hInstance;

	ID3D11Device* mDevice;
	ID3D11DeviceContext* mContext;
	ID3D11Debug* mDebugDevice;
	IDXGISwapChain* mSwapChain;
	ID3D11RenderTargetView* mBackBufferRTV;

	bool initializeGame();
	void initializeWindow();
	HRESULT createSwapChain();
	long long timer();
};