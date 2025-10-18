#pragma once

#include <parallel_hashmap/phmap.h>

namespace plg {
	using namespace phmap;

	template <typename K> using HashEqual = priv::hash_default_eq<K>;
	template <typename V> using HashFn = priv::hash_default_hash<V>;
	template <typename K> using Allocator = priv::Allocator<K>;

	template <typename K, typename V, typename Hash = HashFn<K>, typename Eq = HashEqual<K>, size_t N = 4>
	using parallel_flat_hash_map = parallel_flat_hash_map<K, V, Hash, Eq, Allocator<priv::Pair<K, V>>, N, std::shared_mutex>;

	template <class K, class Hash = HashFn<K>, class Eq = HashEqual<K>, size_t N = 4>
	using parallel_flat_hash_set = parallel_flat_hash_set<K, Hash, Eq, Allocator<K>, N, std::shared_mutex>;

	template<typename Map>
	constexpr auto find(const Map& map, const typename Map::key_type& key) {
		typename Map::mapped_type result;
		map.if_contains(key, [&](const auto& v) {
			result = v.second;
		});
		return result;
	}

	template<typename Enum>
	constexpr size_t enum_size() {
	    return static_cast<size_t>(Enum::Count);
	}

	// Enum-indexed array wrapper
	template<typename T, typename Enum>
	class enum_map {
	private:
	    std::array<T, enum_size<Enum>()> data_;

	    // Convert enum to index
	    static constexpr size_t to_index(Enum e) {
	        return static_cast<size_t>(e);
	    }

	public:
	    using value_type = T;
	    using size_type = size_t;
	    using reference = T&;
	    using const_reference = const T&;
	    using iterator = typename std::array<T, enum_size<Enum>()>::iterator;
	    using const_iterator = typename std::array<T, enum_size<Enum>()>::const_iterator;

	    // Default constructor
	    enum_map() = default;

	    // Initialize with default value
	    explicit enum_map(const T& default_value) {
	        data_.fill(default_value);
	    }

	    // Initialize with initializer list
	    enum_map(std::initializer_list<T> init) {
	        if (init.size() != size()) {
	            throw std::invalid_argument("Initializer list size must match enum count");
	        }
	        std::copy(init.begin(), init.end(), data_.begin());
	    }

	    // Access with enum key (non-const)
	    reference operator[](Enum e) {
	        return data_[to_index(e)];
	    }

	    // Access with enum key (const)
	    const_reference operator[](Enum e) const {
	        return data_[to_index(e)];
	    }

	    // Bounds-checked access
	    reference at(Enum e) {
	        size_t idx = to_index(e);
	        if (idx >= size()) {
	            throw std::out_of_range("Enum value out of range");
	        }
	        return data_[idx];
	    }

	    const_reference at(Enum e) const {
	        size_t idx = to_index(e);
	        if (idx >= size()) {
	            throw std::out_of_range("Enum value out of range");
	        }
	        return data_[idx];
	    }

	    // Size
	    constexpr size_type size() const noexcept {
	        return data_.size();
	    }

	    // Iterators
	    iterator begin() noexcept { return data_.begin(); }
	    const_iterator begin() const noexcept { return data_.begin(); }
	    iterator end() noexcept { return data_.end(); }
	    const_iterator end() const noexcept { return data_.end(); }

	    // Direct access to underlying array
	    std::array<T, enum_size<Enum>()>& array() { return data_; }
	    const std::array<T, enum_size<Enum>()>& array() const { return data_; }

	    // Fill with value
	    void fill(const T& value) {
	        data_.fill(value);
	    }
	};
} // namespace plg
