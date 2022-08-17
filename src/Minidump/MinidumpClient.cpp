#include "pch.h"

#include "MinidumpClient.h"

#include <cpr/api.h>
#include <nlohmann/json.hpp>
#include <fmt/printf.h>
#include <fmt/chrono.h>
#include <client/crash_report_database.h>
#include <client/crashpad_client.h>
#include <client/crashpad_info.h>
#include <client/prune_crash_reports.h>
#include <client/settings.h>
#include <util/misc/uuid.h>
#include <util/win/termination_codes.h>

nlohmann::json g_Session;

void UploadSession(nlohmann::json& data)
{
	std::stringstream bodyData;
	bodyData << "{}\n";
	bodyData << R"({"type":"session"})" << "\n";
	bodyData << data.dump(-1, ' ', false, nlohmann::detail::error_handler_t::replace) << "\n";

	cpr::Post(
		cpr::Url{ fmt::sprintf("%s/api/%d/envelope/", SENTRY_URL, SENTRY_PROJECT_ID) },
		cpr::Body{ bodyData.str() },
		cpr::VerifySsl{ false },
		cpr::Header{
			{
				"X-Sentry-Auth",
				fmt::sprintf("Sentry sentry_version=7, sentry_key=%s", SENTRY_KEY)
			}
		},
		cpr::Timeout{ 2500 }
	);
}

bool CrashpadHandler(EXCEPTION_POINTERS*)
{
	bool shouldDump = true;

	g_Session["status"] = "crashed";
	UploadSession(g_Session);

	if (!shouldDump)
		TerminateProcess(GetCurrentProcess(), crashpad::TerminationCodes::kTerminationCodeCrashNoDump);

	return false;
}

bool StartCrashpad()
{
	wchar_t currentPath[MAX_PATH];
	GetModuleFileNameW(NULL, currentPath, MAX_PATH);

	base::FilePath handlerPath(currentPath);

	std::wstring databasePathStr = _wgetenv(L"APPDATA");
	databasePathStr.append(L"\\Guardian");

	base::FilePath databasePath(databasePathStr);
	auto database = crashpad::CrashReportDatabase::Initialize(databasePath);
	if (database)
	{
		auto settings = database->GetSettings();
		if (settings)
			settings->SetUploadsEnabled(true);
	}
	
	std::map<std::string, std::string> annotations;
	annotations["sentry[release]"] = "guardian-client@" BUILD_VERSION;
	annotations["sentry[environment]"] = BUILD_CHANNEL;

	std::vector<std::string> arguments;
	arguments.push_back("--crashpad-handler");
	arguments.push_back("--no-rate-limit");

	crashpad::CrashpadClient client;
	if (!client.StartHandler(
		handlerPath,
		databasePath,
		databasePath,
		fmt::sprintf("%s/api/%d/minidump/?sentry_key=%s", SENTRY_URL, SENTRY_PROJECT_ID, SENTRY_KEY),
		annotations,
		arguments,
		true,
		false
	))
		return false;

	crashpad::CrashpadClient::SetFirstChanceExceptionHandler(&CrashpadHandler);


	crashpad::CrashpadInfo* crashpad_info = crashpad::CrashpadInfo::GetCrashpadInfo();
	crashpad_info->set_system_crash_reporter_forwarding(crashpad::TriState::kDisabled);

	return true;
}

bool StartMinidumpClient()
{
	crashpad::UUID uuid;
	uuid.InitializeWithNew();

	g_Session = nlohmann::json::object({
		{ "sid", uuid.ToString()},
		{ "did", "default" },
		{ "init", true },
		{ "started", fmt::format("{:%Y-%m-%dT%H:%M:%S}Z", fmt::gmtime(std::time(nullptr))) },
		{ "attrs", {
			{ "release", "guardian-client@" BUILD_VERSION },
			{ "environment", BUILD_CHANNEL }
		}}
	});

	UploadSession(g_Session);

	g_Session["init"] = false;

	bool success = StartCrashpad();
	if (!success)
	{
		g_Session["status"] = "abnormal";
		UploadSession(g_Session);
	}

	return success;
}

void CloseMinidumpClient()
{
	g_Session["status"] = "exited";
	UploadSession(g_Session);
}
