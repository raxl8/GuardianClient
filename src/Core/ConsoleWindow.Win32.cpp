#include "pch.h"

#include "ConsoleWindow.h"

ConsoleWindow::ConsoleWindow()
{
	AllocConsole();

	FILE* dummyStream;
	freopen_s(&dummyStream, "CONIN$", "r", stdin);
	freopen_s(&dummyStream, "CONOUT$", "w", stdout);
	freopen_s(&dummyStream, "CONOUT$", "w", stderr);
}

ConsoleWindow::~ConsoleWindow()
{
	FreeConsole();
}
