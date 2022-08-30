#pragma once

#include <imgui.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

namespace ImGuardian
{
	bool Button(const char* label, const ImVec2& sizeArg = ImVec2(0, 0));
}
