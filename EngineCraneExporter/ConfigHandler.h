#pragma once
#include "pch.h"
#include "exception.h"
#include <toml11/toml.hpp>

class ConfigHandler;

namespace config {
	class LoadFailed : public WideStringException {
	public:
		LoadFailed(const std::string& message);
		virtual ~LoadFailed();
	};

	class StoreFailed : public WideStringException {
	public:
		StoreFailed(const std::string& message);
		virtual ~StoreFailed();
	};

	template <typename ConfigCallback>
	bool load_config_and_call(ConfigCallback callback)
	{
		ConfigHandler configHandler;
		try {
			configHandler.load();
		}
		catch (const config::LoadFailed& e) {
			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
			std::wstring error = L"Could not load prefs: ";
			error += converter.from_bytes(e.what());
			MessageBox(nullptr, error.c_str(), TEXT("Failed to load config"), MB_OK);
			return false;
		}
		return callback(configHandler);
	}
}

class ConfigHandler
{
public:
	static constexpr const char* CONFIG_FILE_PATH = "engine-crane-export-conf.toml";
	ConfigHandler();
	virtual ~ConfigHandler();

	void load();
	void store();

	std::optional<std::wstring> exportDirectory() const;
	void exportDirectory(std::optional<std::wstring> dir);

	std::optional<std::wstring> exporterScriptPath() const;
	void exporterScriptPath(std::optional<std::wstring> dir);


private:
	toml::value m_config;
};

