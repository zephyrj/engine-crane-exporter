#include "pch.h"
#include "TomlBuilder.h"

namespace {
	std::vector<std::string> splitKey(std::string fullKey, const char delimiter)
	{
		std::vector<std::string> keyParts;
		std::stringstream ss(fullKey);
		std::string current_segment;
		while (std::getline(ss, current_segment, delimiter)) {
			keyParts.push_back(current_segment);
		}
		return keyParts;
	}
}

TomlBuilder::TomlBuilder() = default;
TomlBuilder::~TomlBuilder() = default;

void TomlBuilder::add(std::string luaKey, std::string luaVal)
{
	auto keyParts = splitKey(luaKey, '.');
	if (keyParts.size() < 2)
		return;

	addTableData(keyParts[0], keyParts[1], luaVal);
}

void TomlBuilder::add(std::string luaKey, float luaVal)
{
	auto keyParts = splitKey(luaKey, '.');
	if (keyParts.size() < 2)
		return;

	if (keyParts[0] == "Curve") {
		if (keyParts.size() < 3)
			return;
		const std::string& curveName = keyParts[1];
		size_t index = std::stoi(keyParts[2]);
		addCurveData(curveName, index, luaVal);
		return;
	}
	else {
		addTableData(keyParts[0], keyParts[1], luaVal);
	}
}

void TomlBuilder::addTableData(const std::string& tableName, std::string key, std::string value)
{
	if (!m_data.count(tableName)) {
		m_data.emplace(tableName, std::map<std::string, toml::value>{ { key, toml::value(value) } });
	}
	else {
		m_data[tableName].emplace(std::move(key), toml::value(std::move(value)));
	}
}

void TomlBuilder::addTableData(const std::string& tableName, std::string key, float value)
{
	if (!m_data.count(tableName)) {
		m_data.emplace(tableName, std::map<std::string, toml::value>{ { key, toml::value(value) } });
	}
	else {
		m_data[tableName].emplace(std::move(key), toml::value(value));
	}
}

void TomlBuilder::addCurveData(const std::string& curveName, size_t index, float value)
{
	if (!m_curveData.count(curveName)) {
		m_curveData.insert(std::make_pair(curveName, std::map<size_t, float>()));
	}
	m_curveData[curveName].emplace(index, value);
}

toml::value TomlBuilder::build() const
{
	toml::table out;
	for (const auto& table : m_data) {
		const auto& tableName = table.first;
		const auto& dataMap = table.second;
		toml::table t;
		for (const auto& tableData : dataMap) {
			const std::string& key = tableData.first;
			const toml::value& value = tableData.second;
			t[key] = value;
		}
		out[tableName] = std::move(t);
	}

	toml::table curves;
	for (const auto& curve : m_curveData) {
		const auto& curveName = curve.first;
		toml::array a;
		for (const auto& curveData : curve.second) {
			a.push_back(curveData.second);
		}
		curves[curveName] = std::move(a);
	}
	out["Curves"] = std::move(curves);
	return toml::value(out);
}
