#pragma once

#include <tuple>

#include "Scanner/Stages/Stage.h"

struct ScanData
{
	int64_t RecycleBinModifiedAt;
	std::vector<std::tuple<std::string_view, StageException>> Exceptions;
};

enum class ScannerStatus
{
	Idle = 0,
	InProgress,
	FatalError,
	Success
};

class Scanner
{
public:
	Scanner();
	~Scanner();

	void Start();

	int GetCurrentStage() { return m_CurrentStage; }
	int GetStageCount() { return static_cast<int>(m_Stages.size()); }
	ScanData& GetScanData() { return m_Data; }
	ScannerStatus GetStatus() { return m_Status; }

private:
	std::thread m_ScanningThread;
	int m_CurrentStage;
	std::vector<UniquePtr<Stage>> m_Stages;
	ScanData m_Data;
	ScannerStatus m_Status;
};
