#include "pch.h"

#include "RecycleBinStage.h"

#include "Scanner/Scanner.h"

#include <sddl.h>

StageResult RecycleBinStage::Run()
{
	ScopedHANDLE tokenHandle;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, tokenHandle.Get()))
		return GDN_STAGERESULT(ErrorContinue, "OpenProcessToken returned %d", GetLastError());

	auto userToken = (TOKEN_USER*)malloc(sizeof(TOKEN_USER));
	if (!userToken)
		return GDN_STAGERESULT(Error, "Could not allocate %d bytes", sizeof(TOKEN_USER));

	DWORD size = 0;
	if (!GetTokenInformation(tokenHandle, TOKEN_INFORMATION_CLASS::TokenUser, nullptr, size, &size))
	{
		auto lastError = GetLastError();
		if (lastError != ERROR_INSUFFICIENT_BUFFER)
		{
			free(userToken);
			return GDN_STAGERESULT(Error, "GetTokenInformation returned %d", lastError);
		}
		
		auto newUserToken = (TOKEN_USER*)realloc(userToken, size);
		if (!newUserToken)
			return GDN_STAGERESULT(Error, "Could not allocate %d bytes", size);

		userToken = newUserToken;
	}

	if (!GetTokenInformation(tokenHandle, TOKEN_INFORMATION_CLASS::TokenUser, userToken, size, &size))
	{
		free(userToken);
		return GDN_STAGERESULT(ErrorContinue, "GetTokenInformation returned %d", GetLastError());
	}

	WCHAR* sidStr = nullptr;
	if (!ConvertSidToStringSidW(userToken->User.Sid, &sidStr))
	{
		free(userToken);
		return GDN_STAGERESULT(ErrorContinue, "ConvertSidToStringSidW returned %d", GetLastError());
	}

	free(userToken);

	std::wstring recycleBinPath = _wgetenv(L"SystemDrive");
	recycleBinPath.append(L"\\$Recycle.Bin\\");
	recycleBinPath.append(sidStr);

	LocalFree(sidStr);

	WIN32_FIND_DATAW findData;
	HANDLE fileHandle = FindFirstFileW(recycleBinPath.c_str(), &findData);
	if (fileHandle == INVALID_HANDLE_VALUE)
		return GDN_STAGERESULT(ErrorContinue, "FindFirstFileW returned %d with path %s", GetLastError(), ToNarrow(recycleBinPath));

	FindClose(fileHandle);
	
	const auto& fileTime = findData.ftLastWriteTime;

	ULARGE_INTEGER ull;
	ull.LowPart = fileTime.dwLowDateTime;
	ull.HighPart = fileTime.dwHighDateTime;

	m_Scanner->GetScanData().RecycleBinModifiedAt = ull.QuadPart / 10000000ULL - 11644473600ULL;

	return StageStatus::Success;
}
