#include "memaccessor.hpp"
#include "memprotector.hpp"

CMemProtector::CMemProtector(CAddress address, size_t length, ProtFlag prot, bool unsetOnDestroy)
	: m_address(address)
	, m_length(length)
	, m_status(false)
	, m_unsetLater(unsetOnDestroy)
{
	m_origProtection = CMemAccessor::MemProtect(address, length, prot, m_status);
}

CMemProtector::~CMemProtector()
{
	if (m_origProtection == ProtFlag::UNSET || !m_unsetLater)
		return;

	CMemAccessor::MemProtect(m_address, m_length, m_origProtection, m_status);
}
