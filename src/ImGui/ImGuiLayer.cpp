#include "pch.h"

#include "ImGuiLayer.h"

#include "Core/Window.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>

ImGuiLayer::ImGuiLayer(Window* window)
{
#ifdef GDN_DEBUG
	IMGUI_CHECKVERSION();
#endif

	ImGui::CreateContext();

	auto& io = ImGui::GetIO();
	io.IniFilename = nullptr;

	ImGui_ImplGlfw_InitForOpenGL(window->GetNativeWindow(), true);
	ImGui_ImplOpenGL2_Init();
}

ImGuiLayer::~ImGuiLayer()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::Begin()
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::End()
{
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}
