#pragma once

#include <Windows.h>
#include <Camera.h>
#include <Renderer.h>
#include <Game.h>
#include "Keyboard.h"

class Engine
{
public:
	Engine(HINSTANCE hInstance, int width, int height, LPWSTR *cmdLine, int args);
	virtual ~Engine();

	int run();
private:
	Logic::Game game;
	HWND window;
	int mWidth;
	int mHeight;
	HINSTANCE hInstance;
	
	Graphics::Renderer* renderer;

	ID3D11Device* mDevice;
	ID3D11DeviceContext* mContext;
	ID3D11Debug* mDebugDevice;
	IDXGISwapChain* mSwapChain;
	ID3D11RenderTargetView* mBackBufferRTV;
	std::unique_ptr<DirectX::Keyboard> mKeyboard;
	std::unique_ptr<DirectX::Mouse> mMouse;
    std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> mTracker;
	BOOL isFullscreen;

	void initializeWindow();
	HRESULT createSwapChain();
	long long timer(); //A long long timer ago
};