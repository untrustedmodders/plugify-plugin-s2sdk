#pragma once

#include <set>
#include <map>
#include <shared_mutex>
#include <optional>
#include <functional>
#include <iterator>

namespace plg {
	// Thread-safe wrapper for std::set
	template<typename Key, typename Compare, typename Allocator, typename Container>
	class safe_set {
	    mutable std::shared_mutex mutex_;
	    Container set_;

	public:
	    using key_type = Key;
	    using value_type = Key;
	    using size_type = typename Container::size_type;
	    using difference_type = typename Container::difference_type;
	    using key_compare = Compare;
	    using value_compare = Compare;
	    using allocator_type = Allocator;
	    using reference = value_type&;
	    using const_reference = const value_type&;
	    using pointer = typename std::allocator_traits<Allocator>::pointer;
	    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;

	    // Constructors
	    safe_set() = default;

	    explicit safe_set(const Compare& comp, const Allocator& alloc = Allocator())
	        : set_(comp, alloc) {}

	    template<typename InputIt>
	    safe_set(InputIt first, InputIt last,
	                  const Compare& comp = Compare(), const Allocator& alloc = Allocator())
	        : set_(first, last, comp, alloc) {}

	    safe_set(std::initializer_list<Key> init,
	                  const Compare& comp = Compare(), const Allocator& alloc = Allocator())
	        : set_(init, comp, alloc) {}

	    // Copy constructor
	    safe_set(const safe_set& other) {
	        std::shared_lock lock(other.mutex_);
	        set_ = other.set_;
	    }

	    // Move constructor
	    safe_set(safe_set&& other) noexcept {
	        std::unique_lock lock(other.mutex_);
	        set_ = std::move(other.set_);
	    }

	    // Assignment operators
	    safe_set& operator=(const safe_set& other) {
	        if (this != &other) {
	            std::unique_lock lock1(mutex_, std::defer_lock);
	            std::shared_lock lock2(other.mutex_, std::defer_lock);
	            std::lock(lock1, lock2);
	            set_ = other.set_;
	        }
	        return *this;
	    }

	    safe_set& operator=(safe_set&& other) noexcept {
	        if (this != &other) {
	            std::unique_lock lock1(mutex_, std::defer_lock);
	            std::unique_lock lock2(other.mutex_, std::defer_lock);
	            std::lock(lock1, lock2);
	            set_ = std::move(other.set_);
	        }
	        return *this;
	    }

	    // Capacity
	    bool empty() const {
	        std::shared_lock lock(mutex_);
	        return set_.empty();
	    }

	    size_type size() const {
	        std::shared_lock lock(mutex_);
	        return set_.size();
	    }

	    size_type max_size() const {
	        std::shared_lock lock(mutex_);
	        return set_.max_size();
	    }

	    // Modifiers
	    void clear() {
	        std::unique_lock lock(mutex_);
	        set_.clear();
	    }

	    auto insert(const value_type& value) {
	        std::unique_lock lock(mutex_);
	        return set_.insert(value);
	    }

	    auto insert(value_type&& value) {
	        std::unique_lock lock(mutex_);
	        return set_.insert(std::move(value));
	    }

	    template<typename InputIt>
	    void insert(InputIt first, InputIt last) {
	        std::unique_lock lock(mutex_);
	        set_.insert(first, last);
	    }

	    void insert(std::initializer_list<value_type> ilist) {
	        std::unique_lock lock(mutex_);
	        set_.insert(ilist);
	    }

	    void insert(Container::node_type&& node) {
	        std::unique_lock lock(mutex_);
	        set_.insert(std::move(node));
	    }

	    template<typename... Args>
	    auto emplace(Args&&... args) {
	        std::unique_lock lock(mutex_);
	        return set_.emplace(std::forward<Args>(args)...);
	    }

	    auto erase(auto it) {
	        std::unique_lock lock(mutex_);
	        return set_.erase(it);
	    }

	    size_type erase(const key_type& key) {
	        std::unique_lock lock(mutex_);
	        return set_.erase(key);
	    }

	    void swap(safe_set& other) noexcept {
	        if (this != &other) {
	            std::unique_lock lock1(mutex_, std::defer_lock);
	            std::unique_lock lock2(other.mutex_, std::defer_lock);
	            std::lock(lock1, lock2);
	            set_.swap(other.set_);
	        }
	    }

	    // Lookup
	    size_type count(const key_type& key) const {
	        std::shared_lock lock(mutex_);
	        return set_.count(key);
	    }

