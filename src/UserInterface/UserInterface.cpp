#include "pch.h"

#include "UserInterface.h"

#include "Core/Window.h"
#include "Views/HomeView.h"
#include "Views/ErrorView.h"

#include <imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>
#include <fonts/Roboto/Roboto-Light.h>
#include <fonts/Roboto/Roboto-Medium.h>
#include <fonts/Roboto/Roboto-Regular.h>

UserInterface::UserInterface(SharedPtr<Window> window)
	: m_Window(window)
{
	ApplyStyles();

	auto& io = ImGui::GetIO();
	m_TitleFont = io.Fonts->AddFontFromMemoryCompressedTTF(Roboto_Light_compressed_data, Roboto_Light_compressed_size, 30.f);
	m_RegularFont = io.Fonts->AddFontFromMemoryCompressedTTF(Roboto_Medium_compressed_data, Roboto_Medium_compressed_size, 17.f);
	m_FooterFont = io.Fonts->AddFontFromMemoryCompressedTTF(Roboto_Regular_compressed_data, Roboto_Regular_compressed_size, 17.f);
}

void UserInterface::PushTitleFont()
{
	ImGui::PushFont(m_TitleFont);
}

void UserInterface::PushRegularFont()
{
	ImGui::PushFont(m_RegularFont);
}

void UserInterface::RenderImGui()
{
	if (!m_CurrentView)
		m_CurrentView = MakeUnique<HomeView>(this);

	ImGui::SetNextWindowPos({ 0.f, 0.f });
	ImGui::SetNextWindowSize({ WINDOW_WIDTH, WINDOW_HEIGHT });

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
	ImGui::Begin("##window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
	ImGui::PopStyleVar();

	m_CurrentView->RenderImGui();

	ImGui::PushFont(m_FooterFont);
	ImGui::SetCursorPosY(WINDOW_HEIGHT - ImGui::CalcTextSize(BUILD_DESCRIPTION).y - ImGui::GetStyle().WindowPadding.y);
	ImGui::TextUnformatted(BUILD_DESCRIPTION);
	ImGui::PopFont();

	ImGui::End();
}

void UserInterface::SetView(UniquePtr<View>&& newView)
{
	std::unique_lock lock(m_CurrentViewMutex);

	if (m_CanChangeView)
		m_CurrentView = std::move(newView);
}

void UserInterface::DisplayError(const std::string& title, const std::string& description)
{
	std::unique_lock lock(m_CurrentViewMutex);

	m_CanChangeView = false;
	m_CurrentView = MakeUnique<ErrorView>(this, title, description);
}

void UserInterface::ApplyStyles()
{
	ImGui::StyleColorsDark();

	auto& style = ImGui::GetStyle();
	style.FrameRounding = 3.f;

	const ImColor accent = ImColor(109, 35, 225);
	style.Colors[ImGuiCol_Button] = accent;
	style.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_Button] + ImVec4(0.1f, 0.1f, 0.1f, 0.f);
	style.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_Button] + ImVec4(0.05f, 0.05f, 0.05f, 0.f);
	style.Colors[ImGuiCol_TextSelectedBg] = style.Colors[ImGuiCol_Button] - ImVec4(0.1f, 0.1f, 0.1f, 0.5f);
	style.Colors[ImGuiCol_FrameBg] = ImColor(30, 30, 30);
	style.Colors[ImGuiCol_Border] = accent;
	style.Colors[ImGuiCol_WindowBg] = ImColor(24, 24, 24);
}
