#include "pch.h"

#include "HomeView.h"

#include "Core/Window.h"
#include "Core/Window.h"
#include "ImGui/ImGuiCustom.h"
#include "UserInterface/UserInterface.h"
#include "UserInterface/Views/ScanningView.h"

#include <images/Logo.h>

HomeView::HomeView(UserInterface* userInterface)
	: View(userInterface)
{
	ZeroMemory(m_PINBuffer, sizeof(m_PINBuffer));

	m_LogoTexture = (void*)(intptr_t)m_UserInterface->GetWindow()->LoadTexture(Logo_compressed_data, Logo_compressed_size);
}

static int DigitsTextFilter(ImGuiInputTextCallbackData* data)
{
	if ('0' > data->EventChar || data->EventChar > '9')
		return -1; // reject anything but digits

	return 0;
}

void HomeView::RenderImGui()
{
	ImGui::Dummy({ 0.f, WINDOW_HEIGHT / 6.f });

	constexpr auto boxWidth = WINDOW_WIDTH / 2.5f;
	constexpr auto boxHeight = WINDOW_HEIGHT / 8.f;

	constexpr auto logoWidth = boxWidth * 1.2f;
	constexpr auto logoHeight = boxHeight * 1.2f * 1.2f;
	ImGui::SetCursorPosX((WINDOW_WIDTH - logoWidth) / 2.f);
	ImGui::Image(m_LogoTexture, { logoWidth, logoHeight });

	m_UserInterface->PushRegularFont();

	ImGui::Dummy({ 0.f, boxHeight / 2.f });

	{
		ImGui::SetCursorPosX((WINDOW_WIDTH - boxWidth) / 2.f);
		const auto cursor = ImGui::GetCursorPos();
		ImGui::GetWindowDrawList()->AddShadowRect(cursor, cursor + ImVec2{ boxWidth, boxHeight }, ImGui::GetColorU32(ImGuiCol_WindowShadow), 15.f, ImVec2{3.f, 3.f});

		ImGui::SetNextItemWidth(boxWidth);
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_Button) + ImVec4{ 0.2f, 0.2f, 0.2f, 0.f });
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 15.f, boxHeight / 2.f - ImGui::CalcTextSize("").y / 2.f });
		ImGui::InputText("##pinInput", m_PINBuffer, std::size(m_PINBuffer), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CallbackCharFilter, DigitsTextFilter);
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();
	}

	ImGui::Dummy({ 0.f, boxHeight / 5.f });

	{
		ImGui::SetCursorPosX((WINDOW_WIDTH - boxWidth) / 2.f);
		const auto cursor = ImGui::GetCursorPos();
		ImGui::GetWindowDrawList()->AddShadowRect(cursor, cursor + ImVec2{ boxWidth, boxHeight }, ImGui::GetColorU32(ImGuiCol_WindowShadow), 15.f, ImVec2{ 3.f, 3.f });
		ImGuardian::Button("Start a Scan", ImVec2{ boxWidth, boxHeight });
	}

	ImGui::PopFont();
}
