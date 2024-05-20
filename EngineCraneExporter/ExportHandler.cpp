#include "pch.h"
#include "ExportHandler.h"

#include "constants.h"
#include "TomlBuilder.h"
#include "UIParameters.h"
#include "utils.h"

#define GET_UI_STRING(UIEnum) m_UiData->GetStringData(ui::getUIIndex(UIEnum))->Value
#define GET_UI_BOOL(UIEnum) m_UiData->GetBoolData(ui::getUIIndex(UIEnum))->Value

namespace {
	struct ScopedOutputStream {
		explicit ScopedOutputStream(const std::wstring& filepath) : stream()
		{
			auto attributes = GetFileAttributes(filepath.c_str());
			auto file_exists = (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY));
			auto open_options = file_exists ? (std::ios::out | std::ios::trunc) : std::ios::app;
			stream.open(filepath, open_options);
		}
		~ScopedOutputStream() { stream.flush(); stream.close(); }
		std::ofstream stream;
	};

	struct ScopedInputStream {
		explicit ScopedInputStream(const std::wstring& filepath) : stream(filepath, std::ios::binary) {}
		~ScopedInputStream() { stream.close(); }
		std::ifstream stream;
	};

	class ScopedResource 
	{
	public:
		explicit ScopedResource(unsigned int resourceId) : 
			handle(), 
			data_ptr(nullptr), 
			locked(false)
		{
			module = GetModuleHandle(PROJECT_FILENAME);
			resourceInfo = FindResource(module, MAKEINTRESOURCE(IDR_EXPORTER_LUA), TEXT("BINARY"));
			if (!resourceInfo) throw std::runtime_error("Resource with id " + std::to_string(resourceId) + " not found");
		}

		~ScopedResource() {
			if (locked) {
				UnlockResource(handle);
				data_ptr = nullptr;
			}
		}

		size_t size()
		{
			return SizeofResource(module, resourceInfo);
		}

		const char* const data()
		{
			if (!locked) {
				handle = LoadResource(module, resourceInfo);
				data_ptr = (const char* const)LockResource(handle);
				locked = true;
			}
			return data_ptr;
		}
		
	private:
		HGLOBAL handle;
		HMODULE module;
		HRSRC resourceInfo;
		const char* data_ptr;
		bool locked;
	};

	bool isDirectory(const std::wstring& path)
	{
		DWORD att = GetFileAttributes(path.c_str());
		if (att == INVALID_FILE_ATTRIBUTES || !(att & FILE_ATTRIBUTE_DIRECTORY)) 
			return false;
		return true;
	}

	std::vector<std::string> splitKey(std::string fullKey, const char delimiter)
	{
		std::vector<std::string> keyParts;
		std::stringstream ss(fullKey);
		std::string current_segment;
		while (std::getline(ss, current_segment, delimiter)) {
			keyParts.emplace_back(current_segment);
		}
		return keyParts;
	}
}

const wchar_t* ExportHandler::NAME = L"EngineCrane Exporter";
ExportHandler* ExportHandler::s_Instance = nullptr;

ExportHandler* const ExportHandler::Instance() { return s_Instance; }
void ExportHandler::CreateInstance() { if (!s_Instance) s_Instance = new ExportHandler(); }
void ExportHandler::DestroyInstance() { delete s_Instance; s_Instance = nullptr; }

ExportHandler::~ExportHandler() = default;

ExportHandler::ExportHandler() : 
	m_IsExportInProcess(false),
	m_UiData(nullptr),
	m_ExportDirectory(), 
	m_ExporterScript(),
	m_exporter_p(nullptr),
	m_LuaDataFiles()
{}

AuCarExpErrorCode ExportHandler::Init(const AuCarExpCarData* exportUiData)
{
	m_exporter_p = std::make_unique<Exporter>();
	try {
		m_exporter_p->init(0);
		std::cout << "Loaded " << EXPORTER_DLL_NAME << ": " << std::hex << m_exporter_p << std::endl;
	} 
	catch (const FailedToLoadLibrary& e) {
		MessageBox(nullptr, reinterpret_cast<const wchar_t*>(e.what()), TEXT("Exporter load error"), MB_OK);
		return AuCarExpErrorCode_CouldNotObtainOutputPathFatal;
	}
	
	m_UiData = exportUiData;
	auto exportDirRes = setupExportDirectory();
	if (exportDirRes) return exportDirRes;
	return setupExporterScript();
}

