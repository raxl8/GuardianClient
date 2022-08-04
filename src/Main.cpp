#include "pch.h"

#include "Core/Application.h"
#include "Hooking/HookFunction.h"

#ifdef GDN_WINDOWS
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	HookFunction::RunAll();

	auto application = MakeUnique<Application>();
	return application->Run();
}
#endif
