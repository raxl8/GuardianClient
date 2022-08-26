#include "pch.h"

#include "Application.h"

Application::Application()
{
	m_Websocket = MakeUnique<Websocket>(WEBSOCKET_ENDPOINT);
	m_Window = MakeShared<Window>(PRODUCT_NAME, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_ImGuiLayer = MakeUnique<ImGuiLayer>(m_Window);
	m_UserInterface = MakeUnique<UserInterface>(m_Window);
	m_Scanner = MakeUnique<Scanner>();
}

int Application::Run()
{
	m_Websocket->Start();

	m_Window->Show();

	m_RenderingThread = std::thread([this]
	{
		m_Window->SetRenderThread();

		while (!m_Window->ShouldClose())
		{
			if (m_Window->IsMinimized())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				continue;
			}

			m_ImGuiLayer->Begin();

			m_UserInterface->RenderImGui();

			m_ImGuiLayer->End();

			m_Window->Update();
		}
	});

	while (!m_Window->ShouldClose())
	{
		m_Window->Poll();

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	m_RenderingThread.join();

	return EXIT_SUCCESS;
}
