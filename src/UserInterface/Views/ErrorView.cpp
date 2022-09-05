#include "pch.h"

#include "ErrorView.h"

#include "UserInterface/UserInterface.h"

#include <imgui.h>
#include <images/error-icon.h>

ErrorView::ErrorView(UserInterface* userInterface, const std::string& title, const std::string& description)
	: m_Title(title), m_Description(description), m_IconTexture(nullptr), View(userInterface)
{
}

void ErrorView::OnLoad(Renderer* renderer)
{
	m_IconTexture = renderer->LoadTexture(error_icon_compressed_data, error_icon_compressed_size);
}

void ErrorView::RenderImGui()
{
	ImGui::Dummy({ 0.f, WINDOW_HEIGHT / 7.f });

	constexpr auto logoSize = WINDOW_WIDTH / 10.f;
	ImGui::SetCursorPosX((WINDOW_WIDTH - logoSize) / 2.f);
	ImGui::Image(m_IconTexture, { logoSize, logoSize });

	ImGui::Dummy({ 0.f, WINDOW_HEIGHT / 17.f });

	m_UserInterface->PushTitleFont();

	ImGui::SetCursorPosX((WINDOW_WIDTH - ImGui::CalcTextSize(m_Title.c_str()).x) / 2.f);
	ImGui::TextUnformatted(m_Title.c_str());

	ImGui::PopFont();
	m_UserInterface->PushRegularFont();

	ImGui::SetCursorPosX((WINDOW_WIDTH - ImGui::CalcTextSize(m_Description.c_str()).x) / 2.f);
	ImGui::TextUnformatted(m_Description.c_str());

	ImGui::PopFont();
}
