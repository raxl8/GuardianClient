#pragma once

#include <functional>

template<typename... Args>
class Callback
{
public:
	bool operator ()(Args... args)
	{
		for (auto subscription : m_Subscriptions)
		{
			if (!std::invoke(subscription, args...))
				return false;
		}

		return true;
	}

	template<typename T>
	void Subscribe(T func)
	{
		if constexpr (std::is_same_v<std::invoke_result_t<T, Args...>, bool>)
		{
			m_Subscriptions.push_back(func);
			return;
		}

		m_Subscriptions.push_back([func](Args... args)
		{
			std::invoke(func, args...);
			return true;
		});
	}

private:
	std::vector<std::function<bool(Args...)>> m_Subscriptions;
};
