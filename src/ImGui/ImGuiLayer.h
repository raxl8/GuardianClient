#pragma once

class Window;
class ImGuiLayer
{
public:
	ImGuiLayer(Window* window);
	~ImGuiLayer();

	void Begin();
	void End();
};
