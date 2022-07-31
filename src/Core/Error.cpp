#include "pch.h"

void FatalError()
{
#ifdef _DEBUG
	__debugbreak();
#endif

	TerminateProcess(GetCurrentProcess(), EXIT_FAILURE);
}
