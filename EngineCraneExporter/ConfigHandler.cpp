#include "pch.h"
#include "ConfigHandler.h"

namespace {
    constexpr const char* EXPORT_DIR_KEY = "export-directory";
    constexpr const char* EXPORTER_SCRIPT_PATH_KEY = "exporter-script-path";

    std::optional<std::wstring> get_string_config_parameter(const toml::value& toml_val, const char* key)
    {
        try {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            return converter.from_bytes(toml::find<std::string>(toml_val, key));
        }
        catch (const std::out_of_range&) {
            return std::nullopt;
        }
        catch (const std::exception) {
            // TODO don't be rude and crash everything but
            // work out how to log this unexpected error somewhere
            return std::nullopt;
        }
    }

    void set_string_config_parameter(toml::value& toml_val,
                                     const char* key,
                                     std::optional<std::wstring> opt)
    {
        if (opt.has_value()) {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            toml_val[key] = converter.to_bytes(opt.value());
        }
        else if (toml_val.contains(key)) {
            auto& t = toml_val.as_table();
            t.erase(key);
        }
    }
}

namespace config {
    LoadFailed::LoadFailed(const std::string& message)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring m;
        m += L"Failed to load config. " + converter.from_bytes(message) + L". ";
        set_error_message(m);
    }

    StoreFailed::StoreFailed(const std::string& message)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring m;
        m += L"Failed to store config. " + converter.from_bytes(message) + L". ";
        set_error_message(m);
    }

    LoadFailed::~LoadFailed() = default;
    StoreFailed::~StoreFailed() = default;
}
ConfigHandler::ConfigHandler() = default;
ConfigHandler::~ConfigHandler() = default;

void ConfigHandler::load()
{
    if (!std::filesystem::exists(CONFIG_FILE_PATH)) {
        return;
    }
    try {
        toml::value config = toml::parse(CONFIG_FILE_PATH);
        if (!config.is_table()) {
            throw config::LoadFailed(std::string("Unexpected TOML format: Expected a table structure"));
        }
        m_config = config;
    }
    catch (const toml::syntax_error& err) {
        throw config::LoadFailed(std::string("Syntax error in TOML file: ") + err.what());
    }
    catch (const std::exception& err) {
        throw config::LoadFailed(std::string("Failed to load TOML file: ") + err.what());
    }
}

void ConfigHandler::store()
{
    std::ofstream ofs(CONFIG_FILE_PATH);
    if (ofs.is_open()) {
        ofs << toml::format(m_config);
        ofs.close();
    }
    else {
        throw config::StoreFailed("Error: Could not open " + std::string(EXPORT_DIR_KEY) + " for writing.");
    }
}

std::optional<std::wstring> ConfigHandler::exportDirectory() const
{
    return ::get_string_config_parameter(m_config, EXPORT_DIR_KEY);
}

void ConfigHandler::exportDirectory(std::optional<std::wstring> dir_opt)
{
    ::set_string_config_parameter(m_config, EXPORT_DIR_KEY, dir_opt);
}

std::optional<std::wstring> ConfigHandler::exporterScriptPath() const
{
    return ::get_string_config_parameter(m_config, EXPORTER_SCRIPT_PATH_KEY);
}

void ConfigHandler::exporterScriptPath(std::optional<std::wstring> path_opt)
{
    ::set_string_config_parameter(m_config, EXPORTER_SCRIPT_PATH_KEY, path_opt);
}
