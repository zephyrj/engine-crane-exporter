#include "pch.h"
#include "plugin_exception.h"

namespace {
    std::wstring getLastWinErrorText(DWORD error_code)
    {
        std::wstring return_string;
        LPWSTR error_message_buffer = nullptr;
        DWORD result = FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            error_code,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPWSTR>(&error_message_buffer),
            0,
            NULL
        );

        if (result != 0) {
            return_string = error_message_buffer;
            LocalFree(error_message_buffer);
        }
        else {
            return_string += L"Operation returned error code " + std::to_wstring(error_code);
        }
        return return_string;
    }
}

FailedToLoadLibrary::FailedToLoadLibrary(std::string library_name, unsigned long error_code)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    error_message += L"Failed to load " + converter.from_bytes(library_name) + L". ";
    error_message += ::getLastWinErrorText(error_code);
}

FailedToLoadLibrary::~FailedToLoadLibrary() = default;

FailedToFindFunction::FailedToFindFunction(const std::string& func_name,
                                           const std::string& library_name,
                                           unsigned long error_code)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    error_message += L"Failed to locate address of '" + converter.from_bytes(func_name) + L"' in " +
        converter.from_bytes(library_name) + L". ";
    error_message += ::getLastWinErrorText(error_code);
}

FailedToFindFunction::~FailedToFindFunction() = default;
