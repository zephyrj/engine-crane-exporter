#pragma once

#include "pch.h"
#include "AuCarExport.h"

namespace ui {
	constexpr unsigned int NUM_STRINGS = 1;
	extern const std::array<std::pair<std::wstring, std::wstring>, NUM_STRINGS> STRING_DATA;

	constexpr unsigned int NUM_BOOLS = 0;
	extern const std::array<std::pair<std::wstring, bool>, NUM_BOOLS> BOOL_DATA;

	AuCarExpErrorCode SetupStringData(AuCarExpArray<AuCarExpUIStringData>& stringData);
	AuCarExpErrorCode SetupBoolData(AuCarExpArray<AuCarExpUIBoolData>& boolData);
}
