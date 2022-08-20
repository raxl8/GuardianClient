#include "pch.h"

#include "MinidumpServer.h"

#include <fmt/printf.h>
#include <handler/handler_main.h>
#include <handler/crash_report_upload_thread.h>

void OnUploadedReport(std::string& id)
{
	int result = MessageBoxA(NULL, fmt::sprintf(
		"Whoopsie! " PRODUCT_NAME R"( has crashed, a crash report has been uploaded so we can look further to prevent this error from happening.

Crash ID: %s

Would you like to copy this ID for further assistance?)",
		id).c_str(), PRODUCT_NAME, MB_TOPMOST | MB_ICONERROR | MB_OK | MB_YESNO);

	switch (result)
	{
	case IDYES:
	{
		if (OpenClipboard(NULL))
		{
			EmptyClipboard();
			auto data = GlobalAlloc(GMEM_MOVEABLE, id.size() + 1);
			if (!data)
			{
				CloseClipboard();
				break;
			}

			auto lockedData = GlobalLock(data);
			if (!lockedData)
			{
				GlobalFree(data);
				CloseClipboard();
				break;
			}

			memcpy(lockedData, id.c_str(), id.size() + 1);
			GlobalUnlock(data);

			SetClipboardData(CF_TEXT, data);
			CloseClipboard();
			GlobalFree(data);
		}
		break;
	}
	case IDNO:
	default:
		break;
	}
}

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

	crashpad::CrashReportUploadThread::SetOnUploadedReportHandler(OnUploadedReport);

	return crashpad::HandlerMain(crashpadArgc, crashpadArgv.get(), nullptr);
}
