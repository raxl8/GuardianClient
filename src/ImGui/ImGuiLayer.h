#pragma once

#include "Core/Layer.h"
#include "Core/Window.h"
#include "DirectX/DirectXDevice.h"

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer(const UniquePtr<Window>& window, const UniquePtr<DirectXDevice>& directX);
	~ImGuiLayer();

	virtual void OnNewFrame() override;
	virtual void OnImGuiRender() override;
	virtual void OnRender() override;
};
