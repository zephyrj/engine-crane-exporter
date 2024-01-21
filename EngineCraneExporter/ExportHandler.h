#pragma once

#include "AuCarExport.h"

class ExportHandler
{
public:
	static constexpr unsigned int VERSION = 202401001;
	static const wchar_t* NAME;

	static ExportHandler* const Instance();
	static void CreateInstance();
	static void DestroyInstance();

	virtual ~ExportHandler();

	AuCarExpErrorCode Init(const AuCarExpCarData* carData);
	void EndExport();
	void writeDataFile();

	void AddLuaFloatData(const AuCarExpArray<AuCarExpLuaFloatData>& Data);
	void AddLuaStringData(const AuCarExpArray<AuCarExpLuaStringData>& Data);
	void AddLuaFiles(const AuCarExpArray<AuCarLuaDataFile>& data);

	const wchar_t* GetExportDirectory() const { return m_ExportDirectory.c_str(); }
	const wchar_t* GetCarName() const { return m_UiData->GetCarName(); }
	bool IsExportInProcess() const { return m_IsExportInProcess; }

	unsigned int GetExporterScriptLength() const { return m_ExporterScript.length(); }
	const std::string& GetExporterScript() const { return m_ExporterScript; }

protected:
	static ExportHandler* s_Instance;

	ExportHandler();

	AuCarExpErrorCode setupExportDirectory();
	AuCarExpErrorCode setupExporterScript();

	template<typename ValueType>
	ValueType getOrDefault(const std::map<std::wstring, ValueType>& map, const wchar_t* lookupKey, ValueType default_value)
	{
		auto it = map.find(lookupKey);
		auto val = (it == std::end(map)) ? default_value : it->second;
		return val;
	}

	bool m_IsExportInProcess;
	const AuCarExpCarData* m_UiData;
	std::wstring m_ExportDirectory;
	std::string m_ExporterScript;

	std::map<std::wstring, float> m_LuaFloatData;
	std::map<std::wstring, std::wstring> m_LuaStringData;
	std::map<std::wstring, AuCarLuaDataFile> m_LuaDataFiles;
};
