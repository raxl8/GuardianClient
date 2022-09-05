#pragma once

#include <tbb/concurrent_queue.h>

class Renderer
{
public:
	Renderer() = default;
	~Renderer() = default;

	void OnNewFrame();
	void EnqueueTask(std::function<void()>&& task);
	void* LoadTexture(const void* data, const int size);

private:
	tbb::concurrent_queue<std::function<void()>> m_TaskQueue;
};
