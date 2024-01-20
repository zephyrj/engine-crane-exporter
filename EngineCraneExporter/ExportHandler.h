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
	void ExportInternal();

	void AddLuaFloatData(const AuCarExpArray<AuCarExpLuaFloatData>& Data);
	void AddLuaStringData(const AuCarExpArray<AuCarExpLuaStringData>& Data);
	void AddLuaFiles(const AuCarExpArray<AuCarLuaDataFile>& data);

	const wchar_t* GetExportDirectory() const { return m_ExportDirectory.c_str(); }
	const wchar_t* GetCarName() const { return m_ExportUiData->GetCarName(); }
	bool IsExportInProcess() const { return m_IsExportInProcess; }

protected:
	static ExportHandler* s_Instance;

	ExportHandler();

	AuCarExpErrorCode setExportDirectory();

	bool m_IsExportInProcess;
	const AuCarExpCarData* m_ExportUiData;
	std::wstring m_ExportDirectory;

	std::map<std::wstring, float> m_LuaFloatData;
	std::map<std::wstring, std::wstring> m_LuaStringData;
	std::map<std::wstring, AuCarLuaDataFile> m_LuaDataFiles;
};
