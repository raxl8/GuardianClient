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
	if (RegOpenKeyExW(
		HKEY_CURRENT_USER, L"Software\\Guardian",
		0, KEY_ALL_ACCESS, &m_RegistryKey) != ERROR_SUCCESS)
	{
		RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Guardian", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &m_RegistryKey, NULL);
	}

	DWORD useDarkTheme = 0, systemUseLightTheme = 0;
	DWORD dwordSize = sizeof(DWORD);
	if (RegGetValueW(
		m_RegistryKey, nullptr, L"DarkTheme",
		RRF_RT_REG_DWORD, nullptr, &useDarkTheme, &dwordSize) == ERROR_SUCCESS)
	{
		m_DarkMode = (useDarkTheme == 1);
	}
	else if (RegGetValueW(
		HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", L"AppsUseLightTheme",
		RRF_RT_REG_DWORD, nullptr, &systemUseLightTheme, &dwordSize) == ERROR_SUCCESS)
	{
		m_DarkMode = (systemUseLightTheme == 0);
	}
#endif

	SetDarkMode(m_DarkMode);
}

Application::~Application()
{
#ifdef GDN_WINDOWS
	RegCloseKey(m_RegistryKey);
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

#ifdef GDN_WINDOWS
	DWORD keyValue = m_DarkMode;
	RegSetValueExW(
		m_RegistryKey, L"DarkTheme", 0,
		REG_DWORD, (BYTE*)&keyValue, sizeof(DWORD));
#endif

	m_Window->ChangeTitleBarTheme(enabled);
	m_UserInterface->SetDarkMode(enabled);
}
