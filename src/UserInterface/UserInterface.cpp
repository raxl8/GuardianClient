#include "pch.h"

#include "UserInterface.h"

#include <imgui.h>
#include <fonts/Roboto/Roboto-Regular.h>

UserInterface::UserInterface()
{
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

	ImGui::SetCursorPosY(WINDOW_HEIGHT - ImGui::CalcTextSize(VERSION_DESCRIPTION).y - ImGui::GetStyle().WindowPadding.y);
	ImGui::TextUnformatted(VERSION_DESCRIPTION);

	ImGui::End();
}
