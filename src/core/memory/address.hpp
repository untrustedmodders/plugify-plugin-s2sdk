#pragma once

class CAddress
{
public:
    constexpr CAddress() = default;
    constexpr CAddress(uintptr_t ptr) : m_value(ptr) {}
    constexpr CAddress(const void* addr) : m_value(reinterpret_cast<uintptr_t>(addr)) {}

	constexpr explicit operator bool() const noexcept
    {
    	return m_value != 0;
    }

	constexpr operator uintptr_t() const noexcept
    {
    	return m_value;
    }

    constexpr operator uint8_t*() const noexcept
    {
        return reinterpret_cast<uint8_t*>(m_value);
    }

    constexpr operator void*() const noexcept
    {
        return reinterpret_cast<void*>(m_value);
    }

    template <typename T>
    [[nodiscard]] constexpr T As() noexcept
    {
        return reinterpret_cast<T>(m_value);
    }

    template <typename T>
    [[nodiscard]] constexpr T As() const noexcept
    {
        return reinterpret_cast<T>(m_value);
    }

    template <typename T>
    [[nodiscard]] constexpr T Get() noexcept
    {
        return *reinterpret_cast<T*>(m_value);
    }

    template <typename T>
    [[nodiscard]] constexpr T Get() const noexcept
    {
        return *reinterpret_cast<T*>(m_value);
    }

	[[nodiscard]] constexpr CAddress Offset(ptrdiff_t offset) const noexcept
	{
	    return m_value + offset;
    }

	constexpr CAddress& OffsetSelf(ptrdiff_t offset) noexcept
	{
	    m_value += offset;
    	return *this;
    }

	[[nodiscard]] constexpr CAddress Deref(ptrdiff_t deref = 1, ptrdiff_t offset = 0) const noexcept
    {
    	uintptr_t base = m_value;

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
    		m_value = *reinterpret_cast<uintptr_t*>(m_value + offset);
    	}

    	return *this;
    }

    [[nodiscard]] constexpr bool IsValid() const noexcept
    {
        return m_value >= 0x1000 && m_value < 0x7FFFFFFEFFFF;
    }

	[[nodiscard]] constexpr uintptr_t GetPtr() const
    {
    	return m_value;
    }

	// ── Equality operators ────────────────────────────────────────────────────

	[[nodiscard]] constexpr bool operator==(const auto& other) const noexcept
    {
    	return m_value == static_cast<uintptr_t>(other);
    }

	[[nodiscard]] constexpr auto operator<=>(const auto& other) const noexcept
    {
    	return m_value <=> static_cast<uintptr_t>(other);
    }

	// ── Arithmetic operators ────────────────────────────────────────────────────

	[[nodiscard]] constexpr CAddress operator+(const auto& offset) const noexcept
    {
    	return m_value + static_cast<uintptr_t>(offset);
    }

	[[nodiscard]] constexpr CAddress operator-(const auto& offset) const noexcept
    {
    	return m_value - static_cast<uintptr_t>(offset);
    }

	[[nodiscard]] constexpr ptrdiff_t operator-(const CAddress& other) const noexcept
    {
    	return static_cast<ptrdiff_t>(m_value) - static_cast<ptrdiff_t>(other.m_value);
    }

	constexpr CAddress& operator+=(const auto& offset) noexcept
    {
    	m_value += static_cast<uintptr_t>(offset);
    	return *this;
    }

	constexpr CAddress& operator-=(const auto& offset) noexcept
    {
    	m_value -= static_cast<uintptr_t>(offset);
    	return *this;
    }

	// ── Bitwise operators ───────────────────────────────────────────────────────

	[[nodiscard]] constexpr CAddress operator&(const auto& mask) const noexcept
	{
	    return m_value & static_cast<uintptr_t>(mask);
	}

	[[nodiscard]] constexpr CAddress operator|(const auto& mask) const noexcept
	{
	    return m_value | static_cast<uintptr_t>(mask);
	}

	[[nodiscard]] constexpr CAddress operator^(const auto& mask) const noexcept
	{
	    return m_value ^ static_cast<uintptr_t>(mask);
	}

	[[nodiscard]] constexpr CAddress operator>>(const auto& shift) const noexcept
	{
	    return m_value >> static_cast<uintptr_t>(shift);
	}

	[[nodiscard]] constexpr CAddress operator<<(const auto& shift) const noexcept
	{
	    return m_value << static_cast<uintptr_t>(shift);
	}

	// ── Increment / Decrement ───────────────────────────────────────────────────

	constexpr CAddress& operator++() noexcept    { ++m_value; return *this; }
	constexpr CAddress  operator++(int) noexcept { auto t = *this; ++m_value; return t; }
	constexpr CAddress& operator--() noexcept    { --m_value; return *this; }
	constexpr CAddress  operator--(int) noexcept { auto t = *this; --m_value; return t; }

private:
	uintptr_t m_value;
};