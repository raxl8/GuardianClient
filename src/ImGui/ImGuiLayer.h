#pragma once

#include "Core/Layer.h"
#include "Core/Window.h"

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer(const UniquePtr<Window>& window);
	~ImGuiLayer();

	virtual void OnNewFrame() override;
	virtual void OnImGuiRender() override;
	virtual void OnRender() override;
};
