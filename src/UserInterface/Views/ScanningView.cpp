#include "pch.h"

#include "ScanningView.h"

#include <imgui.h>

ScanningView::ScanningView(UserInterface* userInterface)
	: View(userInterface)
{
}

void ScanningView::RenderImGui()
{
	ImGui::TextUnformatted("Scanning...");
}