	    bool contains(const key_type& key) const {
	        std::shared_lock lock(mutex_);
	        return set_.contains(key);
	    }

	    auto find(const key_type& key) {
	        std::shared_lock lock(mutex_);
	        return set_.find(key);
	    }

	    auto find(const key_type& key) const {
	        std::shared_lock lock(mutex_);
	        return set_.find(key);
	    }

		auto begin() {
	    	std::shared_lock lock(mutex_);
	    	return set_.begin();
	    }

		auto begin() const {
	    	std::shared_lock lock(mutex_);
	    	return set_.begin();
	    }

		auto end() {
	    	std::shared_lock lock(mutex_);
	    	return set_.end();
	    }

		auto end() const {
	    	std::shared_lock lock(mutex_);
	    	return set_.end();
	    }

		auto extract(Container::const_iterator it) {
		    std::unique_lock lock(mutex_);
	    	return set_.extract(it);
	    }

	    // Apply function to all elements (read-only)
	    template<typename Func>
	    void for_each(Func func) const {
	        std::shared_lock lock(mutex_);
	        for (const auto& elem : set_) {
	            func(elem);
	        }
	    }

	    // Apply function to all elements (modifying)
	    template<typename Func>
	    void apply(Func func) {
	        std::unique_lock lock(mutex_);
	        func(set_);
	    }

	    // Get a snapshot of the set
	    Container snapshot() const {
	        std::shared_lock lock(mutex_);
	        return set_;
	    }
	};

	// Thread-safe wrapper for std::map
	template<typename Key, typename T, typename Compare, typename Allocator, typename Container>
	class safe_map {
	private:
	    mutable std::shared_mutex mutex_;
	    Container map_;

	public:
	    using key_type = Key;
	    using mapped_type = T;
	    using value_type = std::pair<const Key, T>;
	    using size_type = typename Container::size_type;
	    using difference_type = typename Container::difference_type;
	    using key_compare = Compare;
	    using allocator_type = Allocator;
	    using reference = value_type&;
	    using const_reference = const value_type&;
	    using pointer = typename std::allocator_traits<Allocator>::pointer;
	    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;

	    // Constructors
	    safe_map() = default;

	    explicit safe_map(const Compare& comp, const Allocator& alloc = Allocator())
	        : map_(comp, alloc) {}

	    template<typename InputIt>
	    safe_map(InputIt first, InputIt last,
	                  const Compare& comp = Compare(), const Allocator& alloc = Allocator())
	        : map_(first, last, comp, alloc) {}

	    safe_map(std::initializer_list<value_type> init,
	                  const Compare& comp = Compare(), const Allocator& alloc = Allocator())
	        : map_(init, comp, alloc) {}

	    // Copy constructor
	    safe_map(const safe_map& other) {
	        std::shared_lock lock(other.mutex_);
	        map_ = other.map_;
	    }

	    // Move constructor
	    safe_map(safe_map&& other) noexcept {
	        std::unique_lock lock(other.mutex_);
	        map_ = std::move(other.map_);
	    }

	    // Assignment operators
	    safe_map& operator=(const safe_map& other) {
	        if (this != &other) {
	            std::unique_lock lock1(mutex_, std::defer_lock);
	            std::shared_lock lock2(other.mutex_, std::defer_lock);
	            std::lock(lock1, lock2);
	            map_ = other.map_;
	        }
	        return *this;
	    }

	    safe_map& operator=(safe_map&& other) noexcept {
	        if (this != &other) {
	            std::unique_lock lock1(mutex_, std::defer_lock);
	            std::unique_lock lock2(other.mutex_, std::defer_lock);
	            std::lock(lock1, lock2);
	            map_ = std::move(other.map_);
	        }
	        return *this;
	    }

	    // Element access
	    T& operator[](const key_type& key) {
	        std::unique_lock lock(mutex_);
	        return map_[key];
	    }

	    T& operator[](key_type&& key) {
	        std::unique_lock lock(mutex_);
	        return map_[std::move(key)];
	    }

	    // Capacity
	    bool empty() const {
	        std::shared_lock lock(mutex_);
	        return map_.empty();
	    }

	    size_type size() const {
	        std::shared_lock lock(mutex_);
	        return map_.size();
	    }

	    size_type max_size() const {
	        std::shared_lock lock(mutex_);
	        return map_.max_size();
	    }

	    // Modifiers
	    void clear() {
	        std::unique_lock lock(mutex_);
	        map_.clear();
	    }

	    auto insert(const value_type& value) {
	        std::unique_lock lock(mutex_);
	        return map_.insert(value);
	    }

