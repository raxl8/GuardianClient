#pragma once

#include "Core/Window.h"

#include "ImGui/ImGuiLayer.h"

#include "UserInterface/UserInterface.h"

class Application
{
public:
	Application();

	int Run();

private:
	UniquePtr<Window> m_Window;
	UniquePtr<ImGuiLayer> m_ImGuiLayer;
	UniquePtr<UserInterface> m_UserInterface;
};
