#pragma once

#include "schema.hpp"

#include <schemasystem/schemasystem.h>
#include <tier1/utlvector.h>

// Typed access to entity data by raw offset or by schema member name.
// Shared by the EntData / EntSchema exports.
namespace schema {
	//
	// Raw values
	//

	// Calls `fn.template operator()<S>()` with the storage type S matching `dataSize`:
	// int8_t/int16_t/int32_t/int64_t for integer T, float/double for floating-point T.
	template<typename T, typename F>
	bool DispatchSize(int dataSize, F&& fn) {
		if constexpr (std::is_floating_point_v<T>) {
			switch (dataSize) {
				case sizeof(float):
					fn.template operator()<float>();
					return true;
				case sizeof(double):
					fn.template operator()<double>();
					return true;
				default:
					break;
			}
		} else {
			switch (dataSize) {
				case sizeof(int8_t):
					fn.template operator()<int8_t>();
					return true;
				case sizeof(int16_t):
					fn.template operator()<int16_t>();
					return true;
				case sizeof(int32_t):
					fn.template operator()<int32_t>();
					return true;
				case sizeof(int64_t):
					fn.template operator()<int64_t>();
					return true;
				default:
					break;
			}
		}
		plg::print(LS_WARNING, "Invalid {} element size: {}\n", std::is_floating_point_v<T> ? "float" : "integer", dataSize);
		return false;
	}

	template<typename T>
	bool IsValidElementSize(int dataSize) {
		return DispatchSize<T>(dataSize, []<typename S>() {});
	}

	// Converts a stored element to T; `isUnsigned` zero-extends integers instead of sign-extending them.
	template<typename T, typename S>
	T FromStorage(S value, bool isUnsigned) {
		if constexpr (std::is_integral_v<S>) {
			if (isUnsigned) {
				return static_cast<T>(static_cast<std::make_unsigned_t<S>>(value));
			}
		}
		return static_cast<T>(value);
	}

	template<typename T>
	T ReadValue(const void* src, int dataSize, bool isUnsigned = false) {
		T result{};
		DispatchSize<T>(dataSize, [&]<typename S>() {
			result = FromStorage<T>(*static_cast<const S*>(src), isUnsigned);
		});
		return result;
	}

	template<typename T>
	void WriteValue(void* dst, int dataSize, T value) {
		DispatchSize<T>(dataSize, [&]<typename S>() {
			*static_cast<S*>(dst) = static_cast<S>(value);
		});
	}

	template<typename T>
	plg::vector<T> ReadValues(const void* src, size_t count, int dataSize, bool isUnsigned = false) {
		plg::vector<T> result;
		DispatchSize<T>(dataSize, [&]<typename S>() {
			const S* p = static_cast<const S*>(src);
			result.resize(count);
			std::transform(p, p + count, result.begin(), [isUnsigned](const S v) { return FromStorage<T>(v, isUnsigned); });
		});
		return result;
	}

	template<typename T>
	void WriteValues(void* dst, const T* src, size_t count, int dataSize) {
		DispatchSize<T>(dataSize, [&]<typename S>() {
			std::transform(src, src + count, static_cast<S*>(dst), [](const T v) { return static_cast<S>(v); });
		});
	}

	// Resizes a CUtlVector whose elements are `dataSize` bytes and returns its data pointer.
	void* ResizeUtlVector(intptr_t address, int count, int dataSize);

	//
	// Offset-based data
	//

	template<typename T>
	T& DataAt(CEntityInstance* entity, int offset) {
		return *reinterpret_cast<T*>(reinterpret_cast<intptr_t>(entity) + offset);
	}

	void NotifyDataChanged(CEntityInstance* entity, int offset, int chainOffset, bool changeState);

	template<typename T>
	void SetData(CEntityInstance* entity, int offset, const T& value, bool changeState, int chainOffset) {
		NotifyDataChanged(entity, offset, chainOffset, changeState);
		DataAt<T>(entity, offset) = value;
	}

	// Fixed-size array (T[N]) of integers or floats.
	template<typename T>
	plg::vector<T> GetDataCArray(CEntityInstance* entity, int offset, int arraySize, int dataSize) {
		if (arraySize <= 0) {
			return {};
		}
		return ReadValues<T>(&DataAt<uint8>(entity, offset), static_cast<size_t>(arraySize), dataSize);
	}

