#pragma once
#include "pch.h"
#include "exception.h"

class FailedToLoadLibrary : public WideStringException {
public:
    FailedToLoadLibrary(std::string library_name, unsigned long error_code);
    virtual ~FailedToLoadLibrary();
};

class FailedToFindFunction : public WideStringException {
public:
    FailedToFindFunction(const std::string& func_name,
                         const std::string& library_name,
                         unsigned long error_code);
    virtual ~FailedToFindFunction();
};
