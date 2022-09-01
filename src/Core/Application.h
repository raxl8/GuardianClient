#pragma once

#include "Core/Window.h"
#include "ImGui/ImGuiLayer.h"
#include "Networking/Websocket.h"
#include "Scanner/Scanner.h"
#include "UserInterface/UserInterface.h"

class Application : public Instance<Application>
{
public:
	Application();
	~Application() = default;

	int Run();

	const UniquePtr<UserInterface>& GetUserInterface() const { return m_UserInterface; }
	SharedPtr<Scanner> GetScanner() { return m_Scanner; }

private:
	UniquePtr<Websocket> m_Websocket;
	SharedPtr<Window> m_Window;
	UniquePtr<ImGuiLayer> m_ImGuiLayer;
	UniquePtr<UserInterface> m_UserInterface;
	SharedPtr<Scanner> m_Scanner;

	std::thread m_RenderingThread;
};

DEFINE_INSTANCE_TYPE(Application);
