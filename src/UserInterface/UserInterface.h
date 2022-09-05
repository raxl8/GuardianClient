#pragma once

#include "UserInterface/Views/View.h"

#include <mutex>

struct ImFont;
class UserInterface
{
public:
	UserInterface();
	~UserInterface() = default;

	void PushTitleFont();
	void PushRegularFont();
	void RenderImGui();
	void SetView(SharedPtr<View> newView);
	void DisplayError(const std::string& title, const std::string& description);

	void SetDarkMode(bool enabled);
	bool IsDarkMode() { return m_DarkMode; }

private:
	void ApplyStyles();

private:
	bool m_DarkMode;
	ImFont* m_TitleFont, *m_RegularFont, *m_FooterFont, *m_IconFont;

	bool m_DisplayingError;
	std::mutex m_CurrentViewMutex;
	SharedPtr<View> m_CurrentView;
};
