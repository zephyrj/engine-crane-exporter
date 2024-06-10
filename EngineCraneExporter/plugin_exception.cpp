#include "pch.h"
#include "plugin_exception.h"

FailedToLoadLibrary::FailedToLoadLibrary(std::string library_name, unsigned long error_code)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring message;
    message += L"Failed to load " + converter.from_bytes(library_name) + L". ";
    message += ::getLastWinErrorText(error_code);
    set_error_message(message);
}

FailedToLoadLibrary::~FailedToLoadLibrary() = default;

FailedToFindFunction::FailedToFindFunction(const std::string& func_name,
                                           const std::string& library_name,
                                           unsigned long error_code)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring message;
    message += L"Failed to locate address of '" + converter.from_bytes(func_name) + L"' in " +
        converter.from_bytes(library_name) + L". ";
    message += ::getLastWinErrorText(error_code);
    set_error_message(message);
}

FailedToFindFunction::~FailedToFindFunction() = default;
