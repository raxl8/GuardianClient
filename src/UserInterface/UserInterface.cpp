#include "pch.h"

#include "UserInterface.h"

#include <imgui.h>

void UserInterface::RenderImGui()
{
	ImGui::SetNextWindowPos({ 0.f, 0.f });
	ImGui::SetNextWindowSize({ WINDOW_WIDTH, WINDOW_HEIGHT });

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
	ImGui::Begin("##window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::PopStyleVar();

	ImGui::TextUnformatted("Hello world!");

	ImGui::End();
}
