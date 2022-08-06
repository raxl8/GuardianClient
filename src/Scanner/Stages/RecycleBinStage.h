#pragma once

#include "Scanner/Stages/Stage.h"

class RecycleBinStage : public Stage
{
public:
	RecycleBinStage(Scanner* scanner)
		: Stage(scanner, "RecycleBinStage")
	{
	}

	~RecycleBinStage() = default;

	virtual StageResult Run() override;
};
