#include "pch.h"

#include "Window.h"

#include "Core/Application.h"

#include <glfw/glfw3.h>

#ifdef GDN_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3native.h>
#endif

#include <dwmapi.h>

static void IconifyCallback(GLFWwindow* window, int iconified)
{
	auto windowPtr = (Window*)glfwGetWindowUserPointer(window);
	windowPtr->SetMinimized(iconified);
}

Window::Window(const std::string& title, int width, int height)
	: m_Title(title), m_Minimized(false)
{
	// We can init here as we know there's only going to be 1 window
	glfwInit();

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	m_Window = glfwCreateWindow(width, height, m_Title.c_str(), nullptr, nullptr);
	if (m_Window == nullptr)
		FatalError();

	glfwSetWindowUserPointer(m_Window, this);
	glfwSetWindowIconifyCallback(m_Window, IconifyCallback);

#ifdef GDN_WINDOWS
	auto hwnd = glfwGetWin32Window(m_Window);

	WTA_OPTIONS opts;
	opts.dwFlags = WTNCA_NODRAWICON | WTNCA_NODRAWCAPTION;
	opts.dwMask = WTNCA_NODRAWICON | WTNCA_NODRAWCAPTION;
	SetWindowThemeAttribute(hwnd, WTA_NONCLIENT, &opts, sizeof(opts));
#endif GDN_WINDOWS
}

Window::~Window()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void Window::SetRenderThread()
{
	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(1);
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(m_Window);
}

void Window::ChangeTitleBarTheme(bool dark)
{
#ifdef GDN_WINDOWS
	if (IsMinimumWindows10())
	{
		auto hwnd = glfwGetWin32Window(m_Window);
		// https://github.com/qt/qtbase/blob/15ccc7e499ac8dd1f75dfa75346d15c4b4d06324/src/plugins/platforms/windows/qwindowswindow.cpp#L3139
		const auto DwmaUseImmersiveDarkMode = IsMinimumWindows10Build(19041) ? 20 : 19;
		BOOL toggle = dark;
		DwmSetWindowAttribute(hwnd, DwmaUseImmersiveDarkMode, &toggle, sizeof(toggle));

		if (IsMinimumWindows11())
		{
			const auto DwmaCaptionColor = 35;
			COLORREF color = dark ? 0x181818 : 0xFFFFFF;
			DwmSetWindowAttribute(hwnd, DwmaCaptionColor, &color, sizeof(color));
		}
		else
		{
			// HACKHACK: Windows 10 only updates title bar on window creation
			ShowWindow(hwnd, SW_HIDE);
			ShowWindow(hwnd, SW_SHOW);
		}
	}
#endif
}

void Window::Poll()
{
	glfwWaitEvents();
}

void Window::Update()
{
	glfwSwapBuffers(m_Window);
}

void Window::Show()
{
	ChangeTitleBarTheme(Instance<Application>::Get()->IsDarkMode());

	glfwShowWindow(m_Window);
}

void Window::Hide()
{
	glfwHideWindow(m_Window);
}

void Window::SetMinimized(bool minimized)
{
	std::unique_lock lock(m_MinimizedMutex);
	m_Minimized = minimized;
}

bool Window::IsMinimized()
{
	std::shared_lock lock(m_MinimizedMutex);
	return m_Minimized;
}
