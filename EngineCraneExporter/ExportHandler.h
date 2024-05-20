#pragma once

#include "AuCarExport.h"
#include "plugin_helper.h"

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

	bool m_IsExportInProcess;
	const AuCarExpCarData* m_UiData;
	std::wstring m_ExportDirectory;
	std::string m_ExporterScript;

	std::wstring_convert<std::codecvt_utf8<wchar_t>> m_utf8Converter;
	std::unique_ptr<Exporter> m_exporter_p;

	std::map<std::wstring, AuCarLuaDataFile> m_LuaDataFiles;
};
