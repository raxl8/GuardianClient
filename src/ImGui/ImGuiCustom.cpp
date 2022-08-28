#include "pch.h"

#include "ImGuiCustom.h"

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
