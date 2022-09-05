#include "pch.h"

#include "Renderer.h"

#include "Core/Decompression.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <gl/GL.h>

void Renderer::OnNewFrame()
{
	std::function<void()> task;
	while (m_TaskQueue.try_pop(task))
	{
		task();
	}
}

void Renderer::EnqueueTask(std::function<void()>&& task)
{
	m_TaskQueue.push(std::move(task));
}

void* Renderer::LoadTexture(const void* data, const int size)
{
	auto decompressLength = stb_decompress_length((const uint8_t*)data);
	auto decompressedBuffer = new uint8_t[decompressLength];
	stb_decompress(decompressedBuffer, (const uint8_t*)data, size);

	int imageWidth = 0;
	int imageHeight = 0;
	stbi_uc* imageData = stbi_load_from_memory(decompressedBuffer, decompressLength, &imageWidth, &imageHeight, nullptr, 4);

	delete[] decompressedBuffer;

	if (imageData == nullptr)
		return nullptr;

	GLuint imageTexture;
	glGenTextures(1, &imageTexture);
	glBindTexture(GL_TEXTURE_2D, imageTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);

	return (void*)(intptr_t)imageTexture;
}
