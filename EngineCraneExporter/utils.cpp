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

bool isDirectory(const std::wstring& path)
{
	DWORD att = GetFileAttributes(path.c_str());
	if (att == INVALID_FILE_ATTRIBUTES || !(att & FILE_ATTRIBUTE_DIRECTORY))
		return false;
	return true;
}

void createDirectoryTree(std::wstring dir_path)
{
	if (!isDirectory(dir_path)) {
		//create directory, one level at a time:
		size_t slashPos = FindDirDelimiter(dir_path, 0);
		size_t offset = 0;
		while (slashPos != std::wstring::npos)
		{
			CreateDirectory(dir_path.substr(offset, slashPos - offset).c_str(), nullptr);
			slashPos = FindDirDelimiter(dir_path, slashPos + 1);
		}
		//last one:
		CreateDirectory(dir_path.c_str(), nullptr);
	}
}

bool tryWriteToDirectory(const std::wstring& path) 
{
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
