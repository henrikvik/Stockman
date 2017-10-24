#include "Engine.h"
#include <Graphics\include\Structs.h>

#include "Profiler.h"

#include <Windows.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <iostream>
#pragma comment (lib, "d3d11.lib")
#include "Typing.h"

extern LRESULT ImGui_ImplDX11_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//if (
	ImGui_ImplDX11_WndProcHandler(hwnd, msg, wparam, lparam);
		//)
		//return true;
	Typing* theChar = Typing::getInstance(); //might need to be deleted
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
	case WM_CHAR:
		theChar->setSymbol(char((wchar_t)wparam));
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

	this->isFullscreen = false;
	this->mKeyboard = std::make_unique<DirectX::Keyboard>();
	this->mMouse = std::make_unique<DirectX::Mouse>();
	this->mMouse->SetWindow(window);

	this->game.init();

}

Engine::~Engine()
{
	ImGui_ImplDX11_Shutdown();
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
	desc.BufferDesc.RefreshRate.Denominator = 0;
	desc.BufferDesc.RefreshRate.Numerator = 0;


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

Profiler *g_Profiler;

int Engine::run()
{
	MSG msg = { 0 };
	this->createSwapChain();
	Graphics::Camera cam(mDevice, mWidth, mHeight, 250, DirectX::XMConvertToRadians(90));
    cam.update({ 0,0,-15 }, { 0,0,1 }, mContext);

	ImGui_ImplDX11_Init(window, mDevice, mContext);

	this->renderer = new Graphics::Renderer(mDevice, mContext, mBackBufferRTV, &cam);

	long long start = this->timer();
	long long prev = start;
	long long currentTime = 0;
	long long deltaTime = 0; 
    long long totalTime = 0;
	bool showProfiler = false;

	bool running = true;

	g_Profiler = new Profiler(mDevice, mContext);
	g_Profiler->registerThread("Main Thread");

	while (running)
	{
		currentTime = this->timer();
		deltaTime = currentTime - prev;
		prev = currentTime;
        totalTime += deltaTime;

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);


			DispatchMessage(&msg);

            if (WM_QUIT == msg.message)
            {
                running = false;
                if (!isFullscreen)
                {
                    mSwapChain->SetFullscreenState(false, NULL);
                }
                
            }
				
                
		}

		//To enable/disable fullscreen
		DirectX::Keyboard::State ks = this->mKeyboard->GetState();
		if (ks.F11)
		{
			mSwapChain->SetFullscreenState(!isFullscreen, NULL);
			this->isFullscreen = !isFullscreen;
		}

		if (ks.F1)
		{
			g_Profiler->capture();
			showProfiler = true;
		}

        static bool F2wasPressed = false;
        bool F2keyDown = !F2wasPressed && ks.F2;
        F2wasPressed = ks.F2;

		if (F2keyDown)
		{
            showProfiler = !showProfiler;

		}

		if (ks.Escape)
			PostQuitMessage(0);

		g_Profiler->start();

		PROFILE_BEGINC("ImGui_ImplDX11_NewFrame", EventColor::PinkLight);
		ImGui_ImplDX11_NewFrame();
		PROFILE_END();

		PROFILE_BEGINC("Game::update()", EventColor::Magenta);
		game.update(float(deltaTime));
		PROFILE_END();

		PROFILE_BEGINC("Game::render()", EventColor::Red);
		game.render(*renderer);
		PROFILE_END();

		static DirectX::SimpleMath::Vector3 oldPos = { 0, 0, 0 };
		if (DirectX::Keyboard::Get().GetState().IsKeyDown(DirectX::Keyboard::LeftControl)) cam.update(oldPos, game.getPlayerForward(), mContext);
		else
		{
			oldPos = game.getPlayerPosition();
			cam.update(game.getPlayerPosition(), game.getPlayerForward(), mContext);
		}

		//cam.update(DirectX::SimpleMath::Vector3(2, 2, -3), DirectX::SimpleMath::Vector3(-0.5f, -0.5f, 0.5f), mContext);
        //cam.update({ 0,0,-8 -5*sin(totalTime * 0.001f) }, { 0,0,1 }, mContext);

        //////////////TEMP/////////////////
        //Graphics::RenderInfo staticSphere = {
        //    true, //bool render;
        //    Graphics::ModelID::WATER, //ModelID meshId;
        //    0, //int materialId;
        //    DirectX::SimpleMath::Matrix() // DirectX::SimpleMath::Matrix translation;
        //};

		Graphics::FoliageRenderInfo grass = {
			true, //bool render;
			Graphics::ModelID::GRASS, //ModelID meshId;
			DirectX::SimpleMath::Matrix() // DirectX::SimpleMath::Matrix translation;
		};

		Graphics::Light light;
		light.color = DirectX::SimpleMath::Vector3(1, 1, 0);
		light.positionWS = DirectX::SimpleMath::Vector3(0, 2, 0);
		light.intensity = 1;
		light.range = 5;



		//Graphics::FoliageRenderInfo bush = {
		//	true, //bool render;
		//	Graphics::ModelID::BUSH, //ModelID meshId;
		//	DirectX::SimpleMath::Matrix() // DirectX::SimpleMath::Matrix translation;
		//};

		//grass.translation = DirectX::SimpleMath::Matrix::CreateScale(5, 5, 5);
		//bush.translation = DirectX::SimpleMath::Matrix::CreateScale(10, 10, 10);//* DirectX::SimpleMath::Matrix::CreateTranslation({ 0, 30, 0 });
		//staticSphere.translation = DirectX::SimpleMath::Matrix::CreateScale(5, 5, 5) * DirectX::SimpleMath::Matrix::CreateTranslation({ 0, 20, 0 });
		
        Graphics::TextString text{
            L"The hills!",
            DirectX::SimpleMath::Vector2(50, 50),
            DirectX::SimpleMath::Color(DirectX::Colors::Black),
            Graphics::Font::SMALL
        };

        
        ///////////////////////////////////

        if (game.getState() == Logic::gameStateGame)
        {
			renderer->queueFoliageRender(&grass);
			//renderer->queueFoliageRender(&bush);


			//renderer->queueRender(&staticSphere);
         // renderer->queueText(&text);
			renderer->queueLight(light);

			renderer->updateLight(deltaTime, &cam);
			renderer->updateShake(deltaTime);

			PROFILE_BEGINC("Renderer::render()", EventColor::PinkDark);
            renderer->render(&cam);
			PROFILE_END();
        }

		if (game.getState() == Logic::gameStateGameUpgrade)
		{
			renderer->updateLight(deltaTime, &cam);

			PROFILE_BEGINC("Renderer::render()", EventColor::PinkDark);
			renderer->render(&cam);
			PROFILE_END();

			game.menuRender(renderer);
		}

		 
		g_Profiler->poll();

		if (showProfiler) {
			
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImVec2(WIN_WIDTH, 250));
			g_Profiler->render();
		}

		mContext->OMSetRenderTargets(1, &mBackBufferRTV, nullptr);
		PROFILE_BEGINC("ImGui::Render()", EventColor::PinkLight);
		ImGui::Render();
		PROFILE_END();

		PROFILE_BEGINC("IDXGISwapChain::Present()", EventColor::Cyan);
		mSwapChain->Present(0, 0);
		PROFILE_END();
		g_Profiler->frame();
		
	}

	g_Profiler->end();
	delete g_Profiler;

	return 0;
}
