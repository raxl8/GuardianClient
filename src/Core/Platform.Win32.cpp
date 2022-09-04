#include "pch.h"

#include "Hooking/HookFunction.h"

LONG (WINAPI *RtlVerifyVersionInfo)(OSVERSIONINFOEXW*, ULONG, ULONGLONG);

BOOL IsWindowsBuildOrGreater(DWORD major, DWORD minor, DWORD build)
{
	OSVERSIONINFOEXW osvi = {
		.dwOSVersionInfoSize = sizeof(osvi),
		.dwMajorVersion = major,
		.dwMinorVersion = minor,
		.dwBuildNumber = build,
		.dwPlatformId = 0,
		.szCSDVersion = { 0 },
		.wServicePackMajor = 0,
		.wServicePackMinor = 0,
		.wSuiteMask = 0,
		.wProductType = 0,
		.wReserved = 0
	};

	DWORD mask = VER_MAJORVERSION | VER_MINORVERSION | VER_BUILDNUMBER;
	ULONGLONG cond = VerSetConditionMask(0, VER_MAJORVERSION, VER_GREATER_EQUAL);
	cond = VerSetConditionMask(cond, VER_MINORVERSION, VER_GREATER_EQUAL);
	cond = VerSetConditionMask(cond, VER_BUILDNUMBER, VER_GREATER_EQUAL);
	return RtlVerifyVersionInfo(&osvi, mask, cond) == 0;
}

BOOL IsWindowsVersionOrGreater(DWORD major, DWORD minor, WORD sp)
{
	OSVERSIONINFOEXW osvi = {
		.dwOSVersionInfoSize = sizeof(osvi),
		.dwMajorVersion = major,
		.dwMinorVersion = minor,
		.dwBuildNumber = 0,
		.dwPlatformId = 0,
		.szCSDVersion = { 0 },
		.wServicePackMajor = sp,
		.wServicePackMinor = 0,
		.wSuiteMask = 0,
		.wProductType = 0,
		.wReserved = 0
	};

	DWORD mask = VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR;
	ULONGLONG cond = VerSetConditionMask(0, VER_MAJORVERSION, VER_GREATER_EQUAL);
	cond = VerSetConditionMask(cond, VER_MINORVERSION, VER_GREATER_EQUAL);
	cond = VerSetConditionMask(cond, VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);
	return RtlVerifyVersionInfo(&osvi, mask, cond) == 0;
}

static HookFunction hookFunction([]()
{
	auto ntdllHandle = LoadLibraryW(L"ntdll.dll");
	if (!ntdllHandle)
	{
		FatalError();
		return;
	}

	RtlVerifyVersionInfo = (decltype(RtlVerifyVersionInfo))GetProcAddress(ntdllHandle, "RtlVerifyVersionInfo");
});
