#include "pch.h"

#include "ImGuiCustom.h"

#include "ImGui/GFX/cubic_bezier.h"

#include <unordered_map>
#include <algorithm>
#include <numbers>

static bool ButtonEx(const char* label, const ImVec2& size_arg = ImVec2(0, 0), ImGuiButtonFlags flags = 0)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ImGui::ItemSize(size, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	if (g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat)
		flags |= ImGuiButtonFlags_Repeat;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);

	static std::unordered_map<ImGuiID, float> times;
	if (times.find(id) == times.end())
		times.emplace(id, 0.f);

	constexpr const auto easeInOutSine = [](float t)
	{
		return 0.5f * (1.f + sinf(std::numbers::pi_v<float> * (t - 0.5f)));
	};

	const auto animationSpeed = 8.f;

	auto& time = times[id];
	if (hovered && time < 1.f)
	{
		time += animationSpeed * g.IO.DeltaTime;
	}
	else if (!hovered && time > 0.f)
	{
		time -= animationSpeed * g.IO.DeltaTime;
	}

	time = std::clamp(time, 0.f, 1.f);

	// Render

	ImU32 col;
	if (!held)
	{
		const auto baseColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
		const auto hoveredColor = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);

		auto currentColor = baseColor;

		if (time > 0.f)
		{
			auto factor = easeInOutSine(time);
			auto diff = (hoveredColor - baseColor) * ImVec4(factor, factor, factor, 1.f);
			currentColor.x += diff.x;
			currentColor.y += diff.y;
			currentColor.z += diff.z;
		}

		col = ImGui::GetColorU32(currentColor);
	}
	else
	{
		col = ImGui::GetColorU32(ImGuiCol_ButtonActive);
	}

	ImGui::RenderNavHighlight(bb, id);
	ImGui::RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);

	if (g.LogEnabled)
		ImGui::LogSetNextTextDecoration("[", "]");
	ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

	return pressed;
}

bool ImGuardian::Button(const char* label, const ImVec2& size_arg /*= ImVec2(0, 0)*/)
{
	return ButtonEx(label, size_arg, ImGuiButtonFlags_None);
}

static float bezier(float t)
{
	static gfx::CubicBezier fast_out_slow_in(0.4, 0.0, 0.2, 1.0);
	return static_cast<float>(fast_out_slow_in.Solve(t));
}

static auto interval(float T0, float T1, float t)
{
	return t < T0 ? 0.0f : t > T1 ? 1.0f : bezier((t - T0) / (T1 - T0));
}

template <int N>
float sawtooth(float t)
{
	return ImFmod(((float)N) * t, 1.0f);
}

// Credits: https://github.com/ocornut/imgui/issues/1901#issuecomment-552185000
bool ImGuardian::Spinner(const char* label, float radius, float thickness, float speed, const ImU32& color)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
	ImGui::ItemSize(bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	// Render
	const ImVec2 center = bb.Min + ImVec2(radius, radius + thickness + style.FramePadding.y);

	const float start_angle = -IM_PI / 2.0f;         // Start at the top
	const int num_detents = 5;                       // how many rotations we want before a repeat
	const int skip_detents = 3;                      // how many steps we skip each rotation
	const float t = ImFmod(static_cast<float>(g.Time), speed) / speed; // map period into [0, 1]

	// Tweening functions for each part of the spinner
	auto stroke_head_tween = [](float t)
	{
		t = sawtooth<num_detents>(t);
		return interval(.0f, .5f, t);
	};

	auto stroke_tail_tween = [](float t)
	{
		t = sawtooth<num_detents>(t);
		return interval(.5f, 1.f, t);
	};

	auto step_tween = [=](float t)
	{
		return std::floorf(std::lerp(0.f, (float)num_detents, t));
	};

	auto rotation_tween = sawtooth<num_detents>;

	const float head_value = stroke_head_tween(t);
	const float tail_value = stroke_tail_tween(t);
	const float step_value = step_tween(t);
	const float rotation_value = rotation_tween(t);

	const float min_arc = 30.0f / 360.0f * 2.0f * IM_PI;
	const float max_arc = 270.0f / 360.0f * 2.0f * IM_PI;
	const float step_offset = skip_detents * 2.0f * IM_PI / num_detents;
	const float rotation_compensation = ImFmod(4.0 * IM_PI - step_offset - max_arc, 2 * IM_PI);

	const float a_min = start_angle + tail_value * max_arc + rotation_value * rotation_compensation - step_value * step_offset;
	const float a_max = a_min + (head_value - tail_value) * max_arc + min_arc;

	window->DrawList->PathClear();

	int num_segments = 24;
	for (int i = 0; i < num_segments; i++)
	{
		const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
		window->DrawList->PathLineTo(ImVec2(center.x + ImCos(a) * radius,
			center.y + ImSin(a) * radius));
	}

	window->DrawList->PathStroke(color, false, thickness);

	return true;
}
