#include "pch.h"

#include "Application.h"

#include "Core/Window.h"


Application::Application()
{
	m_Window = MakeUnique<Window>(L"Guardian");
	m_Device = MakeUnique<DirectXDevice>(m_Window);
	m_ImGuiLayer = MakeUnique<ImGuiLayer>(m_Window, m_Device);
}

int Application::Run()
{
	m_Window->SetVisibility(true);

	while (true)
	{
		if (!m_Window->Poll())
			break;

		m_ImGuiLayer->OnNewFrame();
		m_ImGuiLayer->OnImGuiRender();

		m_Device->SetRenderTargets();

		m_ImGuiLayer->OnRender();

		m_Device->Present();
	}

	m_Window->SetVisibility(false);

	return EXIT_SUCCESS;
}
