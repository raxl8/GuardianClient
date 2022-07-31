#pragma once

#include "Core/Callback.h"

class Window
{
public:
	Window(const std::wstring& title);
	~Window();

	void SetVisibility(bool visible = true);
	bool Poll();

	inline HWND GetNativeWindow() { return m_Handle; }

public:
	Callback<HWND, UINT, WPARAM, LPARAM> OnProcedure;

private:
	std::wstring m_Title;
	WNDCLASSEX m_Class;
	HWND m_Handle;
};
