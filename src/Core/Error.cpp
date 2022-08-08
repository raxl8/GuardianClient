#include "pch.h"

void FatalError()
{
#ifdef GDN_DEBUG
	__debugbreak();
#endif

	TerminateProcess(GetCurrentProcess(), EXIT_FAILURE);
}
