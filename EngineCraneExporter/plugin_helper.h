#pragma once

#include "pch.h"
#include "plugin_exception.h"

namespace {
    static constexpr const char* EXPORTER_DLL_NAME = "ccrate_engine_builder.dll";

    template<typename Function>
    auto lookup_exporter_func(HMODULE hDll)
    {
        auto functionName = Function::Name;
        FARPROC functionAddress = GetProcAddress(hDll, functionName);
        if (functionAddress == nullptr) {
            throw FailedToFindFunction(functionName, EXPORTER_DLL_NAME, GetLastError());
        }
        return reinterpret_cast<Function::PtrType>(functionAddress);
    }
}

class Exporter
{
public:
    Exporter();
    virtual ~Exporter();

    const char* get_last_error();

    void init(uint32_t script_version);
    bool finalise(const char* output_path);
    void destroy();

    void add_string(const char* group, const char* key, const char* value);
    void add_float(const char* group, const char* key, float value);
    void add_curve_data(const char* curve_name, unsigned long index, float value);

    bool dump_json(const char* output_path);

    void* get_exporter_dll_address() const;
    void* get_exporter_handle_address() const;

private:
    template<typename Function, typename... Args>
    auto call_exporter_func(Args&&... args)
    {
        auto iter = m_cachedFuncAddresses.find(Function::Name);
        if (iter == std::end(m_cachedFuncAddresses)) {
            m_cachedFuncAddresses.emplace(Function::Name, ::lookup_exporter_func<Function>(m_hDll));
            iter = m_cachedFuncAddresses.find(Function::Name);
        }
        return reinterpret_cast<Function::PtrType>(iter->second)(std::forward<Args>(args)...);
    }

    HMODULE m_hDll;
    void* m_exporterHandle_p;
    std::unordered_map<const char*, void*> m_cachedFuncAddresses;
};
