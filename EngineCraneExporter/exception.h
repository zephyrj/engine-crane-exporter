#pragma once
#include "pch.h"

class WideStringException : public std::exception {
public:
    WideStringException();
    virtual ~WideStringException();

    const char* what() const noexcept override {
        return reinterpret_cast<const char*>(error_message.c_str());
    }

protected:
    void set_error_message(std::wstring error_message) { error_message = std::move(error_message);  }

private:
    std::wstring error_message;
};

std::wstring getLastWinErrorText(DWORD error_code);
