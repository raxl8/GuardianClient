#include "pch.h"

#include "Application.h"

Application::Application()
{
	m_Websocket = MakeUnique<Websocket>(WEBSOCKET_ENDPOINT);
	m_Websocket->Connect();

	m_Scanner = MakeUnique<Scanner>();
	m_Window = MakeUnique<Window>(PRODUCT_NAME, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_ImGuiLayer = MakeUnique<ImGuiLayer>(m_Window);
	m_UserInterface = MakeUnique<UserInterface>();
}

int Application::Run()
{
	while (!m_Window->ShouldClose())
	{
		m_Window->Poll();

		m_ImGuiLayer->Begin();

		m_UserInterface->RenderImGui();

		m_ImGuiLayer->End();

		m_Window->Update();
	}

	return EXIT_SUCCESS;
}
