#include "pch.h"

#include "MinidumpServer.h"

#include "Core/CommandLine.h"

#include <optional>
#include <thread>
#include <CommCtrl.h>
#include <shellapi.h>

#include <fmt/printf.h>
#include <fmt/xchar.h>
#include <handler/handler_main.h>
#include <handler/crash_report_upload_thread.h>
#include <handler/win/crash_report_exception_handler.h>

static std::optional<std::string> tempCrashId;
static std::string crashId;
static std::thread taskDialogThread;

void OnCrash()
{
	taskDialogThread = std::thread([]()
	{
		TASKDIALOGCONFIG taskDialogConfig = { 0 };
		taskDialogConfig.cbSize = sizeof(taskDialogConfig);
		taskDialogConfig.hInstance = GetModuleHandleW(nullptr);
		taskDialogConfig.dwFlags = TDF_ENABLE_HYPERLINKS | TDF_EXPAND_FOOTER_AREA | TDF_SHOW_PROGRESS_BAR | TDF_CALLBACK_TIMER | TDF_USE_COMMAND_LINKS | TDF_EXPANDED_BY_DEFAULT;
		taskDialogConfig.dwCommonButtons = TDCBF_CLOSE_BUTTON;

		const TASKDIALOG_BUTTON buttons[] = { { IDOK, L"Copy Crash ID" } };
		taskDialogConfig.cButtons = ARRAYSIZE(buttons);
		taskDialogConfig.pButtons = buttons;

		taskDialogConfig.pszWindowTitle = L"" PRODUCT_NAME;
		taskDialogConfig.pszMainIcon = TD_ERROR_ICON;
		taskDialogConfig.pszMainInstruction = L"" PRODUCT_NAME " has crashed";
		taskDialogConfig.pszContent = L"An error occured during the execution causing it to stop working. A crash report is being uploaded to our developers.\nIf you need immediate support please visit <a href=\"https://support.guardian.ac/\">our Support page</a> and mention the Crash ID below and an explanation of the issue.";
		taskDialogConfig.pszExpandedInformation = L"Crash ID: [uploading...]";
		taskDialogConfig.lpCallbackData = (LONG_PTR)nullptr;
		taskDialogConfig.pfCallback = [](HWND hWnd, UINT type, WPARAM wParam, LPARAM lParam, LONG_PTR data)
		{
			if (type == TDN_HYPERLINK_CLICKED)
			{
				ShellExecuteW(nullptr, L"open", (LPCWSTR)lParam, nullptr, nullptr, SW_NORMAL);
			}
			else if (type == TDN_BUTTON_CLICKED)
			{
				if (wParam == IDOK)
				{
					if (OpenClipboard(NULL))
					{
						EmptyClipboard();
						auto data = GlobalAlloc(GMEM_MOVEABLE, crashId.size() + 1);
						if (!data)
						{
							CloseClipboard();
							return S_OK;
						}

						auto lockedData = GlobalLock(data);
						if (!lockedData)
						{
							GlobalFree(data);
							CloseClipboard();
							return S_OK;
						}

						memcpy(lockedData, &crashId[0], crashId.size() + 1);
						GlobalUnlock(data);

						SetClipboardData(CF_TEXT, data);
						CloseClipboard();
						GlobalFree(data);
					}
				}
				else
				{
					return S_OK;
				}
			}
			else if (type == TDN_CREATED)
			{
				SendMessageW(hWnd, TDM_ENABLE_BUTTON, IDCLOSE, 0);
				SendMessageW(hWnd, TDM_ENABLE_BUTTON, IDOK, 0);
				SendMessageW(hWnd, TDM_SET_MARQUEE_PROGRESS_BAR, 1, 0);
				SendMessageW(hWnd, TDM_SET_PROGRESS_BAR_MARQUEE, 1, 15);
			}
			else if (type == TDN_TIMER && tempCrashId)
			{
				if (!tempCrashId->empty())
				{
					crashId = *tempCrashId;
					SendMessageW(hWnd, TDM_SET_ELEMENT_TEXT, TDE_EXPANDED_INFORMATION, (LPARAM)fmt::sprintf(L"Crash ID: %s", ToWide(crashId)).c_str());
					SendMessageW(hWnd, TDM_ENABLE_BUTTON, IDOK, 1);
				}
				else
					SendMessageW(hWnd, TDM_SET_ELEMENT_TEXT, TDE_EXPANDED_INFORMATION, (LPARAM)L"Crash ID: [failed upload]");

				SendMessageW(hWnd, TDM_ENABLE_BUTTON, IDCLOSE, 1);
				SendMessageW(hWnd, TDM_SET_MARQUEE_PROGRESS_BAR, 0, 0);
				SendMessageW(hWnd, TDM_SET_PROGRESS_BAR_POS, 100, 0);
				SendMessageW(hWnd, TDM_SET_PROGRESS_BAR_STATE, PBST_NORMAL, 0);

				if (tempCrashId->empty())
					SendMessageW(hWnd, TDM_SET_PROGRESS_BAR_STATE, PBST_ERROR, 0);

				tempCrashId.reset();
			}

			return S_FALSE;
		};

		TaskDialogIndirect(&taskDialogConfig, nullptr, nullptr, nullptr);
	});
}

void OnUploadedReport(crashpad::CrashReportUploadThread::UploadResult result, const std::string& id)
{
	if (result == crashpad::CrashReportUploadThread::UploadResult::kSuccess)
	{
		tempCrashId = id;
		return;
	}

	tempCrashId = "";
}

int StartMinidumpServer(const CommandLine& commandLine)
{
	const auto& args = commandLine.GetArgs();

	// Having extra arguments upsets Crashpad and we can't increase
	// argv pointer as it needs argv[0] so we remap whole argv
	std::vector<char*> crashpadArgs;
	crashpadArgs.reserve(args.size());

	for (int i = 0; i < args.size(); i++)
	{
		if (args[i].find("--crashpad-handler") != std::string::npos)
			continue;

		crashpadArgs.push_back((char*)args[i].c_str());
	}

	crashpad::CrashReportExceptionHandler::SetOnCrashHandler(OnCrash);
	crashpad::CrashReportUploadThread::SetOnUploadedReportHandler(OnUploadedReport);

	auto exitCode = crashpad::HandlerMain(static_cast<int>(crashpadArgs.size()), &crashpadArgs[0], nullptr);

	if (taskDialogThread.joinable())
		taskDialogThread.join();

	return exitCode;
}
