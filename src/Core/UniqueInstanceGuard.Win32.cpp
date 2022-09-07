#include "pch.h"

#include "UniqueInstanceGuard.h"

UniqueInstanceGuard::UniqueInstanceGuard()
{
	m_Mutex = CreateMutexW(NULL, FALSE, L"Local\\$Guardian$");
	if (m_Mutex != NULL && GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBoxW(NULL, L"An instance of Guardian is already running!", L"Guardian", MB_ICONERROR | MB_OK);
		ExitProcess(1);
	}
}

UniqueInstanceGuard::~UniqueInstanceGuard()
{
	ReleaseMutex(m_Mutex);
}
