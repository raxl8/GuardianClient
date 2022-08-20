#pragma once

#define GDN_CONCAT(x, y) x ## y

class CallOnDtor
{
	std::function<void()> m_Function;
public:
	template<typename TFunction, typename... Args>
	CallOnDtor(TFunction function, Args... args)
	{
		m_Function = [function = std::move(function), ...args = std::move(args)]()
		{
			std::invoke(function, args...);
		};
	}

	~CallOnDtor()
	{
		m_Function();
	}
};

std::string ToNarrow(const std::wstring& wide);
std::wstring ToWide(const std::string& narrow);
