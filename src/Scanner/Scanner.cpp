#include "pch.h"

#include "Scanner.h"

#include "Scanner/Stages/RecycleBinStage.h"

Scanner::Scanner()
	: m_Data{ 0, {} }, m_CurrentStage(0), m_Status(ScannerStatus::Idle)
{
	m_Stages.push_back(MakeUnique<RecycleBinStage>(this));
}

Scanner::~Scanner()
{
	if (m_ScanningThread.joinable())
		m_ScanningThread.join();
}

void Scanner::Start()
{
	m_Status = ScannerStatus::InProgress;
	m_ScanningThread = std::thread([this]()
	{
		for (auto& stage : m_Stages)
		{
			auto stageResult = stage->Run();
			m_CurrentStage++;
			switch (stageResult.Status)
			{
			case StageStatus::Success:
				break;
			case StageStatus::ErrorContinue:
			{
				m_Data.Exceptions.push_back(std::make_tuple(stage->GetName(), stageResult.Exception));
				break;
			}
			case StageStatus::Error:
			default:
				m_Status = ScannerStatus::FatalError;
				return;
			}
		}

		m_Status = ScannerStatus::Success;
	});
}
