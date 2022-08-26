#pragma once

#include "Core/Window.h"

class ImGuiLayer
{
public:
	ImGuiLayer(SharedPtr<Window> window);
	~ImGuiLayer();

	void Begin();
	void End();
};
