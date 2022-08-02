#pragma once

#include "UserInterface/Views/View.h"

class UserInterface
{
public:
	UserInterface();

	void RenderImGui();
	void SetView(UniquePtr<View>&& newView);

private:
	UniquePtr<View> m_CurrentView;
};
