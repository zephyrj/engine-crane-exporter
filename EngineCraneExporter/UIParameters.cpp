#include "pch.h"
#include "UIParameters.h"

namespace ui {
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
