#include "pch.h"

#include "ScopedConsole.h"

ScopedConsole::ScopedConsole()
{
	AllocConsole();

	FILE* dummyStream;
	freopen_s(&dummyStream, "CONIN$", "r", stdin);
	freopen_s(&dummyStream, "CONOUT$", "w", stdout);
	freopen_s(&dummyStream, "CONOUT$", "w", stderr);
}

ScopedConsole::~ScopedConsole()
{
	FreeConsole();
}
