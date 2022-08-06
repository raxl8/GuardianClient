#include "pch.h"

#include "Utils.h"

#include <utf8.h>

std::string ToNarrow(const std::wstring& wide)
{
	std::vector<char> outVec;
	outVec.reserve(wide.size());

#ifdef GDN_WINDOWS
	utf8::unchecked::utf16to8(wide.begin(), wide.end(), std::back_inserter(outVec));
#else
	utf8::utf32to8(wide.begin(), wide.end(), std::back_inserter(outVec));
#endif

	return std::string(outVec.begin(), outVec.end());
}

std::wstring ToWide(const std::string& narrow)
{
	std::vector<char> cleanVec;
	cleanVec.reserve(narrow.size());

	std::vector<wchar_t> outVec;
	outVec.reserve(cleanVec.size());

	try
	{
		utf8::replace_invalid(narrow.begin(), narrow.end(), std::back_inserter(cleanVec));

#ifdef GDN_WINDOWS
		utf8::utf8to16(cleanVec.begin(), cleanVec.end(), std::back_inserter(outVec));
#else
		utf8::utf8to32(cleanVec.begin(), cleanVec.end(), std::back_inserter(outVec));
#endif
	}
	catch (utf8::exception&)
	{
	}

	return std::wstring(outVec.begin(), outVec.end());
}
