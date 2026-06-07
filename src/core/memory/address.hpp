#pragma once

class CAddress
{
public:
	CAddress() noexcept = default;

	CAddress(nullptr_t) noexcept : m_value(0) {}

	CAddress(uintptr_t addr) noexcept : m_value(addr) {}

	template <typename T> requires std::is_pointer_v<T>
	CAddress(T addr) noexcept : m_value(reinterpret_cast<uintptr_t>(addr)) {}

	template <typename T> requires std::is_integral_v<T> && (sizeof(T) <= sizeof(uintptr_t))
	CAddress(T addr) noexcept : m_value(static_cast<uintptr_t>(addr)) {}

	operator uintptr_t() const noexcept
    {
    	return m_value;
    }

	explicit operator bool() const noexcept
    {
    	return m_value != 0;
    }

    operator uint8_t*() const noexcept
    {
        return reinterpret_cast<uint8_t*>(m_value);
    }

    operator void*() const noexcept
    {
        return reinterpret_cast<void*>(m_value);
    }

    template <typename T>
    [[nodiscard]] T As() noexcept
    {
        return reinterpret_cast<T>(m_value);
    }

    template <typename T>
    [[nodiscard]] T As() const noexcept
    {
        return reinterpret_cast<T>(m_value);
    }

    template <typename T>
    [[nodiscard]] T Get() noexcept
    {
        return *reinterpret_cast<T*>(m_value);
    }

    template <typename T>
    [[nodiscard]] T Get() const noexcept
    {
        return *reinterpret_cast<T*>(m_value);
    }

	[[nodiscard]] uintptr_t GetPtr() const noexcept
    {
    	return m_value;
    }

	[[nodiscard]] bool IsValid() const noexcept
    {
    	return m_value >= 0x1000 && m_value < 0x7FFFFFFEFFFF;
    }

	[[nodiscard]] CAddress Offset(auto offset) const noexcept
	{
	    return m_value + static_cast<uintptr_t>(offset);
    }

	CAddress& OffsetSelf(auto offset) noexcept
	{
	    m_value += static_cast<uintptr_t>(offset);
    	return *this;
    }

	[[nodiscard]] CAddress Deref(auto offset, ptrdiff_t deref = 1) const noexcept
    {
    	uintptr_t base = m_value;

    	while (deref--)
    	{
    		base = *reinterpret_cast<uintptr_t*>(base + static_cast<uintptr_t>(offset));
    	}

    	return base;
    }

	CAddress& DerefSelf(auto offset, ptrdiff_t deref = 1) noexcept
    {
    	while (deref--)
    	{
    		m_value = *reinterpret_cast<uintptr_t*>(m_value + static_cast<uintptr_t>(offset));
    	}

    	return *this;
    }

	// ── Equality operators ────────────────────────────────────────────────────

	[[nodiscard]] bool operator==(auto other) const noexcept
    {
    	return m_value == static_cast<uintptr_t>(other);
    }

	[[nodiscard]] auto operator<=>(auto other) const noexcept
    {
    	return m_value <=> static_cast<uintptr_t>(other);
    }

	// ── Arithmetic operators ────────────────────────────────────────────────────

	[[nodiscard]] CAddress operator+(auto offset) const noexcept
    {
    	return m_value + static_cast<uintptr_t>(offset);
    }

	[[nodiscard]] CAddress operator-(auto offset) const noexcept
    {
    	return m_value - static_cast<uintptr_t>(offset);
    }

	CAddress& operator+=(auto offset) noexcept
    {
    	m_value += static_cast<uintptr_t>(offset);
    	return *this;
    }

	CAddress& operator-=(auto offset) noexcept
    {
    	m_value -= static_cast<uintptr_t>(offset);
    	return *this;
    }

	[[nodiscard]] ptrdiff_t operator-(CAddress other) const noexcept
    {
    	return static_cast<ptrdiff_t>(m_value) - static_cast<ptrdiff_t>(other.m_value);
    }

	template <typename T> requires (!std::is_same_v<T, CAddress>)
	[[nodiscard]] friend CAddress operator+(T offset, CAddress addr) noexcept
    {
    	return static_cast<uintptr_t>(offset) + addr.m_value;
    }

	template <typename T> requires (!std::is_same_v<T, CAddress>)
	[[nodiscard]] friend CAddress operator-(T offset, CAddress addr) noexcept
    {
    	return static_cast<uintptr_t>(offset) - addr.m_value;
    }

	// ── Bitwise operators ───────────────────────────────────────────────────────

	[[nodiscard]] CAddress operator&(auto mask) const noexcept
	{
	    return m_value & static_cast<uintptr_t>(mask);
	}

	[[nodiscard]] CAddress operator|(auto mask) const noexcept
	{
	    return m_value | static_cast<uintptr_t>(mask);
	}

	[[nodiscard]] CAddress operator^(auto mask) const noexcept
	{
	    return m_value ^ static_cast<uintptr_t>(mask);
	}

	[[nodiscard]] CAddress operator>>(auto shift) const noexcept
	{
	    return m_value >> static_cast<uintptr_t>(shift);
	}

	[[nodiscard]] CAddress operator<<(auto shift) const noexcept
	{
	    return m_value << static_cast<uintptr_t>(shift);
	}

	// ── Increment / Decrement ───────────────────────────────────────────────────

	CAddress& operator++() noexcept { ++m_value; return *this; }
	[[nodiscard]] CAddress operator++(int) noexcept { const auto t = *this; ++m_value; return t; }
	CAddress& operator--() noexcept { --m_value; return *this; }
	[[nodiscard]] CAddress operator--(int) noexcept { const auto t = *this; --m_value; return t; }

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
	auto format(CAddress addr, FormatContext& ctx) const
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