#include "pch.h"

#include "Application.h"

#include "Core/Window.h"


Application::Application()
{
	m_Window = MakeUnique<Window>("Guardian");
	m_ImGuiLayer = MakeUnique<ImGuiLayer>(m_Window);
}

int Application::Run()
{
	while (!m_Window->ShouldClose())
	{
		m_Window->Poll();

		m_ImGuiLayer->OnNewFrame();
		m_ImGuiLayer->OnImGuiRender();

		m_ImGuiLayer->OnRender();

		m_Window->Update();
	}

	return EXIT_SUCCESS;
}
