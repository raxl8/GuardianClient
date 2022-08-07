#include "pch.h"

#include "UserInterface.h"

#include "Views/HomeView.h"
#include "Views/ErrorView.h"

#include <imgui.h>
#include <fonts/Roboto/Roboto-Regular.h>

UserInterface::UserInterface()
{
	m_CurrentView = MakeUnique<HomeView>(this);

	auto& io = ImGui::GetIO();
	io.Fonts->AddFontFromMemoryCompressedTTF(Roboto_Regular_compressed_data, Roboto_Regular_compressed_size, 18.f);
}

void UserInterface::RenderImGui()
{
	ImGui::SetNextWindowPos({ 0.f, 0.f });
	ImGui::SetNextWindowSize({ WINDOW_WIDTH, WINDOW_HEIGHT });

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
	ImGui::Begin("##window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
	ImGui::PopStyleVar();

	m_CurrentView->RenderImGui();

	ImGui::SetCursorPosY(WINDOW_HEIGHT - ImGui::CalcTextSize(BUILD_DESCRIPTION).y - ImGui::GetStyle().WindowPadding.y);
	ImGui::TextUnformatted(BUILD_DESCRIPTION);

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
