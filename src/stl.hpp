#pragma once

#include <plugin_export.h>

#include <plg/bitmask.hpp>
#include <plg/enum.hpp>
#include <plg/enum_array.hpp>
#include <plg/hybrid_vector.hpp>
#include <plg/path.hpp>
#include <plg/plugin.hpp>

#ifdef __cpp_lib_flat_map
#include <flat_map>
#else
#include <map>
#endif

#ifdef __cpp_lib_flat_set
#include <flat_set>
#else
#include <set>
#endif

#include <parallel_hashmap/phmap.h>

#define UNUSED(x) [[maybe_unused]] auto _ = x

#define NONCOPYABLE(x) \
	x(const x&) = delete; \
	x(x&&) = delete; \
	x& operator=(const x&) = delete; \
	x& operator=(x&&) = delete;

template<typename T>
using Result = std::expected<T, std::string>;

template <plg::string_like First>
constexpr auto MakeError(First&& error) {
	return std::unexpected(std::forward<First>(error));
}

template <typename... Args>
constexpr auto MakeError(std::format_string<Args...> fmt, Args&&... args) {
	return std::unexpected(std::format(fmt, std::forward<Args>(args)...));
}

namespace plg {
#ifdef __cpp_lib_flat_map
	template<typename Key, typename T, typename Compare = std::less<Key>>
	using flat_map = std::flat_map<Key, T, Compare>;
#else
	template<typename Key, typename T, typename Compare = std::less<Key>>
	using flat_map = std::map<Key, T, Compare>;
#endif

#ifdef __cpp_lib_flat_set
	template<typename Key,typename Compare = std::less<Key>>
	using flat_set = std::flat_set<Key, Compare>;
#else
	template<typename Key, typename Compare = std::less<Key>>
	using flat_set = std::set<Key, Compare>;
#endif

	using namespace phmap;

	template <typename K> using HashEqual = priv::hash_default_eq<K>;
	template <typename V> using HashFn = priv::hash_default_hash<V>;
	template <typename K> using Allocator = priv::Allocator<K>;

	template <typename K, typename V, typename Hash = HashFn<K>, typename Eq = HashEqual<K>, size_t N = 4>
	using parallel_flat_hash_map_s = parallel_flat_hash_map<K, V, Hash, Eq, Allocator<priv::Pair<K, V>>, N, std::shared_mutex>;

	template <class K, class Hash = HashFn<K>, class Eq = HashEqual<K>, size_t N = 4>
	using parallel_flat_hash_set_s = parallel_flat_hash_set<K, Hash, Eq, Allocator<K>, N, std::shared_mutex>;

	template <typename K, typename V, typename Hash = HashFn<K>, typename Eq = HashEqual<K>, size_t N = 4>
	using parallel_node_hash_map_s = parallel_node_hash_map<K, V, Hash, Eq, Allocator<priv::Pair<K, V>>, N, std::shared_mutex>;

	template <class K, class Hash = HashFn<K>, class Eq = HashEqual<K>, size_t N = 4>
	using parallel_node_hash_set_s = parallel_node_hash_set<K, Hash, Eq, Allocator<K>, N, std::shared_mutex>;
} // namespace plg
