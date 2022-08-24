#include "pch.h"

#include "CommandLine.h"

CommandLine::CommandLine(int argc, char* argv[])
{
	m_Args.reserve(argc);

	for (int i = 0; i < argc; i++)
		m_Args.push_back(argv[i]);
}

CommandLine::CommandLine(int argc, wchar_t* argv[])
{
	m_Args.reserve(argc);

	for (int i = 0; i < argc; i++)
		m_Args.push_back(ToNarrow(argv[i]));
}

bool CommandLine::Contains(std::string_view arg)
{
	return std::find(m_Args.begin(), m_Args.end(), arg) != m_Args.end();
}
