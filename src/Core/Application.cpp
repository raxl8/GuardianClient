#include "pch.h"

#include "Application.h"

Application::Application()
	: m_DarkMode(true)
{
	m_Websocket = MakeUnique<Websocket>(WEBSOCKET_ENDPOINT);
	m_Renderer = MakeUnique<Renderer>();
	m_Window = MakeUnique<Window>(PRODUCT_NAME, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_ImGuiLayer = MakeUnique<ImGuiLayer>(m_Window.get());
	m_Scanner = MakeUnique<Scanner>();
	m_UserInterface = MakeUnique<UserInterface>();

#ifdef GDN_WINDOWS
	DWORD useLightTheme;
	DWORD valueSize = sizeof(useLightTheme);
	m_DarkMode = RegGetValueW(
		HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", L"AppsUseLightTheme",
		RRF_RT_REG_DWORD, nullptr, &useLightTheme, &valueSize) == ERROR_SUCCESS && useLightTheme == 0;
#endif
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

			m_Renderer->OnNewFrame();

			m_ImGuiLayer->Begin();

			m_UserInterface->RenderImGui();

			m_ImGuiLayer->End();

			m_Window->Update();
		}
	});

	while (!m_Window->ShouldClose())
	{
		m_Window->Poll();
	}

	m_RenderingThread.join();

	return EXIT_SUCCESS;
}

void Application::SetDarkMode(bool enabled)
{
	m_DarkMode = enabled;

	m_Window->ChangeTitleBarTheme(enabled);
	m_UserInterface->SetDarkMode(enabled);
}
