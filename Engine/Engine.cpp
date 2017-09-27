#include "Engine.h"
#include <Graphics\include\Structs.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		DirectX::Keyboard::ProcessMessage(msg, wparam, lparam);
		
		break;

	case WM_ACTIVATEAPP:
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
	DirectX::Mouse::ProcessMessage(msg, wparam, lparam);
		break;

	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
		break;
	}

	return 0;
}

Engine::Engine(HINSTANCE hInstance, int width, int height)
{
	srand(time(NULL));				// Set random seed
	this->mHeight = height;
	this->mWidth = width;
	this->hInstance = hInstance;
	this->initializeWindow();
	this->game.init();

	this->isFullscreen = false;
	this->mKeyboard = std::make_unique<DirectX::Keyboard>();
	this->mMouse = std::make_unique<DirectX::Mouse>();
	this->mMouse->SetWindow(window);

}

Engine::~Engine()
{

	delete this->renderer;

	this->mDevice->Release();
	this->mContext->Release();
	this->mSwapChain->Release();
	this->mBackBufferRTV->Release();

	//Enable this to get additional information about live objects
    //this->mDebugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
    this->mDebugDevice->Release();
}

void Engine::initializeWindow()
{
	WNDCLASS wc = { 0 };

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = this->hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = "Basic test";


   

	if (!RegisterClass(&wc))
	{
		MessageBox(this->window, "registerClass failed", "Error", MB_OK);
	}

	RECT rc = { 0, 0, mWidth, mHeight };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	this->window = CreateWindow(
		"Basic test",
		"Stort spel",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		0,
		0,
		this->hInstance,
		0);

	if (this->window == 0)
	{
		MessageBox(this->window, "window creation failed", "Error", MB_OK);
	}

	SetWindowPos(GetConsoleWindow(), 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	SetWindowPos(this->window, 0, 100, 150, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	ShowWindow(this->window, SW_SHOWDEFAULT);
	UpdateWindow(this->window);

}

HRESULT Engine::createSwapChain()
{
	
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	desc.BufferCount = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = this->window;
	desc.SampleDesc.Count = 1;
	desc.Windowed = true;
	desc.BufferDesc.Height = this->mHeight;
	desc.BufferDesc.Width = this->mWidth;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
  
  

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&desc,
		&mSwapChain,
		&this->mDevice,
		NULL,
		&this->mContext);

	if (SUCCEEDED(hr))
	{
		ID3D11Texture2D* backBuffer = nullptr;
		hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		if (FAILED(hr))
		{
			MessageBox(0, "swap chain getBuffer failed", "error", MB_OK);
			return hr;
		}

		hr = mDevice->CreateRenderTargetView(backBuffer, NULL, &mBackBufferRTV);
		if (FAILED(hr))
		{
			MessageBox(0, "RTV creation failed", "error", MB_OK);
			return hr;
		}
		backBuffer->Release();

		//Creates a debug device to check for memory leaks etc
		HRESULT hr = this->mDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast <void **>(&mDebugDevice)); 
		if (FAILED(hr))
		{
			MessageBox(0, "debug device creation failed", "error", MB_OK);
		}

	}
	else
	{
		MessageBox(0, "remove debug flag", "error", MB_OK);
	}

	return hr;
}

long long Engine::timer()
{
	static LARGE_INTEGER frequency;
	static BOOL highResCounter = QueryPerformanceFrequency(&frequency);
	if (highResCounter)
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		//returns time in ms
		return(1000LL * now.QuadPart) / frequency.QuadPart;
	}
	else
	{
		return GetTickCount();
	}
}

int Engine::run()
{
	MSG msg = { 0 };
	this->createSwapChain();
	Graphics::Camera cam(mDevice, mWidth, mHeight, 250);
    cam.update({ 0,0,-15 }, { 0,0,1 }, mContext);

	this->renderer = new Graphics::Renderer(mDevice, mContext, mBackBufferRTV, &cam);

	long long start = this->timer();
	long long prev = start;
	long long currentTime = 0;
	long long deltaTime = 0; 
    long long totalTime = 0;


	while (WM_QUIT != msg.message)
	{
		currentTime = this->timer();
		deltaTime = currentTime - prev;
		prev = currentTime;
        totalTime += deltaTime;

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);



			DispatchMessage(&msg);
		}
		
		//To enable/disable fullscreen
		DirectX::Keyboard::State ks = this->mKeyboard->GetState();
		if (ks.F11)
		{
			mSwapChain->SetFullscreenState(!isFullscreen, NULL);
			this->isFullscreen = !isFullscreen;
		}

		if (ks.Escape)
			PostQuitMessage(0);

		game.update(float(deltaTime));
		game.render(*renderer);

        cam.update(game.getPlayerPosition(), game.getPlayerForward(), mContext);

		//cam.update(DirectX::SimpleMath::Vector3(2, 2, -3), DirectX::SimpleMath::Vector3(-0.5f, -0.5f, 0.5f), mContext);
        //cam.update({ 0,0,-8 -5*sin(totalTime * 0.001f) }, { 0,0,1 }, mContext);

        //////////////TEMP/////////////////
        Graphics::RenderInfo staticCube = {
            true, //bool render;
            Graphics::ModelID::CUBE, //ModelID meshId;
            0, //int materialId;
            DirectX::SimpleMath::Matrix()// DirectX::SimpleMath::Matrix translation;
        };

        Graphics::RenderInfo staticSphere = {
            true, //bool render;
            Graphics::ModelID::SPHERE, //ModelID meshId;
            0, //int materialId;
            DirectX::SimpleMath::Matrix() // DirectX::SimpleMath::Matrix translation;
        };

        //staticCube.translation *= DirectX::SimpleMath::Matrix::CreateRotationY(deltaTime * 0.001f);
        //staticCube.translation *= DirectX::SimpleMath::Matrix::CreateRotationX(deltaTime * 0.0005f);
        staticCube.translation = DirectX::SimpleMath::Matrix::CreateTranslation({ 0, 3 + cosf(totalTime * 0.001f),0 });

        staticSphere.translation = DirectX::SimpleMath::Matrix::CreateTranslation({ 0, 3 + sinf(totalTime * 0.001f),0 });

		renderer->updateLight(deltaTime, &cam);

        renderer->queueRender(&staticCube);
        renderer->queueRender(&staticSphere);
        ///////////////////////////////////


		renderer->render(&cam);
		mSwapChain->Present(0, 0);
		
		
	}

	return 0;
}
