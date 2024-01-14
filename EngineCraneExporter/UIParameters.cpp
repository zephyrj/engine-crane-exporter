#include "pch.h"
#include "UIParameters.h"

namespace ui {
	// TODO: Try FamilyName/VariantName
	const std::array<std::pair<std::wstring, std::wstring>, NUM_STRINGS> STRING_DATA = {
		std::make_pair(L"File Name", L"[PlayerName] [PlatformName] [TrimName]")
	};
	const std::array<std::pair<std::wstring, bool>, NUM_BOOLS> BOOL_DATA;

	AuCarExpErrorCode SetupStringData(AuCarExpArray<AuCarExpUIStringData>& stringData)
	{
		if (stringData.GetCount() != STRING_DATA.size())
			return AuCarExpErrorCode_UnknownError;

		int idx = 0;
		for (auto& pair : STRING_DATA)
		{
			wcscpy_s(stringData[idx].Label, pair.first.c_str());
			wcscpy_s(stringData[idx].Value, pair.second.c_str());
			idx += 1;
		}
		return AuCarExpErrorCode_Success;
	}

	AuCarExpErrorCode SetupBoolData(AuCarExpArray<AuCarExpUIBoolData>& boolData)
	{
		if (boolData.GetCount() != BOOL_DATA.size())
			return AuCarExpErrorCode_UnknownError;

		int idx = 0;
		for (auto& pair : BOOL_DATA)
		{
			wcscpy_s(boolData[idx].Label, pair.first.c_str());
			boolData[0].Value = pair.second;
			idx += 1;
		}
		return AuCarExpErrorCode_Success;
	}
}
