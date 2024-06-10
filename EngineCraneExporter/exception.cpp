#include "pch.h"
#include "exception.h"

WideStringException::WideStringException() = default;
WideStringException::~WideStringException() = default;

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
