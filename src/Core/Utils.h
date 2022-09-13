#pragma once

#define GDN_CONCAT(x, y) x ## y

std::string ToNarrow(const std::wstring& wide);
std::wstring ToWide(const std::string& narrow);
