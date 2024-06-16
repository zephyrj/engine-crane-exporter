#pragma once

#include "AuCarExport.h"

namespace ui {
	// To add in a new element add a value here and add a corresponding enty into
	// STRING_ELEMENTS
	enum class StringElement : size_t {
		ExporterScriptPath = 0,
		ExportDirectory,
		Count
	};

	// To add in a new element add a value here and add a corresponding enty into
	// BOOL_ELEMENTS
	enum class BoolElement : size_t {
		DumpJson = 0,
		Count
	};

	constexpr size_t NUM_STRINGS = static_cast<size_t>(StringElement::Count);
	constexpr size_t NUM_BOOLS = static_cast<size_t>(BoolElement::Count);

	constexpr unsigned int getUIIndex(StringElement e) { return static_cast<unsigned int>(e); }
	constexpr unsigned int getUIIndex(BoolElement e) { return static_cast<unsigned int>(e); }

	template<unsigned int value, std::size_t max>
	constexpr void check_bounds() {
		static_assert(value < max, "Index is out of bounds");
	}

	template <StringElement e> constexpr void checkUIIndex() { check_bounds<getUIIndex(e), NUM_STRINGS>(); };
	template <BoolElement e> constexpr void checkUIIndex() { check_bounds<getUIIndex(e), NUM_BOOLS>(); };

	AuCarExpErrorCode SetupStringData(AuCarExpArray<AuCarExpUIStringData>& stringData);
	AuCarExpErrorCode SetupBoolData(AuCarExpArray<AuCarExpUIBoolData>& boolData);

	class StringElementType {
	public:
		using AUDataType = AuCarExpUIStringData;
		StringElementType();
		StringElementType(const wchar_t* label, const wchar_t* tooltip, const wchar_t* default_value);
		void update(AUDataType& data) const;
		std::wstring label;
		std::wstring tooltip;
		std::wstring default_value;
	};
	

	class BoolElementType {
	public:
		using AUDataType = AuCarExpUIBoolData;
		BoolElementType();
		BoolElementType(const wchar_t* label, const wchar_t* tooltip, bool default_value);
		void update(AUDataType& data) const;
		std::wstring label;
		std::wstring tooltip;
		bool default_value = false;
	};

	extern std::array<StringElementType, NUM_STRINGS> STRING_ELEMENTS;
	extern std::array<BoolElementType, NUM_BOOLS> BOOL_ELEMENTS;

	template<typename UiElementType, std::size_t N>
	AuCarExpErrorCode SetupUIData(std::array<UiElementType, N> elements,
		                          AuCarExpArray<typename UiElementType::AUDataType>& data)
	{
		if (data.GetCount() != elements.size())
			return AuCarExpErrorCode_UnknownError;

		int idx = 0;
		for (auto& element : elements)
		{
			element.update(data[idx]);
			idx += 1;
		}
		return AuCarExpErrorCode_Success;
	}

	template <StringElement idx>
	void set_default(std::wstring val)
	{
		checkUIIndex<idx>();
		STRING_ELEMENTS[getUIIndex(idx)].default_value = std::move(val);
	}

	template <BoolElement idx>
	void set_default(bool val)
	{
		checkUIIndex<idx>();
		BOOL_ELEMENTS[getUIIndex(idx)].default_value = val;
	}
}
