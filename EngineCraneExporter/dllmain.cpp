// Code based off of the provided Exporter template projects written by Camshaft Software
// https://github.com/EndfinityDev/csvExporter
// https://github.com/AutomationStaff/ExporterSDK

// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "AuCarExportDLL.h"
#include "UIParameters.h"
#include "ExportHandler.h"

//Gets the plugin name, to be displayed in the drop-down list of available plugins
AuCarExpErrorCode AuCarExportDLL::GetExporterName(AuCarExpArray<wchar_t>& name, wchar_t const* locale)
{
	wcscpy_s(name.GetData(), name.GetCount(), ExportHandler::NAME);
	return AuCarExpErrorCode_Success;
}

//Gets the plugin version number
//This is for display in the UI only (to help when users report bugs, etc.). Automation does nothing else with this information.
AuCarExpErrorCode AuCarExportDLL::GetExporterVersion(unsigned int* VersionNumber)
{
	*VersionNumber = ExportHandler::VERSION;
	return AuCarExpErrorCode_Success;
}

//Gets the number of user-supplied strings that the plugin will be requesting
AuCarExpErrorCode AuCarExportDLL::GetRequiredStringDataCount(unsigned int* retCount)
{
	*retCount = ui::NUM_STRINGS;
	return AuCarExpErrorCode_Success;
}

//Gets the user-supplied string information
AuCarExpErrorCode AuCarExportDLL::GetRequiredStringData(AuCarExpArray<AuCarExpUIStringData>& stringData, wchar_t const* locale)
{
	return ui::SetupStringData(stringData);
}

//Gets the number of user-supplied booleans that the plugin will be requesting
AuCarExpErrorCode AuCarExportDLL::GetRequiredBoolDataCount(unsigned int* retCount)
{
	*retCount = ui::NUM_BOOLS;
	return AuCarExpErrorCode_Success;
}

//Gets the user-supplied boolean information
AuCarExpErrorCode AuCarExportDLL::GetRequiredBoolData(AuCarExpArray<AuCarExpUIBoolData>& boolData, wchar_t const* locale)
{
	return ui::SetupBoolData(boolData);
}

AuCarExpErrorCode AuCarExportDLL::BeginExport(const AuCarExpCarData* carData, AuCarExpArray<wchar_t>& retDir, unsigned int* retFlags)
{
	ExportHandler::CreateInstance();
	AuCarExpErrorCode error = ExportHandler::Instance()->Init(carData);
	wcscpy_s(retDir.GetData(), retDir.GetCount(), ExportHandler::Instance()->GetExportDirectory());
	*retFlags = AuCarExpExporterFlags_None;
	return error;
}

//Called after all data has been supplied to the export plugin, and all texture information has been finalised
AuCarExpErrorCode AuCarExportDLL::EndExport()
{
	ExportHandler::Instance()->EndExport();
	return AuCarExpErrorCode_Success;
}

//Called after EndExport(), to give the plugin the chance to finish any threaded operations that may still be running
AuCarExpErrorCode AuCarExportDLL::IsExportInProgress(bool* retInProgress)
{
	*retInProgress = ExportHandler::Instance()->IsExportInProcess();
	return AuCarExpErrorCode_Success;
}

//Called after the export has finished, and IsExportInProgress() has reported nothing in progress.
//There will be no further function calls for this export.
//All outstanding plugin-allocated memory should be cleaned up here
AuCarExpErrorCode AuCarExportDLL::FreeAllData()
{
	ExportHandler::DestroyInstance();
	return AuCarExpErrorCode_Success;
}

AuCarExpErrorCode AuCarExportDLL::AddLuaFiles(const AuCarExpArray<AuCarLuaDataFile>& Data)
{
	ExportHandler::Instance()->AddLuaFiles(Data);
	return AuCarExpErrorCode_Success;
}

AuCarExpErrorCode AuCarExportDLL::AddLuaFloatData(const AuCarExpArray<AuCarExpLuaFloatData>& Data)
{
	ExportHandler::Instance()->AddLuaFloatData(Data);
	return AuCarExpErrorCode_Success;
}

