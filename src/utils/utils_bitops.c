/* see https://graphics.stanford.edu/~seander/bithacks.html#IntegerLogDeBruijn */

#include <stdlib.h>
#include <stdint.h>

#include "utils_bitops.h"

__BEGIN_DECLS
const uint8_t BitsSetTable256[256] =
{
#define B2(n) n,     n+1,     n+1,     n+2
#define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
#define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
    B6(0), B6(1), B6(1), B6(2)
};

/* 从低位找到第一个为1的位 */
int util_bitops_ffs08(uint32_t u)
{
    static const int MultiplyDeBruijnBitPosition[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };

    if (0 == u) {
        return -1;
    }

    u &=0xff;
	return MultiplyDeBruijnBitPosition[((u & -u) * (uint32_t)0x077CB531UL) >> 27];
}

int util_bitops_ffs16(uint32_t u)
{
    static const int MultiplyDeBruijnBitPosition[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };

    if (0 == u) {
        return -1;
    }

    u &=0xffff;
	return MultiplyDeBruijnBitPosition[((u & -u) * (uint32_t)0x077CB531UL) >> 27];
}

int util_bitops_ffs32(uint32_t u)
{
    static const int MultiplyDeBruijnBitPosition[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };

    if (0 == u) {
        return -1;
    }

	return MultiplyDeBruijnBitPosition[((u & -u) * (uint32_t)0x077CB531UL) >> 27];
}

int util_bitops_ffs64(uint64_t u)
{
    static const int MultiplyDeBruijnBitPosition[64] = {
        0, 1, 2, 56, 3, 32, 57, 46, 29, 4, 20, 33, 7, 58, 11, 47,
        62, 30, 18, 5, 16, 21, 34, 23, 53, 8, 59, 36, 25, 12, 48, 39,
        63, 55, 31, 45, 28, 19, 6, 10, 61, 17, 15, 22, 52, 35, 24, 38,
        54, 44, 27, 9, 60, 14, 51, 37, 43, 26, 13, 50, 42, 49, 41, 40
    };

    if (0 == u) {
        return -1;
    }

	return MultiplyDeBruijnBitPosition[((u & -u) * (uint64_t)0x26752B916FC7B0DULL) >> 58];
}

/* 统计1的个数 */
int util_bitops_cs08(uint32_t u)
{
    uint8_t* p = (uint8_t *)&u;
    return BitsSetTable256[p[0]];
}

int util_bitops_cs16(uint32_t u)
{
    uint8_t* p = (uint8_t *)&u;
    return (BitsSetTable256[p[0]] + BitsSetTable256[p[1]]);
}

int util_bitops_cs32(uint32_t u)
{
    uint8_t* p = (uint8_t *)&u;
    return (BitsSetTable256[p[0]] + BitsSetTable256[p[1]] + BitsSetTable256[p[2]] +  BitsSetTable256[p[3]]);
}

int util_bitops_cs64(uint64_t u)
{
    uint8_t* p = (uint8_t *)&u;
    return (BitsSetTable256[p[0]] + BitsSetTable256[p[1]] + BitsSetTable256[p[2]] +  BitsSetTable256[p[3]] +\
               BitsSetTable256[p[4]] + BitsSetTable256[p[5]] + BitsSetTable256[p[6]] +  BitsSetTable256[p[7]]);
}

/* 统计0的个数 */
int util_bitops_cz08(uint32_t u)
{
    return 8 - util_bitops_cs08(u);
}

int util_bitops_cz16(uint32_t u)
{
    return 16 - util_bitops_cs16(u);
}

int util_bitops_cz32(uint32_t u)
{
    return 32 - util_bitops_cs32(u);
}

int util_bitops_cz64(uint64_t u)
{
    return 64 - util_bitops_cs64(u);
}

/* 统计前导0的个数 */
int util_bitops_clz08(uint32_t u)
{
	int i;
	for (i = 0; i < 8; i++) {
        if (u & ((uint8_t)1 << (7 - i))) {
            return i;
        }
    }
			
    return 0;
}

int util_bitops_clz16(uint32_t u)
{
	int i;
	for (i = 0; i < 16; i++) {
        if (u & ((uint16_t)1 << (15 - i))) {
            return i;
        }
    }
	
    return 0;
}

int util_bitops_clz32(uint32_t u)
{
	int i;
	for (i = 0; i < 32; i++) {
        if (u & ((uint32_t)1 << (31 - i))) {
            return i;
        }
    }

    return 0;
}

int util_bitops_clz64(uint64_t u)
{
	int i;
	for (i = 0; i < 64; i++) {
        if (u & ((uint64_t)1 << (63 - i))) {
            return i;
        }
    }

    return 0;
}

/* 统计后导0的个数 */
int util_bitops_ctz08(uint32_t u)
{
	return util_bitops_ffs08(u);
}

int util_bitops_ctz16(uint32_t u)
{
	return util_bitops_ffs16(u);
}

int util_bitops_ctz32(uint32_t u)
{
	return util_bitops_ffs32(u);
}

int util_bitops_ctz64(uint64_t u)
{
	return util_bitops_ffs64(u);
}

