#include "pch.h"

#include "MinidumpServer.h"

#include <handler/handler_main.h>

int StartMinidumpServer(int argc, char* argv[])
{
	// Having extra arguments upsets Crashpad and we can't increase
	// argv pointer as it needs argv[0] so we remap whole argv
	int crashpadArgc = 0;
	UniquePtr<char* []> crashpadArgv(new char* [argc + 1]);

	for (int i = 0; i < argc; ++i) {
		if (strstr(argv[i], "--crashpad-handler"))
			continue;

		crashpadArgv[crashpadArgc++] = argv[i];
	}

	crashpadArgv[argc] = nullptr;

	return crashpad::HandlerMain(crashpadArgc, crashpadArgv.get(), nullptr);
}
