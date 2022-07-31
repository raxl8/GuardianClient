#include "pch.h"

#include "ImGuiLayer.h"

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ImGuiLayer::ImGuiLayer(const UniquePtr<Window>& window, const UniquePtr<DirectXDevice>& device)
{
#ifdef _DEBUG
	IMGUI_CHECKVERSION();
#endif

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = nullptr;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window->GetNativeWindow());
	ImGui_ImplDX11_Init(device->GetNativeDevice(), device->GetNativeDeviceContext());

	window->OnProcedure.Subscribe([this](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return false;

		return true;
	});
}

ImGuiLayer::~ImGuiLayer()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::OnNewFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
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
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
