#include "pch.h"

#include "Window.h"

#include "Core/Decompression.h"

#include <glfw/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3native.h>

#include <dwmapi.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// https://github.com/qt/qtbase/blob/15ccc7e499ac8dd1f75dfa75346d15c4b4d06324/src/plugins/platforms/windows/qwindowswindow.cpp#L3139
enum : WORD
{
	DwmwaUseImmersiveDarkMode = 20,
	DwmwaUseImmersiveDarkModeBefore20h1 = 19
};

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
	m_Window = glfwCreateWindow(width, height, m_Title.c_str(), NULL, NULL);
	if (m_Window == NULL)
		FatalError();

	glfwSetWindowUserPointer(m_Window, this);
	glfwSetWindowIconifyCallback(m_Window, IconifyCallback);

#ifdef GDN_WINDOWS
	DWORD useLightTheme;
	DWORD valueSize = sizeof(useLightTheme);
	if (RegGetValueW(
		HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", L"AppsUseLightTheme",
		RRF_RT_REG_DWORD, nullptr, &useLightTheme, &valueSize) == ERROR_SUCCESS && useLightTheme == 0)
	{
		auto hwnd = glfwGetWin32Window(m_Window);
		BOOL darkMode = TRUE;
		bool _ =
			SUCCEEDED(DwmSetWindowAttribute(hwnd, DwmwaUseImmersiveDarkMode, &darkMode, sizeof(darkMode)))
			|| SUCCEEDED(DwmSetWindowAttribute(hwnd, DwmwaUseImmersiveDarkModeBefore20h1, &darkMode, sizeof(darkMode)));
	}
#endif
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

void Window::Poll()
{
	glfwPollEvents();
}

void Window::Update()
{
	glfwMakeContextCurrent(m_Window);
	glfwSwapBuffers(m_Window);
}

void Window::Show()
{
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

GLuint Window::LoadTexture(const void* data, const int size)
{
	auto decompressLength = stb_decompress_length((const uint8_t*)data);
	auto decompressedBuffer = new uint8_t[decompressLength];
	stb_decompress(decompressedBuffer, (const uint8_t*)data, size);

	int imageWidth = 0;
	int imageHeight = 0;
	stbi_uc* imageData = stbi_load_from_memory(decompressedBuffer, decompressLength, &imageWidth, &imageHeight, NULL, 4);

	delete[] decompressedBuffer;

	if (imageData == NULL)
		return NULL;

	GLuint imageTexture;
	glGenTextures(1, &imageTexture);
	glBindTexture(GL_TEXTURE_2D, imageTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);

	return imageTexture;
}
