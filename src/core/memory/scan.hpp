#pragma once

#include <cstdint>
#include <string>
#include <vector>

class CAddress;

namespace scan
{
    CAddress              FindPattern(const uint8_t* data, size_t size, std::string_view pattern) noexcept;
    std::vector<CAddress> FindPatternMulti(const uint8_t* data, size_t size, std::string_view pattern) noexcept;

	CAddress              FindData(const uint8_t* data, size_t size, const uint8_t* needle, size_t needle_size) noexcept;
	std::vector<CAddress> FindDataMulti(const uint8_t* data, size_t size, const uint8_t* needle, size_t needle_size) noexcept;

    CAddress              FindRVA(uintptr_t data, size_t size, uint32_t rva) noexcept;
    std::vector<CAddress> FindRVAs(uintptr_t data, size_t size, uint32_t rva) noexcept;

    CAddress              FindPtr(uintptr_t data, size_t size, uintptr_t ptr) noexcept;
    std::vector<CAddress> FindPtrs(uintptr_t data, size_t size, uintptr_t ptr) noexcept;

    CAddress              FindStr(const uint8_t* data, size_t size, std::string_view str, bool zero_terminated = false, bool exact = false) noexcept;
	std::vector<CAddress> FindStrMulti(const uint8_t* data, size_t size, std::string_view str, bool zero_terminated, bool exact) noexcept;
} // namespace scan