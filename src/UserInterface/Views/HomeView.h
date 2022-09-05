#pragma once

#include "UserInterface/Views/View.h"

class HomeView : public View
{
public:
	HomeView(UserInterface* userInterface);
	~HomeView() = default;

	virtual void OnLoad(Renderer* renderer) override;
	virtual void RenderImGui() override;

private:
	void* m_LogoTexture;
	char m_PINBuffer[PIN_SIZE + 1];
};
