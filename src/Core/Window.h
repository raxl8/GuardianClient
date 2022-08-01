#pragma once

#include "Core/Callback.h"

struct GLFWwindow;
class Window
{
public:
	Window(const std::string& title);
	~Window();

	bool ShouldClose();
	void Poll();
	void Update();

	inline GLFWwindow* GetNativeWindow() { return m_Window; }

private:
	std::string m_Title;
	GLFWwindow* m_Window;
};
