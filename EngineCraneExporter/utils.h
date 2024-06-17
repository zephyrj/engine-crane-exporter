#pragma once

void sanitizeFileName(std::wstring& in);
size_t FindDirDelimiter(std::wstring dir, size_t start);
bool isDirectory(const std::wstring& path);
void createDirectoryTree(std::wstring dir_path);
bool tryWriteToDirectory(const std::wstring& path);

void debugDialog(std::string title, std::string err_msg);
void debugDialog(std::wstring title, std::wstring err_msg);
void debugDialog(std::wstring title, std::wstring err_prefix, const std::exception& e);
