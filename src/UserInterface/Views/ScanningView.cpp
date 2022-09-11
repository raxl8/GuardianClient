#include "pch.h"

#include "ScanningView.h"

#include "Core/Application.h"
#include "ImGui/ImGuiCustom.h"
#include "UserInterface/UserInterface.h"

#include <fmt/printf.h>

ScanningView::ScanningView(UserInterface* userInterface)
	: m_ProgressAnimating(false), m_ProgressAnimTime(0.f),
	m_PrevProgress(0.f), m_Progress(0.f), m_TargetProgress(0.f), View(userInterface)
{
	m_Scanner = Instance<Application>::Get()->GetScanner();
	m_StageCount = m_Scanner->GetStageCount();
	m_Scanner->Start();
}

void ScanningView::RenderImGui()
{
	ImGui::Dummy({ 0.f, WINDOW_HEIGHT / 7.f });

	const auto cursorY = ImGui::GetCursorPosY();
	for (int i = 0; i < 3; i++)
	{
		const auto radiusIncrease = 10.f; // Increase by 10 each spinner
		const auto radius = 25.f + i * radiusIncrease; // Start at 25, increase by 10 each spinner
		ImGui::SetCursorPos(ImVec2((WINDOW_WIDTH - radius * 2.f) / 2.f, cursorY - i * radiusIncrease));
		ImGuardian::Spinner(fmt::sprintf("##spinner_%d", i).c_str(), radius, 3.f, 5.f + i, ImGui::GetColorU32(ImGuiCol_Button));
	}

	ImGui::Dummy({ 0.f, WINDOW_HEIGHT / 25.f });

	m_UserInterface->PushTitleFont();

	ImGui::SetCursorPosX((WINDOW_WIDTH - ImGui::CalcTextSize("Scanning").x) / 2.f);
	ImGui::TextUnformatted("Scanning");

	ImGui::PopFont();
	m_UserInterface->PushRegularFont();

	ImGui::SetCursorPosX((WINDOW_WIDTH - ImGui::CalcTextSize("Wait patiently while we examine your machine.").x) / 2.f);
	ImGui::TextUnformatted("Wait patiently while we examine your machine.");

	ImGui::Dummy({ 0.f, WINDOW_HEIGHT / 18.f });

	const ImVec2 progressBarSize = {
		WINDOW_WIDTH - (WINDOW_WIDTH / 10.f),
		WINDOW_HEIGHT / 14.f,
	};

	constexpr const auto easeOutExpo = [](float t)
	{
		float t2 = (--t) * t;
		return 1.f + t * t2 * t2;
	};

	const auto currentStage = m_Scanner->GetCurrentStage();
	m_TargetProgress = static_cast<float>(currentStage) / static_cast<float>(m_StageCount);

	if (!m_ProgressAnimating && m_PrevProgress != m_TargetProgress)
	{
		m_Progress = m_PrevProgress;
		m_ProgressAnimating = true;
		m_ProgressAnimTime = 0.f;
	}

	if (m_ProgressAnimating && m_ProgressAnimTime < 1.f)
	{
		m_ProgressAnimTime += ImGui::GetIO().DeltaTime;
		// We need to clamp here as if the window is minimized,
		// when restored DeltaTime could be > 1 and therefore
		// make easeOutQuad return crazy values
		m_ProgressAnimTime = std::clamp(m_ProgressAnimTime, 0.f, 1.f);
		m_Progress = m_PrevProgress + (m_TargetProgress - m_PrevProgress) * easeOutExpo(m_ProgressAnimTime);
	}
	else if (m_ProgressAnimTime >= 1.)
	{
		m_ProgressAnimating = false;
		m_ProgressAnimTime = 0.f;
		m_PrevProgress = m_TargetProgress;
		
		if (currentStage == m_StageCount)
		{
			if (m_Scanner->GetStatus() == ScannerStatus::FatalError)
				m_UserInterface->DisplayError("Scanner Error", "A fatal error occured during the scan resulting in it failing.");
		}
	}

	ImGui::SetCursorPosX((WINDOW_WIDTH - progressBarSize.x) / 2.f);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));
	ImGui::ProgressBar(m_Progress, progressBarSize);
	ImGui::PopStyleColor();

	ImGui::PopFont();
}
