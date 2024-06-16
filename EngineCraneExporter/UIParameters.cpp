#include "pch.h"
#include "UIParameters.h"

namespace ui {
	std::array<StringElementType, NUM_STRINGS> STRING_ELEMENTS = {
		StringElementType(
			L"Exporter Script Path",
			L"The exporter script will be found automatically - if you'd like to use a custom script you can enter the path here",
			L""),
		StringElementType(
			L"Export Directory",
			L"The directory to write the crate engines to",
			L""),
	};

	std::array<BoolElementType, NUM_BOOLS> BOOL_ELEMENTS = {
		BoolElementType(
			L"Dump JSON file",
			L"Dump all of the data collected by the exporter to a json file. Useful for debugging purposes",
			false)
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
		wcscpy_s(stringData.Label, label.c_str());
		wcscpy_s(stringData.ToolTip, tooltip.c_str());
		wcscpy_s(stringData.Value, default_value.c_str());
	}

	BoolElementType::BoolElementType() = default;
	BoolElementType::BoolElementType(const wchar_t* label, const wchar_t* tooltip, bool default_value)
		: label(label), tooltip(tooltip), default_value(default_value) {}

	void BoolElementType::update(BoolElementType::AUDataType& boolData) const
	{
		wcscpy_s(boolData.Label, label.c_str());
		wcscpy_s(boolData.ToolTip, tooltip.c_str());
		boolData.Value = default_value;
	}
}
