#pragma once

#include "UserInterface/Views/View.h"

class UserInterface;
class ScanningView : public View
{
public:
	ScanningView(UserInterface* userInterface);
	~ScanningView() = default;

	virtual void RenderImGui() override;
};