	    auto insert(value_type&& value) {
	        std::unique_lock lock(mutex_);
	        return map_.insert(std::move(value));
	    }

	    template<typename InputIt>
	    void insert(InputIt first, InputIt last) {
	        std::unique_lock lock(mutex_);
	        map_.insert(first, last);
	    }

	    void insert(std::initializer_list<value_type> ilist) {
	        std::unique_lock lock(mutex_);
	        map_.insert(ilist);
	    }

	    template<typename M>
	    auto insert_or_assign(const key_type& key, M&& obj) {
	        std::unique_lock lock(mutex_);
	        return map_.insert_or_assign(key, std::forward<M>(obj));
	    }

	    template<typename M>
	    auto insert_or_assign(key_type&& key, M&& obj) {
	        std::unique_lock lock(mutex_);
	        return map_.insert_or_assign(std::move(key), std::forward<M>(obj));
	    }

	    template<typename... Args>
	    auto emplace(Args&&... args) {
	        std::unique_lock lock(mutex_);
	        return map_.emplace(std::forward<Args>(args)...);
	    }

	    template<typename... Args>
	    auto try_emplace(const key_type& key, Args&&... args) {
	        std::unique_lock lock(mutex_);
	        return map_.try_emplace(key, std::forward<Args>(args)...);
	    }

	    template<typename... Args>
	    auto try_emplace(key_type&& key, Args&&... args) {
	        std::unique_lock lock(mutex_);
	        return map_.try_emplace(std::move(key), std::forward<Args>(args)...);
	    }

	    auto erase(auto it) {
	        std::unique_lock lock(mutex_);
	        return map_.erase(it);
	    }

	    size_type erase(const key_type& key) {
	        std::unique_lock lock(mutex_);
	        return map_.erase(key);
	    }

	    void swap(safe_map& other) noexcept {
	        if (this != &other) {
	            std::unique_lock lock1(mutex_, std::defer_lock);
	            std::unique_lock lock2(other.mutex_, std::defer_lock);
	            std::lock(lock1, lock2);
	            map_.swap(other.map_);
	        }
	    }

	    // Lookup
	    size_type count(const key_type& key) const {
	        std::shared_lock lock(mutex_);
	        return map_.count(key);
	    }

	    bool contains(const key_type& key) const {
	        std::shared_lock lock(mutex_);
	        return map_.contains(key);
	    }

	    auto find(const key_type& key) {
	        std::shared_lock lock(mutex_);
	        return map_.find(key);
	    }

	    auto find(const key_type& key) const {
	        std::shared_lock lock(mutex_);
	        return map_.find(key);
	    }

		auto begin() {
	        std::shared_lock lock(mutex_);
		    return map_.begin();
	    }

		auto begin() const {
	        std::shared_lock lock(mutex_);
		    return map_.begin();
	    }

		auto end() {
	        std::shared_lock lock(mutex_);
		    return map_.end();
	    }

		auto end() const {
	        std::shared_lock lock(mutex_);
		    return map_.end();
	    }

	    // Apply function to all elements (read-only)
	    template<typename Func>
	    void for_each(Func func) const {
	        std::shared_lock lock(mutex_);
	        for (const auto& [key, value] : map_) {
	            func(key, value);
	        }
	    }

	    // Apply function to all elements (modifying)
	    template<typename Func>
	    void apply(Func func) {
	        std::unique_lock lock(mutex_);
	        func(map_);
	    }

	    // Get a snapshot of the map
	    Container snapshot() const {
	        std::shared_lock lock(mutex_);
	        return map_;
	    }
	};

	// Convenience aliases for specific container types
	template<typename Key, typename Compare = std::less<Key>, typename Allocator = std::allocator<Key>>
	using set = safe_set<Key, Compare, Allocator, std::set<Key, Compare, Allocator>>;

	template<typename Key, typename Compare = std::less<Key>, typename Allocator = std::allocator<Key>>
	using multiset = safe_set<Key, Compare, Allocator, std::multiset<Key, Compare, Allocator>>;

	template<typename Key, typename T, typename Compare = std::less<Key>,
			 typename Allocator = std::allocator<std::pair<const Key, T>>>
	using map = safe_map<Key, T, Compare, Allocator, std::map<Key, T, Compare, Allocator>>;

	template<typename Key, typename T, typename Compare = std::less<Key>,
			 typename Allocator = std::allocator<std::pair<const Key, T>>>
	using multimap = safe_map<Key, T, Compare, Allocator, std::multimap<Key, T, Compare, Allocator>>;
} // namespace plg
