#pragma once

#include "Core/Renderer.h"

class UserInterface;
class View
{
public:
	View(UserInterface* userInterface);
	virtual ~View() = default;

	virtual void OnLoad(Renderer* renderer) {}
	virtual void RenderImGui() = 0;

protected:
	UserInterface* m_UserInterface;
};
