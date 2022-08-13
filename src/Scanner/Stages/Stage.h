#pragma once

#include <fmt/printf.h>

enum class StageStatus
{
	Error = -1,
	Success,
	ErrorContinue,
};

struct StageException
{
	std::string Message;
	const char* Function;
	const char* File;
	int Line;
};

struct StageResult
{
	StageStatus Status;
	StageException Exception;

	StageResult(StageStatus status)
		: Status(status)
	{
	}

	StageResult(StageStatus status, StageException exception)
		: Status(status), Exception(exception)
	{
	}
};

StageResult MakeStageResultV(StageStatus status, const char* function, const char* file, int line, const char* format, fmt::printf_args args);

template<typename... TArgs>
StageResult MakeStageResult(StageStatus status, const char* function, const char* file, int line, const char* format, const TArgs&... args)
{
	return MakeStageResultV(status, function, file, line, format, fmt::make_printf_args(args...));
}

#define GDN_STAGERESULT(type, message, ...) MakeStageResult(StageStatus::##type, __FUNCTION__, __FILE__, __LINE__, ##message, __VA_ARGS__)

class Scanner;
class Stage
{
public:
	Stage(Scanner* scanner, std::string_view name);
	virtual ~Stage() = default;

	virtual StageResult Run() = 0;

	std::string_view GetName() const { return m_Name; }

protected:
	Scanner* m_Scanner;
	std::string_view m_Name;
};
