#pragma once

class Layer
{
public:
	Layer() = default;
	virtual ~Layer() = default;

	virtual void OnNewFrame() {}
	virtual void OnImGuiRender() {}
	virtual void OnRender() {}
};
