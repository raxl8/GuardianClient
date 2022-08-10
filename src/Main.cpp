#include "pch.h"

#include "Core/Application.h"
#include "Hooking/HookFunction.h"

int RealMain(int argc, char* argv[])
{
	HookFunction::RunAll();

	auto application = new Application;
	Instance<Application>::Set(application);

	auto exitCode = application->Run();

	delete application;

	return exitCode;
}

#ifdef GDN_WINDOWS
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
#ifndef GDN_PRODUCTION
	AllocConsole();

	FILE* dummyStream;
	freopen_s(&dummyStream, "CONIN$", "r", stdin);
	freopen_s(&dummyStream, "CONOUT$", "w", stdout);
	freopen_s(&dummyStream, "CONOUT$", "w", stderr);
#endif

	auto exitCode = RealMain(__argc, __argv);

#ifndef GDN_PRODUCTION
	FreeConsole();
#endif

	return exitCode;
}
#else
int main(int argc, char* argv[])
{
	return RealMain(argc, argv);
}
#endif
