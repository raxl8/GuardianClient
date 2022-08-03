#include "pch.h"

#include "Core/Application.h"

#ifdef GDN_WINDOWS
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	auto application = MakeUnique<Application>();
	return application->Run();
}
#endif
