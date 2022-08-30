#pragma once

#include "UserInterface/Views/View.h"

class UserInterface;
class Scanner;
class ScanningView : public View
{
public:
	ScanningView(UserInterface* userInterface);
	~ScanningView() = default;

	virtual void RenderImGui() override;

private:
	SharedPtr<Scanner> m_Scanner;
	int m_StageCount;
	bool m_ProgressAnimating;
	float m_ProgressAnimTime;
	float m_PrevProgress, m_Progress, m_TargetProgress;
};
