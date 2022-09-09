#include "pch.h"

#include "NotificationArea.h"

#include "Core/Application.h"

#include <winrt/Windows.Data.Xml.Dom.h>

#define AppUserModelId L"Guardian.Guardian"

namespace winrt
{
	using namespace Windows::Data::Xml::Dom;
}

template<std::size_t N>
void SetRegistryKeyValue(HKEY key, const wchar_t* subKey, const wchar_t* valueName, const wchar_t(&value)[N])
{
	RegSetKeyValueW(
		key, subKey, valueName, REG_SZ,
		(LPCVOID)&value[0], static_cast<DWORD>(N * sizeof(WCHAR))
	);
}

NotificationArea::NotificationArea()
	: m_Available(IsMinimumWindows10()), m_History(nullptr)
{
	if (!m_Available)
		return;

	m_History = winrt::ToastNotificationManager::History();
	SetRegistryKeyValue(HKEY_CURRENT_USER, L"SOFTWARE\\Classes\\AppUserModelId\\" AppUserModelId, L"DisplayName", L"Guardian");
	SetRegistryKeyValue(HKEY_CURRENT_USER, L"SOFTWARE\\Classes\\AppUserModelId\\" AppUserModelId, L"IconBackgroundColor", L"FFFFFFDD");
}

NotificationArea::~NotificationArea()
{
	if (!m_Available)
		return;

	try
	{
		m_History.Clear(AppUserModelId);
	}
	catch (...)
	{
	}
	
	RegDeleteKeyW(HKEY_CURRENT_USER, L"SOFTWARE\\Classes\\AppUserModelId\\" AppUserModelId);
}

void NotificationArea::ShowToast(std::string message)
{
	if (!m_Available)
		return;

	winrt::XmlDocument document;
	document.LoadXml(L"<toast>\
    <visual>\
        <binding template=\"ToastGeneric\">\
            <text></text>\
        </binding>\
    </visual>\
</toast>");

	document.SelectSingleNode(L"//text[1]").InnerText(ToWide(message));

	winrt::ToastNotification notification(document);

	auto notificationManager = winrt::ToastNotificationManager::CreateToastNotifier(AppUserModelId);
	notificationManager.Show(notification);
}
