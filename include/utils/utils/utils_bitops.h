#ifndef H_UTILS_BITOPS_H
#define H_UTILS_BITOPS_H

#include <stdint.h>
#include <stdbool.h>

#define 	util_bitops_set(v, bit)			(v |= (1u<<bit))
#define 	util_bitops_chk(v, bit)			((v >> bit) & 1u)
#define 	util_bitops_clr(v, bit)			(v &= ~(1u<<bit))

#define 	util_bitops_set64(v, bit)		(v |= (1ull<<bit))
#define 	util_bitops_chk64(v, bit)		((v >> bit) & 1ull)
#define 	util_bitops_clr64(v, bit)		(v &= ~(1ull<<bit))

/**
 * bitops_ffs8/16/32/64: find first set bit in a uint32_t
 *
 * Returns 1 for least significant bit, 32 for most significant bit, 0
 * for no bits set.
 */
int util_bitops_ffs08(uint32_t u);
int util_bitops_ffs16(uint32_t u);
int util_bitops_ffs32(uint32_t u);
int util_bitops_ffs64(uint64_t u);

/**
 * bitops_clz8/16/32/64: count set bit in a uint32_t
 *
 * Returns 0 if most significant bit is set, 31 if only least
 * significant bit is set.
 */
int util_bitops_cs08(uint32_t u);
int util_bitops_cs16(uint32_t u);
int util_bitops_cs32(uint32_t u);
int util_bitops_cs64(uint64_t u);

/**
 * bitops_ctz8/16/32/64: count zeros in a uint32_t
 *
 * Returns 0 if least significant bit is set, 31 if only most
 * significant bit is set.
 */
int util_bitops_cz08(uint32_t u);
int util_bitops_cz16(uint32_t u);
int util_bitops_cz32(uint32_t u);
int util_bitops_cz64(uint64_t u);

/**
 * bitops_clz8/16/32/64: count leading zeros in a uint32_t
 *
 * Returns 0 if most significant bit is set, 31 if only least
 * significant bit is set.
 */
int util_bitops_clz08(uint32_t u);
int util_bitops_clz16(uint32_t u);
int util_bitops_clz32(uint32_t u);
int util_bitops_clz64(uint64_t u);

/**
 * bitops_ctz8/16/32/64: count trailing zeros in a uint32_t
 *
 * Returns 0 if least significant bit is set, 31 if only most
 * significant bit is set.
 */
int util_bitops_ctz08(uint32_t u);
int util_bitops_ctz16(uint32_t u);
int util_bitops_ctz32(uint32_t u);
int util_bitops_ctz64(uint64_t u);


#endif // H_UTILS_BITOPS_H