AuCarExpErrorCode ExportHandler::setupExportDirectory()
{
	PWSTR path = NULL;
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, 0, &path);
	if (FAILED(hr))
		return AuCarExpErrorCode_CouldNotObtainOutputPathFatal;

	m_ExportDirectory = path;
	m_ExportDirectory += EngineCraneAppDataExportPath;
	if (!isDirectory(m_ExportDirectory)) {
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
	}

	if (!isDirectory(m_ExportDirectory)) {
		std::wstring error = L"Could not create directory: ";
		error += m_ExportDirectory;
		MessageBox(nullptr, error.c_str(), TEXT("Error creating directory"), MB_OK);
		return AuCarExpErrorCode_CouldNotObtainOutputPathFatal;
	}
	return AuCarExpErrorCode_Success;
}

AuCarExpErrorCode ExportHandler::setupExporterScript()
{
	// If there is a custom script dir provided then use that else use the bundled resource
	std::wstring scriptPath = GET_UI_STRING(ui::StringElement::ExporterScriptPath);
	if (!scriptPath.empty()) {
		ScopedInputStream s(scriptPath);
		if (s.stream.is_open()) {
			m_ExporterScript.assign(std::istreambuf_iterator<char>(s.stream), {});
		}
	}
	if (m_ExporterScript.empty()) {
		try {
			ScopedResource exporterScriptRes(IDR_EXPORTER_LUA);
			m_ExporterScript = exporterScriptRes.data();
		}
		catch (const std::runtime_error& e) {
			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
			std::wstring error = L"Could not load exporter script resource: ";
			error += converter.from_bytes(e.what());
			MessageBox(nullptr, error.c_str(), TEXT("Error loading resource"), MB_OK);
			return AuCarExpErrorCode_CouldNotObtainOutputPathFatal;
		}
	}
	return AuCarExpErrorCode_Success;
}

void ExportHandler::EndExport()
{
	m_IsExportInProcess = true;
	bool dump_json = GET_UI_BOOL(ui::BoolElement::DumpJson);
	if (dump_json) {
		std::string path = m_utf8Converter.to_bytes(m_ExportDirectory);
		m_exporter_p->dump_json(path.c_str());
	}
	m_IsExportInProcess = false;
}

void ExportHandler::AddLuaFloatData(const AuCarExpArray<AuCarExpLuaFloatData>& Data)
{
	for (unsigned int i = 0; i < Data.GetCount(); i++) {
		std::string luaKey = m_utf8Converter.to_bytes(std::wstring(Data[i].ValueName));
		auto keyParts = splitKey(luaKey, '.');
		if (keyParts.size() < 2)
			continue;

		if (keyParts[0] == "Curve") {
			if (keyParts.size() < 3)
				continue;
			const std::string& curveName = keyParts[1];
			size_t index = std::stoi(keyParts[2]);
			m_exporter_p->add_curve_data(curveName.c_str(), index, Data[i].Value);
		}
		else {
			m_exporter_p->add_float(keyParts[0].c_str(), keyParts[1].c_str(), Data[i].Value);
		}
	}
}

void ExportHandler::AddLuaStringData(const AuCarExpArray<AuCarExpLuaStringData>& Data)
{
	for (unsigned int i = 0; i < Data.GetCount(); i++) {
		std::string luaKey = m_utf8Converter.to_bytes(std::wstring(Data[i].ValueName));
		auto keyParts = splitKey(luaKey, '.');
		if (keyParts.size() < 2)
			continue;

		m_exporter_p->add_string(keyParts[0].c_str(),
			keyParts[1].c_str(),
			m_utf8Converter.to_bytes(std::wstring(Data[i].Buffer)).c_str());
	}
}

void ExportHandler::AddLuaFiles(const AuCarExpArray<AuCarLuaDataFile>& data)
{
}
