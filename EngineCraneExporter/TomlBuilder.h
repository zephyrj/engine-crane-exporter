#pragma once

#include "pch.h"
#include <toml11/toml.hpp>

class TomlBuilder
{
public:
	static constexpr char KEY_DELIMITER = '.';

	TomlBuilder();
	~TomlBuilder();

	void add(std::string luaKey, std::string luaVal);
	void add(std::string luaKey, float luaVal);

	toml::value build() const;

private:
	void addTableData(const std::string& tableName, std::string key, std::string value);
	void addTableData(const std::string& tableName, std::string key, float value);

	void addCurveData(const std::string& curveName, size_t index, float value);

	std::map<std::string, std::map<std::string, toml::value>> m_data;
	std::map<std::string, std::map<size_t, float>> m_curveData;
};
