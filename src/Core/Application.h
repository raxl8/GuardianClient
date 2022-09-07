#pragma once

#include "Core/Renderer.h"
#include "Core/Window.h"
#include "ImGui/ImGuiLayer.h"
#include "Networking/Websocket.h"
#include "Scanner/Scanner.h"
#include "UserInterface/UserInterface.h"

class Application : public Instance<Application>
{
public:
	Application();
	~Application();

	int Run();

	bool IsDarkMode() { return m_DarkMode; }
	void SetDarkMode(bool enabled);
	auto GetRenderer() const { return m_Renderer.get(); }
	auto GetUserInterface() const { return m_UserInterface.get(); }
	auto GetScanner() { return m_Scanner.get(); }

private:
	bool m_DarkMode;

	UniquePtr<Websocket> m_Websocket;
	UniquePtr<Renderer> m_Renderer;
	UniquePtr<Window> m_Window;
	UniquePtr<ImGuiLayer> m_ImGuiLayer;
	UniquePtr<UserInterface> m_UserInterface;
	UniquePtr<Scanner> m_Scanner;

	std::thread m_RenderingThread;

#ifdef GDN_WINDOWS
	HKEY m_RegistryKey;
#endif
};

DEFINE_INSTANCE_TYPE(Application);
