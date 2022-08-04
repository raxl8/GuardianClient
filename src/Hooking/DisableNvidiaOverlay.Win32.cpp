#include "pch.h"

#include "HookFunction.h"

#include <MinHook.h>

#include <winternl.h>

static NTSTATUS(NTAPI* g_origLoadDll)(WCHAR*, ULONG*, UNICODE_STRING*, HANDLE*);

NTSTATUS NTAPI LdrLoadDllStub(WCHAR* fileName, ULONG* flags, UNICODE_STRING* moduleFileName, HANDLE* moduleHandle)
{
	std::wstring_view moduleFileNameView(moduleFileName->Buffer, moduleFileName->Length / sizeof(WCHAR));

	if (moduleFileNameView.find(L"nvspcap64.dll") != std::wstring::npos)
	{
		// STATUS_INVALID_IMAGE_HASH, basically tell windows kernel to never try to load again
		return 0xC0000428;
	}

	return g_origLoadDll(fileName, flags, moduleFileName, moduleHandle);
}

static HookFunction hookFunction([]() {
	MH_Initialize();
	MH_CreateHookApi(L"ntdll.dll", "LdrLoadDll", LdrLoadDllStub, (void**)&g_origLoadDll);
	MH_EnableHook(MH_ALL_HOOKS);
});
