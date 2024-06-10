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

bool tryWriteToDirectory(const std::wstring& path) {
	std::wstring test_file = path + L"/test_write_permissions.tmp";
	std::ofstream ofs(test_file);
	if (!ofs) {
		return false;
	}

	try {
		ofs.close();
		std::filesystem::remove(test_file);
		return true;
	}
	catch (const std::exception&) {
		return false;
	}
}
