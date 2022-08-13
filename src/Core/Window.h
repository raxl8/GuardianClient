#pragma once

#include "Core/Callback.h"

struct GLFWwindow;
class Window
{
public:
	Window(const std::string& title, int width, int height);
	~Window();

	bool ShouldClose();
	void Poll();
	void Update();
	void Show();
	void Hide();

	GLFWwindow* GetNativeWindow() const { return m_Window; }

private:
	std::string m_Title;
	GLFWwindow* m_Window;
};
