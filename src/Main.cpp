#include "pch.h"

#include "Core/Application.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	auto application = std::make_unique<Application>();
	return application->Run();
}
