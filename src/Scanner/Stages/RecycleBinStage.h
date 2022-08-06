#pragma once

#include "Scanner/Stages/Stage.h"

class RecycleBinStage : public Stage
{
public:
	RecycleBinStage(Scanner* scanner)
		: Stage(scanner, "RecycleBinStage")
	{
	}

	virtual ~RecycleBinStage() = default;
};

class RecycleBinStageWindows : public RecycleBinStage
{
public:
	RecycleBinStageWindows(Scanner* scanner);
	~RecycleBinStageWindows() = default;

	virtual StageResult Run() override;
};
