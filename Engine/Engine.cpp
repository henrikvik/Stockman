#include "Engine.h"

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
		//DirectX::Keyboard::ProcessMessage(msg, wparam, lparam);
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
		//DirectX::Mouse::ProcessMessage(msg, wparam, lparam);
		break;

	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
		break;
	}

	return 0;
}

Engine::Engine(HINSTANCE hInstance, int width, int height)
{
	this->height = height;
	this->width = width;
	this->hInstance = hInstance;
	this->initializeWindow();
}

Engine::~Engine()
{
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

	this->window = CreateWindow(
		"Basic test",
		"Stort spel",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		this->width,
		this->height,
		0,
		0,
		this->hInstance,
		0);

	if (this->window == 0)
	{
		MessageBox(this->window, "window creation failed", "Error", MB_OK);
	}



	ShowWindow(this->window, SW_SHOWDEFAULT);
	UpdateWindow(this->window);

}

int Engine::run()
{
	MSG msg = { 0 };

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			DispatchMessage(&msg);
		}
		else
		{
		//	system("pause");
		}
	}
	return 0;
}
