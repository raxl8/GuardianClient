#include "pch.h"

#include "Core/Application.h"
#include "Core/CommandLine.h"
#include "Core/ConsoleWindow.h"
#include "Hooking/HookFunction.h"
#include "Minidump/MinidumpClient.h"
#include "Minidump/MinidumpServer.h"

int RealMain(CommandLine commandLine)
{
#ifdef GDN_RELEASE
	if (commandLine.Contains("--crashpad-handler"))
		return StartMinidumpServer(commandLine);

	MinidumpClient minidumpClient;
#endif

	HookFunction::RunAll();

	auto application = new Application;

	auto exitCode = application->Run();

	delete application;

	return exitCode;
}

#ifdef GDN_WINDOWS
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
#ifndef GDN_RELEASE
	ConsoleWindow console;
#endif

	return RealMain(CommandLine(__argc, __wargv));
}
#else
int main(int argc, char* argv[])
{
	return RealMain(CommandLine(argc, argv));
}
#endif
