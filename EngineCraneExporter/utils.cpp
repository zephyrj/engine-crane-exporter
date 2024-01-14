#include "utils.h"
#include "pch.h"

const std::wstring IllegalCharacters = L"\\/:?\"<>|";

void sanitizeFileName(std::wstring& in)
{
	for (std::wstring::iterator i = in.begin(); i < in.end(); i++)
	{
		if (IllegalCharacters.find(*i) != std::wstring::npos)
		{
			*i = L'_';
		}
	}
}

size_t FindDirDelimiter(std::wstring dir, size_t start)
{
	size_t slashPos = dir.find(L"\\", start);

	if (slashPos == std::wstring::npos)
	{
		return dir.find(L"/", start);
	}

	return slashPos;
}
