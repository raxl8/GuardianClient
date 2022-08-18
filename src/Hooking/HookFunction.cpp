#include "pch.h"

#include "HookFunction.h"

static HookFunction* g_HookFunctions = nullptr;

HookFunction::HookFunction(std::function<void()>&& function)
	: m_Function(std::move(function)), m_Next(nullptr)
{
	!g_HookFunctions ? g_HookFunctions = this : g_HookFunctions->m_Next = this;
}

void HookFunction::RunAll()
{
	for (auto hookFunction = g_HookFunctions; hookFunction; hookFunction = hookFunction->m_Next) {
		(*hookFunction)();
	}
}

void HookFunction::operator()()
{
	m_Function();
}
