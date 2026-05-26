#pragma once

class CAddress
{
public:
    CAddress() = default;
    CAddress(const void* val) : m_value(reinterpret_cast<uintptr_t>(val)) {}
    CAddress(const uintptr_t ptr) : m_value(ptr) {}

    operator uintptr_t() const noexcept
    {
        return m_value;
    }

    operator void*() const noexcept
    {
        return reinterpret_cast<void*>(m_value);
    }

    operator uint8_t*() const noexcept
    {
        return reinterpret_cast<uint8_t*>(m_value);
    }

    explicit operator bool() const noexcept
    {
        return m_value != 0;
    }

    template <typename T = void*>
    T As() noexcept
    {
        return T(m_value);
    }

    template <typename T = void*>
    T Get() noexcept
    {
        return *reinterpret_cast<T*>(m_value);
    }

    [[nodiscard]] CAddress ToAbsolute(intptr_t offset = 0x1) const noexcept
    {
        auto displacement = *reinterpret_cast<int32_t*>(m_value + offset);
        return m_value + offset + sizeof(int) + displacement;
    }

	[[nodiscard]] CAddress Offset(ptrdiff_t offset) const noexcept
	{
	    return m_value + offset;
    }
	
	[[nodiscard]] CAddress& OffsetSelf(ptrdiff_t offset) noexcept
	{
	    m_value += offset;
    	return *this;
    }

	[[nodiscard]] CAddress Deref(uintptr_t deref = 1, ptrdiff_t offset = 0) const noexcept
    {
    	uintptr_t base = m_value;

    	while (deref)
    	{
    		base = *reinterpret_cast<uintptr_t*>(base + offset);
    		deref--;
    	}

    	return base;
    }
	
	[[nodiscard]] CAddress& DerefSelf(int deref = 1, ptrdiff_t offset = 0) noexcept
    {
    	while (deref)
    	{
    		m_value = *reinterpret_cast<uintptr_t*>(m_value + offset);
    		deref--;
    	}

    	return *this;
    }

    [[nodiscard]] bool IsValid() const
    {
        return m_value >= 0x1000 && m_value < 0x7FFFFFFEFFFF;
    }

    template <std::integral T>
    bool operator==(T val) const noexcept
    {
        return m_value == static_cast<uintptr_t>(val);
    }

    template <std::integral T>
    auto operator<=>(T val) const noexcept
    {
    	return m_value <=> static_cast<uintptr_t>(val);
    }

    bool operator==(void* val) const
    {
        return (m_value == reinterpret_cast<uintptr_t>(val));
    }

    auto operator<=>(void* val) const
    {
        return *this <=> reinterpret_cast<uintptr_t>(val);
    }
	
    uintptr_t GetPtr() const
    {
        return m_value;
    }

private:
    uintptr_t m_value{};
};