#pragma once

#ifdef _WIN32
#define GDN_WINDOWS

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

BOOL IsWindowsBuildOrGreater(DWORD major, DWORD minor, DWORD build);
BOOL IsWindowsVersionOrGreater(DWORD major, DWORD minor, WORD sp);

#define IsMinimumWindows10() IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN10), LOBYTE(_WIN32_WINNT_WIN10), 0)
#define IsMinimumWindows10Build(build) IsWindowsBuildOrGreater(HIBYTE(_WIN32_WINNT_WIN10), LOBYTE(_WIN32_WINNT_WIN10), build)
#define IsMinimumWindows11() IsWindowsBuildOrGreater(HIBYTE(_WIN32_WINNT_WIN10), LOBYTE(_WIN32_WINNT_WIN10), 22000)

class ScopedHANDLE
{
public:
	ScopedHANDLE();
	ScopedHANDLE(HANDLE handle);
	~ScopedHANDLE();

	auto Get() { return &m_Handle; }
	operator HANDLE() { return m_Handle; }

private:
	HANDLE m_Handle;
};
#endif
