#include "pch.h"
#include "UIParameters.h"

namespace ui {
	// TODO: Try FamilyName/VariantName
	static const std::array<StringElementType, NUM_STRINGS> STRING_ELEMENTS = {
		StringElementType(L"Filename", L"", L"[PlayerName][PlatformName][TrimName]"),
		StringElementType(L"Exporter Script Path", L"", L""),
	};

	static const std::array<BoolElementType, NUM_BOOLS> BOOL_ELEMENTS = {
	};

	AuCarExpErrorCode SetupStringData(AuCarExpArray<AuCarExpUIStringData>& stringData)
	{
		return ui::SetupUIData(STRING_ELEMENTS, stringData);
	}

	AuCarExpErrorCode SetupBoolData(AuCarExpArray<AuCarExpUIBoolData>& boolData)
	{
		return ui::SetupUIData(BOOL_ELEMENTS, boolData);
	}

	StringElementType::StringElementType() = default;
	StringElementType::StringElementType(const wchar_t* label, const wchar_t* tooltip, const wchar_t* default_value)
		: label(label), tooltip(tooltip), default_value(default_value) {}
	void StringElementType::update(StringElementType::AUDataType& stringData) const
	{
		wcscpy_s(stringData.Label, label);
		wcscpy_s(stringData.ToolTip, tooltip);
		wcscpy_s(stringData.Value, default_value);
	}

	BoolElementType::BoolElementType() = default;
	BoolElementType::BoolElementType(const wchar_t* label, const wchar_t* tooltip, bool default_value)
		: label(label), tooltip(tooltip), default_value(default_value) {}
	void BoolElementType::update(BoolElementType::AUDataType& boolData) const
	{
		wcscpy_s(boolData.Label, label);
		wcscpy_s(boolData.ToolTip, tooltip);
		boolData.Value = default_value;
	}
}
