#pragma once

class CommandLine
{
public:
	CommandLine(int argc, char* argv[]);
	CommandLine(int argc, wchar_t* argv[]);

	bool Contains(std::string_view arg);

	inline const std::vector<std::string>& GetArgs() const { return m_Args; }

private:
	std::vector<std::string> m_Args;
};
