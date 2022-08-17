#include "pch.h"

#include "Core/Application.h"
#include "Hooking/HookFunction.h"
#include "Minidump/MinidumpClient.h"
#include "Minidump/MinidumpServer.h"

int RealMain(int argc, char* argv[])
{
#if GDN_PRODUCTION || 1
	if (wcsstr(GetCommandLineW(), L"--crashpad-handler"))
		return StartMinidumpServer(argc, argv);

	if (!StartMinidumpClient())
		return EXIT_FAILURE;
#endif

	HookFunction::RunAll();

	auto application = new Application;
	Instance<Application>::Set(application);

	auto exitCode = application->Run();

	delete application;

	CloseMinidumpClient();

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

	auto argc = __argc;
	auto argv = __wargv;

	UniquePtr<char* []> utf8Argv(new char* [argc + 1]);
	std::vector<std::string> storage;
	storage.reserve(argc);

	for (int i = 0; i < argc; ++i) {
		storage.push_back(ToNarrow(argv[i]));
		utf8Argv[i] = &storage[i][0];
	}

	utf8Argv[argc] = nullptr;

	auto exitCode = RealMain(__argc, utf8Argv.get());

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
