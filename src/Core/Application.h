#pragma once

#include "Core/Window.h"
#include "ImGui/ImGuiLayer.h"
#include "Networking/Websocket.h"
#include "Scanner/Scanner.h"
#include "UserInterface/UserInterface.h"

class Application
{
public:
	Application();
	~Application() = default;

	int Run();

	inline const UniquePtr<UserInterface>& GetUserInterface() { return m_UserInterface; }
	inline const UniquePtr<Scanner>& GetScanner() { return m_Scanner; }

private:
	UniquePtr<Websocket> m_Websocket;
	UniquePtr<Window> m_Window;
	UniquePtr<ImGuiLayer> m_ImGuiLayer;
	UniquePtr<UserInterface> m_UserInterface;
	UniquePtr<Scanner> m_Scanner;
};

DEFINE_INSTANCE_TYPE(Application);
