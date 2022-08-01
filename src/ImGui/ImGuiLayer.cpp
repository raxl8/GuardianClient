#include "pch.h"

#include "ImGuiLayer.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>

ImGuiLayer::ImGuiLayer(const UniquePtr<Window>& window)
{
#ifdef _DEBUG
	IMGUI_CHECKVERSION();
#endif

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = nullptr;

	ImGui::StyleColorsDark();

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
