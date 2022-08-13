#pragma once

#include <tuple>

#include "Scanner/Stages/Stage.h"

struct ScanData
{
	int64_t RecycleBinModifiedAt;
	std::vector<std::tuple<std::string_view, StageException>> Exceptions;
};

class Scanner
{
public:
	Scanner();
	~Scanner() = default;

	bool Run();

	ScanData& GetScanData() { return m_ScanData; }

private:
	ScanData m_ScanData;
	std::vector<UniquePtr<Stage>> m_Stages;
};
