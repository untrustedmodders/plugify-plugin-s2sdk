#pragma once

class CAddress
{
public:
    constexpr CAddress() = default;
    constexpr CAddress(const void* val) : m_value(reinterpret_cast<uintptr_t>(val)) {}
    constexpr CAddress(const uintptr_t ptr) : m_value(ptr) {}

    constexpr operator uintptr_t() const noexcept
    {
        return m_value;
    }

    constexpr operator void*() const noexcept
    {
        return reinterpret_cast<void*>(m_value);
    }

    constexpr operator uint8_t*() const noexcept
    {
        return reinterpret_cast<uint8_t*>(m_value);
    }

    constexpr explicit operator bool() const noexcept
    {
        return m_value != 0;
    }

    template <typename T = void*>
    [[nodiscard]] constexpr T As() noexcept
    {
        return reinterpret_cast<T>(m_value);
    }

    template <typename T = const void*>
    [[nodiscard]] constexpr T As() const noexcept
    {
        return reinterpret_cast<T>(m_value);
    }

    template <typename T = void*>
    [[nodiscard]] constexpr T Get() noexcept
    {
        return *reinterpret_cast<T*>(m_value);
    }

    template <typename T = const void*>
    [[nodiscard]] constexpr T Get() const noexcept
    {
        return *reinterpret_cast<T*>(m_value);
    }

    [[nodiscard]] constexpr CAddress ToAbsolute(intptr_t offset = 0x1) const noexcept
    {
        auto displacement = *reinterpret_cast<int32_t*>(m_value + offset);
        return m_value + offset + sizeof(int) + displacement;
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

	[[nodiscard]] constexpr CAddress Deref(uintptr_t deref = 1, ptrdiff_t offset = 0) const noexcept
    {
    	uintptr_t base = m_value;

    	while (deref)
    	{
    		base = *reinterpret_cast<uintptr_t*>(base + offset);
    		deref--;
    	}

    	return base;
    }

	constexpr CAddress& DerefSelf(int deref = 1, ptrdiff_t offset = 0) noexcept
    {
    	while (deref)
    	{
    		m_value = *reinterpret_cast<uintptr_t*>(m_value + offset);
    		deref--;
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

    template <std::integral T>
    [[nodiscard]] constexpr bool operator==(T val) const noexcept
    {
        return m_value == static_cast<uintptr_t>(val);
    }

    template <std::integral T>
    [[nodiscard]] constexpr auto operator<=>(T val) const noexcept
    {
    	return m_value <=> static_cast<uintptr_t>(val);
    }

    [[nodiscard]] constexpr bool operator==(void* val) const noexcept
    {
        return (m_value == reinterpret_cast<uintptr_t>(val));
    }

    [[nodiscard]] constexpr auto operator<=>(void* val) const noexcept
    {
        return *this <=> reinterpret_cast<uintptr_t>(val);
    }

    [[nodiscard]] constexpr bool operator==(const CAddress& val) const = default;
    [[nodiscard]] constexpr auto operator<=>(const CAddress& val) const = default;

private:
    uintptr_t m_value;
};