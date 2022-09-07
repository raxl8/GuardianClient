#pragma once

#include <shared_mutex>

struct GLFWwindow;
class Window
{
public:
	Window(const std::string& title, int width, int height);
	~Window();

	void SetRenderThread();
	bool ShouldClose();
	void ChangeTitleBarTheme(bool dark);
	void Poll();
	void Update();
	void Show();
	void Hide();
	void SetMinimized(bool minimized);
	bool IsMinimized();
	void DisableCloseButton(bool disable);

	auto GetNativeWindow() const { return m_Window; }

private:
	std::string m_Title;
	std::shared_mutex m_MinimizedMutex;
	bool m_Minimized;
	GLFWwindow* m_Window;
};
