#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>

#include "utils/utils_assert.h"
#include "utils/utils_bitops.h"

void test_bitops(void)
{
    printf("++++++++++++++++++++++++bitops++++++++++++++++++++++++\n");
    {
        uint8_t value08 = 0xff;
        for (int i = 0; i < 8; i++) {
            UTIL_ASSERT_RET(i == util_bitops_ffs08(value08), "util_bitops_ffs08[0x%X] = %d \n", value08, util_bitops_ffs08(value08));
            UTIL_ASSERT_RET((8 - i) == util_bitops_cs08(value08), "util_bitops_cs08[0x%X] = %d \n", value08, util_bitops_cs08(value08));
            UTIL_ASSERT_RET(i == util_bitops_cz08(value08), "util_bitops_cz08[0x%X] = %d \n", value08, util_bitops_cz08(value08));
            UTIL_ASSERT_RET(i == util_bitops_ctz08(value08), "util_bitops_ctz08[0x%X] = %d \n", value08, util_bitops_ctz08(value08));
            UTIL_ASSERT_RET(0 == util_bitops_clz08(value08), "util_bitops_clz08[0x%X] = %d \n", value08, util_bitops_clz08(value08));
            value08 <<= 1;
        }

        uint16_t value16 = 0xffff;
        for (int i = 0; i < 16; i++) {
            UTIL_ASSERT_RET(i == util_bitops_ffs16(value16), "util_bitops_ffs16[0x%X] = %d \n", value16, util_bitops_ffs16(value16));
            UTIL_ASSERT_RET((16 - i) == util_bitops_cs16(value16), "util_bitops_cs16[0x%X] = %d \n", value16, util_bitops_cs16(value16));
            UTIL_ASSERT_RET(i == util_bitops_cz16(value16), "util_bitops_cz16[0x%X] = %d \n", value16, util_bitops_cz16(value16));
            UTIL_ASSERT_RET(i == util_bitops_ctz16(value16), "util_bitops_ctz16[0x%X] = %d \n", value16, util_bitops_ctz16(value16));
            UTIL_ASSERT_RET(0 == util_bitops_clz16(value16), "util_bitops_clz16[0x%X] = %d \n", value16, util_bitops_clz16(value16));
            value16 <<= 1;
        }

        uint32_t value32 = 0xffffffff;
        for (int i = 0; i < 32; i++) {
            UTIL_ASSERT_RET(i == util_bitops_ffs32(value32), "util_bitops_ffs32[0x%X] = %d \n", value32, util_bitops_ffs32(value32));
            UTIL_ASSERT_RET((32 - i) == util_bitops_cs32(value32), "util_bitops_cs32[0x%X] = %d \n", value32, util_bitops_cs32(value32));
            UTIL_ASSERT_RET(i == util_bitops_cz32(value32), "util_bitops_cz32[0x%X] = %d \n", value32, util_bitops_cz32(value32));
            UTIL_ASSERT_RET(i == util_bitops_ctz32(value32), "util_bitops_ctz32[0x%X] = %d \n", value32, util_bitops_ctz32(value32));
            UTIL_ASSERT_RET(0 == util_bitops_clz32(value32), "util_bitops_clz32[0x%X] = %d \n", value32, util_bitops_clz32(value32));
            value32 <<= 1;
        }

        uint64_t value64 = 0xffffffffffffffff;
        for (int i = 0; i < 64; i++) {
            UTIL_ASSERT_RET(i == util_bitops_ffs64(value64), "util_bitops_ffs64[0x%lX] = %d \n", value64, util_bitops_ffs64(value64));
            UTIL_ASSERT_RET((64 - i) == util_bitops_cs64(value64), "util_bitops_cs64[0x%lX] = %d \n", value64, util_bitops_cs64(value64));
            UTIL_ASSERT_RET(i == util_bitops_cz64(value64), "util_bitops_cz64[0x%lX] = %d \n", value64, util_bitops_cz64(value64));
            UTIL_ASSERT_RET(i == util_bitops_ctz64(value64), "util_bitops_ctz64[0x%lX] = %d \n", value64, util_bitops_ctz64(value64));
            UTIL_ASSERT_RET(0 == util_bitops_clz64(value64), "util_bitops_clz64[0x%lX] = %d \n", value64, util_bitops_clz64(value64));
            value64 <<= 1;
        }

        value08 = 0xff;
        for (int i = 0; i < 8; i++) {
            UTIL_ASSERT_RET(0 == util_bitops_ffs08(value08), "util_bitops_ffs08[0x%X] = %d \n", value08, util_bitops_ffs08(value08));
            UTIL_ASSERT_RET((8 - i) == util_bitops_cs08(value08), "util_bitops_cs08[0x%X] = %d \n", value08, util_bitops_cs08(value08));
            UTIL_ASSERT_RET(i == util_bitops_cz08(value08), "util_bitops_cz08[0x%X] = %d \n", value08, util_bitops_cz08(value08));
            UTIL_ASSERT_RET(0 == util_bitops_ctz08(value08), "util_bitops_ctz08[0x%X] = %d \n", value08, util_bitops_ctz08(value08));
            UTIL_ASSERT_RET(i == util_bitops_clz08(value08), "util_bitops_clz08[0x%X] = %d \n", value08, util_bitops_clz08(value08));
            value08 >>= 1;
        }

        value16 = 0xffff;
        for (int i = 0; i < 16; i++) {
            UTIL_ASSERT_RET(0 == util_bitops_ffs16(value16), "util_bitops_ffs16[0x%X] = %d \n", value16, util_bitops_ffs16(value16));
            UTIL_ASSERT_RET((16 - i) == util_bitops_cs16(value16), "util_bitops_cs16[0x%X] = %d \n", value16, util_bitops_cs16(value16));
            UTIL_ASSERT_RET(i == util_bitops_cz16(value16), "util_bitops_cz16[0x%X] = %d \n", value16, util_bitops_cz16(value16));
            UTIL_ASSERT_RET(0 == util_bitops_ctz16(value16), "util_bitops_ctz16[0x%X] = %d \n", value16, util_bitops_ctz16(value16));
            UTIL_ASSERT_RET(i == util_bitops_clz16(value16), "util_bitops_clz16[0x%X] = %d \n", value16, util_bitops_clz16(value16));
            value16 >>= 1;
        }

        value32 = 0xffffffff;
        for (int i = 0; i < 32; i++) {
            UTIL_ASSERT_RET(0 == util_bitops_ffs32(value32), "util_bitops_ffs32[0x%X] = %d \n", value32, util_bitops_ffs32(value32));
            UTIL_ASSERT_RET((32 - i) == util_bitops_cs32(value32), "util_bitops_cs32[0x%X] = %d \n", value32, util_bitops_cs32(value32));
            UTIL_ASSERT_RET(i == util_bitops_cz32(value32), "util_bitops_cz32[0x%X] = %d \n", value32, util_bitops_cz32(value32));
            UTIL_ASSERT_RET(0 == util_bitops_ctz32(value32), "util_bitops_ctz32[0x%X] = %d \n", value32, util_bitops_ctz32(value32));
            UTIL_ASSERT_RET(i == util_bitops_clz32(value32), "util_bitops_clz32[0x%X] = %d \n", value32, util_bitops_clz32(value32));
            value32 >>= 1;
        }

        value64 = 0xffffffffffffffff;
        for (int i = 0; i < 64; i++) {
            UTIL_ASSERT_RET(0 == util_bitops_ffs64(value64), "util_bitops_ffs64[0x%lX] = %d \n", value64, util_bitops_ffs64(value64));
            UTIL_ASSERT_RET((64 - i) == util_bitops_cs64(value64), "util_bitops_cs64[0x%lX] = %d \n", value64, util_bitops_cs64(value64));
            UTIL_ASSERT_RET(i == util_bitops_cz64(value64), "util_bitops_cz64[0x%lX] = %d \n", value64, util_bitops_cz64(value64));
            UTIL_ASSERT_RET(0 == util_bitops_ctz64(value64), "util_bitops_ctz64[0x%lX] = %d \n", value64, util_bitops_ctz64(value64));
            UTIL_ASSERT_RET(i == util_bitops_clz64(value64), "util_bitops_clz64[0x%lX] = %d \n", value64, util_bitops_clz64(value64));
            value64 >>= 1;
        }
    }
}
