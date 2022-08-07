#pragma once

#include "UserInterface/Views/View.h"

#include <mutex>

class UserInterface
{
public:
	UserInterface();

	void RenderImGui();
	void SetView(UniquePtr<View>&& newView);
	void DisplayError(const std::string& title, const std::string& description);

private:
bool m_CanChangeView;
	std::mutex m_CurrentViewMutex;
	UniquePtr<View> m_CurrentView;
};
