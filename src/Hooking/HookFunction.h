#pragma once

class HookFunction
{
public:
	HookFunction(std::function<void()>&& function);

	static void RunAll();

private:
	void operator ()();

private:
	std::function<void()> m_Function;
	HookFunction* m_Next;
};
