#pragma once

void sanitizeFileName(std::wstring& in);
size_t FindDirDelimiter(std::wstring dir, size_t start);
bool isDirectory(const std::wstring& path);
void createDirectoryTree(std::wstring dir_path);
bool tryWriteToDirectory(const std::wstring& path);
