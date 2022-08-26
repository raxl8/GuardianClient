#pragma once

#include "Core/Callback.h"

#include <shared_mutex>

struct GLFWwindow;
class Window
{
public:
	Window(const std::string& title, int width, int height);
	~Window();

	void SetRenderThread();
	bool ShouldClose();
	void Poll();
	void Update();
	void Show();
	void Hide();
	void SetMinimized(bool minimized);
	bool IsMinimized();

	GLFWwindow* GetNativeWindow() const { return m_Window; }

private:
	std::shared_mutex m_MinimizedMutex;
	bool m_Minimized;
	std::string m_Title;
	GLFWwindow* m_Window;
};
