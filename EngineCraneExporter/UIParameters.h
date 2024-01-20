#include "AuCarExport.h"

namespace ui {
	enum class StringElement : size_t {
		Filename = 0,
		ExporterScriptPath,
		Count
	};

	enum class BoolElement : size_t {
		Count = 0,
	};

	template <typename UiElement> unsigned int getUIIndex(UiElement e) = delete;
	template <> unsigned int getUIIndex<StringElement>(StringElement e) { return static_cast<unsigned int>(e); };
	template <> unsigned int getUIIndex<BoolElement>(BoolElement e) { return static_cast<unsigned int>(e); };

	constexpr size_t NUM_STRINGS = static_cast<size_t>(StringElement::Count);
	constexpr size_t NUM_BOOLS = static_cast<size_t>(BoolElement::Count);

	AuCarExpErrorCode SetupStringData(AuCarExpArray<AuCarExpUIStringData>& stringData);
	AuCarExpErrorCode SetupBoolData(AuCarExpArray<AuCarExpUIBoolData>& boolData);

	class StringElementType {
	public:
		using AUDataType = AuCarExpUIStringData;
		StringElementType();
		StringElementType(const wchar_t* label, const wchar_t* tooltip, const wchar_t* default_value);
		void update(AUDataType& data) const;
		const wchar_t* label = nullptr;
		const wchar_t* tooltip = nullptr;
		const wchar_t* default_value = nullptr;
	};

	class BoolElementType {
	public:
		using AUDataType = AuCarExpUIBoolData;
		BoolElementType();
		BoolElementType(const wchar_t* label, const wchar_t* tooltip, bool default_value);
		void update(AUDataType& data) const;
		const wchar_t* label = nullptr;
		const wchar_t* tooltip ;
		bool default_value = false;
	};

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
}
