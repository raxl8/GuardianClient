#include "pch.h"

#include "Window.h"

static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	if (msg == WM_NCCREATE)
	{
		auto createStruct = (CREATESTRUCT*)lParam;
		SetWindowLongPtrA(hWnd, 0, (LONG_PTR)createStruct->lpCreateParams);
	}

	auto window = (Window*)GetWindowLongPtrA(hWnd, 0);
	// WM_NCCREATE isn't the first message in some case
	// so we still need to check for valid pointer
	if (window)
	{
		if (!window->OnProcedure(hWnd, msg, wParam, lParam))
			return true;
	}

	if (msg == WM_DESTROY)
		PostQuitMessage(0);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::Window(const std::wstring& title)
	: m_Title(title), m_Handle(nullptr)
{
	ZeroMemory(&m_Class, sizeof(m_Class));
	m_Class.cbSize = sizeof(WNDCLASSEX);
	m_Class.style = CS_CLASSDC;
	m_Class.lpfnWndProc = &WndProc;
	m_Class.cbClsExtra = 0L;
	m_Class.cbWndExtra = sizeof(this);
	m_Class.hInstance = GetModuleHandle(NULL);
	m_Class.hIcon = NULL;
	m_Class.hCursor = NULL;
	m_Class.hbrBackground = NULL;
	m_Class.lpszMenuName = NULL;
	m_Class.lpszClassName = m_Title.c_str();
	m_Class.hIconSm = NULL;
	
	::RegisterClassEx(&m_Class);

	m_Handle = CreateWindowEx(0L, m_Class.lpszClassName, m_Title.c_str(), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, m_Class.hInstance, this);
}

Window::~Window()
{
	DestroyWindow(m_Handle);
	UnregisterClass(m_Class.lpszClassName, m_Class.hInstance);
}

void Window::SetVisibility(bool visible /*= true*/)
{
	ShowWindow(m_Handle, visible ? SW_SHOW : SW_HIDE);
	UpdateWindow(m_Handle);
}

bool Window::Poll()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
			return false;
	}

	return true;
}
