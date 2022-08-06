#include "pch.h"

#include "Stage.h"

Stage::Stage(Scanner* scanner, std::string_view name)
	: m_Scanner(scanner), m_Name(name)
{
}

StageResult MakeStageResultV(StageStatus status, const char* function, const char* file, int line, const char* format, fmt::printf_args args)
{
	return StageResult{ status, { fmt::vsprintf(format, args), function, file, line } };
}