AuCarExpErrorCode AuCarExportDLL::AddLuaStringData(const AuCarExpArray<AuCarExpLuaStringData>& Data)
{
	ExportHandler::Instance()->AddLuaStringData(Data);
	return AuCarExpErrorCode_Success;
}

AuCarExpErrorCode AuCarExportDLL::GetLuaFileCount(unsigned int* fileCount)
{
	*fileCount = 1;
	return AuCarExpErrorCode_Success;
}

AuCarExpErrorCode AuCarExportDLL::GetLuaFileLength(unsigned int* retLength, unsigned int FileNum)
{
	*retLength = 0;
	return AuCarExpErrorCode_Success;
}

AuCarExpErrorCode AuCarExportDLL::GetLuaFile(AuCarExpArray<wchar_t>& stringBuffer, unsigned int FileNum)
{
	if (!stringBuffer.GetData())
	{
		return AuCarExpErrorCode_UnknownError;
	}
	return AuCarExpErrorCode_Success;
}

// Unused by this exporter
AuCarExpErrorCode AuCarExportDLL::AddBodyMesh(AuCarExpMesh* mesh) { return AuCarExpErrorCode_Success; }
AuCarExpErrorCode AuCarExportDLL::AddEngineMesh(const AuCarExpMesh* mesh, const wchar_t* name) { return AuCarExpErrorCode_Success; }
AuCarExpErrorCode AuCarExportDLL::AddEngineMeshInstance(const AuCarExpMeshInstance* meshInstance) { return AuCarExpErrorCode_Success; }
AuCarExpErrorCode AuCarExportDLL::AddExhaust(const AuCarExpArray<AuCarExpMesh*>& meshes, const AuCarExpArray<AuCarExpVector>& points) { return AuCarExpErrorCode_Success; }
AuCarExpErrorCode AuCarExportDLL::AddCameraPositions(const AuCarExpVector* driverCamPosition, const AuCarExpVector* bonnetCamPosition) { return AuCarExpErrorCode_Success; }
AuCarExpErrorCode AuCarExportDLL::AddWheelPair(const AuCarExpWheelData& wheelData, const bool isFront) { return AuCarExpErrorCode_Success; }
AuCarExpErrorCode AuCarExportDLL::AddChassis(const AuCarExpArray<AuCarExpMesh*>& meshes) { return AuCarExpErrorCode_Success; }
AuCarExpErrorCode AuCarExportDLL::SetStampTexture(AuCarExpTexture* stampTexture) { return AuCarExpErrorCode_Success; }
AuCarExpErrorCode AuCarExportDLL::AddPreviewImage(AuCarExpTexture* image) { return AuCarExpErrorCode_Success; }
AuCarExpErrorCode AuCarExportDLL::AddFixtureMeshes(const AuCarExpArray<AuCarExpMesh*>& meshes, bool isBreakable, const wchar_t* name, const bool isTowBar, const AuCarExpVector& towPosition)
{
	return AuCarExpErrorCode_Success;
}

AuCarExpErrorCode AuCarExportDLL::GetPreviewImageData(unsigned int* retTextureWidth, unsigned int* retTextureHeight, float* retFOV)
{
	*retTextureWidth = 500;//required image width in pixels
	*retTextureHeight = 281;//required image height in pixels
	*retFOV = 17.0f;//required field of view
	return AuCarExpErrorCode_Success;
}

//Get camera transform information required to generate a preview image
AuCarExpErrorCode AuCarExportDLL::GetPreviewTransformData(const AuCarExpVector* carMin, const AuCarExpVector* carMax, AuCarExpVector* retPosition, AuCarExpVector* retLookAt)
{
	float carLength = carMax->z - carMin->z;
	float carHeight = carMax->y - carMin->y;

	retLookAt->x = (carMin->x + carMax->x) * 0.0f;
	retLookAt->y = (carMin->y + carMax->y) * 0.5f;
	retLookAt->z = (carMin->z + carMax->z) * 0.5f;

	retLookAt->y -= 0.02f * carHeight;

	*retPosition = *retLookAt;
	retPosition->x -= 3.5f * carLength;

	return AuCarExpErrorCode_Success;
}

//_declspec(dllimport) void CheckExporterVersion();