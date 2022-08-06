#pragma once

#include "Core/Window.h"
#include "ImGui/ImGuiLayer.h"
#include "Scanner/Scanner.h"
#include "UserInterface/UserInterface.h"

class Application
{
public:
	Application();

	int Run();

	inline const UniquePtr<Scanner>& GetScanner() { return m_Scanner; }

private:
	UniquePtr<Scanner> m_Scanner;
	UniquePtr<Window> m_Window;
	UniquePtr<ImGuiLayer> m_ImGuiLayer;
	UniquePtr<UserInterface> m_UserInterface;
};

DEFINE_INSTANCE_TYPE(Application);
