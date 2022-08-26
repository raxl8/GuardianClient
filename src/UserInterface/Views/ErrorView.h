#pragma once

#include "UserInterface/Views/View.h"

class ErrorView : public View
{
public:
	ErrorView(UserInterface* userInterface, const std::string& title, const std::string& description);
	~ErrorView() = default;

	virtual void RenderImGui() override;

private:
	void* m_IconTexture;
	std::string m_Title, m_Description;
};
