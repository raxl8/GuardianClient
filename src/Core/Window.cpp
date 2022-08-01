#include "pch.h"

#include "Window.h"

#include <GLFW/glfw3.h>

Window::Window(const std::string& title, int width, int height)
	: m_Title(title)
{
	// We can init here as we know there's only going to be 1 window
	glfwInit();

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_Window = glfwCreateWindow(width, height, m_Title.c_str(), NULL, NULL);
	if (m_Window == NULL)
		FatalError();

	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(1);
}

Window::~Window()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
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
