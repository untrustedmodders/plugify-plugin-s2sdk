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

	// Calls `fn.template operator()<S>()` with the C++ type S of `storage`.
	// Returns false (without calling `fn`) for Storage::Invalid.
	template<typename F>
	bool Dispatch(Storage storage, F&& fn) {
		switch (storage) {
			case Storage::I8:
				fn.template operator()<int8_t>();
				return true;
			case Storage::U8:
				fn.template operator()<uint8_t>();
				return true;
			case Storage::I16:
				fn.template operator()<int16_t>();
				return true;
			case Storage::U16:
				fn.template operator()<uint16_t>();
				return true;
			case Storage::I32:
				fn.template operator()<int32_t>();
				return true;
			case Storage::U32:
				fn.template operator()<uint32_t>();
				return true;
			case Storage::I64:
				fn.template operator()<int64_t>();
				return true;
			case Storage::U64:
				fn.template operator()<uint64_t>();
				return true;
			case Storage::F32:
				fn.template operator()<float>();
				return true;
			case Storage::F64:
				fn.template operator()<double>();
				return true;
			default:
				return false;
		}
	}

	// Storage for raw offset access, where only the size is known: signed integers for integer T,
	// floats for floating-point T. Warns and returns Storage::Invalid on an unsupported size.
	template<typename T>
	Storage StorageFor(int dataSize) {
		const Storage storage = std::is_floating_point_v<T> ? FloatStorage(dataSize) : IntStorage(dataSize);
		if (storage == Storage::Invalid) {
			plg::print(LS_WARNING, "Invalid {} element size: {}\n", std::is_floating_point_v<T> ? "float" : "integer", dataSize);
		}
		return storage;
	}

	template<typename T>
	T ReadValue(const void* src, Storage storage) {
		T result{};
		Dispatch(storage, [&]<typename S>() {
			result = static_cast<T>(*static_cast<const S*>(src));
		});
		return result;
	}

	template<typename T>
	T ReadValue(const void* src, int dataSize) {
		return ReadValue<T>(src, StorageFor<T>(dataSize));
	}

	template<typename T>
	void WriteValue(void* dst, Storage storage, T value) {
		Dispatch(storage, [&]<typename S>() {
			*static_cast<S*>(dst) = static_cast<S>(value);
		});
	}

	template<typename T>
	void WriteValue(void* dst, int dataSize, T value) {
		WriteValue<T>(dst, StorageFor<T>(dataSize), value);
	}

	template<typename T>
	plg::vector<T> ReadValues(const void* src, size_t count, Storage storage) {
		plg::vector<T> result;
		Dispatch(storage, [&]<typename S>() {
			const S* p = static_cast<const S*>(src);
			result.resize(count);
			std::transform(p, p + count, result.begin(), [](const S v) { return static_cast<T>(v); });
		});
		return result;
	}

	template<typename T>
	void WriteValues(void* dst, const T* src, size_t count, Storage storage) {
		Dispatch(storage, [&]<typename S>() {
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
		const Storage storage = StorageFor<T>(dataSize);
		if (arraySize <= 0 || storage == Storage::Invalid) {
			return {};
		}
		return ReadValues<T>(&DataAt<uint8>(entity, offset), static_cast<size_t>(arraySize), storage);
	}

	template<typename T>
	void SetDataCArray(CEntityInstance* entity, int offset, const plg::vector<T>& value, int arraySize, int dataSize, bool changeState, int chainOffset) {
		const Storage storage = StorageFor<T>(dataSize);
		if (arraySize <= 0 || storage == Storage::Invalid) {
			return;
		}
		NotifyDataChanged(entity, offset, chainOffset, changeState);
		const size_t n = std::min(value.size(), static_cast<size_t>(arraySize));
		WriteValues<T>(&DataAt<uint8>(entity, offset), value.data(), n, storage);
	}

	// CUtlVector of integers or floats.
	template<typename T>
	plg::vector<T> GetDataVector(CEntityInstance* entity, int offset, int dataSize) {
		const Storage storage = StorageFor<T>(dataSize);
		const auto& vec = DataAt<CUtlVector<uint8>>(entity, offset);
		if (vec.Count() <= 0 || storage == Storage::Invalid) {
			return {};
		}
		return ReadValues<T>(vec.Base(), static_cast<size_t>(vec.Count()), storage);
	}

	template<typename T>
	void SetDataVector(CEntityInstance* entity, int offset, const plg::vector<T>& value, int dataSize, bool changeState, int chainOffset) {
		const Storage storage = StorageFor<T>(dataSize);
		if (storage == Storage::Invalid) {
			return;
		}
		NotifyDataChanged(entity, offset, chainOffset, changeState);
		const int count = static_cast<int>(value.size());
		void* data = ResizeUtlVector(reinterpret_cast<intptr_t>(entity) + offset, count, dataSize);
		if (data && count > 0) {
			WriteValues<T>(data, value.data(), value.size(), storage);
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

		const void* ptr = field.Element(sized.type, element, numericName<T>);
		return ptr ? ReadValue<T>(ptr, sized.storage) : T{};
	}

	template<typename T>
	void SetNumeric(CEntityInstance* entity, std::string_view className, std::string_view memberName, T value, bool changeState, int element) {
		const auto field = FindWritableField(entity, className, memberName);
		if (!field) {
			return;
		}

		const auto sized = ClassifyNumeric<T>(field.key.type);
		void* ptr = field.Element(sized.type, element, numericName<T>);
		if (!ptr) {
			return;
		}

		field.NotifyChanged(changeState);
		WriteValue<T>(ptr, sized.storage, value);
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

		const auto [elementType, storage] = ClassifyNumeric<T>(field.key.type);
		switch (elementType) {
			case Array:
			case Collection: {
				const auto view = GetArrayView(field.Base(), field.key.type, elementType);
				if (view.count <= 0) {
					return {};
				}
				return ReadValues<T>(reinterpret_cast<const void*>(view.data), static_cast<size_t>(view.count), storage);
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

		const auto [elementType, storage] = ClassifyNumeric<T>(field.key.type);
		switch (elementType) {
			case Array: {
				const int count = static_cast<const CSchemaType_FixedArray*>(field.key.type)->m_nElementCount;
				if (value.size() > static_cast<size_t>(count)) {
					plg::print(LS_WARNING, "Schema array '{}::{}' can hold {} elements, {} were given, extra elements are ignored\n", className, memberName, count, value.size());
				}
				field.NotifyChanged(changeState);
				const size_t n = std::min(value.size(), static_cast<size_t>(count));
				WriteValues<T>(reinterpret_cast<void*>(field.Base()), value.data(), n, storage);
				break;
			}
			case Collection: {
				field.NotifyChanged(changeState);
				const int count = static_cast<int>(value.size());
				void* data = ResizeUtlVector(field.Base(), count, SizeOf(storage));
				if (data && count > 0) {
					WriteValues<T>(data, value.data(), value.size(), storage);
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
