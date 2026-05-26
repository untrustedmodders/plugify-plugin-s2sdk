#ifdef PLATFORM_POSIX
#include "os.h"

#include "memaccessor.hpp"
#include "memprotector.hpp"

int TranslateProtection(ProtFlag flags) noexcept
{
	int nativeFlag = PROT_NONE;
	if (flags & ProtFlag::X)
		nativeFlag |= PROT_EXEC;

	if (flags & ProtFlag::R)
		nativeFlag |= PROT_READ;

	if (flags & ProtFlag::W)
		nativeFlag |= PROT_WRITE;

	if (flags & ProtFlag::N)
		nativeFlag = PROT_NONE;

	return nativeFlag;
}

ProtFlag TranslateProtection(int prot) noexcept
{
	ProtFlag flags = ProtFlag::UNSET;

	if (prot & PROT_EXEC)
		flags = flags | ProtFlag::X;

	if (prot & PROT_READ)
		flags = flags | ProtFlag::R;

	if (prot & PROT_WRITE)
		flags = flags | ProtFlag::W;

	if (prot == PROT_NONE)
		flags = flags | ProtFlag::N;

	return flags;
}
#endif