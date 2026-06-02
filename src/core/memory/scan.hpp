#pragma once

#include <cstdint>
#include <string>
#include <vector>

class CAddress;

namespace scan
{
	CAddress              FindPattern(uint8_t* data, std::size_t size, std::string_view pattern) noexcept;
	std::vector<CAddress> FindPatternMulti(uint8_t* data, std::size_t size, std::string_view pattern) noexcept;
	CAddress              FindStr(uint8_t* data, std::size_t size, std::string_view str, bool zero_terminated = false, bool exact = false) noexcept;

	CAddress              FindRVA(uintptr_t data, std::size_t size, uint32_t rva) noexcept;
	std::vector<CAddress> FindRVAs(uintptr_t data, std::size_t size, uint32_t rva) noexcept;

	CAddress              FindPtr(uintptr_t data, std::size_t size, uintptr_t ptr) noexcept;
	std::vector<CAddress> FindPtrs(uintptr_t data, std::size_t size, uintptr_t ptr) noexcept;

	CAddress              FindData(uint8_t* data, std::size_t size, const uint8_t* needle, std::size_t needle_size) noexcept;
	std::vector<CAddress> FindDataMulti(uint8_t* data, std::size_t size, const uint8_t* needle, std::size_t needle_size) noexcept;
} // namespace scan