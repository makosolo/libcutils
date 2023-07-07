#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>

#include "utils_log.h"
#include "utils_file.h"


void test_file(void)
{
    UTIL_ASSERT_RET(0 == util_file_mkdirs("/tmp/test/0/1/2/3"), "util_file_mkdirs fail");
}
