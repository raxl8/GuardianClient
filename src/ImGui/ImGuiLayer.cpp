#include "pch.h"

#include "ImGuiLayer.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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

void ImGuiLayer::OnNewFrame()
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::OnImGuiRender()
{
	ImGui::SetNextWindowPos({ 0.f, 0.f });
	ImGui::ShowDemoWindow(nullptr);
}

void ImGuiLayer::OnRender()
{
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}
