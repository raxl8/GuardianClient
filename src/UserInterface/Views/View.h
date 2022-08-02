#pragma once

class UserInterface;
class View
{
public:
	View(UserInterface* userInterface);
	virtual ~View() = default;

	virtual void RenderImGui() = 0;

protected:
	UserInterface* m_UserInterface;
};
