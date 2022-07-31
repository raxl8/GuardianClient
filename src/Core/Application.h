#pragma once

#include "Core/Window.h"
#include "DirectX/DirectXDevice.h"

#include "ImGui/ImGuiLayer.h"

class Application
{
public:
	Application();

	int Run();

private:
	std::unique_ptr<Window> m_Window;
	std::unique_ptr<DirectXDevice> m_Device;
	std::unique_ptr<ImGuiLayer> m_ImGuiLayer;
};
