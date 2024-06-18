#include "pch.h"
#include "plugin_helper.h"

#define DEFINE_EXPORTER_FUNCTION(id, func_name, func_ptr_type) \
 struct id {                                            \
   static constexpr const char* Name = func_name;       \
   using PtrType = func_ptr_type;                       \
 };

namespace {
	using ExporterHandle = void*;
	template<typename Func> struct FuncLookupHelper {};
    DEFINE_EXPORTER_FUNCTION(GetLastExporterError, "get_last_error", const char* (*)())
	DEFINE_EXPORTER_FUNCTION(ExporterInit, "init", ExporterHandle(*)(uint32_t))
    DEFINE_EXPORTER_FUNCTION(ExporterDestroy, "destroy", void (*)(ExporterHandle))
    DEFINE_EXPORTER_FUNCTION(ExporterAddString, "add_string", void (*)(ExporterHandle, const char*, const char*, const char*))
    DEFINE_EXPORTER_FUNCTION(ExporterAddFloat, "add_float", void (*)(ExporterHandle, const char*, const char*, float))
    DEFINE_EXPORTER_FUNCTION(ExporterAddCurveData, "add_curve_data", void (*)(ExporterHandle, const char*, unsigned long, float))
    DEFINE_EXPORTER_FUNCTION(ExporterFinalise, "finalise", bool (*)(ExporterHandle, const char*))
    DEFINE_EXPORTER_FUNCTION(ExporterDumpJson, "dump_json", bool (*)(ExporterHandle, const char*))
}

Exporter::Exporter() :
    m_hDll(nullptr), 
    m_exporterHandle_p(nullptr)
{}

Exporter::~Exporter()
{
    if (m_hDll) {
        destroy();
        FreeLibrary(m_hDll);
    }
}

const char* Exporter::get_last_error()
{
    return call_exporter_func<GetLastExporterError>();
}

void Exporter::init(uint32_t script_version)
{
    m_hDll = LoadLibraryA(EXPORTER_DLL_NAME);
    if (m_hDll == nullptr) {
        throw FailedToLoadLibrary(EXPORTER_DLL_NAME, GetLastError());
    }
    std::cout << "Loaded " << EXPORTER_DLL_NAME << ": " << std::hex << m_hDll << std::endl;
    m_exporterHandle_p = call_exporter_func<ExporterInit>(script_version);
    std::cout << "Initialised exporter handle " << m_exporterHandle_p << std::endl;
}

void Exporter::destroy()
{
    if (m_exporterHandle_p) {
        call_exporter_func<ExporterDestroy>(m_exporterHandle_p);
        m_exporterHandle_p = nullptr;
    }
}

void Exporter::add_string(const char* group, const char* key, const char* value)
{
    if (m_exporterHandle_p) {
        call_exporter_func<ExporterAddString>(m_exporterHandle_p, group, key, value);
    }
}

void Exporter::add_float(const char* group, const char* key, float value)
{
    if (m_exporterHandle_p) {
        call_exporter_func<ExporterAddFloat>(m_exporterHandle_p, group, key, value);
    }
}

void Exporter::add_curve_data(const char* curve_name, unsigned long index, float value)
{
    if (m_exporterHandle_p) {
        call_exporter_func<ExporterAddCurveData>(m_exporterHandle_p, curve_name, index, value);
    }
}

bool Exporter::dump_json(const char* output_path)
{
    auto res = false;
    if (m_exporterHandle_p) {
        call_exporter_func<ExporterDumpJson>(m_exporterHandle_p, output_path);
    }
    return res;
}

void* Exporter::get_exporter_dll_address() const
{
    return m_hDll;
}

void* Exporter::get_exporter_handle_address() const
{
    return m_exporterHandle_p;
}

bool Exporter::finalise(const char* output_path)
{
    auto res = false;
    if (m_exporterHandle_p) {
        res = call_exporter_func<ExporterFinalise>(m_exporterHandle_p, output_path);
        m_exporterHandle_p = nullptr;
    }
    return res;
}
