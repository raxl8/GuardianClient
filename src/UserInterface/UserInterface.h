#pragma once

#include "UserInterface/Views/View.h"

#include <mutex>
#include <tbb/concurrent_queue.h>

class Window;
struct ImFont;
class UserInterface
{
public:
	UserInterface(SharedPtr<Window> window);

	void PushTitleFont();
	void PushRegularFont();
	void RenderImGui();
	void SetView(SharedPtr<View> newView);
	void DisplayError(const std::string& title, const std::string& description);

	SharedPtr<Window> GetWindow() { return m_Window; }

private:
	void ApplyStyles();

private:
	SharedPtr<Window> m_Window;
	ImFont* m_TitleFont, *m_RegularFont, *m_FooterFont;

	tbb::concurrent_queue<std::function<void()>> m_RendererTasks;
	bool m_CanChangeView;
	std::mutex m_CurrentViewMutex;
	SharedPtr<View> m_CurrentView;
};
