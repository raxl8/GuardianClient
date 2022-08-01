#pragma once

#include "Core/Window.h"

class ImGuiLayer
{
public:
	ImGuiLayer(const UniquePtr<Window>& window);
	~ImGuiLayer();

	void Begin();
	void End();
};
