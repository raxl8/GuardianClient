#include "pch.h"

#include "Scanner.h"

#include "Scanner/Stages/RecycleBinStage.h"

Scanner::Scanner()
	: m_ScanData{ 0, {} }
{
	m_Stages.push_back(MakeUnique<GDN_PLATFORMTYPE(RecycleBinStage)>(this));
}

bool Scanner::Run()
{
	for (auto& stage : m_Stages)
	{
		auto stageResult = stage->Run();
		switch (stageResult.Status)
		{
		case StageStatus::Success:
			break;
		case StageStatus::ErrorContinue:
		{
			m_ScanData.Exceptions.push_back(std::make_tuple(stage->GetName(), stageResult.Exception));
			break;
		}
		case StageStatus::Error:
		default:
			return false;
		}
	}

	return true;
}
