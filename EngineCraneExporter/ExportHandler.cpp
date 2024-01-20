#include "pch.h"
#include "constants.h"
#include "ExportHandler.h"
#include "utils.h"

#include "UIParameters.h"

const wchar_t* ExportHandler::NAME = L"EngineCrane Exporter";
ExportHandler* ExportHandler::s_Instance = nullptr;

ExportHandler* const ExportHandler::Instance() { return s_Instance; }
void ExportHandler::CreateInstance() { if (!s_Instance) s_Instance = new ExportHandler(); }
void ExportHandler::DestroyInstance() { delete s_Instance; s_Instance = nullptr; }

ExportHandler::~ExportHandler() = default;

ExportHandler::ExportHandler() : 
	m_IsExportInProcess(false),
	m_ExportUiData(nullptr),
	m_ExportDirectory(), 
	m_LuaFloatData(),
	m_LuaStringData(),
	m_LuaDataFiles()
{}

AuCarExpErrorCode ExportHandler::Init(const AuCarExpCarData* exportUiData)
{
	m_ExportUiData = exportUiData;
	return setExportDirectory();
}

AuCarExpErrorCode ExportHandler::setExportDirectory()
{
	PWSTR path = NULL;
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, 0, &path);
	if (FAILED(hr))
		return AuCarExpErrorCode_CouldNotObtainOutputPathFatal;

	m_ExportDirectory = path;
	m_ExportDirectory += ENGINE_CRANE_APP_DATA_PATH;
	std::wstring exportFileName = m_ExportUiData->GetStringData(ui::getUIIndex(ui::StringElement::Filename))->Value;
	sanitizeFileName(exportFileName);
	m_ExportDirectory += exportFileName;

	//ensure target directory exists:
	DWORD att = GetFileAttributes(m_ExportDirectory.c_str());
	if (att == INVALID_FILE_ATTRIBUTES)
	{
		//create directory, one level at a time:
		size_t slashPos = FindDirDelimiter(m_ExportDirectory, 0);
		size_t offset = 0;

		while (slashPos != std::wstring::npos)
		{
			CreateDirectory(m_ExportDirectory.substr(offset, slashPos - offset).c_str(), nullptr);
			slashPos = FindDirDelimiter(m_ExportDirectory, slashPos + 1);
		}

		//last one:
		CreateDirectory(m_ExportDirectory.c_str(), nullptr);
		att = GetFileAttributes(m_ExportDirectory.c_str());
	}

	if (att != INVALID_FILE_ATTRIBUTES && att & FILE_ATTRIBUTE_DIRECTORY)
	{
		//directory exists, all good to go:
		return AuCarExpErrorCode_Success;
	}
	else
	{
		std::wstring error = L"Could not create directory: ";
		error += m_ExportDirectory;
		MessageBox(nullptr, error.c_str(), TEXT("Error creating directory"), MB_OK);
		return AuCarExpErrorCode_CouldNotObtainOutputPathFatal;
	}
}

void ExportHandler::EndExport()
{
	m_IsExportInProcess = true;
	ExportInternal();
	m_IsExportInProcess = false;
}

void ExportHandler::ExportInternal()
{
}

void ExportHandler::AddLuaFloatData(const AuCarExpArray<AuCarExpLuaFloatData>& Data)
{
	for (unsigned int i = 0; i < Data.GetCount(); i++)
	{
		m_LuaFloatData[Data[i].ValueName] = Data[i].Value;
	}
}

void ExportHandler::AddLuaStringData(const AuCarExpArray<AuCarExpLuaStringData>& Data)
{
	for (unsigned int i = 0; i < Data.GetCount(); i++)
	{
		m_LuaStringData[Data[i].ValueName] = Data[i].Buffer;
	}
}

void ExportHandler::AddLuaFiles(const AuCarExpArray<AuCarLuaDataFile>& data)
{
}
