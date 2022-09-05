#include "pch.h"

#include "MinidumpClient.h"

#include <cpr/api.h>
#include <nlohmann/json.hpp>
#include <fmt/chrono.h>
#include <client/crash_report_database.h>
#include <client/crashpad_client.h>
#include <client/settings.h>
#include <util/misc/uuid.h>
#include <util/win/termination_codes.h>

static nlohmann::json g_Session;

void UploadSession(nlohmann::json& data)
{
	std::stringstream bodyData;
	bodyData << "{}\n";
	bodyData << R"({"type":"session"})" << "\n";
	bodyData << data.dump(-1, ' ', false, nlohmann::detail::error_handler_t::replace) << "\n";

	cpr::Post(
		cpr::Url{ SENTRY_URL "/api/" SENTRY_PROJECT_ID "/envelope/" },
		cpr::Body{ bodyData.str() },
		cpr::VerifySsl{ false },
		cpr::Header{
			{
				"X-Sentry-Auth",
				"Sentry sentry_version=7, sentry_key=" SENTRY_KEY
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

	std::string url = SENTRY_URL "/api/" SENTRY_PROJECT_ID "/minidump/?sentry_key=" SENTRY_KEY;
	
	std::map<std::string, std::string> annotations;
	annotations["commit"] = BUILD_COMMIT;
	annotations["version"] = BUILD_VERSION;
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
		url,
		annotations,
		arguments,
		true,
		false
	))
		return false;

	crashpad::CrashpadClient::SetFirstChanceExceptionHandler(&CrashpadHandler);

	return true;
}

MinidumpClient::MinidumpClient()
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

	if (!StartCrashpad())
	{
		g_Session["status"] = "abnormal";
		UploadSession(g_Session);
	}
}

MinidumpClient::~MinidumpClient()
{
	g_Session["status"] = "exited";
	UploadSession(g_Session);
}
