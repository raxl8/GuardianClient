#include "pch.h"

#include "ScanningView.h"

#include "Core/Application.h"
#include "UserInterface/UserInterface.h"

#include <imgui.h>

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
	ImGui::Dummy({ 0.f, WINDOW_HEIGHT / 2.5f });

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

	constexpr const auto easeOutQuad = [](float t)
	{
		return 1.f - powf(2.f, -8.f * t);
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
		m_Progress = (m_TargetProgress - m_PrevProgress) * easeOutQuad(m_ProgressAnimTime);
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
	ImGui::ProgressBar(m_Progress, progressBarSize);

	ImGui::PopFont();
}
