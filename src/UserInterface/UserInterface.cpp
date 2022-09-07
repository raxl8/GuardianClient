#include "pch.h"

#include "UserInterface.h"

#include "Core/Application.h"
#include "ImGui/ImGuiCustom.h"
#include "Views/HomeView.h"
#include "Views/ErrorView.h"

#include <fonts/Font-Awesome/fa-solid.h>
#include <fonts/Roboto/Roboto-Light.h>
#include <fonts/Roboto/Roboto-Medium.h>
#include <fonts/Roboto/Roboto-Regular.h>

UserInterface::UserInterface()
	: m_DisplayingError(false), m_ViewAnimationTime(0.f)
{
	m_DarkMode = Instance<Application>::Get()->IsDarkMode();

	ApplyStyles();

	auto& io = ImGui::GetIO();
	m_TitleFont = io.Fonts->AddFontFromMemoryCompressedTTF(Roboto_Light_compressed_data, Roboto_Light_compressed_size, 30.f);
	m_RegularFont = io.Fonts->AddFontFromMemoryCompressedTTF(Roboto_Medium_compressed_data, Roboto_Medium_compressed_size, 17.f);
	m_FooterFont = io.Fonts->AddFontFromMemoryCompressedTTF(Roboto_Regular_compressed_data, Roboto_Regular_compressed_size, 17.f);

	{
		static ImWchar glyphRange[] = { 0xe005, 0xf8ff, 0 };
		ImFontConfig config;
		config.MergeMode = true;
		m_IconFont = io.Fonts->AddFontFromMemoryCompressedTTF(fa_solid_compressed_data, fa_solid_compressed_size, 18.f, &config, glyphRange);
	}

	SetView(MakeShared<HomeView>(this));
}

void UserInterface::PushTitleFont()
{
	ImGui::PushFont(m_TitleFont);
}

void UserInterface::PushRegularFont()
{
	ImGui::PushFont(m_RegularFont);
}

void UserInterface::RenderImGui()
{
	if (m_PreviousView.get() && m_ViewAnimationTime < 1.f)
	{
		m_ViewAnimationTime += 2.f * ImGui::GetIO().DeltaTime;
	}
	else if (m_ViewAnimationTime > 0.f)
	{
		m_ViewAnimationTime = 0.f;
		m_PreviousView = {};
	}

	constexpr auto easeInOutQuart = [](float t)
	{
		float t2;
		if (t < 0.5f)
		{
			t2 = t * t;
			return 16.f * t * t2 * t2;
		}
		else
		{
			t2 = (--t) * t;
			return 1.f + 16.f * t * t2 * t2;
		}
	};

	if (m_PreviousView.get() && m_ViewAnimationTime > 0.f)
	{
		ImGui::SetNextWindowPos({ -WINDOW_WIDTH * easeInOutQuart(m_ViewAnimationTime), 0.f});
		BeginView("##previousView");

		m_PreviousView->RenderImGui();

		EndView();
	}

	ImGui::SetNextWindowPos({ m_ViewAnimationTime > 0.f ? WINDOW_WIDTH - WINDOW_WIDTH * easeInOutQuart(m_ViewAnimationTime) : 0.f, 0.f});
	BeginView("##view");

	m_CurrentView->RenderImGui();

	EndView();
}

void UserInterface::SetView(SharedPtr<View> newView)
{
	std::unique_lock lock(m_CurrentViewMutex);

	if (m_CurrentView.get())
		m_PreviousView = std::move(m_CurrentView);

	m_CurrentView = std::move(newView);

	SharedPtr<View> selfRef = m_CurrentView;
	auto renderer = Instance<Application>::Get()->GetRenderer();
	renderer->EnqueueTask([selfRef, renderer]()
	{
		selfRef->OnLoad(renderer);
	});
}

void UserInterface::DisplayError(const std::string& title, const std::string& description)
{
	if (m_DisplayingError)
		return;

	m_DisplayingError = true;

	SetView(MakeShared<ErrorView>(this, title, description));
}

void UserInterface::SetDarkMode(bool enabled)
{
	m_DarkMode = enabled;
	ApplyStyles();
}

void UserInterface::BeginView(const char* name)
{
	ImGui::SetNextWindowSize({ WINDOW_WIDTH, WINDOW_HEIGHT });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
	ImGui::Begin(name, nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
	ImGui::PopStyleVar();
}

void UserInterface::EndView()
{
	{ // "1.0 (production)" text
		ImGui::PushFont(m_FooterFont);

		ImGui::SetCursorPosY(WINDOW_HEIGHT - ImGui::CalcTextSize(BUILD_DESCRIPTION).y - ImGui::GetStyle().WindowPadding.y);
		ImGui::TextUnformatted(BUILD_DESCRIPTION);

		ImGui::PopFont();
	}

	{ // Sun/Moon icon button for theme switching
		ImGui::PushFont(m_IconFont);

		const auto themeButtonSize = ImVec2(
			19.f, 19.f
		);

		const auto windowPadding = ImGui::GetStyle().WindowPadding;
		ImGui::SetCursorPos(ImVec2(
			WINDOW_WIDTH - themeButtonSize.x - windowPadding.x,
			WINDOW_HEIGHT - themeButtonSize.y - windowPadding.y
		));

		const auto sunIcon = "\xef\x86\x85";
		const auto moonIcon = "\xef\x86\x86";
		if (ImGuardian::Button(m_DarkMode ? sunIcon : moonIcon, themeButtonSize, /*invisible =*/ true))
		{
			m_DarkMode = !m_DarkMode;
			Instance<Application>::Get()->SetDarkMode(m_DarkMode);
		}

		ImGui::PopFont();
	}

	ImGui::End();
}

void UserInterface::ApplyStyles()
{
	ImGui::StyleColorsDark(); // Default ImGui Colors

	auto& style = ImGui::GetStyle();
	style.FrameRounding = 3.f;
	style.WindowShadowSize = 0.f;

	const ImColor accent = ImColor(109, 35, 225);
	style.Colors[ImGuiCol_Button] = accent;
	style.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_Button] + ImVec4(0.1f, 0.1f, 0.1f, 0.f);
	style.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_Button] + ImVec4(0.05f, 0.05f, 0.05f, 0.f);
	style.Colors[ImGuiCol_TextSelectedBg] = style.Colors[ImGuiCol_Button] - ImVec4(0.1f, 0.1f, 0.1f, 0.5f);
	style.Colors[ImGuiCol_FrameBg] = m_DarkMode ? ImColor(30, 30, 30) : ImColor(180, 180, 180);
	style.Colors[ImGuiCol_PlotHistogram] = accent;
	style.Colors[ImGuiCol_Border] = accent;
	style.Colors[ImGuiCol_Text] = m_DarkMode ? style.Colors[ImGuiCol_Text] : ImVec4(0.f, 0.f, 0.f, 1.f);
	style.Colors[ImGuiCol_WindowBg] = m_DarkMode ? ImColor(24, 24, 24) : ImColor(255, 255, 255);
	style.Colors[ImGuiCol_WindowShadow] = ImVec4(0.f, 0.f, 0.f, .5f);
}