	template<typename T>
	void SetDataCArray(CEntityInstance* entity, int offset, const plg::vector<T>& value, int arraySize, int dataSize, bool changeState, int chainOffset) {
		if (arraySize <= 0 || !IsValidElementSize<T>(dataSize)) {
			return;
		}
		NotifyDataChanged(entity, offset, chainOffset, changeState);
		const size_t n = std::min(value.size(), static_cast<size_t>(arraySize));
		WriteValues<T>(&DataAt<uint8>(entity, offset), value.data(), n, dataSize);
	}

	// CUtlVector of integers or floats.
	template<typename T>
	plg::vector<T> GetDataVector(CEntityInstance* entity, int offset, int dataSize) {
		const auto& vec = DataAt<CUtlVector<uint8>>(entity, offset);
		if (vec.Count() <= 0) {
			return {};
		}
		return ReadValues<T>(vec.Base(), static_cast<size_t>(vec.Count()), dataSize);
	}

	template<typename T>
	void SetDataVector(CEntityInstance* entity, int offset, const plg::vector<T>& value, int dataSize, bool changeState, int chainOffset) {
		if (!IsValidElementSize<T>(dataSize)) {
			return;
		}
		NotifyDataChanged(entity, offset, chainOffset, changeState);
		const int count = static_cast<int>(value.size());
		void* data = ResizeUtlVector(reinterpret_cast<intptr_t>(entity) + offset, count, dataSize);
		if (data && count > 0) {
			WriteValues<T>(data, value.data(), value.size(), dataSize);
		}
	}

	//
	// Schema fields
	//

	// Location, length and stride of the elements of an array-like schema field.
	struct ArrayView {
		intptr_t data{};
		int count{};
		int stride{};
	};

	ArrayView GetArrayView(intptr_t base, CSchemaType* type, ElementType elementType);

	// A resolved schema member of a concrete entity.
	struct EntityField {
		CEntityInstance* entity{};
		std::string_view className;
		std::string_view memberName;
		SchemaKey key;

		explicit operator bool() const noexcept {
			return static_cast<bool>(key);
		}

		intptr_t Base() const {
			return reinterpret_cast<intptr_t>(entity) + key.offset;
		}

		void WarnType(std::string_view expected) const;

		// Address of `element` for arrays and collections, or of the field itself for single values.
		// Warns and returns nullptr on a type mismatch or when the element is out of range.
		void* Element(ElementType elementType, int element, std::string_view expected) const;

		// Element address plus its size in bytes, for fields classified by IsIntType/IsFloatType.
		std::pair<void*, int> SizedElement(ElementSizedType sized, int element, std::string_view expected) const;

		// Marks the field as changed for networking if `changeState` is set.
		void NotifyChanged(bool changeState) const;
	};

	EntityField FindField(CEntityInstance* entity, std::string_view className, std::string_view memberName);

	// Same as FindField, but also refuses members blocked by the "FollowCS2ServerGuidelines" option.
	EntityField FindWritableField(CEntityInstance* entity, std::string_view className, std::string_view memberName, std::string_view action = "set");

	// Integer (int64_t) or floating-point (double) schema values.
	template<typename T>
	ElementSizedType ClassifyNumeric(CSchemaType* type) {
		if constexpr (std::is_floating_point_v<T>) {
			return IsFloatType(type);
		} else {
			return IsIntType(type);
		}
	}

	template<typename T>
	constexpr std::string_view numericName = std::is_floating_point_v<T> ? "float" : "integer";

	template<typename T>
	T GetNumeric(CEntityInstance* entity, std::string_view className, std::string_view memberName, int element) {
		const auto field = FindField(entity, className, memberName);
		if (!field) {
			return {};
		}

		const auto sized = ClassifyNumeric<T>(field.key.type);
		if constexpr (std::is_integral_v<T>) {
			// Structures larger than 8 bytes are returned by address.
			if (sized.type == Class) {
				return static_cast<T>(field.Base());
			}
		}

		const auto [ptr, size] = field.SizedElement(sized, element, numericName<T>);
		return ptr ? ReadValue<T>(ptr, size, IsUnsignedElement(field.key.type)) : T{};
	}

