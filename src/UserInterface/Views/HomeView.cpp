#include "pch.h"

#include "HomeView.h"

#include "UserInterface/UserInterface.h"
#include "UserInterface/Views/ScanningView.h"

#include <imgui.h>

HomeView::HomeView(UserInterface* userInterface)
	: View(userInterface)
{
	ZeroMemory(m_PINBuffer, sizeof(m_PINBuffer));
}

void HomeView::RenderImGui()
{
	if (ImGui::InputText("PIN", m_PINBuffer, sizeof(m_PINBuffer)))
	{
		if (strlen(m_PINBuffer) == 6)
		{
			m_UserInterface->SetView(MakeUnique<ScanningView>(m_UserInterface));
		}
	}
}
