#pragma once
#include "pch.h"

class FailedToLoadLibrary : public std::exception {
public:
    FailedToLoadLibrary(std::string library_name, unsigned long error_code);
    virtual ~FailedToLoadLibrary();

    const char* what() const noexcept override {
        return reinterpret_cast<const char*>(error_message.c_str());
    }

private:
    std::wstring error_message;
};

class FailedToFindFunction : public std::exception {
public:
    FailedToFindFunction(const std::string& func_name,
                         const std::string& library_name,
                         unsigned long error_code);
    virtual ~FailedToFindFunction();

private:
    std::wstring error_message;
};
