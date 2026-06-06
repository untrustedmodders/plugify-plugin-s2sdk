#pragma once

class CAddress
{
public:
    constexpr CAddress() = default;
    constexpr CAddress(const void* addr) : m_addr(const_cast<void*>(addr)) {}
    constexpr CAddress(uintptr_t ptr) : m_ptr(ptr) {}

    constexpr operator uintptr_t() const noexcept
    {
        return m_ptr;
    }

    constexpr operator uint8_t*() const noexcept
    {
        return static_cast<uint8_t*>(m_addr);
    }

    constexpr operator void*() const noexcept
    {
        return m_addr;
    }

    constexpr explicit operator bool() const noexcept
    {
        return m_ptr != 0;
    }

    template <typename T = void*>
    [[nodiscard]] constexpr T As() noexcept
    {
        return reinterpret_cast<T>(m_ptr);
    }

    template <typename T = const void*>
    [[nodiscard]] constexpr T As() const noexcept
    {
        return reinterpret_cast<T>(m_ptr);
    }

    template <typename T = void*>
    [[nodiscard]] constexpr T Get() noexcept
    {
        return *reinterpret_cast<T*>(m_ptr);
    }

    template <typename T = const void*>
    [[nodiscard]] constexpr T Get() const noexcept
    {
        return *reinterpret_cast<T*>(m_ptr);
    }

	[[nodiscard]] constexpr CAddress Offset(ptrdiff_t offset) const noexcept
	{
	    return m_ptr + offset;
    }

	constexpr CAddress& OffsetSelf(ptrdiff_t offset) noexcept
	{
	    m_ptr += offset;
    	return *this;
    }

	[[nodiscard]] constexpr CAddress Deref(ptrdiff_t deref = 1, ptrdiff_t offset = 0) const noexcept
    {
    	uintptr_t base = m_ptr;

    	while (deref--)
    	{
    		base = *reinterpret_cast<uintptr_t*>(base + offset);
    	}

    	return base;
    }

	constexpr CAddress& DerefSelf(ptrdiff_t deref = 1, ptrdiff_t offset = 0) noexcept
    {
    	while (deref--)
    	{
    		m_ptr = *reinterpret_cast<uintptr_t*>(m_ptr + offset);
    	}

    	return *this;
    }

    [[nodiscard]] constexpr bool IsValid() const noexcept
    {
        return m_ptr >= 0x1000 && m_ptr < 0x7FFFFFFEFFFF;
    }

	[[nodiscard]] constexpr uintptr_t GetPtr() const
    {
    	return m_ptr;
    }

	// ── Equality operators ────────────────────────────────────────────────────

	[[nodiscard]] constexpr bool operator==(const auto& other) const noexcept
    {
    	return m_ptr == static_cast<uintptr_t>(other);
    }

	[[nodiscard]] constexpr auto operator<=>(const auto& other) const noexcept
    {
    	return m_ptr <=> static_cast<uintptr_t>(other);
    }

	// ── Arithmetic operators ────────────────────────────────────────────────────

	[[nodiscard]] constexpr ptrdiff_t operator-(const CAddress& other) const noexcept
    {
    	return static_cast<ptrdiff_t>(m_ptr) - static_cast<ptrdiff_t>(other.m_ptr);
    }

	[[nodiscard]] constexpr CAddress operator+(const auto& offset) const noexcept
    {
    	return m_ptr + static_cast<uintptr_t>(offset);
    }

	[[nodiscard]] constexpr CAddress operator-(const auto& offset) const noexcept
    {
    	return m_ptr - static_cast<uintptr_t>(offset);
    }

	template <typename T> requires (!std::is_same_v<T, CAddress>)
	[[nodiscard]] friend constexpr CAddress operator+(const T& offset, const CAddress& addr)noexcept
    {
    	return addr.m_ptr + static_cast<uintptr_t>(offset);
    }

	template <typename T> requires (!std::is_same_v<T, CAddress>)
	[[nodiscard]] friend constexpr CAddress operator-(const T& offset, const CAddress& addr)noexcept
    {
    	return addr.m_ptr - static_cast<uintptr_t>(offset);
    }

	constexpr CAddress& operator+=(const auto& offset) noexcept
    {
    	m_ptr += static_cast<uintptr_t>(offset);
    	return *this;
    }

	constexpr CAddress& operator-=(const auto& offset) noexcept
    {
    	m_ptr -= static_cast<uintptr_t>(offset);
    	return *this;
    }

	// ── Bitwise operators ───────────────────────────────────────────────────────

	[[nodiscard]] constexpr CAddress operator&(const auto& mask) const noexcept
	{
	    return m_ptr & static_cast<uintptr_t>(mask);
	}

	[[nodiscard]] constexpr CAddress operator|(const auto& mask) const noexcept
	{
	    return m_ptr | static_cast<uintptr_t>(mask);
	}

	[[nodiscard]] constexpr CAddress operator^(const auto& mask) const noexcept
	{
	    return m_ptr ^ static_cast<uintptr_t>(mask);
	}

	[[nodiscard]] constexpr CAddress operator>>(const auto& shift) const noexcept
	{
	    return m_ptr >> static_cast<uintptr_t>(shift);
	}

	[[nodiscard]] constexpr CAddress operator<<(const auto& shift) const noexcept
	{
	    return m_ptr << static_cast<uintptr_t>(shift);
	}

	// ── Increment / Decrement ───────────────────────────────────────────────────

	constexpr CAddress& operator++() noexcept    { ++m_ptr; return *this; }
	constexpr CAddress  operator++(int) noexcept { auto t = *this; ++m_ptr; return t; }
	constexpr CAddress& operator--() noexcept    { --m_ptr; return *this; }
	constexpr CAddress  operator--(int) noexcept { auto t = *this; --m_ptr; return t; }

private:
	union {
		void* m_addr;
		uintptr_t m_ptr;
	};
};