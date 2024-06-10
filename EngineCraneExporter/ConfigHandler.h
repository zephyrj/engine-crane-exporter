#pragma once
#include "pch.h"
#include "exception.h"
#include <toml11/toml.hpp>

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
}

class ConfigHandler
{
public:
	const char* CONFIG_FILE_PATH = "engine-crane-export-conf.toml";
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

