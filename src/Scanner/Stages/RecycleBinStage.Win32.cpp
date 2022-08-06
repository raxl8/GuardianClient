#include "pch.h"

#include "RecycleBinStage.h"

#include "Scanner/Scanner.h"

#include <sddl.h>

RecycleBinStage::RecycleBinStage(Scanner* scanner)
	: RecycleBinStage(scanner)
{
}

StageResult RecycleBinStage::Run()
{
	HANDLE token;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token))
		return GDN_STAGERESULT(ErrorContinue, "OpenProcessToken returned %d", GetLastError());

	auto userToken = (TOKEN_USER*)malloc(sizeof(TOKEN_USER));
	DWORD size = 0;
	if (!GetTokenInformation(token, TOKEN_INFORMATION_CLASS::TokenUser, nullptr, size, &size))
	{
		auto lastError = GetLastError();
		if (lastError != ERROR_INSUFFICIENT_BUFFER)
			return GDN_STAGERESULT(Error, "GetTokenInformation returned %d", lastError);

		free(userToken);
		userToken = (TOKEN_USER*)malloc(size);
	}

	if (!GetTokenInformation(token, TOKEN_INFORMATION_CLASS::TokenUser, userToken, size, &size) || !userToken)
		return GDN_STAGERESULT(ErrorContinue, "GetTokenInformation returned %d", GetLastError());

	WCHAR* sidStr = nullptr;
	BOOL succeeded = ConvertSidToStringSidW(userToken->User.Sid, &sidStr);
	free(userToken);

	if (!succeeded)
		return GDN_STAGERESULT(ErrorContinue, "ConvertSidToStringSidW returned %d", GetLastError());

	std::wstring recycleBinPath = _wgetenv(L"SystemDrive");
	recycleBinPath.append(L"\\$Recycle.Bin\\");
	recycleBinPath.append(sidStr);

	LocalFree(sidStr);

	WIN32_FIND_DATAW findData;
	HANDLE fileHandle = FindFirstFileW(recycleBinPath.c_str(), &findData);
	if (fileHandle == INVALID_HANDLE_VALUE)
		return GDN_STAGERESULT(ErrorContinue, "FindFirstFileW returned %d with path %s", GetLastError(), ToNarrow(recycleBinPath));

	FindClose(fileHandle);
	
	auto& fileTime = findData.ftLastWriteTime;

	ULARGE_INTEGER ull;
	ull.LowPart = fileTime.dwLowDateTime;
	ull.HighPart = fileTime.dwHighDateTime;

	m_Scanner->GetScanData().RecycleBinModifiedAt = ull.QuadPart / 10000000ULL - 11644473600ULL;

	return StageStatus::Success;
}
