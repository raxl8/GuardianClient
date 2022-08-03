#pragma once

#include "UserInterface/Views/View.h"

class HomeView : public View
{
public:
	HomeView(UserInterface* userInterface);
	~HomeView() = default;

	virtual void RenderImGui() override;

private:
	char m_PINBuffer[PIN_SIZE + 1];
};