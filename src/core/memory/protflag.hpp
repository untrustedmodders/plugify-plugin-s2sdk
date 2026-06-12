#pragma once

#include <cstdint>

/**
 * @enum ProtFlag
 * @brief Enum representing memory protection flags.
 */
enum class ProtFlag : uint32_t
{
	UNSET = 0, /**< Value means this gives no information about protection state (un-read) */
	X = 1 << 1, /**< Execute permission */
	R = 1 << 2, /**< Read permission */
	W = 1 << 3, /**< Write permission */
	S = 1 << 4, /**< Shared memory */
	P = 1 << 5, /**< Private memory */
	N = 1 << 6, /**< Value equaling the Linux flag PROT_UNSET (read the prot, and the prot is unset) */
	RWX = R | W | X /**< Read, Write, and Execute permissions */
};

consteval void enable_bitmask_operators(ProtFlag);