	template<typename T>
	void SetNumeric(CEntityInstance* entity, std::string_view className, std::string_view memberName, T value, bool changeState, int element) {
		const auto field = FindWritableField(entity, className, memberName);
		if (!field) {
			return;
		}

		const auto [ptr, size] = field.SizedElement(ClassifyNumeric<T>(field.key.type), element, numericName<T>);
		if (!ptr || !IsValidElementSize<T>(size)) {
			return;
		}

		field.NotifyChanged(changeState);
		WriteValue<T>(ptr, size, value);
	}

	// Values stored as-is (vectors), matched by their size.
	template<typename T, size_t Size = sizeof(T)>
	T GetPlain(CEntityInstance* entity, std::string_view className, std::string_view memberName, int element, std::string_view expected) {
		const auto field = FindField(entity, className, memberName);
		if (!field) {
			return {};
		}

		const auto* ptr = static_cast<const T*>(field.Element(IsPlainType(field.key.type, Size), element, expected));
		return ptr ? *ptr : T{};
	}

	template<typename T, size_t Size = sizeof(T)>
	void SetPlain(CEntityInstance* entity, std::string_view className, std::string_view memberName, const T& value, bool changeState, int element, std::string_view expected) {
		const auto field = FindWritableField(entity, className, memberName);
		if (!field) {
			return;
		}

		auto* ptr = static_cast<T*>(field.Element(IsPlainType(field.key.type, Size), element, expected));
		if (!ptr) {
			return;
		}

		field.NotifyChanged(changeState);
		*ptr = value;
	}

	// Whole fixed arrays and collections of integers or floats.
	template<typename T>
	plg::vector<T> GetArray(CEntityInstance* entity, std::string_view className, std::string_view memberName) {
		const auto field = FindField(entity, className, memberName);
		if (!field) {
			return {};
		}

		const auto [elementType, elementSize] = ClassifyNumeric<T>(field.key.type);
		switch (elementType) {
			case Array:
			case Collection: {
				const auto view = GetArrayView(field.Base(), field.key.type, elementType);
				if (view.count <= 0) {
					return {};
				}
				return ReadValues<T>(reinterpret_cast<const void*>(view.data), static_cast<size_t>(view.count), elementSize, IsUnsignedElement(field.key.type));
			}
			case Single:
				plg::print(LS_WARNING, "Schema field '{}::{}' is not an array\n", className, memberName);
				return {};
			default:
				field.WarnType(std::is_floating_point_v<T> ? "float array" : "integer array");
				return {};
		}
	}

	template<typename T>
	void SetArray(CEntityInstance* entity, std::string_view className, std::string_view memberName, const plg::vector<T>& value, bool changeState) {
		const auto field = FindWritableField(entity, className, memberName);
		if (!field) {
			return;
		}

		const auto [elementType, elementSize] = ClassifyNumeric<T>(field.key.type);
		switch (elementType) {
			case Array: {
				if (!IsValidElementSize<T>(elementSize)) {
					return;
				}
				const int count = static_cast<const CSchemaType_FixedArray*>(field.key.type)->m_nElementCount;
				if (value.size() > static_cast<size_t>(count)) {
					plg::print(LS_WARNING, "Schema array '{}::{}' can hold {} elements, {} were given, extra elements are ignored\n", className, memberName, count, value.size());
				}
				field.NotifyChanged(changeState);
				const size_t n = std::min(value.size(), static_cast<size_t>(count));
				WriteValues<T>(reinterpret_cast<void*>(field.Base()), value.data(), n, elementSize);
				break;
			}
			case Collection: {
				if (!IsValidElementSize<T>(elementSize)) {
					return;
				}
				field.NotifyChanged(changeState);
				const int count = static_cast<int>(value.size());
				void* data = ResizeUtlVector(field.Base(), count, elementSize);
				if (data && count > 0) {
					WriteValues<T>(data, value.data(), value.size(), elementSize);
				}
				break;
			}
			case Single:
				plg::print(LS_WARNING, "Schema field '{}::{}' is not an array\n", className, memberName);
				break;
			default:
				field.WarnType(std::is_floating_point_v<T> ? "float array" : "integer array");
				break;
		}
	}
} // namespace schema
