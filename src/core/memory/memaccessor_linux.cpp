#ifdef PLATFORM_POSIX
#include "os.h"

#include "memaccessor.hpp"
#include "memprotector.hpp"

#include <cstring>
#include <fstream>

struct region_t {
	uintptr_t start;
	uintptr_t end;
	ProtFlag prot;
};

static region_t get_region_from_addr(uintptr_t addr)
{
	region_t res{};

	std::ifstream f("/proc/self/maps");
	std::string s;
	while (std::getline(f, s))
	{
		if (s.empty() || s.contains("vdso") || s.contains("vsyscall"))
			continue;

		char* strend = &s[0];
		uintptr_t start = strtoul(strend, &strend, 16);
		uintptr_t end = strtoul(strend + 1, &strend, 16);
		if (start != 0 && end != 0 && start <= addr && addr < end)
		{
			res.start = start;
			res.end = end;

			++strend;
			if (strend[0] == 'r')
				res.prot = res.prot | ProtFlag::R;

			if (strend[1] == 'w')
				res.prot = res.prot | ProtFlag::W;

			if (strend[2] == 'x')
				res.prot = res.prot | ProtFlag::X;

			if (res.prot == ProtFlag::UNSET)
				res.prot = ProtFlag::N;

			break;
		}
	}
	return res;
}

bool CMemAccessor::MemCopy(CAddress dest, CAddress src, size_t size)
{
	std::memcpy(dest, src, size);
	return true;
}

bool CMemAccessor::SafeMemCopy(CAddress dest, CAddress src, size_t size, size_t& written) noexcept
{
	region_t region_infos = get_region_from_addr(src);

	// Make sure that the region we query is writable
	if (!(region_infos.prot & ProtFlag::W))
		return false;

	size = std::min<uintptr_t>(region_infos.end - src, size);

	std::memcpy(dest, src, size);
	written = size;

	return true;
}

bool CMemAccessor::SafeMemRead(CAddress src, CAddress dest, size_t size, size_t& read) noexcept
{
	region_t region_infos = get_region_from_addr(src);

	// Make sure that the region we query is readable
	if (!(region_infos.prot & ProtFlag::R))
		return false;

	size = std::min<uintptr_t>(region_infos.end - src, size);

	std::memcpy(dest, src, size);
	read = size;

	return true;
}

ProtFlag CMemAccessor::MemProtect(CAddress dest, size_t size, ProtFlag prot, bool& status)
{
	static auto pageSize = static_cast<size_t>(sysconf(_SC_PAGESIZE));
	region_t regionInfo = get_region_from_addr(dest);
	uintptr_t alignedDest = MemoryRound(dest, pageSize);
	uintptr_t alignedSize = MemoryRoundUp(size, pageSize);
	status = mprotect(reinterpret_cast<void*>(alignedDest), alignedSize, TranslateProtection(prot)) == 0;
	return regionInfo.prot;
}
#endif