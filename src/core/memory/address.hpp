#pragma once

class CAddress
{
public:
    constexpr CAddress() noexcept = default;
    constexpr CAddress(uintptr_t ptr) noexcept : m_value(ptr) {}
    constexpr CAddress(const void* addr) noexcept : m_value(std::bit_cast<uintptr_t>(addr)) {}

	constexpr operator uintptr_t() const noexcept
    {
    	return m_value;
    }

	constexpr explicit operator bool() const noexcept
    {
    	return m_value != 0;
    }

    constexpr operator uint8_t*() const noexcept
    {
        return std::bit_cast<uint8_t*>(m_value);
    }

    constexpr operator void*() const noexcept
    {
        return std::bit_cast<void*>(m_value);
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

	[[nodiscard]] constexpr uintptr_t GetPtr() const noexcept
    {
    	return m_value;
    }

	[[nodiscard]] constexpr bool IsValid() const noexcept
    {
    	return m_value >= 0x1000 && m_value < 0x7FFFFFFEFFFF;
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

	[[nodiscard]] constexpr CAddress Deref(ptrdiff_t offset, ptrdiff_t deref = 1) const noexcept
    {
    	uintptr_t base = m_value;

    	while (deref--)
    	{
    		base = *reinterpret_cast<uintptr_t*>(base + offset);
    	}

    	return base;
    }

	constexpr CAddress& DerefSelf(ptrdiff_t offset, ptrdiff_t deref = 1) noexcept
    {
    	while (deref--)
    	{
    		m_value = *reinterpret_cast<uintptr_t*>(m_value + offset);
    	}

    	return *this;
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

	[[nodiscard]] constexpr ptrdiff_t operator-(const CAddress& other) const noexcept
    {
    	return static_cast<ptrdiff_t>(m_value) - static_cast<ptrdiff_t>(other.m_value);
    }

	template <typename T> requires (!std::is_same_v<T, CAddress>)
	[[nodiscard]] friend constexpr CAddress operator+(const T& offset, const CAddress& addr) noexcept
    {
    	return static_cast<uintptr_t>(offset) + addr.m_value;
    }

	template <typename T> requires (!std::is_same_v<T, CAddress>)
	[[nodiscard]] friend constexpr CAddress operator-(const T& offset, const CAddress& addr) noexcept
    {
    	return static_cast<uintptr_t>(offset) - addr.m_value;
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

	constexpr CAddress& operator++() noexcept { ++m_value; return *this; }
	[[nodiscard]] constexpr CAddress operator++(int) noexcept { const auto t = *this; ++m_value; return t; }
	constexpr CAddress& operator--() noexcept { --m_value; return *this; }
	[[nodiscard]] constexpr CAddress operator--(int) noexcept { const auto t = *this; --m_value; return t; }

private:
	uintptr_t m_value;
};

template <>
struct std::formatter<CAddress>
{
	// parse() controls the format spec between { and }
	// e.g. {:x} {:X} {:d} {:p} — default is hex with 0x prefix
	enum class Style : uint8_t { Hex, HEX, Dec, Ptr } style = Style::Hex;

	constexpr auto parse(std::format_parse_context& ctx)
	{
		auto it = ctx.begin();

		if (it == ctx.end() || *it == '}')
			return it;

		switch (*it)
		{
			case 'x': style = Style::Hex; ++it; break;  // 0x00007ff612340000
			case 'X': style = Style::HEX; ++it; break;  // 0X00007FF612340000
			case 'd': style = Style::Dec; ++it; break;  // 140694837248000
			case 'p': style = Style::Ptr; ++it; break;  // 00007ff612340000 (no prefix)
			default:
				throw std::format_error("CAddress: invalid format spec, use x/X/d/p");
		}

		return it;
	}

	template<class FormatContext>
	auto format(const CAddress& addr, FormatContext& ctx) const
	{
		uintptr_t ptr = addr;

		switch (style)
		{
			case Style::Hex: return std::format_to(ctx.out(), "0x{:016x}", ptr);
			case Style::HEX: return std::format_to(ctx.out(), "0X{:016X}", ptr);
			case Style::Dec: return std::format_to(ctx.out(), "{}", ptr);
			case Style::Ptr: return std::format_to(ctx.out(), "{:016x}", ptr);
		}

		std::unreachable();
	}
};