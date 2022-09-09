#pragma once

#ifdef GDN_WINDOWS
#include <winrt/Windows.UI.Notifications.h>

namespace winrt
{
	using namespace Windows::UI::Notifications;
}
#endif

class NotificationArea
{
public:
	NotificationArea();
	~NotificationArea();

	void ShowToast(std::string message);

private:
#ifdef GDN_WINDOWS
	bool m_Available;
	winrt::ToastNotificationHistory m_History;
#endif
};
