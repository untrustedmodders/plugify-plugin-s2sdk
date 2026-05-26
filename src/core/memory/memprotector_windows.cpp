#ifdef PLATFORM_WINDOWS
#include "os.h"

#include "memaccessor.hpp"
#include "memprotector.hpp"

int TranslateProtection(ProtFlag flags) noexcept
{
	int nativeFlag = 0;
	if (flags == ProtFlag::X)
		nativeFlag = PAGE_EXECUTE;

	if (flags == ProtFlag::R)
		nativeFlag = PAGE_READONLY;

	if (flags == ProtFlag::W || (flags == (ProtFlag::R | ProtFlag::W)))
		nativeFlag = PAGE_READWRITE;

	if ((flags & ProtFlag::X) && (flags & ProtFlag::R))
		nativeFlag = PAGE_EXECUTE_READ;

	if ((flags & ProtFlag::X) && (flags & ProtFlag::W))
		nativeFlag = PAGE_EXECUTE_READWRITE;

	if (flags & ProtFlag::N)
		nativeFlag = PAGE_NOACCESS;
	return nativeFlag;
}

ProtFlag TranslateProtection(int prot) noexcept
{
	ProtFlag flags = ProtFlag::UNSET;
	switch (prot) {
		case PAGE_EXECUTE:
			flags = flags | ProtFlag::X;
			break;
		case PAGE_READONLY:
			flags = flags | ProtFlag::R;
			break;
		case PAGE_READWRITE:
			flags = flags | ProtFlag::W;
			flags = flags | ProtFlag::R;
			break;
		case PAGE_EXECUTE_READWRITE:
			flags = flags | ProtFlag::X;
			flags = flags | ProtFlag::R;
			flags = flags | ProtFlag::W;
			break;
		case PAGE_EXECUTE_READ:
			flags = flags | ProtFlag::X;
			flags = flags | ProtFlag::R;
			break;
		case PAGE_NOACCESS:
			flags = flags | ProtFlag::N;
			break;
	}
	return flags;
}
#endif
