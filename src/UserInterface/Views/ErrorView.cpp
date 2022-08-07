#include "pch.h"

#include "ErrorView.h"

#include <imgui.h>

ErrorView::ErrorView(UserInterface* userInterface, const std::string& title, const std::string& description)
	: m_Title(title), m_Description(description), View(userInterface)
{
}

void ErrorView::RenderImGui()
{
	ImGui::TextUnformatted(m_Title.c_str());
	ImGui::TextUnformatted(m_Description.c_str());
}
